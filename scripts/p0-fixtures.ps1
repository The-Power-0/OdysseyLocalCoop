[CmdletBinding()]
param(
    [ValidateSet("Capture", "Restore", "PrepareTest", "List")]
    [string]$Action = "List",

    [ValidateSet("MetroBoss", "LakePainting", "LostLava", "DoubleFall", "CapPylon")]
    [string]$Scenario,

    [string]$SaveDir,
    [string]$TestSaveDir,
    [string]$FixtureRoot,
    [switch]$ConfirmRestore,
    [switch]$ReplaceFixture
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

if ([string]::IsNullOrWhiteSpace($FixtureRoot)) {
    $localBase = if ($env:LOCALAPPDATA) {
        $env:LOCALAPPDATA
    } elseif ($env:HOME) {
        Join-Path $env:HOME ".local/share"
    } else {
        throw "Specify -FixtureRoot."
    }
    $FixtureRoot = Join-Path $localBase "OdysseyLocalCoop/TestFixtures"
}

$Root = [IO.Path]::GetFullPath($FixtureRoot)
$RepoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..")).TrimEnd('\', '/')
$RootPrefix = $RepoRoot + [IO.Path]::DirectorySeparatorChar
if ($Root.Equals($RepoRoot, [StringComparison]::OrdinalIgnoreCase) -or
    $Root.StartsWith($RootPrefix, [StringComparison]::OrdinalIgnoreCase)) {
    throw "Fixture files must be stored OUTSIDE the repository."
}

function Get-SnapshotHashes {
    param([string]$Directory)
    $absolute = [IO.Path]::GetFullPath($Directory).TrimEnd('\', '/')
    $rows = @()
    foreach ($file in @(Get-ChildItem -LiteralPath $Directory -Recurse -File -Force |
                         Sort-Object FullName)) {
        $relative = $file.FullName.Substring($absolute.Length).TrimStart('\', '/').Replace('\', '/')
        $rows += [ordered]@{
            path = $relative
            sha256 = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash
        }
    }
    return $rows
}

if ($Action -eq "List") {
    if (-not (Test-Path -LiteralPath $Root -PathType Container)) {
        Write-Host "No local P0 fixtures captured yet."
        exit 0
    }
    foreach ($name in @("MetroBoss", "LakePainting", "LostLava", "DoubleFall", "CapPylon")) {
        $fixture = Join-Path $Root $name
        if (Test-Path -LiteralPath (Join-Path $fixture "manifest.json") -PathType Leaf) {
            Write-Host "$name : captured"
        } else {
            Write-Host "$name : not captured"
        }
    }
    exit 0
}

if (-not $Scenario) { throw "Specify -Scenario MetroBoss, LakePainting, LostLava, DoubleFall, or CapPylon." }
if (-not $SaveDir) { throw "Specify the game's exact Ryujinx save-data directory using -SaveDir." }

function Test-PathOverlap {
    param([string]$First, [string]$Second)
    $a = [IO.Path]::GetFullPath($First).TrimEnd('\', '/')
    $b = [IO.Path]::GetFullPath($Second).TrimEnd('\', '/')
    $separator = [IO.Path]::DirectorySeparatorChar
    return ($a.Equals($b, [StringComparison]::OrdinalIgnoreCase) -or
            $a.StartsWith($b + $separator, [StringComparison]::OrdinalIgnoreCase) -or
            $b.StartsWith($a + $separator, [StringComparison]::OrdinalIgnoreCase))
}

function Assert-PlainDirectory {
    param([string]$Directory)
    $item = Get-Item -LiteralPath $Directory -Force
    if (-not $item.PSIsContainer) { throw "Not a directory: $Directory" }
    while ($null -ne $item) {
        if ($item.Attributes -band [IO.FileAttributes]::ReparsePoint) {
            throw "Refusing redirected or linked directory: $($item.FullName)"
        }
        $item = $item.Parent
    }
    foreach ($entry in @(Get-ChildItem -LiteralPath $Directory -Force -Recurse)) {
        if ($entry.Attributes -band [IO.FileAttributes]::ReparsePoint) {
            throw "Refusing redirected or linked entry: $($entry.FullName)"
        }
    }
}

$Save = [IO.Path]::GetFullPath($SaveDir)
if ($Action -eq "PrepareTest") {
    if (-not $TestSaveDir) { throw "Supply -TestSaveDir for a separate disposable Ryujinx profile." }
    $test = [IO.Path]::GetFullPath($TestSaveDir)
    if (Test-PathOverlap $Save $test) { throw "Test save and normal save must be separate directories." }
    Assert-PlainDirectory $Save
    Assert-PlainDirectory $test
    if (Test-PathOverlap $Root $Save) { throw "Fixture root overlaps normal save." }
    if (Test-PathOverlap $RepoRoot $test) { throw "Test save must be outside the repository." }
    Write-Host "Normal save (read-only): $Save"
    Write-Host "Disposable test save (restore target): $test"
    $Save = $test
}
if (Test-PathOverlap $Root $Save) { throw "Fixture root and save directory must not overlap." }
if (-not (Test-Path -LiteralPath $Save -PathType Container)) {
    throw "Save directory not found: $Save"
}
Assert-PlainDirectory $Save
if (@(Get-Process -ErrorAction SilentlyContinue |
      Where-Object { $_.ProcessName -match '^Ryujinx' }).Count -gt 0) {
    throw "Close Ryujinx completely before modifying save data."
}

$Fixture = Join-Path $Root $Scenario
$Snapshot = Join-Path $Fixture "snapshot"
$Manifest = Join-Path $Fixture "manifest.json"
New-Item -ItemType Directory -Path $Root -Force | Out-Null
Assert-PlainDirectory $Root

if ($Action -eq "Capture") {
    if ((Test-Path -LiteralPath $Fixture) -and -not $ReplaceFixture) {
        throw "Fixture already exists; use -ReplaceFixture only after reviewing the saved snapshot."
    }
    $temporary = Join-Path $Root ("." + $Scenario + "-capture-temp")
    if (Test-Path -LiteralPath $temporary) {
        throw "Stale temporary fixture: $temporary. Inspect before retrying."
    }
    New-Item -ItemType Directory -Path $temporary | Out-Null
    try {
        Copy-Item -LiteralPath $Save -Destination (Join-Path $temporary "snapshot") -Recurse -Force
        $hashes = @(Get-SnapshotHashes -Directory (Join-Path $temporary "snapshot"))
        if ($hashes.Count -eq 0) { throw "Refusing to capture an empty save directory." }
        $manifestJson = [ordered]@{
            schema = 1
            scenario = $Scenario
            captured_utc = [DateTime]::UtcNow.ToString("o")
            files = $hashes
        } | ConvertTo-Json -Depth 6
        [IO.File]::WriteAllText((Join-Path $temporary "manifest.json"), $manifestJson)
        if (Test-Path -LiteralPath $Fixture) {
            $previous = Join-Path $Root ("old-" + $Scenario + "-" + [DateTime]::UtcNow.ToString("yyyyMMddHHmmssfff"))
            Move-Item -LiteralPath $Fixture -Destination $previous
            Write-Host "Previous fixture preserved: $previous"
        }
        Move-Item -LiteralPath $temporary -Destination $Fixture
        Write-Host "Captured local $Scenario snapshot ($($hashes.Count) files). No upload performed."
    } finally {
        if (Test-Path -LiteralPath $temporary) {
            Remove-Item -LiteralPath $temporary -Recurse -Force
        }
    }
    exit 0
}

if (-not $ConfirmRestore) {
    throw "Restore changes the target save. Supply -ConfirmRestore and ensure you have a backup."
}
if (-not (Test-Path -LiteralPath $Snapshot -PathType Container) -or
    -not (Test-Path -LiteralPath $Manifest -PathType Leaf)) {
    throw "Fixture not captured for scenario $Scenario."
}
$expected = Get-Content -LiteralPath $Manifest -Raw | ConvertFrom-Json
if ($expected.schema -ne 1 -or $expected.scenario -ne $Scenario) {
    throw "Fixture manifest mismatch."
}
$actual = @(Get-SnapshotHashes -Directory $Snapshot)
$stored = @($expected.files)
if ($actual.Count -ne $stored.Count) { throw "Fixture file count mismatch." }
for ($i = 0; $i -lt $actual.Count; $i++) {
    if ($actual[$i].path -cne $stored[$i].path -or
        $actual[$i].sha256 -cne $stored[$i].sha256) {
        throw "Fixture checksum mismatch: $($actual[$i].path)"
    }
}

$Backup = Join-Path $Root ("pre-restore-" + $Scenario + "-" +
                           [DateTime]::UtcNow.ToString("yyyyMMddHHmmssfff"))
$Moved = $false
try {
    Move-Item -LiteralPath $Save -Destination $Backup
    $Moved = $true
    Copy-Item -LiteralPath $Snapshot -Destination $Save -Recurse -Force
    $restored = @(Get-SnapshotHashes -Directory $Save)
    if ($restored.Count -ne $actual.Count) { throw "Post-restore file count mismatch." }
    for ($i = 0; $i -lt $actual.Count; $i++) {
        if ($restored[$i].path -cne $actual[$i].path -or
            $restored[$i].sha256 -cne $actual[$i].sha256) {
            throw "Post-restore checksum mismatch."
        }
    }
    Write-Host "Restored $Scenario fixture. Previous active save preserved: $Backup"
} catch {
    if ($Moved -and (Test-Path -LiteralPath $Backup)) {
        if (Test-Path -LiteralPath $Save) {
            Remove-Item -LiteralPath $Save -Recurse -Force
        }
        Move-Item -LiteralPath $Backup -Destination $Save
        Write-Warning "Restore failed; rolled back the active save."
    }
    throw
}
