#version 430 core                 
  
layout (location = 0) in vec3 pos;   
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 color;

uniform mat4 viewMat;
uniform mat4 projMat;

out vec2 vsTexCoord;
out vec3 vsColor;

void main(void)                    
{
	gl_Position = projMat * viewMat * vec4(pos.x, pos.y, pos.z, 1.0);
	vsTexCoord = texCoord;
	vsColor = color;
}															