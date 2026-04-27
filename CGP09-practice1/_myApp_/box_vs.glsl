#version 430 core
  
layout (location = 0) in vec3 pos;   
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 rotMat;
uniform mat4 transMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec2 vsTexCoord;
out vec3 vsNormal; // 프래그먼트별로 보간이 되도록 프래그먼트 쉐이더로 넘기기
out vec3 vsPos;

void main(void)
{
    mat4 modelMat = transMat * rotMat;
    
    // 월드 좌표계에서의 정점 위치
    vsPos = vec3(modelMat * vec4(pos, 1.0));
    // 월드공간에서 라이팅 계산을 하기 위해 뷰, 프로젝션 메트릭스 변환은 적용 안함
    
    // Normal Matrix
    vsNormal = mat3(transpose(inverse(modelMat))) * normal;

    // 최종 화면상의 위치 계산
    gl_Position = projMat * viewMat * vec4(vsPos, 1.0);

    vsTexCoord = texCoord;
}
