param(
    [Parameter(Mandatory=$true)]
    [string] $newVersionStr
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Path $PSCommandPath
$projectDir = Resolve-Path -Path (Join-Path -Path $scriptDir -ChildPath "..\storage_gui")

$versionHeaderPath = Join-Path -Path $projectDir -ChildPath "AppVersion.h"
$mswResourcePath = Join-Path -Path $projectDir -ChildPath "msw\App.rc"

$versionHeaderContent = [IO.File]::ReadAllText($versionHeaderPath, [Text.Encoding]::Unicode)
$mswResourceContent = [IO.File]::ReadAllText($mswResourcePath, [Text.Encoding]::Unicode)

$newVersion = $newVersionStr -split "\."
$newFullVersion = $newVersion
for ($i = $newVersion.Count; $i -lt 4; $i++) {
    $newFullVersion += 0
}


function getDefinedString($name, $content) {
    $pattern = '#define\s+{0}\s+(?:_\(|L)?"([^"]*)' -f $name
    $rx = [regex]$pattern
    $m = $rx.Match($content)
    if (-not $m) {
        throw "No match: $name"
    }

    return $m.Groups[1].Value
}

function setDefinedString($name, $content, $str) {
    $pattern = '(?<=#define\s+{0}\s+(?:_\(|L)?")([^"]*)' -f $name
    $rx = [regex]$pattern
    $m = $rx.Replace($content, $str)
    return $m
}

$versionHeaderDefines = @{
    AppFriendlyName = getDefinedString "APP_FRIENDLY_NAME" $versionHeaderContent;
    AppVendorName = getDefinedString "APP_VENDOR_NAME" $versionHeaderContent;
    AppVersionStr = getDefinedString "APP_VERSION_STR" $versionHeaderContent;
    AppDescription = getDefinedString "APP_DESCRIPTION_I18N" $versionHeaderContent
}


# Make changes to header file

$versionHeaderContent = setDefinedString "APP_VERSION_STR" $versionHeaderContent $newVersionStr

# Make changes to version info in resource file (MSW)

$newVersionInResFormat = $newFullVersion -join ","

$mswResourceContent = $mswResourceContent -replace '(?<=FILEVERSION\s+).+', $newVersionInResFormat
$mswResourceContent = $mswResourceContent -replace '(?<=PRODUCTVERSION\s+).+', $newVersionInResFormat
$mswResourceContent = $mswResourceContent -replace '(?<=VALUE\s+"CompanyName",\s+")[^"]*', $versionHeaderDefines.AppVendorName
$mswResourceContent = $mswResourceContent -replace '(?<=VALUE\s+"FileDescription",\s+")[^"]*', $versionHeaderDefines.AppDescription
$mswResourceContent = $mswResourceContent -replace '(?<=VALUE\s+"FileVersion",\s+")[^"]*', $newVersionStr
$mswResourceContent = $mswResourceContent -replace '(?<=VALUE\s+"ProductName",\s+")[^"]*', $versionHeaderDefines.AppFriendlyName
$mswResourceContent = $mswResourceContent -replace '(?<=VALUE\s+"ProductVersion",\s+")[^"]*', $newVersionStr


# Save changes

[IO.File]::WriteAllText($versionHeaderPath, $versionHeaderContent, [Text.Encoding]::Unicode)
[IO.File]::WriteAllText($mswResourcePath, $mswResourceContent, [Text.Encoding]::Unicode)
