param(
    [ValidateSet("Ryujinx", "Atmosphere")]
    [string]$Target = "Ryujinx",
    [string]$ContentsRoot
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest
$PackageRoot = (Resolve-Path -LiteralPath $PSScriptRoot).Path
$ManifestPath = Join-Path $PackageRoot "PACKAGE_MANIFEST.json"
$ChecksumsPath = Join-Path $PackageRoot "CHECKSUMS.txt"
foreach ($path in @($ManifestPath, $ChecksumsPath)) { if (-not (Test-Path -LiteralPath $path -PathType Leaf)) { throw "Invalid package; missing $path" } }
$manifest = Get-Content -Raw -LiteralPath $ManifestPath | ConvertFrom-Json
if ($manifest.schema_version -ne 1 -or $manifest.program_id -ne "0100000000010000") { throw "Unsupported package manifest." }

foreach ($line in @(Get-Content -LiteralPath $ChecksumsPath)) {
    if ([string]::IsNullOrWhiteSpace($line)) { continue }
    if ($line -notmatch '^([0-9A-Fa-f]{64}) \*(.+)$') { throw "Malformed checksum line: $line" }
    $relative = $Matches[2].Replace('/', [IO.Path]::DirectorySeparatorChar)
    $source = [IO.Path]::GetFullPath((Join-Path $PackageRoot $relative))
    $prefix = $PackageRoot.TrimEnd('\', '/') + [IO.Path]::DirectorySeparatorChar
    if (-not $source.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase) -or -not (Test-Path -LiteralPath $source -PathType Leaf)) { throw "Checksum path is invalid: $relative" }
    if ((Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash -ne $Matches[1].ToUpperInvariant()) { throw "Package checksum mismatch: $relative" }
}

# Portable Ryujinx keeps its user data under "portable" next to
# Ryujinx.exe. Search upward from the EXTRACTED PACKAGE (not the current
# PowerShell working directory), so nested folders such as
# Ryujinx/P0-Tests/OdysseyLocalCoop-v0.1.0-alpha.5/install.ps1 work.
function Find-PortableRyujinxRoot {
    param([string]$StartDirectory)
    $directory = [IO.DirectoryInfo]::new([IO.Path]::GetFullPath($StartDirectory))
    while ($null -ne $directory) {
        $executable = Join-Path $directory.FullName "Ryujinx.exe"
        $portable = Join-Path $directory.FullName "portable"
        if ((Test-Path -LiteralPath $executable -PathType Leaf) -and
            (Test-Path -LiteralPath $portable -PathType Container)) {
            return $directory.FullName
        }
        $directory = $directory.Parent
    }
    return $null
}

if ([string]::IsNullOrWhiteSpace($ContentsRoot)) {
    if ($Target -eq "Ryujinx") {
        $portableRoot = Find-PortableRyujinxRoot -StartDirectory $PackageRoot
        if ($portableRoot) {
            $ContentsRoot = Join-Path (Join-Path (Join-Path $portableRoot "portable") "mods") "contents"
            Write-Host "Portable Ryujinx detected: $portableRoot"
        } else {
            # Preserve conventional installations. Print the fallback to make it
            # obvious if the installer is not located below the portable root.
            if ([string]::IsNullOrWhiteSpace($env:APPDATA)) {
                throw "Portable Ryujinx not found above this package and APPDATA is unavailable. Place it under the folder containing Ryujinx.exe and portable, or pass -ContentsRoot."
            }
            $ContentsRoot = Join-Path $env:APPDATA "Ryujinx\mods\contents"
            Write-Warning "No portable Ryujinx root found above the package. Using the standard APPDATA location. For portable installations, move the package under the folder containing Ryujinx.exe and portable, or pass -ContentsRoot."
        }
    } else {
        throw "Atmosphere installation is experimental; specify the SD card's atmosphere/contents path with -ContentsRoot."
    }
}
$ContentsRoot = [IO.Path]::GetFullPath($ContentsRoot)
$TitleRoot = Join-Path $ContentsRoot $manifest.program_id
New-Item -ItemType Directory -Force -Path $TitleRoot | Out-Null
# Ryujinx recursively scans the title's mod directory, including hidden folders.
$backupBase = Join-Path (Split-Path -Parent $ContentsRoot) ".ocoop-backup"
$backupRoot = Join-Path (Join-Path $backupBase $manifest.program_id) (Get-Date -Format "yyyyMMdd-HHmmss-fffffff")
$backupCount = 0

foreach ($record in @($manifest.files)) {
    $prefixText = "payload/contents/$($manifest.program_id)/"
    if (-not ([string]$record.path).StartsWith($prefixText, [StringComparison]::Ordinal)) { throw "Unexpected payload path: $($record.path)" }
    $relativeTarget = ([string]$record.path).Substring($prefixText.Length).Replace('/', [IO.Path]::DirectorySeparatorChar)
    $source = Join-Path $PackageRoot ([string]$record.path).Replace('/', [IO.Path]::DirectorySeparatorChar)
    if ((Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash -ne [string]$record.sha256) { throw "Payload hash mismatch: $($record.path)" }
    $destination = [IO.Path]::GetFullPath((Join-Path $TitleRoot $relativeTarget))
    $titlePrefix = [IO.Path]::GetFullPath($TitleRoot).TrimEnd('\', '/') + [IO.Path]::DirectorySeparatorChar
    if (-not $destination.StartsWith($titlePrefix, [StringComparison]::OrdinalIgnoreCase)) { throw "Payload path escapes the title directory: $relativeTarget" }
    if (Test-Path -LiteralPath $destination -PathType Leaf) {
        $backup = Join-Path $backupRoot $relativeTarget
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $backup) | Out-Null
        Copy-Item -LiteralPath $destination -Destination $backup -Force
        $backupCount++
    }
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $destination) | Out-Null
    $temporary = "$destination.ocoop-new"
    Copy-Item -LiteralPath $source -Destination $temporary -Force
    Move-Item -LiteralPath $temporary -Destination $destination -Force
}

Write-Host "Odyssey Local Co-op $($manifest.version) installed for $Target."
if ($manifest.PSObject.Properties["game_version"]) {
    Write-Host "Target Super Mario Odyssey version: $($manifest.game_version)"
}
Write-Host "Destination: $TitleRoot"
if ($backupCount -gt 0) { Write-Host "Backed up $backupCount replaced file(s) under: $backupRoot" }
if ($Target -eq "Atmosphere") { Write-Warning "Physical Switch/Atmosphere support is experimental." }
