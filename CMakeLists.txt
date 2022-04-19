cmake_minimum_required(VERSION 3.19)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

# Set DRK_VERSION
file(READ VERSION.txt VERSION_txt)
string(REGEX REPLACE "[ \t\r\n]" "" DRK_VERSION ${VERSION_txt}) # Trim whitespace

#############################################
project(DrkCraft VERSION ${DRK_VERSION})
#############################################

# Options
option(DRK_EN_PROFILE "Enable profiling" OFF)
option(DRK_EN_DEV_MODE "Enable Dev mode" OFF)
option(DRK_EN_TRACE_LOGGING "Enable trace logging in console" OFF)
option(DRK_DIST "Dist config" OFF)

# Dist Config
if(${DRK_DIST})
    set(CMAKE_BUILD_TYPE Release)
    set(DRK_EN_PROFILE OFF)
    set(DRK_EN_DEV_MODE OFF)
    set(DRK_EN_TRACE_LOGGING OFF)
    set(DRK_EN_LOGGING OFF)
    set(DRK_EN_ASSERTS OFF)
else()
    set(DRK_EN_LOGGING ON)
    set(DRK_EN_ASSERTS ON)
endif()

# Set DRK_PLATFORM
if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    set(DRK_PLATFORM Windows)
    if(NOT ${MSVC})
        message("Warning: Using non-VS generator on Windows")
    endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    set(DRK_PLATFORM Linux)
else()
    message("Error: Unsupported platform")
    return()
endif()

# Set DRK_BUILD_CONFIG
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(DRK_BUILD_CONFIG Debug)
elseif((${CMAKE_BUILD_TYPE} STREQUAL Release) OR (${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo))
    set(DRK_BUILD_CONFIG Release)
else()
    message("Error: Unknown build type")
    return()
endif()

#### Configuration info dump ####

message("=======================================")
message("Configuring DrkCraft v${DRK_VERSION} build:")
message("- Platform: ${DRK_PLATFORM}")
message("- Build config: ${DRK_BUILD_CONFIG}")

if(${DRK_EN_PROFILE})
    message("- Profiling enabled")
endif()
if(${DRK_EN_DEV_MODE})
    message("- Dev mode enabled")
endif()
if(${DRK_EN_TRACE_LOGGING})
    message("- Trace logging enabled")
endif()
if(${DRK_DIST})
    message("- Dist build")
endif()

message("- Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
message("- Compiler: ${CMAKE_CXX_COMPILER}")
message("=======================================")

# C++ Standard - C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##################################
##################################
add_executable(DrkCraft)

add_subdirectory(src)
add_subdirectory(lib)
##################################
##################################

#### Compile definitions ####

string(TOUPPER DRK_PLATFORM_${DRK_PLATFORM}   DRK_PLATFORM_DEFINITION)
string(TOUPPER DRK_CONFIG_${DRK_BUILD_CONFIG} DRK_BUILD_CONFIG_DEFINITION)

target_compile_definitions(DrkCraft
  PUBLIC
    DRK_VERSION_STRING="${DRK_VERSION}"
    ${DRK_PLATFORM_DEFINITION}
    ${DRK_BUILD_CONFIG_DEFINITION}
)

if(${DRK_BUILD_CONFIG} STREQUAL Debug)
    target_compile_definitions(DrkCraft PUBLIC _DEBUG)
else()
    target_compile_definitions(DrkCraft PUBLIC NDEBUG)
endif()

target_compile_definitions(DrkCraft
  PUBLIC
    $<$<BOOL:DRK_EN_ASSERTS>:"DRK_EN_ASSERTS">
    $<$<BOOL:DRK_EN_PROFILE>:"DRK_EN_PROFILE">
    $<$<BOOL:DRK_EN_DEV_MODE>:"DRK_EN_DEV_MODE">
    $<$<BOOL:DRK_EN_TRACE_LOGGING>:"DRK_EN_TRACE_LOGGING">
)

# Output directory
set_target_properties(DrkCraft
  PROPERTIES
    # Use generator expression for MSVC
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<1:${CMAKE_BUILD_TYPE}>"
)

# Disable console window for Release builds
if(${DRK_BUILD_CONFIG} STREQUAL Release)
    if(${DRK_PLATFORM} STREQUAL Windows)
        target_link_options(DrkCraft PUBLIC /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    elseif(${DRK_PLATFORM} STREQUAL Linux)
        message("TODO: Disable console on Linux build?")
    endif()
endif()
