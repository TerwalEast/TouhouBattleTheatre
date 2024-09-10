#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform vec3 startPosition;
uniform vec3 endPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void makeTile(vec4 position)
{    
    gl_Position = projection * view * model * vec4(startPosition, 1.0) ; // 1:左前出发点
    EmitVertex(); 
    
    gl_Position = projection * view * model * vec4(endPosition, 1.0);    // 2:左后+Z
    EmitVertex();

    EndPrimitive();
}


void main()
{
    makeTile(gl_in[0].gl_Position);
}