// SmoothColor Fragment Shader

#version 400

layout(location = 0) in smooth vec3 vert_color;

layout(location = 0) out vec4 frag_color;

void main()
{
    frag_color = vert_color;
}
