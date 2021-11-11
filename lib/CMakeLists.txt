# Header only libraries
target_include_directories(DrkCraft
  PUBLIC
    entt/single_include/entt
    glm
    spdlog/include
)

message("Configuring Glad")
add_subdirectory(glad) # Custom CMakeLists.txt

message("Configuring GLFW")
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(glfw) # Repo CMakeLists.txt

# message("Configuring ImGui")
# add_subdirectory(imgui) # Custom CMakeLists.txt

target_link_libraries(DrkCraft
  PUBLIC
    glad
    glfw
    # imgui
)
