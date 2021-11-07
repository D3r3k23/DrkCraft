cmake_minimum_required(VERSION 3.16)

if($CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(PLATFORM Windows)
elseif($CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(PLATFORM Linux)
else()
    message("Error: Unsupported platform")
    exit()
endif()

project(DrkCraft)
add_executable(DrkCraft)
add_subdirectory(src)

set_target_properties(DrkCraft
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(DrkCraft PUBLIC -Wall)

set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set_target_properties(
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY $<1:${OUTPUT_DIRECTORY}>
      LIBRARY_OUTPUT_DIRECTORY $<1:${OUTPUT_DIRECTORY}/lib>
      ARCHIVE_OUTPUT_DIRECTORY $<1:${OUTPUT_DIRECTORY}/lib>
)
