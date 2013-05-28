@echo off

::
:: This script unpacks all n2pk files in the current directory and sub-directories.
::

set rootDir=%cd%
call :unpack
goto :eof

:unpack
for %%f in (*.N2PK) do (
    set newDir=%cd%.unpacked
    if not exist "%newDir%" mkdir "%newDir%"
    "%rootDir%\unpack_n2pk.exe" "%cd%\%%f" "%newDir%"
)

for /D %%d in (*) do (
    pushd %%d
    call :unpack
    popd
)

exit /b
