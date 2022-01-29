// SmoothColor Vertex Shader

#version 400

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;

out vec3 vert_color;

void main()
{
    vert_color = (a_color, 1.0);

    gl_Position = vec4(a_position, 1.0);
}
