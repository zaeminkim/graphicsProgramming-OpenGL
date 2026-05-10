#version 430 core                
in vec3 vsNormal;
in vec3 vsPos;
in vec2 vsTexCoord; // 텍스처 좌표 받기

// 구조체로 Phong Ligthing Model 각각 요소의 값 조절하기
struct Material {
    // vec3 ambient;
    // vec3 diffuse; 
    sampler2D diffuse;  // Diffuse Map
    sampler2D specular; // Specular Map
    float shininess;
};
// 구조체로 Phong Ligthing Model 각각 요소의 lightColor 값 조절하기
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;     // 카메라의 위치(눈 위치)

out vec4 fragColor; 

void main(void)     
{
    // --- Phong Lighting Model ---
    // 1. Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vsTexCoord));

    // 2. Diffuse
    vec3 norm = normalize(vsNormal);  // 단위벡터로 만들기 위해 정규화
    vec3 lightDir = normalize(light.position - vsPos);  // 광원을 향하는 벡터 구하고 정규화
    float diff = max(dot(norm, lightDir), 0.0);  // 내적을 통해 밝기 계산 음수값이 나올 경우, 0으로 만들기 위해 max()
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));

    // 3. Specular
    vec3 viewDir = normalize(viewPos - vsPos); // 카메라를 향하는 벡터
    vec3 reflectDir = reflect(-lightDir, norm); // 표면에 반사된 광원 벡터
    // 표면에 반사된 광원 벡터를 구하는 방법: reflect(광원을 향하는 벡터, 노멀벡터)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vsTexCoord));

    // --- 최종 조명 연산 ---
    vec3 result = ambient + diffuse + specular;
    // 최종 출력 코드
    fragColor = vec4(result, 1.0);
}