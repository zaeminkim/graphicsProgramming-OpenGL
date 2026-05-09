#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class my_application : public sb7::application
{
public:
	GLuint compile_shaders1(void)
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

	GLuint compile_shaders2(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = sb7::shader::load("ground_vs.glsl", GL_VERTEX_SHADER);
		fragment_shader = sb7::shader::load("ground_fs.glsl", GL_FRAGMENT_SHADER);

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

		rendering_program1 = compile_shaders1();
		rendering_program2 = compile_shaders2();

		// VAO 객체 생성 및 바인드
		glGenVertexArrays(2, VAO);
		glGenBuffers(2, VBO);
		glGenBuffers(2, EBO);
		glGenTextures(1, &texture);

		// ======================================== VAO[0] 바인드 ====================================== //
		glBindVertexArray(VAO[0]); // 정육면체

		// 버텍스, 노멀벡터 정의
		GLfloat vertices[] = {
			// near (normal: 0, 0, 1)
			-0.25f, 0.25f, 0.25f,   0.0f, 0.0f, 1.0f,
			-0.25f, -0.25f, 0.25f,  0.0f, 0.0f, 1.0f,
			0.25f, -0.25f, 0.25f,   0.0f, 0.0f, 1.0f,
			0.25f, 0.25f, 0.25f,    0.0f, 0.0f, 1.0f,
			// far (normal: 0, 0, -1)
			0.25f, 0.25f, -0.25f,   0.0f, 0.0f, -1.0f,
			0.25f, -0.25f, -0.25f,  0.0f, 0.0f, -1.0f,
			-0.25f, -0.25f, -0.25f, 0.0f, 0.0f, -1.0f,
			-0.25f, 0.25f, -0.25f,  0.0f, 0.0f, -1.0f,
			// right (normal: 1, 0, 0)
			0.25f, 0.25f, 0.25f,    1.0f, 0.0f, 0.0f,
			0.25f, -0.25f, 0.25f,   1.0f, 0.0f, 0.0f,
			0.25f, -0.25f, -0.25f,  1.0f, 0.0f, 0.0f,
			0.25f, 0.25f, -0.25f,   1.0f, 0.0f, 0.0f,
			// left (normal: -1, 0, 0)
			-0.25f, 0.25f, -0.25f,  -1.0f, 0.0f, 0.0f,
			-0.25f, -0.25f, -0.25f, -1.0f, 0.0f, 0.0f,
			-0.25f, -0.25f, 0.25f,  -1.0f, 0.0f, 0.0f,
			-0.25f, 0.25f, 0.25f,   -1.0f, 0.0f, 0.0f,
			// top (normal: 0, 1, 0)
			-0.25f, 0.25f, -0.25f,  0.0f, 1.0f, 0.0f,
			-0.25f, 0.25f, 0.25f,   0.0f, 1.0f, 0.0f,
			0.25f, 0.25f, 0.25f,    0.0f, 1.0f, 0.0f,
			0.25f, 0.25f, -0.25f,   0.0f, 1.0f, 0.0f,
			// bottom (normal: 0, -1, 0)
			-0.25f, -0.25f, -0.25f, 0.0f, -1.0f, 0.0f,
			0.25f, -0.25f, -0.25f,  0.0f, -1.0f, 0.0f,
			0.25f, -0.25f, 0.25f,   0.0f, -1.0f, 0.0f,
			-0.25f, -0.25f, 0.25f,  0.0f, -1.0f, 0.0f
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

		// ========================================= VBO[0] 바인드 ====================================== //
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// VBO를 VAO의 Vertex Attributes로 연결
		// location = 0 -> pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// location = 1 -> normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// ========================================= EBO[0] 바인드 ========================================== //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// ======================================== VAO[1] 바인드 ====================================== //
		glBindVertexArray(VAO[1]); // 배경 정사각형

		// 버텍스, 노멀벡터 정의
		GLfloat vertices2[] = {
			// bottom (0~3) => 텍스처 "wall.jpg"
			-2.0f, -0.25f, 2.0f,  0.0f, 0.0f,
			2.0f, -0.25f, 2.0f,	  4.0f, 0.0f,
			2.0f, -0.25f, -2.0f,  4.0f, 4.0f,
			-2.0f, -0.25f, -2.0f, 0.0f, 4.0f
		};

		// 버텍스 순서: 반시계 방향
		GLuint indices2[] = {
			// bottom
			0, 1, 2, 0, 2, 3
		};

		// ========================================= VBO[1] 바인드 ====================================== //
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		// VBO를 VAO의 Vertex Attributes로 연결
		// location = 0 -> pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);
		// location = 1 -> texcoord
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// ========================================= EBO[1] 바인드 ========================================== //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

		// ====================================== texture 바인드 ============================================ //
		glBindTexture(GL_TEXTURE_2D, texture);
		int width, height, nrChannels;
		unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(2, VBO);
		glDeleteBuffers(2, EBO);
		glDeleteProgram(rendering_program1);
		glDeleteProgram(rendering_program2);
	}

	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		float angle = currentTime * 100;

		// 뷰 메트릭스를 위한 작업
		float radius = 2.0f; // 멀리서 바라보기
		vmath::vec3 eye(radius * (float)cos(currentTime * 0.5f), 0.6f, radius * (float)sin(currentTime * 0.5f));   // eye=카메라 위치, 위에서 내려다보기 -> y축 조절, 비스듬히 보기 -> x, z축 조절
		vmath::vec3 center(0.0, 0.0, 0.0);               // center=바라보는 초점
		vmath::vec3 up(0.0, 1.0, 0.0);                   // up=카메라의 정수리, 보통 (0,1,0)으로 고정
		vmath::mat4 vm = vmath::lookat(eye, center, up);

		// 프로젝션 메트릭스를 위한 작업
		// 세로 화각=50도, 화면 비=가로/세로, Near, Far
		vmath::mat4 pm = vmath::perspective(50.0f, (float)(info.windowWidth) / (float)(info.windowHeight), 0.1f, 1000.0f);

		// ================= 정육면체1 그리기 (rendering_program1 + VAO[0]) ===================== //
		glUseProgram(rendering_program1);

		// 회전 메트릭스 값
		vmath::mat4 rm1 = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);

		// 이동 메트릭스 값
		vmath::mat4 tm1 = vmath::translate(-1.0f, 0.0f, 0.0f);

		// 회전 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "rotMat"), 1, GL_FALSE, rm1);

		// 이동 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "transMat"), 1, GL_FALSE, tm1);

		// 뷰 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "viewMat"), 1, GL_FALSE, vm);

		// 프로젝션 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "projMat"), 1, GL_FALSE, pm);

		vmath::vec3 lightColor1(1.0f, 1.0f, 1.0f);
		vmath::vec3 boxColor1(1.0f, 0.5f, 0.0f);
		vmath::vec3 lightPos1(1.2f, 1.0f, 2.0f);
		vmath::vec3 viewPos1 = eye;
		float shininess1 = 64;

		glUniform3fv(glGetUniformLocation(rendering_program1, "lightColor"), 1, lightColor1);
		glUniform3fv(glGetUniformLocation(rendering_program1, "objectColor"), 1, boxColor1);
		glUniform3fv(glGetUniformLocation(rendering_program1, "lightPos"), 1, lightPos1);
		glUniform3fv(glGetUniformLocation(rendering_program1, "viewPos"), 1, viewPos1);
		glUniform1f(glGetUniformLocation(rendering_program1, "shininess"), shininess1);

		// ========================================= VAO 바인드 ============================================= //
		glBindVertexArray(VAO[0]);

		// ========================================= drawElements() ============================================= //
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

		// ================= 정육면체2 그리기 (rendering_program1 + VAO[0]) ===================== //
		glUseProgram(rendering_program1);

		// 회전 메트릭스 값
		vmath::mat4 rm2 = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);

		// 이동 메트릭스 값
		vmath::mat4 tm2 = vmath::translate(1.0f, 0.0f, 0.0f);

		// 회전 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "rotMat"), 1, GL_FALSE, rm2);

		// 이동 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "transMat"), 1, GL_FALSE, tm2);

		// 뷰 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "viewMat"), 1, GL_FALSE, vm);

		// 프로젝션 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "projMat"), 1, GL_FALSE, pm);

		vmath::vec3 lightColor2(1.0f, 1.0f, 1.0f);
		vmath::vec3 boxColor2(0.5f, 0.0f, 1.0f);
		vmath::vec3 lightPos2(1.2f, 1.0f, 2.0f);
		vmath::vec3 viewPos2 = eye;
		float shininess2 = 1;

		glUniform3fv(glGetUniformLocation(rendering_program1, "lightColor"), 1, lightColor2);
		glUniform3fv(glGetUniformLocation(rendering_program1, "objectColor"), 1, boxColor2);
		glUniform3fv(glGetUniformLocation(rendering_program1, "lightPos"), 1, lightPos2);
		glUniform3fv(glGetUniformLocation(rendering_program1, "viewPos"), 1, viewPos2);
		glUniform1f(glGetUniformLocation(rendering_program1, "shininess"), shininess2);

		// ========================================= VAO 바인드 ============================================= //
		glBindVertexArray(VAO[0]);

		// ========================================= drawElements() ============================================= //
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

		// ================= 배경 정사각형 그리기 (rendering_program2 + VAO[1] + texture) ===================== //
		glUseProgram(rendering_program2);

		// 뷰 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "viewMat"), 1, GL_FALSE, vm);
		// 프로젝션 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "projMat"), 1, GL_FALSE, pm);

		// 텍스처를 바인딩한다
		glUniform1i(glGetUniformLocation(rendering_program2, "texture_ground"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO[1]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	GLuint rendering_program1;
	GLuint rendering_program2;
	GLuint VAO[2], VBO[2], EBO[2], texture;
};

DECLARE_MAIN(my_application)