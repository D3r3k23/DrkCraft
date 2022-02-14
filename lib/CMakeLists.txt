set(BUILD_SHARED_LIBS OFF)
set(LIBTYPE STATIC)
set(BUILD_TESTING OFF)

message("Configuring stb")
add_subdirectory(stb)

message("Configuring Glad")
add_subdirectory(glad)

message("Configuring GLFW")
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)
target_compile_definitions(glfw INTERFACE GLFW_INCLUDE_NONE) # Don't need GLFW to include OpenGL header

message("Configuring GLM")
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

message("Configuring {fmt}")
add_subdirectory(fmt)

message("Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory(spdlog)

message("Configuring RapidJSON")
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE rapidjson/include)
target_compile_definitions(rapidjson INTERFACE RAPIDJSON_HAS_STDSTRING=1)

message("Configuring yaml-cpp")
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(yaml-cpp)
target_compile_definitions(yaml-cpp PUBLIC YAML_CPP_STATIC_DEFINE)

message("Configuring EnTT")
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE entt/src)

message("Configuring FastNoiseLite")
add_library(FastNoiseLite INTERFACE)
target_include_directories(FastNoiseLite INTERFACE FastNoiseLite/Cpp)

message("Configuring ImGui")
add_subdirectory(imgui)

message("Configuring nameof")
add_subdirectory(nameof)

message("Configuring minimp3")
add_subdirectory(minimp3)

message("Configuring ogg")
add_subdirectory(ogg)

message("Configuring OpenAL Soft")
set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
add_subdirectory(openal-soft)

message("Configuring Vorbis")
add_subdirectory(vorbis)

target_link_libraries(DrkCraft
  PUBLIC
    entt
    FastNoiseLite
    fmt
    glad
    glfw
    glm
    imgui
    imgui_impl
    minimp3
    nameof
    ogg
    OpenAL
    rapidjson
    spdlog
    stb_image
    stb_rect_pack
    stb_truetype
    vorbis
    vorbisfile
    yaml-cpp
)
