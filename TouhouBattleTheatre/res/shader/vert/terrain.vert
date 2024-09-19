#version 460 core

layout (location = 0) in vec3 in_PositionA;
layout (location = 1) in vec3 in_PositionB;
layout (location = 2) in vec3 in_PositionC;
layout (location = 3) in vec3 in_PositionD;

uniform mat4 model;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};


void main()
{
    gl_Position = projection * view * model * vec4(in_Position, 1.0);
}