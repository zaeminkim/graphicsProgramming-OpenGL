#version 430 core                  
			                       
layout (location = 2) in vec4 pos;       

void main(void)                    
{                                  
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	
}															 