target_include_directories(DrkCraft
  PUBLIC
    entt/src
    imgui
    spdlog/include
)

add_subdirectory(glad)
add_subdirectory(glfw)



add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include)
