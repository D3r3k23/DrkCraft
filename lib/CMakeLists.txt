set(BUILD_SHARED_LIBS OFF)
set(LIBTYPE STATIC)
set(BUILD_TESTING OFF)

###################################################

message("Configuring Glad")
add_subdirectory(glad)

###################################################

message("Configuring GLFW")
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)
target_compile_definitions(glfw INTERFACE GLFW_INCLUDE_NONE)

###################################################

message("Configuring GLM")
add_subdirectory(glm)

###################################################

message("Configuring {fmt}")
add_subdirectory(fmt)

###################################################

message("Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory(spdlog)

###################################################

message("Configuring stb")
add_subdirectory(stb)

###################################################

message("Configuring tinyobjloader")
add_subdirectory(tinyobjloader)

###################################################

message("Configuring magic_enum")
add_library(magic_enum INTERFACE)
target_include_directories(magic_enum INTERFACE magic_enum/include)

###################################################

message("Configuring minimp3")
add_subdirectory(minimp3)

###################################################

message("Configuring ogg")
add_subdirectory(ogg)

###################################################

message("Configuring OpenAL Soft")
set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
add_subdirectory(openal-soft)

###################################################

message("Configuring Vorbis")
add_subdirectory(vorbis)

###################################################

message("Configuring RapidJSON")
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE rapidjson/include)
target_compile_definitions(rapidjson INTERFACE RAPIDJSON_HAS_STDSTRING)

###################################################

message("Configuring yaml-cpp")
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_BUILD_TESTS OFF)
add_subdirectory(yaml-cpp)
target_compile_definitions(yaml-cpp PUBLIC YAML_CPP_STATIC_DEFINE)

###################################################

message("Configuring EnTT")
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE entt/src)

###################################################

message("Configuring FastNoiseLite")
add_library(FastNoiseLite INTERFACE)
target_include_directories(FastNoiseLite INTERFACE FastNoiseLite/Cpp)

###################################################

message("Configuring ImGui")
add_subdirectory(imgui)

###################################################

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
    magic_enum
    ogg
    OpenAL
    rapidjson
    spdlog
    stb_image
    stb_image_write
    stb_rect_pack
    stb_truetype
    tinyobjloader
    vorbis
    vorbisfile
    yaml-cpp
)
