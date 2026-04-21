# 3장_파이프라인따라가기
VAO는 오브젝트당, VBO는 데이터당

오브젝트 렌더링 시 "프로그램 선택 -> 객체 갱신 -> texture 바인드 -> VAO 바인드 -> 그리기"

함수 이름이 gl...() => CPU 코드

vec4, mat4, uniform, out, in 키워드 => GLSL 코드
```
rendering_program = compile_shader();
// 텍스처 적용 여부가 다를 때, 물체의 정점 구조가 다를 때
```

# 4장_3D그래픽스를위한수학_part1, part2
## 행렬 in OpenGL
```
mat4 m1; // 4x4 행렬
m1[0] = vec4(1.4,2.0,3.0,1.0); 
// 열 하나 선택해서 벡터 대입
m1[2][3] = 2.5; // [열][행]

// 직접 값 넣기 (열 우선)
// vmath::vec4 4개를 써서 각 세로 기둥(열)을 채운다
vmath::mat4 Matrix = vmath::mat4(
	vmath::vec4(1.0f, 0.0f, 0.0f, 0.0f), // 1열(x축)
	vmath::vec4(1.0f, 1.0f, 0.0f, 0.0f), // 2열(y축)
	vmath::vec4(0.0f, 0.0f, 1.0f, 0.0f), // 3열(z축)
	vmath::vec4(0.5f, 0.2f, 0.0f, 1.0f)  // 4열(이동량: tx,ty,tz)
);
```
## 지오메트리 변환
```
vmath::translate(tx,ty,tz); // translation
vmath::rotate(angle, x, y, z); // rotation
vmath::scale(sx, sy, sz); // scaling

ex)
vmath::mat4 M1 = vmath::translate(30,15.6,-19) * 
		vmath::rotate(45,1.0,0.0,0.0) * 
		vmath::scale(2.0,1.0,0.5);
vmath::vec4 v2 = M1 * v1;
```

## 좌표 변환
### 프로젝션 변환: Projection matrix
```
// 정사영
mat4 vamth::orth(float left, float right, float bottom, float top, float near, float far)
ex)
vmath::mat4 orthM = vamth::orth(-50,-50,-20,20,-1,-100);
```
```
// 원근투영
mat4 vmath::frustum(float left, float right, float bottom, float top, float near, float far)
ex)										
vmath::mat4 perspM = vmath::frustum(-20,20,-12,12,-10,-100);

mat4 vmath::perspective(float fovy,float aspect,float near,float far)
// (세로화각, 화면비, near, far)
ex)												
vmath::mat4 perspM = vmath::perspective(50,1.78,-10,-100)
```
### 월드-뷰 변환: View(LookAt) matrix
```
// lookat 함수: 4x4 뷰 메트릭스를 만들어준다
mat4 vmath::lookat(vec3 eye,vec3 target,vec3 up)
// (카메라위치, 바라보는 점, 대충 업 방향)
// eye=카메라 위치
// 위에서 내려다보기 -> y축 조절, 비스듬히 보기 -> x, z축 조절
// center=바라보는 초점
// up=카메라의 정수리, 보통 (0,1,0)으로 고정
ex)
vmath::vec3 eye(5.0, 2.0, 3.0);
vmath::vec3 center(0.0, 0.0, 0.0);
vmath::vec3 up(0.0, 1.0, 0.0);
vmath::mat4 perspM = vmath::lookat(eye, center, up);
```
### 모델-월드 변환: Model matrix
지오메트리 변환(T, R, S)과 같음

