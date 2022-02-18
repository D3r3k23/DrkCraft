##### imgui #########################
add_library(imgui)

target_sources(imgui
  PRIVATE
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/misc/cpp/imgui_stdlib.cpp
)

target_include_directories(imgui
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
    imgui
)

target_link_libraries(imgui
  PUBLIC
    glm
    stb_rect_pack
    stb_truetype
)

target_compile_definitions(imgui
  PUBLIC
    IMGUI_USER_CONFIG="drkcraft_imconfig.h"
)

if(${BUILD_CONFIG} STREQUAL Release)
    target_compile_definitions(imgui PUBLIC IMGUI_DISABLE_DEMO_WINDOWS)
endif()

##### imgui_impl ####################
add_library(imgui_impl)

target_sources(imgui_impl
  PRIVATE
    imgui_impl/imgui_impl_glfw.cpp
    imgui_impl/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_impl
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(imgui_impl
  PUBLIC
    imgui
    glad
    glfw
)

target_compile_definitions(imgui_impl
  PUBLIC
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
)
