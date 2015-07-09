@echo off
pushd "%~dp0..\..\..\3rd_party\wxWidgets\samples\dataview"
if not exist "dataview_vcx.vcproj" (
    copy dataview_vc9.vcproj dataview_vcx.vcproj
    devenv /Upgrade dataview_vcx.vcproj
)

msbuild dataview_vcx.vcxproj /t:Rebuild /p:Configuration=Release
popd
