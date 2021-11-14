set(BUILD_SHARED_LIBS OFF)

# Header only libraries

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE entt/single_include)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE spdlog/include)

# Other

message("Configuring Glad")
add_subdirectory(glad) # Custom CMakeLists.txt

message("Configuring GLFW")
add_subdirectory(glfw) # Repo CMakeLists.txt

message("Configuring ImGui")
add_subdirectory(imgui) # Custom CMakeLists.txt

target_link_libraries(DrkCraft
  PRIVATE
    entt
    glad
    glfw
    glm
    imgui
    spdlog
)
