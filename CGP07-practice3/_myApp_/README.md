# 1단계
## 2단계
### 3단계
VAO는 오브젝트당

VBO는 데이터당

오브젝트 렌더링 시 "프로그램 선택 -> 객체 갱신 -> 텍스터 바인드 -> VAO 바인드 -> 그리기"

함수 이름이 gl...() => CPU 코드

vec4, mat4, uniform, out, in 키워드 => GLSL 코드

```
rendering_program = compile_shader();
// 텍스처 적용 여부가 다를 때, 물체의 정점 구조가 다를 때
```

## 행렬 in OpenGL
```
mat4 m1; // 4x4 행렬
m1[0] = vec4(1.4,2.0,3.0,1.0); 
// 열 하나 선택해서 벡터 대입
m1[2][3] = 2.5; // [열][행]
```
## 지오메트리 변환
### Translation
```
vmath::translate(tx,ty,tz);
```
### Rotation
```
vmath::rotate(angle, x, y, z);
```
### Scaling
```
vmath::scale(sx, sy, sz);
```

## 좌표 변환
### projection
```
mat4 vamth::orth(float left,float right, float bottom,float top, float near,float far)

vmath::mat4 orthM = vamth::orth(-50,-50,-20,20,-1,-100);
```
```
mat4 vmath::frustum(float left,float right,float bottom,float top,float near,float far)
										
mat4 perspM = vmath::frustum(-20,20,-12,12,-10,-100);
```
```
mat4 vmath::perspective(float fovy,float aspect,
												float near,float far)
												
mat4 perspM = vmath::perspective(50,1.78,-10,-100)
```
### view(lookat)
```
// lookat 함수: 4x4 뷰 메트릭스를 만들어준다
mat4 vmath::lookat(vec3 eye,vec3 target,vec3 up)
// (카메라위치, 바라보는 점, 대충 업 방향)

vmath::vec3 eye(5.0, 2.0, 3.0);
vmath::vec3 center(0.0, 0.0, 0.0);
vmath::vec3 up(0.0, 1.0, 0.0);

vmath::mat4 perspM = vmath::lookat(eye, center, up);
```
### model-world
지오메트리 변환(T, R, S)과 같음