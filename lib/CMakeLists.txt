set(BUILD_SHARED_LIBS OFF)

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE entt/single_include)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE spdlog/include)

message("Configuring Glad")
add_subdirectory(glad)

message("Configuring GLFW")
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(glfw)
target_compile_definitions(glfw INTERFACE GLFW_INCLUDE_NONE) # Don't need GLFW to include OpenGL header

message("Configuring ImGui")
add_subdirectory(imgui)

message("Configuring yaml-cpp")
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(yaml-cpp)

target_link_libraries(DrkCraft
  PUBLIC
    entt
    glad
    glfw
    glm
    imgui
    spdlog
    yaml-cpp
)
