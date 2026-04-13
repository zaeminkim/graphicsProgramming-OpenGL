#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

class my_application : public sb7::application
{
public:
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = sb7::shader::load("box_vs.glsl", GL_VERTEX_SHADER);
		fragment_shader = sb7::shader::load("box_fs.glsl", GL_FRAGMENT_SHADER);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	virtual void startup()
	{
		stbi_set_flip_vertically_on_load(true); // 텍스처 이미지의 상하 맞추기

		rendering_program = compile_shaders();

		// VAO 객체 생성 및 바인드
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenTextures(6, texture);


		// ======================================== VAO 바인드 ====================================== //
		glBindVertexArray(VAO); // 정육면체

		// 버텍스와 텍스처 버텍스 배열 선언
		GLfloat vertices[] = {
			// 버텍스 4개 = 정육면체 한 면
			// 정육면체 뒷면은 다 똑같은데 시계방향으로 버텍스 정의하면 됨

			// near
			-0.25f, 0.25f, 0.25f,   0.0f, 1.0f,   // 인덱스 0
			-0.25f, -0.25f, 0.25f,  0.0f, 0.0f,   // 인덱스 1
			0.25f, -0.25f, 0.25f,   1.0f, 0.0f,   // 인덱스 2
			0.25f, 0.25f, 0.25f,    1.0f, 1.0f,   // 인덱스 3
			// far
			0.25f, 0.25f, -0.25f,  0.0f, 1.0f,    // 인덱스 4
			0.25f, -0.25f, -0.25f, 0.0f, 0.0f,    // 인덱스 5
			-0.25f, -0.25f, -0.25f,  1.0f, 0.0f,  // 인덱스 6
			-0.25f, 0.25f, -0.25f,   1.0f, 1.0f,  // 인덱스 7
			// right
			0.25f, 0.25f, 0.25f,  0.0f, 1.0f,     // 인덱스 8
			0.25f, -0.25f, 0.25f, 0.0f, 0.0f,     // 인덱스 9
			0.25f, -0.25f, -0.25f,  1.0f, 0.0f,   // 인덱스 10
			0.25f, 0.25f, -0.25f,   1.0f, 1.0f,   // 인덱스 11
			// left
			-0.25f, 0.25f, -0.25f,   0.0f, 1.0f,  // 인덱스 12
			-0.25f, -0.25f, -0.25f,  0.0f, 0.0f,  // 인덱스 13
			-0.25f, -0.25f, 0.25f,   1.0f, 0.0f,  // 인덱스 14
			-0.25f, 0.25f, 0.25f,    1.0f, 1.0f,  // 인덱스 15
			// top
			-0.25f, 0.25f, -0.25f,   0.0f, 1.0f,  // 인덱스 16
			-0.25f, 0.25f, 0.25f,  0.0f, 0.0f,    // 인덱스 17
			0.25f, 0.25f, 0.25f,   1.0f, 0.0f,    // 인덱스 18
			0.25f, 0.25f, -0.25f,    1.0f, 1.0f,  // 인덱스 19
			// bottom
			-0.25f, -0.25f, -0.25f,   0.0f, 1.0f, // 인덱스 20
			0.25f, -0.25f, -0.25f,  0.0f, 0.0f,   // 인덱스 21
			0.25f, -0.25f, 0.25f,   1.0f, 0.0f,   // 인덱스 22
			-0.25f, -0.25f, 0.25f,    1.0f, 1.0f, // 인덱스 23
		};

		// 버텍스 순서: 반시계 방향
		GLuint indices[] = {
			// 면1 (near)
			0, 1, 3,
			3, 1, 2,
			// 면2 (far)
			7, 4, 6,
			4, 5, 6,
			// 면3 (right)
			8, 9, 11,
			11, 9, 10,
			// 면4 (left)
			15, 12, 14,
			12, 13, 14,
			// 면5 (top)
			16, 17, 19,
			19, 17, 18,
			// 면6 (bottom)
			20, 21, 23,
			21, 22, 23
		};

		// ========================================= VBO 바인드 ====================================== //
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// VBO를 VAO의 Vertex Attributes로 연결
		// location = 0 -> pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// location = 1 -> texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// ========================================= EBO 바인드 ========================================== //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// ====================================== texture[0] 바인드 ===================================== //
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		// 이미지의 픽셀 데이터를 배열로 저장
		int width0, height0, nrChannels0;
		unsigned char* data0 = stbi_load("side1.jpg", &width0, &height0, &nrChannels0, 0);

		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data0) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width0, height0, 0, GL_RGB, GL_UNSIGNED_BYTE, data0);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data0);

		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// ====================================== texture[1] 바인드 ===================================== //
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		// 이미지의 픽셀 데이터를 배열로 저장
		int width1, height1, nrChannels1;
		unsigned char* data1 = stbi_load("side2.jpg", &width1, &height1, &nrChannels1, 0);

		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data1) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data1);

		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// ====================================== texture[2] 바인드 ===================================== //
		glBindTexture(GL_TEXTURE_2D, texture[2]);

		// 이미지의 픽셀 데이터를 배열로 저장
		int width2, height2, nrChannels2;
		unsigned char* data2 = stbi_load("side3.jpg", &width2, &height2, &nrChannels2, 0);

		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data2) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data2);

		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// ====================================== texture[3] 바인드 ===================================== //
		glBindTexture(GL_TEXTURE_2D, texture[3]);

		// 이미지의 픽셀 데이터를 배열로 저장
		int width3, height3, nrChannels3;
		unsigned char* data3 = stbi_load("side4.jpg", &width3, &height3, &nrChannels3, 0);

		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data3);

		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// ====================================== texture[4] 바인드 ===================================== //
		glBindTexture(GL_TEXTURE_2D, texture[4]);

		// 이미지의 픽셀 데이터를 배열로 저장
		int width4, height4, nrChannels4;
		unsigned char* data4 = stbi_load("side5.jpg", &width4, &height4, &nrChannels4, 0);

		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width4, height4, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data4);

		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// ====================================== texture[5] 바인드 ===================================== //
		glBindTexture(GL_TEXTURE_2D, texture[5]);

		// 이미지의 픽셀 데이터를 배열로 저장
		int width5, height5, nrChannels5;
		unsigned char* data5 = stbi_load("side6.jpg", &width5, &height5, &nrChannels5, 0);

		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data5) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width5, height5, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data5);

		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteTextures(6, texture);
		glDeleteProgram(rendering_program);
	}

	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// 회전 메트릭스를 위한 작업
		float angle = currentTime * 100;
		vmath::mat4 rm = vmath::rotate(angle, 0.0f, 0.0f, 1.0f);
		GLint rotMatLocation;

		// 이동 메트릭스를 위한 작업
		vmath::mat4 tm = vmath::translate((float)sin(currentTime), 0.0f, float(cos(currentTime)));
		GLint transMatLocation;

		// 뷰 메트릭스를 위한 작업
		vmath::vec3 eye(1.6, 1.3, 2.0);                  // eye=카메라 위치, 위에서 내려다보기 -> y축 조절, 비스듬히 보기 -> x, z축 조절
		vmath::vec3 center(0.0, 0.0, 0.0);               // center=바라보는 초점
		vmath::vec3 up(0.0, 1.0, 0.0);                   // up=카메라의 정수리, 보통 (0,1,0)으로 고정
		vmath::mat4 vm = vmath::lookat(eye, center, up);
		GLint viewMatLocation;

		// 프로젝션 메트릭스를 위한 작업
		// 세로 화각=50도, 화면 비=가로/세로, Near, Far
		vmath::mat4 pm = vmath::perspective(50.0f, (float)(info.windowWidth) / (float)(info.windowHeight), 0.1f, 1000.0f);
		GLint projMatLocation;

		// ================= 정육면체 그리기 (rendering_program + VAO + texture[6]) ===================== //
		// 
		// =================================== 프로그램 객체 사용 =============================================//
		glUseProgram(rendering_program);

		// 회전 메트릭스
		rotMatLocation = glGetUniformLocation(rendering_program, "rotMat");
		glUniformMatrix4fv(rotMatLocation, 1, GL_FALSE, rm);

		// 이동 메트릭스
		transMatLocation = glGetUniformLocation(rendering_program, "transMat");
		glUniformMatrix4fv(transMatLocation, 1, GL_FALSE, tm);

		// 뷰 메트릭스
		viewMatLocation = glGetUniformLocation(rendering_program, "viewMat");
		glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, vm);

		// 프로젝션 메트릭스
		projMatLocation = glGetUniformLocation(rendering_program, "projMat");
		glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, pm);


		// ========================================= VAO 바인드 ============================================= //
		glBindVertexArray(VAO);

		// ======================================== texture 바인드 ======================================= //
		glUniform1i(glGetUniformLocation(rendering_program, "texIndex"), 0);
		glActiveTexture(GL_TEXTURE0);

		// 면1 near (indices 0..5) "숫자 1"
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));

		// 면2 far (indices 6..11) "숫자 6"
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

		// 면3 right (indices 12..17) "숫자 2"
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));

		// 면4 left (indices 18..23) "숫자 5"
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(GLuint)));

		// 면5 top (indices 24..29) "숫자 3"
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));

		// 면6 bottom (indices 30..35) "숫자 4"
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)));
	}

private:
	GLuint rendering_program;
	GLuint VAO, VBO, EBO;
	GLuint texture[6];
};

DECLARE_MAIN(my_application)