// SmoothColor Vertex Shader

#version 400

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

layout(location = 0) out smooth vec3 vert_color;

void main()
{
    vert_color = a_color;

    // gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
    gl_Position = vec4(a_position, 1.0);
}
