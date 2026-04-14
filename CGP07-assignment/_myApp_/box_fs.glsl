#version 430 core                
in vec2 vsTexCoord;

uniform sampler2D texIndex1;
uniform sampler2D texIndex2;

out vec4 fragColor; 

void main(void)     
{                   
	fragColor = texture(texIndex2, vsTexCoord) * texture(texIndex1, vsTexCoord);  
}                   