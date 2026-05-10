#version 430 core

in vec2 vsTexCoord; // 인터폴레이션된 픽셀 좌표
uniform sampler2D texture_ground; // 만들어서 바인딩한 텍스처이미지
// 데이터 타입: uniform sampler2D
out vec4 fragColor;

void main(void) {
	fragColor = texture(texture_ground, vsTexCoord);
	// 실제 픽셀값 vec4 (RGBA)를 가져오는 함수
	// (픽셀값을 가져올 텍스처, 텍스처에서 픽셀값을 가져올 위치)
}