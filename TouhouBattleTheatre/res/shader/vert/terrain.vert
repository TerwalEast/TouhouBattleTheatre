#version 460 core

layout (location = 0) in vec3 in_Offset1;
layout (location = 1) in vec3 in_Offset2;
layout (location = 2) in vec3 in_Offset3;
layout (location = 3) in vec3 in_Offset4;

out VS_OUT
{
    vec3 offset1;
    vec3 offset2;
    vec3 offset3;
    vec3 offset4;
} vs_out;


void main()
{
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    vs_out.offset1 = in_Offset1;    
    vs_out.offset2 = in_Offset2;
    vs_out.offset3 = in_Offset3;
    vs_out.offset4 = in_Offset4;
}

