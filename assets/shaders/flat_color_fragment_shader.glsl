// FlatColor Fragment Shader

#version 400

uniform vec4 color;

out vec4 fragColor;

void main()
{
    fragColor = color;
}
