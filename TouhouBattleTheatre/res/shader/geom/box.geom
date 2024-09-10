#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projection;
uniform vec2 startPosition;
uniform vec2 endPosition; 

void makeTile()
{    
    
    gl_Position = projection * vec4(startPosition, 0.0, 1.0); 
    EmitVertex(); 

    gl_Position = projection * vec4(startPosition.x, endPosition.y ,0.0, 1.0);    
    EmitVertex();
   
    gl_Position = projection *  vec4(endPosition.x, startPosition.y ,0.0, 1.0);  
    EmitVertex();

    gl_Position = projection * vec4(endPosition, 0.0, 1.0); 
    EmitVertex(); 

    EndPrimitive();
}


void main()
{
    makeTile();
}