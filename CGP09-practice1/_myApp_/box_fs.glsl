#version 430 core                
in vec2 vsTexCoord;

uniform sampler2D texIndex1;
uniform sampler2D texIndex2;

out vec4 fragColor; 

void main(void)     
{
	vec4 tex1 = texture(texIndex1, vsTexCoord);
	vec4 tex2 = texture(texIndex2, vsTexCoord);

	// 숫자 텍스처의 하얀 부분 날리기
	// 각각의 R,G,B값이 1.0을 넘지 않으면 tex1을 텍스처로
	// 1.0이면 tex2를 텍스처로
	if (tex1.r < 0.9 || tex1.g < 0.9 || tex1.b < 0.9) {
		fragColor = tex1;
	}
	else {
		fragColor = tex2;
	}
}                   