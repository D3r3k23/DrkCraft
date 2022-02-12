@echo OFF

if exist build (
    if "%1" == "--all" (
        echo Removing all build files
        rmdir /s /q build
    ) else (
        echo Removing DrkCraft build files
        rmdir /s /q build\DrkCraft.dir
    )
)
