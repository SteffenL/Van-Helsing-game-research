param(
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Path $PSCommandPath
$projectDir = Resolve-Path -Path (Join-Path -Path $scriptDir -ChildPath "..\storage_gui")

$versionHeaderPath = Join-Path -Path $projectDir -ChildPath "AppVersion.h"
$versionHeaderContent = [IO.File]::ReadAllText($versionHeaderPath, [Text.Encoding]::Unicode)


function getDefinedString($name, $content) {
    $pattern = '#define\s+{0}\s+(?:_\(|L)?"([^"]*)' -f $name
    $rx = [regex]$pattern
    $m = $rx.Match($content)
    if (-not $m) {
        throw "No match: $name"
    }

    return $m.Groups[1].Value
}

getDefinedString "APP_VERSION_STR" $versionHeaderContent
