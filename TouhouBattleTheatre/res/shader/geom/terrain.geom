#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 7) out;

in VS_OUT
{
    vec3 offset1;
    vec3 offset2;
    vec3 offset3;
    vec3 offset4;
} gs_in[];

uniform mat4 model;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void makeTile(vec4 position)
{    
    gl_Position = projection * view * model * vec4(gs_in[0].offset1, 1.0);
    EmitVertex(); 

    gl_Position = projection * view * model * vec4(gs_in[0].offset2, 1.0);
    EmitVertex();

    gl_Position = projection * view * model * vec4(gs_in[0].offset4, 1.0);
    EmitVertex();

    gl_Position = projection * view * model * vec4(gs_in[0].offset3, 1.0);
    EmitVertex();

    gl_Position = projection * view * model * vec4(gs_in[0].offset2, 1.0);
    EmitVertex();

    gl_Position = projection * view * model * vec4(gs_in[0].offset4, 1.0);
    EmitVertex();

    gl_Position = projection * view * model * vec4(gs_in[0].offset1, 1.0);
    EmitVertex();

    EndPrimitive();
}


void main()
{
    makeTile(gl_in[0].gl_Position);
}