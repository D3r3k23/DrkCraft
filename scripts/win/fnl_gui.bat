@echo OFF

@REM Requires .NET

set "exe=lib\FastNoiseLite\PreviewApp\build\FastNoiseLiteGUI.exe"

if NOT exist %exe% (
    dotnet publish lib\FastNoiseLite\PreviewApp -c Release -o lib\FastNoiseLite\PreviewApp\build
)
%exe%
