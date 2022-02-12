// Cube Fragment Shader

#version 400

uniform sampler2D u_textures[32];

layout(location = 0) in smooth vec2 vert_texCoord;
layout(location = 1) in flat float vert_texIndex;

layout(location = 0) out vec4 frag_color;

void main()
{
    frag_color = texture(u_textures[(int)vert_texIndex], vert_texCoord);
}