# 5장_데이터1(버퍼, 유니폼)
## 쉐이더 프로그램에 데이터를 전달하는 방법
### VBO
```
// VBO를 생성하여 vertices 값들을 복사 //

// 객체 고유 ID를 받기 위해 GLuint로 변수 생성
GLuint VBO;
// 생성
glGenBuffers(1, &VBO); (버퍼 개수, 변수 주소)
// 바인드
glBindBuffer(GL_ARRAY_BUFFER, VBO); (타겟 데이터 타입, 변수 객체 ID)
// 데이터 복사
glBufferData(
		GL_ARRAY_BUFFER,  // 타겟하는 Buffer Object 타입
		sizeof(vertices), // 데이터 사이즈 (Byte)
		vertices,         // 넘길 데이터 배열
 		GL_STATIC_DRAW    // 버퍼 사용 목적
)
```
버퍼사용목적: GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
### Vertex Attribute
```
layout (location=0) in vec3 pos;
...
glVertexAttribPointer(
		0, // layout location 인덱스
		3, // 속성 사이즈 (vec3)
		GL_FLOAT, // 데이터 타입
		GL_FALSE, // 값 정규화 수행 여부 (정수형일 때)
		3 * sizeof(float), // 속성 간 몇 Byte 이동할지
		(void*)(0 * sizeof(float)) // 데이터 시작 위치 오프셋
);
glEnableVertexAttribArray(0);
// 0 : layout location 인덱스
```

### Uniform 변수
GPU 전역변수, GLSL의 다양한 타입을 지원 (float, int, vec3, vec4, mat4, …)

CPU에서 GPU로 데이터 보내기
```
glUniformMatrix4fv(glGetUniformLocation(rendering_program, "rotMat"),1,GL_FALSE,rm);
// 데이터를 넘길 uniform location 지정,
// 넘기는 매트릭스 개수,
// Transpose 수행 여부,
// vmath를 통해 만든 mat4 변수
...
uniform mat4 rotMat;
```
'glUniform' 함수들
glUniform1i(location, 1);
glUniform1f(location, 0.5f);

활성화된 프로그램의 특정 변수에 값을 대입하는 함수

쉐이더(GPU) 코드랑 헷갈리지 않는 법
• **함수 이름이 gl로 시작** → C++ (CPU) 코드
• **vec4, mat4, out, in 같은 키워드 →** GLSL (GPU) 쉐이더 파일 내부 코드

cpp에서 vec4 사용하려면 vmath:: 앞에 붙여야 함

"한 번만 정해두면 되는 데이터"와 "계속 변해야 하는 데이터"의 차이
1. startup( ) : 준비 단계
startup( )은 프로그램이 시작될 때 딱 한 번 실행. 
여기에는 "**변하지 않는 고정 정보**"를 넣음

**정점 위치/색상 배열:** 삼각형의 설계도. "삼각형은 이렇게 생겼고 색은 이래"라는 정보는 한 번 GPU 메모리(VBO)에 복사해두면 지우기 전까지 계속 거기 남아있음.

2. render( ) : 실행 단계
render( )는 화면을 그릴 때마다 계속 호출. 
여기에는 "**시간에 따라 변하는 정보**"를 넣음
**회전 행렬:** 회전 행렬은 보통 currentTime이나 time 변수를 사용해 계산됨. 0.01초마다 각도가 조금씩 커져야 물체가 돌아가는 것처럼 보임

**VBO**는 GPU 메모리에 저장된 데이터 덩어리 (재료)
데이터 종류별로 여러 개
glGenBuffers()
glBindBuffer()
glBufferData()

**VAO**는 데이터 자체가 아니라 VBO에 담긴 데이터를 어떻게 읽어야 하는지에 대한 설정값 (레시피)
오브젝트당 하나
glVertexAttribPointer()
glEnableVertexAttribArray()

만약 프로그램에 삼각형도 있고 사각형도 있다면 
삼각형을 그릴 때에는 삼각형용 VAO를 바인딩하고 그린다
그 다음에 사각형을 그릴 때에는 사각형용 VAO로 갈아 끼워야 한다.

물체를 그릴 때는 반드시 **[1. 프로그램 선택 -> 2. 변수 갱신 -> 3. VAO 바인딩 -> 4. 그리기]** 순서를 각 물체마다 따로 진행해야 함

: glUseProgram() → glBindVertexArray() → glDrawArrays()

uniform과 VBO의 차이점

uniform: 전체 같은 값 공유
VBO: 개별 다른 값 할당

startup()에서 VAO, VBO 객체 생성

render()에서 VAO 선택해서 렌더링

반시계 방향

