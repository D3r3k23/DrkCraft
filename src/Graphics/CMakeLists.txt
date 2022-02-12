target_sources(DrkCraft
  PRIVATE
    Camera.cpp
    CubeRenderer.cpp
    Mesh.cpp
    MeshRenderer.cpp
    OpenGlContext.cpp
    Renderer.cpp
    Shader.cpp
    TextRenderer.cpp
    Texture.cpp
    Transform.cpp
)

add_subdirectory(detail)
