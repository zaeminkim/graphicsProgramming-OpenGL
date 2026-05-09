#version 430 core                
in vec3 vsNormal;
in vec3 vsPos;

uniform vec3 lightPos;    // 광원의 위치
uniform vec3 viewPos;     // 카메라의 위치(눈 위치)
uniform vec3 lightColor;  // 광원의 색
uniform vec3 objectColor; // 반사하는 정도
uniform float shininess;  // 하이라이트 정도

out vec4 fragColor; 

void main(void)     
{
    // --- Phong Lighting Model ---
    // 1. Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 2. Diffuse
    vec3 norm = normalize(vsNormal);  // 단위벡터로 만들기 위해 정규화

    vec3 lightDir = normalize(lightPos - vsPos);  // 광원을 향하는 벡터 구하고 정규화

    float diff = max(dot(norm, lightDir), 0.0);
    // 내적을 통해 밝기 계산 음수값이 나올 경우, 0으로 만들기 위해 max()

    vec3 diffuse = diff * lightColor;

    // 3. Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vsPos); // 카메라를 향하는 벡터
    vec3 reflectDir = reflect(-lightDir, norm); // 표면에 반사된 광원 벡터
    // 표면에 반사된 광원 벡터를 구하는 방법: reflect(광원을 향하는 벡터, 노멀벡터)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // --- 최종 조명 연산 ---
    vec3 result = (ambient + diffuse + specular) * objectColor;
    
    // 최종 출력 코드
    fragColor = vec4(result, 1.0);
}