# 5장_데이터2(엘리먼트버퍼,텍스처)
### EBO
```
// EBO를 생성하여 indices 값들을 복사
GLuint EBO;
glGenBuffers(1, &EBO); 
// (개수, ID)
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// (타겟, ID)
glBufferData(GL_ELEMENT_ARRAY_BUFFER,  // 타겟
			  sizeof(indices),         // byte크기
			  indices,                 // 데이터
			  GL_STATIC_DRAW           // 목적
			); 
```
### Texture Wrapping
: 텍스처 좌표가 (0,0) ~ (1,0)이 아닐 경우 ‘경계를 넘어갈 때’ 처리 방법

### Texture Filtering
텍스처로부터 픽셀 값(컬러)을 읽어오는 방법을 정의한다

실제 렌더링 픽셀과 텍스처 픽셀이 정확히 일치하지 않기 때문
렌더링 오브젝트 크기 > 텍스처 크기 = 가까이 있을 때
: 텍스처 이미지가 확대됨
렌더링 오브젝트 크기 < 텍스처 크기 = 멀리 있을 때 
: 텍스처 이미지가 축소됨

### Mipmaps
다양한 크기의 텍스처 이미지를 거리에 따라 사용하여 해결
거리가 멀어지면 더 작은 크기의 텍스처 이미지 사용
```
//시작은 지금까지 했던 openGL객체(VAO,VBO,EBO)와 유사
GLuint texture;
glGenTextures(1, &texture); 
// (개수, ID)
glBindTexture(GL_TEXTURE_2D, texture);
// (타겟 텍스처 오브젝트 타입, ID)

//이미지 데이터 복사해서 텍스처 만들기
glTexImage2D(GL_TEXTURE_2D, // 타켓 텍스처
			0,            // 밈맵 레벨
			GL_RGB,       // 텍스처 저장할 픽셀 포멧
			width,        // 가로 해상도
			height,       // 세로 해상도
			0,
			GL_RGB,       // 로드한 이미지 데이터 포멧
			GL_UNSIGNED_BYTE, // 이미지 데이터 포멧
			data          // 실제 이미지 데이터
			);
```

```
#version 430 core
...
in vec2 vsTexCoord; // 인터폴레이션된 픽셀 좌표
uniform sampler2D texture1; // 만들어서 바인딩한 텍스처이미지
// 데이터 타입: uniform sampler2D
out vec4 fragColor;

void main(void) {
	fragColor = texture(texture1, vsTexCoord);
	// 실제 픽셀값 vec4 (RGBA)를 가져오는 함수
	// (픽셀값을 가져올 텍스처, 텍스처에서 픽셀값을 가져올 위치)
}
```

glActiveTexture()
glBindTexture()는 한 쌍으로 작성할 것

## Texture Wrapping
텍스처 좌표가 (0,0)~(1,0)를 넘어갔을 때, 남는 빈 공간을 어떻게 채울 것인가

```
GL_REPEAT = 반복
GL_MIRRORED_REPEAT = 거울상 반복
GL_CLAMP_TO_EDGE = 모서리 늘리기
GL_CLAMP_TO_BORDER = 테두리 색상 채우기

glTexParameteri(GL_TEXTURE_2D,     // 타겟 텍스처
		GL_TEXTURE_WRAP_S, // 설정할 축
		GL_MIRRORED_REPEAT // Wrapping모드
);
								
// GL_CLAMP_TO_BORDER인 경우,
// glTexParameterfv()로 보더 색상을 명시해야 함
float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
glTexParameterfv(GL_TEXTURE_2D, 
		GL_TEXTURE_BORDER_COLOR, 
		bordercolor // 4개의 요소(RGBA)를 가진 float 배열
);
```

## Texture Filtering
실제 렌더링 픽셀과 텍스처 픽셀이 정확히 일치하지 않기 때문

렌더링 오브젝트 크기 > 텍스처 크기 = 가까이 있을 때
: 텍스처 이미지가 확대됨
렌더링 오브젝트 크기 < 텍스처 크기 = 멀리 있을 때 
: 텍스처 이미지가 축소됨
```
GL_NEAREST: 가장 가까운 픽셀 값을 그대로 가져옴
GL_LINEAR: 주변 4개 픽셀을 이용해서 최종 픽셀 값을 계산함

glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
// ( , 확대/축소 동작 설정, Filtering 옵션 지정)
```

