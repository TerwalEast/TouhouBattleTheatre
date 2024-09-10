#version 460 core

out vec4 out_FragmentColor;

in vec2 texCoord;

uniform sampler2D tileSheet;

void main()
{
    out_FragmentColor = texture(tileSheet,texCoord);
}