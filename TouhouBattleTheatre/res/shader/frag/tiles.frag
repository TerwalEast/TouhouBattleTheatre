#version 460 core

out vec4 out_FragmentColor;

in vec3 texCoord;

uniform sampler2D tileSheet;
uniform sampler2DArray texarray;

void main()
{
    out_FragmentColor = texture(texarray,texCoord);
}