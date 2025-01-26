#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    float tileId;
} gs_in[];

out vec3 texCoord;

//uniform float gridSize;
uniform mat4 model;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void makeTile(vec4 position)
{    
    gl_Position = projection * view * model * position ; 
    texCoord = vec3(0, 0, gs_in[0].tileId);
    EmitVertex(); 
    
    gl_Position = projection * view * model * ( position + vec4(0.0, 0.0, 10, 0.0) );   
    texCoord = vec3(0, 1, gs_in[0].tileId);
    EmitVertex();

    gl_Position = projection * view * model * ( position + vec4(10, 0.0, 0.0, 0.0) );    
    texCoord = vec3(1, 0, gs_in[0].tileId);
    EmitVertex();

    gl_Position = projection * view * model * ( position + vec4(10, 0.0, 10, 0.0) );    
    texCoord = vec3(1, 1, gs_in[0].tileId);
    EmitVertex();
    EndPrimitive();
}


void main()
{
    makeTile(gl_in[0].gl_Position);
}