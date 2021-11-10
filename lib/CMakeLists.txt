# Header only libraries
target_include_directories(DrkCraft
  PUBLIC
    entt/single_include/entt
    glm
    spdlog/include
)

add_subdirectory(glad) # Custom CMakeLists.txt
add_subdirectory(glfw) # Repo CMakeLists.txt
# add_subdirectory(imgui) # Custom CMakeLists.txt

target_link_libraries(DrkCraft
  PUBLIC
    glad
    glfw
    # imgui
)
