#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 texCoord;

uniform float gridSize;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void makeTile(vec4 position)
{    
    gl_Position = projection * view * model * position ; // 1:��ǰ������
    texCoord = vec2(0,0);
    EmitVertex(); 
    
    gl_Position = projection * view * model * ( position + vec4(0.0, 0.0, 0.1, 0.0) );    // 2:���+Z
    texCoord = vec2(0,1);
    EmitVertex();

    gl_Position = projection * view * model * ( position + vec4(0.1, 0.0, 0.0, 0.0) );    // 3:��ǰ+X
    texCoord = vec2(1,0);
    EmitVertex();


    gl_Position = projection * view * model * ( position + vec4(0.1, 0.0, 0.1, 0.0) );    // 4:�Һ�+X+Z
    texCoord = vec2(1,1);
    EmitVertex();
    EndPrimitive();
}


void main()
{
    makeTile(gl_in[0].gl_Position);
}