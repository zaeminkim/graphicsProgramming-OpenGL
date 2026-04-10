#version 430 core                
in vec3 vsColor;   
out vec4 color; 

void main(void)     
{                   
	color = vec4(vsColor, 1.0);  
}                   