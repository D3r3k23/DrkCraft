add_library(imgui
    imgui/imgui.cpp
    imgui/imgui?impl_opengl3.cpp
    imgui/imguie_impl_glfw.cpp
)

target_include_directories(imgui
  PUBLIC
    imgui
)
