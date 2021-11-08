cmake_minimum_required(VERSION 3.16)

# Set PLATFORM
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(PLATFORM Windows)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
    set(PLATFORM Linux)
else()
    message("Error: Unsupported platform")
    exit(1)
endif()

# Set BUILD_CONFIG
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(BUILD_CONFIG Debug)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(BUILD_CONFIG Release)
else()
    set(CMAKE_BUILD_TYPE Debug)
    set(BUILD_CONFIG Debug)
endif()

message("[${PLATFORM}] ${BUILD_TYPE} build")

project(DrkCraft)
add_executable(DrkCraft)

add_subdirectory(src)
add_subdirectory(lib)

target_link_libraries(DrkCraft
  PUBLIC
    glad
    glfw
)

# C++ 20
set_target_properties(DrkCraft
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Platform & build config defines
string(TOUPPER DRK_PLATFORM_${PLATFORM}   DRK_PLATFORM)
string(TOUPPER DRK_CONFIG_${BUILD_CONFIG} DRK_BUILD_CONFIG)

target_compile_definitions(DrkCraft
  PUBLIC
    ${DRK_PLATFORM}
    ${DRK_BUILD_CONFIG}
)
