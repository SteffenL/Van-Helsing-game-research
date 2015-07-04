$scriptDir = Split-Path -Path $PSCommandPath
$thirdpartyRootDir = Resolve-Path -Path (Join-Path -Path $scriptDir -ChildPath "..\..\3rd_party")
$libNames = ,"wxWidgets"

$libNames | %{
    $thirdpartyLibDir = Join-Path -Path $thirdpartyRootDir -ChildPath $_
    if (-not (Test-Path -Path $thirdpartyLibDir)) {
        throw "Directory not found: $thirdpartyLibDir"
    }

    Write-Output "Building library at ""$thirdpartyLibDir"""
    $libBuildScript = Join-Path -Path $scriptDir -ChildPath "thirdparty\$_.ps1"
    Push-Location -Path $thirdpartyLibDir

    try {
        &$libBuildScript
        if (-not $?) {
            throw "Build script failed"
        }
    }
    catch {
        Pop-Location
        throw
    }

    Pop-Location
}
