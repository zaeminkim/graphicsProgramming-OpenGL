#version 430 core                
in vec2 vsTexCoord;
in vec3 vsNormal;
in vec3 vsPos;

uniform sampler2D texIndex1;
uniform sampler2D texIndex2;

uniform vec3 lightPos;    // 광원의 위치
uniform vec3 viewPos;     // 카메라의 위치(눈 위치)
uniform vec3 lightColor;  // 광원의 색
uniform vec3 objectColor; // 반사하는 정도

out vec4 fragColor; 

void main(void)     
{
    vec4 tex1 = texture(texIndex1, vsTexCoord);
    vec4 tex2 = texture(texIndex2, vsTexCoord);

    vec4 texColor;
    if (tex1.r < 0.9 || tex1.g < 0.9 || tex1.b < 0.9) {
        texColor = tex1;
    } else {
        texColor = tex2;
    }

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
    int shininess = 32;
    vec3 viewDir = normalize(viewPos - vsPos); // 카메라를 향하는 벡터
    vec3 reflectDir = reflect(-lightDir, norm); // 표면에 반사된 광원 벡터
    // 표면에 반사된 광원 벡터를 구하는 방법: reflect(광원을 향하는 벡터, 노멀벡터)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // --- 최종 조명 연산 ---
    vec3 result = (ambient + diffuse + specular) * objectColor;
    
    // 만약 텍스처를 살리고 싶다면 아래 주석을 해제하고, 바로 밑의 코드를 주석 처리
    // fragColor = vec4(result * texColor.rgb, texColor.a);
    
    // 텍스처 없는 최종 출력 코드
    fragColor = vec4(result, 1.0);
}