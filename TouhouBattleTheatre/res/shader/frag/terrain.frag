#version 460 core

out vec4 out_FragmentColor;

in GS_OUT
{
    vec4 outColor;
} fs_in;

void main()
{
    out_FragmentColor = fs_in.outColor;
}