#version 430 core                

in vec2 vsTexCoord;
in vec3 vsColor;

uniform sampler2D texIndex; // sand, sky
uniform sampler2D texIndex2; // water
uniform int texBottom; // bottom만 선택하기 위한 변수, 1 => bottom, 0 => bottom 아님
uniform float time; // currentTime

out vec4 fragColor; 

void main(void)     
{
	vec4 tex1 = texture(texIndex, vsTexCoord) * vec4(vsColor.r, vsColor.g, vsColor.b, 1.0);

	vec2 starTex = vsTexCoord;
	starTex.y += time * 0.5;
	vec4 tex2 = texture(texIndex, starTex);

	if (texBottom == 1) {
		fragColor = tex1;
	}
	else {
		fragColor = tex2;
	}
}                   