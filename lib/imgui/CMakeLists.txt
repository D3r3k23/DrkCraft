add_library(imgui)

target_sources(imgui
  PRIVATE
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/misc/cpp/imgui_stdlib.cpp

    imgui_impl/imgui_impl_glfw.cpp
    imgui_impl/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    imgui
)

target_link_libraries(imgui
  PUBLIC
    glad
    glfw
    glm
)

if(${BUILD_CONFIG} STREQUAL Release)
    target_compile_definitions(imgui PUBLIC IMGUI_DISABLE_DEMO_WINDOWS)
endif()

target_compile_definitions(imgui
  PUBLIC
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
    IMGUI_USER_CONFIG="imgui_impl/imconfig.h"
)
