set(BUILD_SHARED_LIBS OFF)

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE entt/single_include)

add_library(FastNoiseLite INTERFACE)
target_include_directories(FastNoiseLite INTERFACE FastNoiseLite)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE rapidjson/include)

message("Configuring {fmt}")
add_subdirectory(fmt)

message("Configuring GLFW")
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(glfw)
target_compile_definitions(glfw INTERFACE GLFW_INCLUDE_NONE) # Don't need GLFW to include OpenGL header

message("Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory(spdlog)

message("Configuring yaml-cpp")
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(yaml-cpp)

add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(stb_image)

target_link_libraries(DrkCraft
  PUBLIC
    entt
    FastNoiseLite
    fmt
    glad
    glfw
    glm
    imgui
    rapidjson
    spdlog
    stb_image
    yaml-cpp
)
