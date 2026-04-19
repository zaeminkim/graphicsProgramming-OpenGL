#version 430 core                

in vec2 vsTexCoord;
in vec3 vsColor;

uniform sampler2D texIndex; // sand
uniform sampler2D texIndex2; // water
uniform int texBottom; // bottom만 선택하기 위한 변수, 1 => bottom, 0 => bottom 아님
uniform float time; // currentTime

out vec4 fragColor; 

void main(void)     
{
	vec4 tex1 = texture(texIndex, vsTexCoord) * vec4(vsColor.r, vsColor.g, vsColor.b, 1.0);

	// 물 텍스처 좌표에 sin, cos, time을 더해 애니메이션 추가하기
	vec2 waterTex = vsTexCoord;
	//waterTex.x += sin(time * 3.0) * 0.015;
	waterTex.y += time * 0.2;
	vec4 tex2 = texture(texIndex2, waterTex);

	if (texBottom == 1) {
		// sin()을 이용해 물줄기 중심선이 휘어지게 변형
		// y좌표에 따라 x 중심선이 좌우로 흔들림
		float waterCenter = 0.5 + sin(vsTexCoord.y * 4.0) * 0.1;

		// 중심선(waterCenter)을 기준으로 두께 ±0.1 범위 안에 있으면 강물 출력
		if (vsTexCoord.x > waterCenter - 0.1 && vsTexCoord.x < waterCenter + 0.1) {		
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