// FlatColor Vertex Shader

#version 400

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

layout(location = 0) in vec3 a_position;

void main()
{
    gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
}
