// Cube Fragment Shader

#version 410

uniform sampler2D u_textures[32];

layout(location = 0) smooth in vec2 vert_texCoord;
layout(location = 1) flat in float vert_texIndex;

layout(location = 0) out vec4 frag_color;

void main()
{
    frag_color = texture(u_textures[int(vert_texIndex)], vert_texCoord);
}
