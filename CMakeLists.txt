cmake_minimum_required(VERSION 3.16)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(PLATFORM Windows)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
    set(PLATFORM Linux)
else()
    message("Error: Unsupported platform")
    exit(1)
endif()

message("[${PLATFORM}] ${CMAKE_BUILD_TYPE} build")

project(DrkCraft)
add_executable(DrkCraft)
add_subdirectory(src)
add_subdirectory(lib)

target_link_libraries(DrkCraft
  PUBLIC
    glad
    glfw
)

target_link_libraries(DrkCraft glfw)

set_target_properties(DrkCraft
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# target_compile_options(DrkCraft
#   PUBLIC
#     -Wall
# )

if(${PLATFORM} STREQUAL Windows)
    target_compile_definitions(DrkCraft PUBLIC DRK_PLATFORM_WINDOWS)
elseif(${PLATFORM} STREQUAL Linux)
    target_compile_definitions(DrkCraft PUBLIC DRK_PLATFORM_LINUX)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    target_compile_definitions(DrkCraft PUBLIC DRK_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    target_compile_definitions(DrkCraft PUBLIC DRK_RELEASE)
endif()
