#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoord;

uniform mat4 model;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec2 texCoord;

void main()
{
    gl_Position = projection * view * model * vec4(in_Position, 1.0);
    texCoord = in_TexCoord;
}