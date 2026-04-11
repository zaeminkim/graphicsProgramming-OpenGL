#version 430 core                 

layout (location = 0) in vec3 pos;   
// layout (location = 1) in vec3 color; // 필요없음, 텍스처 사용하기 때문
layout (location = 2) in vec2 texCoord; // 텍스처 좌표

// 메트릭스 uniform 변수로 선언
uniform mat4 transMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec2 vsTexCoord;

void main(void)                    
{
	// 모델 메트릭스 -> 뷰 메트릭스 -> 프로젝션 메트릭스 순서로 연산
	gl_Position = projMat * viewMat * transMat * vec4(pos.x, pos.y, pos.z, 1.0);

	// vsColor = color;
	vsTexCoord = texCoord;
}								
