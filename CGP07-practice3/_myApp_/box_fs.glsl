#version 430 core                
in vec2 vsTexCoord;
uniform sampler2D texIndex;

out vec4 fragColor; 

void main(void)     
{                   
    // 텍스처의 원래 색상
    vec4 texColor = texture(texIndex, vsTexCoord);

    // 테두리 설정
    float borderWidth = 0.08; // 테두리 두께
    vec4 borderColor = vec4(0.5, 0.4, 0.3, 1.0); // 짙은 갈색 (R, G, B, Alpha)

    // 3. 현재 픽셀이 상하좌우 가장자리(테두리 영역)에 있는지 검사
    if (vsTexCoord.x < borderWidth || vsTexCoord.x > 1.0 - borderWidth ||
        vsTexCoord.y < borderWidth || vsTexCoord.y > 1.0 - borderWidth)
    {
        // 가장자리에 속하면 테두리 색상으로 덮어쓰기
        fragColor = borderColor;
    }
    else
    {
        // 안쪽 영역이면 원래 텍스처 색상 유지
        fragColor = texColor;
    }
}                   