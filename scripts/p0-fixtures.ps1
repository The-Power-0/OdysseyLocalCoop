[CmdletBinding()]
param(
    [ValidateSet("Capture", "Restore", "List")]
    [string]$Action = "List",

    [ValidateSet("MetroBoss", "LakePainting", "LostLava")]
    [string]$Scenario,

    [string]$SaveDir,
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
    foreach ($name in @("MetroBoss", "LakePainting", "LostLava")) {
        $fixture = Join-Path $Root $name
        if (Test-Path -LiteralPath (Join-Path $fixture "manifest.json") -PathType Leaf) {
            Write-Host "$name : captured"
        } else {
            Write-Host "$name : not captured"
        }
    }
    exit 0
}

if (-not $Scenario) { throw "Specify -Scenario MetroBoss, LakePainting, or LostLava." }
if (-not $SaveDir) { throw "Specify the game's exact Ryujinx save-data directory using -SaveDir." }

$Save = [IO.Path]::GetFullPath($SaveDir)
if (-not (Test-Path -LiteralPath $Save -PathType Container)) {
    throw "Save directory not found: $Save"
}
if (@(Get-Process -ErrorAction SilentlyContinue |
      Where-Object { $_.ProcessName -match '^Ryujinx' }).Count -gt 0) {
    throw "Close Ryujinx completely before modifying save data."
}

$Fixture = Join-Path $Root $Scenario
$Snapshot = Join-Path $Fixture "snapshot"
$Manifest = Join-Path $Fixture "manifest.json"
New-Item -ItemType Directory -Path $Root -Force | Out-Null

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
    throw "Restore changes the active save. Supply -ConfirmRestore and ensure you have a backup."
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
