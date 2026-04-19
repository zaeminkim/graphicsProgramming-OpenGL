#version 430 core                

in vec2 vsTexCoord;
in vec3 vsColor;

uniform sampler2D texIndex; // sand
uniform sampler2D texIndex2; // water
uniform int texBottom; // bottom만 선택하기 위한 변수, 1 => bottom, 0 => bottom 아님

out vec4 fragColor; 

void main(void)     
{
	vec4 tex1 = texture(texIndex, vsTexCoord) * vec4(vsColor.r, vsColor.g, vsColor.b, 1.0);
	vec4 tex2 = texture(texIndex2, vsTexCoord);

	if (texBottom == 1) {
		if (vsTexCoord.x > 0.4 && vsTexCoord.x < 0.6) {
		fragColor = tex2;
		}
		else {
			fragColor = tex1;
		}
	}
	else {
		fragColor = tex1;
	}
	
}                   