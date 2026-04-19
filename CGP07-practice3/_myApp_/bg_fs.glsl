#version 430 core                

in vec2 vsTexCoord;
in vec3 vsColor;
uniform sampler2D texIndex;

out vec4 fragColor; 

void main(void)     
{                   
	fragColor = texture(texIndex, vsTexCoord) * vec4(vsColor.r, vsColor.g, vsColor.b, 1.0); 
}                   