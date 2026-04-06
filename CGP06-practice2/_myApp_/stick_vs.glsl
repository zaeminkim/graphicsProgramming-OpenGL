#version 430 core                  
			                       
layout (location = 0) in vec4 pos; 

uniform mat4 transMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main(void)                    
{                                  
	gl_Position = projMat * viewMat * transMat * vec4(pos.x, pos.y, pos.z, 1.0);
}															 