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
	// 물 텍스처 좌표를 새롭게 초기화
	vec4 tex2 = texture(texIndex2, waterTex);

	// 조건에 따라 렌더할 텍스처 선택하기
	// 두 텍스처를 동시에 바인드 해놓고 조건에 따라 골라서 출력하는 방식
	if (texBottom == 1) {
		//fragColor = tex2;

		// sin()을 이용해 물줄기 중심선이 휘어지게 변형
		// 현재 픽셀의 높이(y)를 기준으로, 이 높이에서의 강물 중심 x좌표를 계산
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


	// 4차 수정본 (물이 휘어지지 않음)
//	if (texBottom == 1) {
//        // x좌표가 0.4 ~ 0.6 사이인 중앙 부분만 강물 텍스처 출력
//        if (vsTexCoord.x > 0.4 && vsTexCoord.x < 0.6) {
//            fragColor = tex2;
//        }
//        else {
//            fragColor = tex1;
//        }
//    }
//    else {
//        // 바닥이 아니면 무조건 모래/하늘 텍스처 출력
//        fragColor = tex1;
//    }
}                   