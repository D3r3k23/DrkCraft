@echo OFF

call scripts\get_build_config.bat %1 %2
%exe% %2
