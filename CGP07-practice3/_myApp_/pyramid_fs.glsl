#version 430 core                

in vec2 vsTexCoord;
uniform sampler2D texIndex;

out vec4 fragColor; 

void main(void)     
{                   
	fragColor = texture(texIndex, vsTexCoord);  
}                   