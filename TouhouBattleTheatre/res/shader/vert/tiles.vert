#version 460 core

layout (location = 0) in vec4 in_Tile;


out VS_OUT {
    float tileId;
} vs_out;

void main()
{
    gl_Position = vec4(in_Tile.x, in_Tile.y, in_Tile.z, 1.0);
    vs_out.tileId = in_Tile.w;
}