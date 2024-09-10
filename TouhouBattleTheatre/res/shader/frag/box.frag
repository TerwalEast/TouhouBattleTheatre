#version 460 core

out vec4 out_FragmentColor;

uniform vec4 color;

void main()
{
    out_FragmentColor = color;
}