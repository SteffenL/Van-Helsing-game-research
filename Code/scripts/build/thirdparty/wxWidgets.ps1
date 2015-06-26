Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"


# Customizations for setup.h

$redefines = @{
    WXWIN_COMPATIBILITY_2_8 = 0;
    WXWIN_COMPATIBILITY_3_0 = 0
}

$defineReplacements = @{
    # Makes life easier while debugging
    # Disabled because it seems like my life can be easier even with this enabled, and it's nice to have to catch exceptions in the main loop
#    wxUSE_EXCEPTIONS = @"
#ifdef NDEBUG
#  #define wxUSE_EXCEPTIONS 1
##else
#  #define wxUSE_EXCEPTIONS 0
##endif
#"@
}

$customReplacements = @(
    # Saves quite a bit of space
    @{
        pattern = "// #ifdef NDEBUG\s*//  #define wxDEBUG_LEVEL \d+\s*// #else\s*//  #define wxDEBUG_LEVEL \d+\s*// #endif";
        replacement = @"
#ifdef NDEBUG
  #define wxDEBUG_LEVEL 0
#else
  #define wxDEBUG_LEVEL 2
#endif
"@;
        options = [System.Text.RegularExpressions.RegexOptions]::Multiline
    }
)

# Build configurations

$buildConfigs =
    @{
        name = "debug";
        buildArgs = "RUNTIME_LIBS=dynamic", "UNICODE=1", "SHARED=1", "BUILD=debug"
    },
    @{
        name = "release";
        buildArgs = "CPPFLAGS=/Os", "RUNTIME_LIBS=dynamic", "UNICODE=1", "SHARED=0", "BUILD=release"
    }


###################################
# Nothing to see beyond this line #
###################################


$EXIT_SUCCESS = 0

$scriptFileName = Split-Path -Path $PSCommandPath -Leaf
$logDir = Join-Path -Path $PSScriptRoot -ChildPath "..\logs\thirdparty" -Resolve


function cleanRepository()
{
    # Clean files and start from scratch
    &git reset --hard
    if (!$?) {
        throw "Failed to reset repository."
    }

    &git clean -d -x -f
    if (!$?) {
        throw "Failed to clean repository."
    }
}

function runBuilds($configs)
{
    Push-Location "build\msw"

    try {
        $processes = @()
        foreach ($config in $configs) {
            $nmakeArgs = ("-f", "makefile.vc") + $config.buildArgs
            Write-Output ("Running build: {0}" -f $config.name)
            $logFilePath = Join-Path $logDir -ChildPath ("{0}.{1}.log" -f $scriptFileName, $config.name)

            $processes += @{
                process = (Start-Process -FilePath nmake -ArgumentList $nmakeArgs -PassThru -RedirectStandardOutput $logFilePath);
                config = $config
            }
        }

        if ($processes.Length) {
            $processes.process.WaitForExit()
            $processes | %{
                if (-not $_.process.ExitCode -eq $EXIT_SUCCESS) {
                    Write-Error ("Build failed: {0}" -f $_.config.name)
                }
            }
        }
    }
    catch {
        Write-Error "One or more builds crashed."
        throw
    }
    finally {
        Pop-Location
    }
}

function writeDefines()
{
    $sourceSetupFilePath = "include\wx\msw\setup0.h"
    $destSetupFilePath = "include\wx\msw\setup.h"
    $setupContent = Get-Content -Path $sourceSetupFilePath -Raw

    $redefineCallback = {
        param($m)
        $matchedName = $m.Groups[1].Value
        if (-not ($redefines.Keys -contains $matchedName)) {
            return $m.Groups[0].Value
        }

        return $redefines[$matchedName]
    }

    $replaceDefineCallback = {
        param($m)
        $matchedName = $m.Groups[1].Value
        if (-not ($defineReplacements.Keys -contains $matchedName)) {
            return $m.Groups[0].Value
        }

        return $defineReplacements[$matchedName]
    }

    $customDefineCallback = {
        param($m)
        $matchedName = $m.Groups[1].Value
        if ($defineReplacements.Keys -ccontains $matchedName) {
            return $defineReplacements[$matchedName]
        }
    }

    $redefinePattern = "(?<=#define\s+(\w+)\s+)\w+"
    $setupContent = [Regex]::Replace($setupContent, $redefinePattern, $redefineCallback, [System.Text.RegularExpressions.RegexOptions]::Singleline)

    $replaceDefinePattern = "#define\s+(\w+)\s+\w+"
    $setupContent = [Regex]::Replace($setupContent, $replaceDefinePattern, $replaceDefineCallback, [System.Text.RegularExpressions.RegexOptions]::Singleline)

    $customReplacements | %{
        $setupContent = [Regex]::Replace($setupContent, $_.pattern, $_.replacement, $_.options)
    }


    $setupContent | Set-Content -Path $destSetupFilePath
}


cleanRepository
writeDefines
runBuilds($buildConfigs)
