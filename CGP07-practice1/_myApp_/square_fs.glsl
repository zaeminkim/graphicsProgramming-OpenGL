#version 430 core                
in vec3 vsColor;   
out vec4 color;

in vec2 vsTexCoord;  // 인터폴레이션된 픽셀좌표
uniform sampler2D texture1; // sampler2D: 2D 텍스처가 있는 인덱스를 저장하는 데이터 타입
out vec4 fragColor;

void main(void)     
{                   
	// color = vec4(vsColor, 1.0);
	fragColor = texture(texture1, vsTexCoord) * vec4(vsColor, 1.0);
	// texture(): 실제 픽셀값(vec4 RGBA)를 가져오는 함수
	// (픽셀값을 가져올 텍스처, 텍스처에서 픽셀값을 가져올 좌표)
}                   