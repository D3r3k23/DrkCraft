target_include_directories(DrkCraft
  PUBLIC
    entt/src
    imgui
    spdlog/include
)

add_subdirectory(glad)
add_subdirectory(glfw)
