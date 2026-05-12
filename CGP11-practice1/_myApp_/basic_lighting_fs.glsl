#version 430 core

in vec3 vsNormal;  
in vec3 vsPos;  
in vec2 vsTexCoord;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;  // Diffuse Map
    sampler2D specular; // Specular Map
    float shininess;
};
uniform Material material;

//1 =============================
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
// ===============================

//2 ==============================
struct PointLight {
    vec3 position;
    float c1, c2;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NUM_POINT_LIGHTS 2
uniform PointLight pointLights[NUM_POINT_LIGHTS];
// ====================================

//3 ================================
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff, outerCutOff;
    float c1, c2;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;
// ==================================

out vec4 fragColor;

void main()
{
//    // --- Phong Lighting Model 1 ---
//    // 1. Ambient
//    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, vsTexCoord));
//
//    // 2. Diffuse
//    vec3 norm = normalize(vsNormal);  // 단위벡터로 만들기 위해 정규화
//    vec3 lightDir = normalize(-dirLight.direction);  // 광원을 향하는 벡터 구하고 정규화
//    float diff = max(dot(norm, lightDir), 0.0);  // 내적을 통해 밝기 계산 음수값이 나올 경우, 0으로 만들기 위해 max()
//    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));
//
//    // 3. Specular
//    vec3 viewDir = normalize(viewPos - vsPos); // 카메라를 향하는 벡터
//    vec3 reflectDir = reflect(-lightDir, norm); // 표면에 반사된 광원 벡터
//    // 표면에 반사된 광원 벡터를 구하는 방법: reflect(광원을 향하는 벡터, 노멀벡터)
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, vsTexCoord));
//
//    // --- 최종 조명 연산 ---
//    vec3 result = ambient + diffuse + specular;
//    // 최종 출력 코드
//    fragColor = vec4(result, 1.0);


//    // --- Phong Lighting Model 2 ---
//    float dist = length(pointLights[0].position - vsPos);
//    float attenuation = 1.0 / (1.0+pointLights[0].c1*dist + pointLights[0].c2*dist*dist);
//
//    // 1. Ambient
//    vec3 ambient = pointLights[0].ambient * vec3(texture(material.diffuse, vsTexCoord));
//
//    // 2. Diffuse
//    vec3 norm = normalize(vsNormal);  // 단위벡터로 만들기 위해 정규화
//    vec3 lightDir = normalize(pointLights[0].position - vsPos);  // 광원을 향하는 벡터 구하고 정규화
//    float diff = max(dot(norm, lightDir), 0.0);  // 내적을 통해 밝기 계산 음수값이 나올 경우, 0으로 만들기 위해 max()
//    vec3 diffuse = pointLights[0].diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));
//
//    // 3. Specular
//    vec3 viewDir = normalize(viewPos - vsPos); // 카메라를 향하는 벡터
//    vec3 reflectDir = reflect(-lightDir, norm); // 표면에 반사된 광원 벡터
//    // 표면에 반사된 광원 벡터를 구하는 방법: reflect(광원을 향하는 벡터, 노멀벡터)
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = pointLights[0].specular * spec * vec3(texture(material.specular, vsTexCoord));
//
//    // --- 최종 조명 연산 ---
//    vec3 result = (ambient + diffuse + specular) * attenuation;
//    // 최종 출력 코드
//    fragColor = vec4(result, 1.0);


    // --- Phong Lighting Model 3 ---
    vec3 lightDir = normalize(spotLight.position - vsPos);  // 광원을 향하는 벡터 구하고 정규화
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
    float dist = length(spotLight.position - vsPos);
    float attenuation = 1.0 / (1.0+spotLight.c1*dist + spotLight.c2*dist*dist);

    // 1. Ambient
    vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, vsTexCoord));

    // 2. Diffuse
    vec3 norm = normalize(vsNormal);  // 단위벡터로 만들기 위해 정규화
    float diff = max(dot(norm, lightDir), 0.0);  // 내적을 통해 밝기 계산 음수값이 나올 경우, 0으로 만들기 위해 max()
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));
    diffuse *= intensity; // ambient값은 건들지 않는다, 최소한의 빛은 유지하기 위해서

    // 3. Specular
    vec3 viewDir = normalize(viewPos - vsPos); // 카메라를 향하는 벡터
    vec3 reflectDir = reflect(-lightDir, norm); // 표면에 반사된 광원 벡터
    // 표면에 반사된 광원 벡터를 구하는 방법: reflect(광원을 향하는 벡터, 노멀벡터)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, vsTexCoord));
    specular *= intensity; // ambient값은 건들지 않는다, 최소한의 빛은 유지하기 위해서

    vec3 result = ( ambient + diffuse + specular ) * attenuation;
    // 최종 출력 코드
    fragColor = vec4(result, 1.0);
} 