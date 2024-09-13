#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 texCoord;

uniform float gridSize;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform vec3 verticalBillVec;
uniform vec3 horizontalBillVec;
uniform vec3 unitPos;

out vec2 TexCoord;

void makeTile()
{    

	mat4 pvm = projection * view;
    gl_Position = pvm * vec4( unitPos + ( - 0.05 * horizontalBillVec), 1.0); // 1:左下
    texCoord = vec2(0,0);
    EmitVertex(); 
    
    gl_Position = pvm * vec4( unitPos + ( + 0.05 * horizontalBillVec), 1.0);    // 3:右下
    texCoord = vec2(1,0);
    EmitVertex();

    gl_Position = pvm * vec4( unitPos + ( + 0.1 * verticalBillVec - 0.05 * horizontalBillVec), 1.0);    // 2:左上
    texCoord = vec2(0,1);
    EmitVertex();

    

    gl_Position = pvm * vec4( unitPos + ( + 0.1 * verticalBillVec + 0.05 * horizontalBillVec), 1.0);    // 4:右上
    texCoord = vec2(1,1);
    EmitVertex();
    EndPrimitive();
}


void main()
{
    makeTile();
}