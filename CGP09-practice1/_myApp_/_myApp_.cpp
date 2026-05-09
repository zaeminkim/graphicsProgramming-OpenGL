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
		glGenTextures(7, texture);


		// ======================================== VAO 바인드 ====================================== //
		glBindVertexArray(VAO); // 정육면체

		// 버텍스와 텍스처 버텍스 배열 선언
		GLfloat vertices[] = {
			// near (normal: 0, 0, 1)
			-0.25f, 0.25f, 0.25f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
			-0.25f, -0.25f, 0.25f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
			0.25f, -0.25f, 0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
			0.25f, 0.25f, 0.25f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			// far (normal: 0, 0, -1)
			0.25f, 0.25f, -0.25f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
			0.25f, -0.25f, -0.25f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
			-0.25f, -0.25f, -0.25f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
			-0.25f, 0.25f, -0.25f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
			// right (normal: 1, 0, 0)
			0.25f, 0.25f, 0.25f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
			0.25f, -0.25f, 0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			0.25f, -0.25f, -0.25f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
			0.25f, 0.25f, -0.25f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
			// left (normal: -1, 0, 0)
			-0.25f, 0.25f, -0.25f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			-0.25f, -0.25f, -0.25f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			-0.25f, -0.25f, 0.25f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			-0.25f, 0.25f, 0.25f,   -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			// top (normal: 0, 1, 0)
			-0.25f, 0.25f, -0.25f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
			-0.25f, 0.25f, 0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
			0.25f, 0.25f, 0.25f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
			0.25f, 0.25f, -0.25f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
			// bottom (normal: 0, -1, 0)
			-0.25f, -0.25f, -0.25f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
			0.25f, -0.25f, -0.25f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			0.25f, -0.25f, 0.25f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			-0.25f, -0.25f, 0.25f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// location = 1 -> normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// location = 2 -> texture
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// ========================================= EBO 바인드 ========================================== //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



		// ========================= texture 배열을 이용해 바인드 하는 방법 ===================================== //
		const char* texData[] = { "side1.jpg","side2.jpg","side3.jpg","side4.jpg","side5.jpg","side6.jpg","container.jpg" };

		// 배열의 크기를 계산하여 반복 횟수를 구하기. (여기서는 7)
		int texNum = sizeof(texData) / sizeof(texData[0]);

		for (int i = 0; i < texNum; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);

			int width, height, nrChannels;
			unsigned char* data = stbi_load(texData[i], &width, &height, &nrChannels, 0);

			if (data) {
				// jpg와 png가 섞여 있는 경우
				GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			stbi_image_free(data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		// =========================================================================================================== //
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteTextures(7, texture);
		glDeleteProgram(rendering_program);
	}

	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// 회전 메트릭스를 위한 작업
		float angle = currentTime * 100;
		vmath::mat4 rm = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);
		GLint rotMatLocation;

		// 이동 메트릭스를 위한 작업
		vmath::mat4 tm = vmath::translate((float)sin(currentTime), 0.0f, (float)(cos(currentTime)));
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

		vmath::vec3 lightColor(1.0f, 1.0f, 1.0f);
		vmath::vec3 boxColor(1.0f, 0.5f, 0.31f);
		vmath::vec3 lightPos(1.2f, 1.0f, 2.0f);
		vmath::vec3 viewPos = eye;

		glUniform3fv(glGetUniformLocation(rendering_program, "lightColor"), 1, lightColor);
		glUniform3fv(glGetUniformLocation(rendering_program, "objectColor"), 1, boxColor);
		glUniform3fv(glGetUniformLocation(rendering_program, "lightPos"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(rendering_program, "viewPos"), 1, viewPos);


		// ========================================= VAO 바인드 ============================================= //
		glBindVertexArray(VAO);

		// ================================ texture 바인드 코드 수정 ========================================= //
		// 1. 공통으로 쓰이는 배경 텍스처(나무 상자)를 먼저
		glUniform1i(glGetUniformLocation(rendering_program, "texIndex2"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[6]);

		// 2. 각 면에 대응하는 숫자 텍스처의 인덱스(0~6)를 배열로 매핑
		// 순서: near(0), far(5), right(1), left(4), top(2), bottom(3)
		int faceTextures[] = { 0, 5, 1, 4, 2, 3 };

		// 3. 0번 슬롯(숫자 텍스처)을 활성화
		glUniform1i(glGetUniformLocation(rendering_program, "texIndex1"), 0);
		glActiveTexture(GL_TEXTURE0);

		// 4. 반복문으로 텍스처를 바꿔가며 6개의 면을 그리기
		for (int i = 0; i < 6; i++) {
			// 배열에서 이번 면에 맞는 텍스처 번호를 꺼내어 바인딩
			glBindTexture(GL_TEXTURE_2D, texture[faceTextures[i]]);

			// i가 0일 땐 offset 0, i가 1일 땐 offset 6, i가 2일 땐 offset 12...
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
		}
	}

private:
	GLuint rendering_program;
	GLuint VAO, VBO, EBO;
	GLuint texture[7];
};

DECLARE_MAIN(my_application)