@echo OFF
@REM Requires .NET

set "build_dir=lib\FastNoiseLite\PreviewApp\build"
set "exe=%build_dir%\FastNoiseLiteGUI.exe"

if NOT exist %exe% (
    dotnet publish lib\FastNoiseLite\PreviewApp -c Release -o %build_dir%
)
%exe%
