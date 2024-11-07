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
    vec3 offset5;
    vec3 offset6;
    vec3 offset7;
    vec3 offset8;
} vs_out;


void main()
{
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    vs_out.offset1 = in_Offset1;    
    vs_out.offset2 = in_Offset2;
    vs_out.offset3 = in_Offset3;
    vs_out.offset4 = in_Offset4;
    vs_out.offset5 = vec3(in_Offset1.x, 0.0, in_Offset1.z);
    vs_out.offset6 = vec3(in_Offset2.x, 0.0, in_Offset2.z);
    vs_out.offset7 = vec3(in_Offset3.x, 0.0, in_Offset3.z);
    vs_out.offset8 = vec3(in_Offset4.x, 0.0, in_Offset4.z);
}

