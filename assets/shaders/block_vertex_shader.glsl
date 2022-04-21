// Cube Vertex Shader

#version 400

uniform mat4 u_viewProjection;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in uint a_texIndex;

layout(location = 0) out smooth vec2 vert_texCoord;
layout(location = 1) out flat float vert_texIndex;

void main()
{
    vert_texIndex = (float)a_texIndex;

    // gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
    gl_Position = vec4(a_position, 1.0);
}