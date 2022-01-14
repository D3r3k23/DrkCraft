// FlatColor Fragment Shader

#version 400

uniform vec4 u_color;

layout(location = 0) out vec4 frag_color;

void main()
{
    frag_color = u_color;
}
