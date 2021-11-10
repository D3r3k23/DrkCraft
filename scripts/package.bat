@echo ON

call scripts\get_build_config.bat %1

set "package_dir=package\DrkCraft"

rmdir /S /Q %package_dir%
mkdir %package_dir%

xcopy %exe% %package_dir%
xcopy data %package_dir%\data\ /E /y /EXCLUDE:scripts\package_exclusion_list.txt
