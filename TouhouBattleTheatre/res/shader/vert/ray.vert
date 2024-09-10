#version 460 core

layout (location = 1) in vec3 in_Tile_Positions;

void main()
{
    gl_Position = vec4(in_Tile_Positions, 1.0);
}