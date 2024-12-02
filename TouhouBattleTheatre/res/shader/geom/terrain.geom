#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 20) out;

in VS_OUT
{
    vec3 offset1;
    vec3 offset2;
    vec3 offset3;
    vec3 offset4;
    vec3 offset5;
    vec3 offset6;
    vec3 offset7;
    vec3 offset8;
} gs_in[];

uniform mat4 model;
uniform vec4 colorSurface;
uniform vec4 colorSide;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out GS_OUT
{
	vec4 outColor;
} gs_out;

void makeTile(vec4 position)
{   
    mat4 pvm = projection * view * model;

    gl_Position = pvm * vec4(gs_in[0].offset1, 1.0);
    gs_out.outColor = colorSurface;
    EmitVertex(); 

    gl_Position = pvm * vec4(gs_in[0].offset2, 1.0);
    gs_out.outColor = colorSurface;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset3, 1.0);
    gs_out.outColor = colorSurface;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset4, 1.0);
    gs_out.outColor = colorSurface;
    EmitVertex();

    EndPrimitive();

    gl_Position = pvm * vec4(gs_in[0].offset3, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset4, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset7, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset8, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    EndPrimitive();

    gl_Position = pvm * vec4(gs_in[0].offset6, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset8, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset2, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset4, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    EndPrimitive();

    gl_Position = pvm * vec4(gs_in[0].offset7, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset5, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset3, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset1, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    EndPrimitive();

    gl_Position = pvm * vec4(gs_in[0].offset5, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset6, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset1, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();

    gl_Position = pvm * vec4(gs_in[0].offset2, 1.0);
    gs_out.outColor = colorSide;
    EmitVertex();


    

    EndPrimitive();
}


void main()
{
    makeTile(gl_in[0].gl_Position);
}