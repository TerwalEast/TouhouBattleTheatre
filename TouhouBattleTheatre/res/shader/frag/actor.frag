#version 460 core

in vec2 texCoord;

out vec4 out_FragmentColor;

uniform sampler2D textureSampler;

void main()
{
    out_FragmentColor = texture(textureSampler, texCoord);
}