#version 460 core

out vec4 out_FragmentColor;

in vec2 texCoord;

uniform sampler2D tileSheet;
uniform bool select;

void main()
{
    vec4 tmp = texture(tileSheet,texCoord);
    if(tmp.a < 0.1)
    {
        discard;
    }
    if(select)
    {
        tmp = vec4(1 - tmp.x,
               1 - tmp.y,
               1 - tmp.z,
               tmp.a);
    }

    out_FragmentColor = tmp;
}