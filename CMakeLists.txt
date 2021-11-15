cmake_minimum_required(VERSION 3.16)

# Set DRK_EN_PROFILE
option(DRK_EN_PROFILE "Enable profiling" OFF)

# Set DRK_EN_DEV_MODE
option(DRK_EN_DEV_MODE "Enable Dev mode" OFF)

# Set DRK_VERSION
file(READ VERSION.txt VERSION_txt)
string(REGEX REPLACE "[ \t\r\n]" "" DRK_VERSION ${VERSION_txt})

project(DrkCraft VERSION ${DRK_VERSION})

# Set PLATFORM
if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    set(PLATFORM Windows)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    set(PLATFORM Linux)
else()
    message("Error: Unsupported platform")
    return()
endif()

# Set BUILD_CONFIG
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(BUILD_CONFIG Debug)
elseif((${CMAKE_BUILD_TYPE} STREQUAL Release) OR (${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo))
    set(BUILD_CONFIG Release)
else()
    message("Error: Unknown build type")
    return()
endif()

message("=======================================")
message("Configuring DrkCraft v${DRK_VERSION} build:")
message("- Platform: ${PLATFORM}")
message("- Build config: ${BUILD_CONFIG}")

if(${DRK_EN_PROFILE})
    message("- Profiling enabled")
endif()
if(${DRK_EN_PROFILE})
    message("- Dev mode enabled")
endif()

message("- Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
message("- Compiler: ${CMAKE_CXX_COMPILER}")
message("=======================================")

add_executable(DrkCraft)
add_subdirectory(src)
add_subdirectory(lib)

# C++ 20
set_target_properties(DrkCraft
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Required because <format> is not included with std:c++20
if(${PLATFORM} STREQUAL Windows)
    target_compile_options(DrkCraft PUBLIC -std:c++latest)
endif()

# Disable console window for Release builds
if(${BUILD_CONFIG} STREQUAL Release)
    if(${PLATFORM} STREQUAL Windows)
        target_link_options(DrkCraft PUBLIC /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    elseif(${PLATFORM} STREQUAL Linux)
        message("TODO: Disable console on Linux build?")
    endif()
endif()

# Compiler definitions
string(TOUPPER DRK_PLATFORM_${PLATFORM}   DRK_PLATFORM)
string(TOUPPER DRK_CONFIG_${BUILD_CONFIG} DRK_BUILD_CONFIG)

target_compile_definitions(DrkCraft
  PUBLIC
    DRK_VERSION_STRING="${DRK_VERSION}"

    ${DRK_PLATFORM}
    ${DRK_BUILD_CONFIG}

    DRK_EN_LOGGING
    DRK_EN_ASSERTS
)

# Enable profiling
if(${DRK_EN_PROFILE})
    target_compile_definitions(DrkCraft PUBLIC DRK_EN_PROFILE)
endif()

# Enable Dev mode
if(${DRK_EN_DEV_MODE})
    target_compile_definitions(DrkCraft PUBLIC DRK_EN_DEV_MODE)
endif()
