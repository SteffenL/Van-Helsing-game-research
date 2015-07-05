@echo off

set appFriendlyName=Van Helsing Storage Editor
set filenamePrefix=storage-editor
set websiteUrl=https://www.steffenl.com/
set certificateSubjectName=Langnes Solutions
set digestAlgorithm=SHA256
set timestampServer=http://timestamp.comodoca.com/rfc3161


set signArgs=/fd "%digestAlgorithm%" /d "%appFriendlyName%" /du "%websiteUrl%" /n "%certificateSubjectName%" /tr "%timestampServer%"

for /F "delims=" %%a in ('powershell -ExecutionPolicy Bypass -File "%~dp0CurrentVersion.ps1"') do set "version=%%a"

if not defined SIGNTOOL (
    echo Not defined: SIGNTOOL
    exit /b 1
)

set solutionDir=%~dp0..
set artifactsDir=%solutionDir%\bin\Release
set endUserDocDir=%solutionDir%\..\Doc\End-users
set artifactsArchiveFilePath=%~dp0\%filenamePrefix%-%version%-artifacts.zip
set releaseArchiveFilePath=%~dp0\%filenamePrefix%-%version%-bin.zip

set zip=%~dp0bin\7z\7za.exe

cmd /c exit /b 0

:: Package artifacts
"%zip%" a -mx9 -tzip "%artifactsArchiveFilePath%" "%artifactsDir%\*.exe" "%artifactsDir%\*.dll" "%artifactsDir%\*.pdb"
if not "%errorlevel%" == "0" exit /b 1

:: Create release-package

set tempFilesDir=%~dp0tempPackageFiles
if exist "%tempFilesDir%" rmdir /S /Q "%tempFilesDir%"
mkdir "%tempFilesDir%"
if not "%errorlevel%" == "0" exit /b 1

:: Copy build artifacts
robocopy "%artifactsDir%" "%tempFilesDir%" *.exe *.dll /XJ /XF *Tests.*
:: Sign files
for %%f in ("%tempFilesDir%\*") do (
    call :sign "%%f"
)

:: Copy documentation
robocopy "%endUserDocDir%" "%tempFilesDir%\doc" /E /XJ

:: Zip everything
"%zip%" a -mx9 -tzip "%releaseArchiveFilePath%" "%tempFilesDir%\*"
if not "%errorlevel%" == "0" exit /b 1


:: Clean up
rmdir /S /Q "%tempFilesDir%"

goto :eof

:sign
    "%SIGNTOOL%" sign %signArgs% "%~1"
    if not "%errorlevel%" == "0" exit /b 1
    goto :eof