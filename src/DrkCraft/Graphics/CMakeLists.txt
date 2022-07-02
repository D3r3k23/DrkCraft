target_sources(DrkCraft
  PRIVATE
    Camera.cpp
    LightSource.cpp
    Mesh.cpp
    OpenGlContext.cpp
    Shader.cpp
    Texture.cpp
    Transform.cpp
)

add_subdirectory(detail)
add_subdirectory(Renderer)
