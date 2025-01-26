#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoord;

out vec2 texCood;

void main()
{
    gl_Position =  vec4(in_Position, 1.0);
    texCood = in_TexCoord;
}