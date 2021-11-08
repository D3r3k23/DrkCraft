target_include_directories(DrkCraft
  PUBLIC
    entt/single_include/entt
    glm
    # imgui
    spdlog/include
)

add_subdirectory(glad)
add_subdirectory(glfw)

target_link_libraries(DrkCraft
  PUBLIC
    glad
    glfw
)
