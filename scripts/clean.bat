@echo OFF

if exist build (
    echo Removing build files
    rmdir /s /q build
)
