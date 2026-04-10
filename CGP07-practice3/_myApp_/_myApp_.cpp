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
		glBindVertexArray(VAO); // 정육면체

		// 위치와 색상 값 배열 선언 (24개 정점: 면당 4개)
		// 각 정점: pos(3), color(3), tex(2) -> stride = 8 floats
		GLfloat vertices[] = {
			// -- Front face (z = +0.25) --
			-0.25f,  0.25f,  0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 0 top-left
			-0.25f, -0.25f,  0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 1 bottom-left
			 0.25f, -0.25f,  0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 2 bottom-right
			 0.25f,  0.25f,  0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 3 top-right

			// -- Back face (z = -0.25) --
			 0.25f,  0.25f, -0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 4 top-left (when looking at back)
			 0.25f, -0.25f, -0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 5 bottom-left
			-0.25f, -0.25f, -0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 6 bottom-right
			-0.25f,  0.25f, -0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 7 top-right

			// -- Left face (x = -0.25) --
			-0.25f,  0.25f, -0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 8 top-left
			-0.25f, -0.25f, -0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 9 bottom-left
			-0.25f, -0.25f,  0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //10 bottom-right
			-0.25f,  0.25f,  0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, //11 top-right

			// -- Right face (x = +0.25) --
			 0.25f,  0.25f,  0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //12 top-left
			 0.25f, -0.25f,  0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //13 bottom-left
			 0.25f, -0.25f, -0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //14 bottom-right
			 0.25f,  0.25f, -0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, //15 top-right

			// -- Top face (y = +0.25) --
			-0.25f,  0.25f, -0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //16 top-left
			-0.25f,  0.25f,  0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //17 bottom-left
			 0.25f,  0.25f,  0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //18 bottom-right
			 0.25f,  0.25f, -0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, //19 top-right

			// -- Bottom face (y = -0.25) --
			-0.25f, -0.25f,  0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //20 top-left
			-0.25f, -0.25f, -0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //21 bottom-left
			 0.25f, -0.25f, -0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //22 bottom-right
			 0.25f, -0.25f,  0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f  //23 top-right
		};

		// 인덱스: 면당 2삼각형 (총 36)
		GLuint indices[] = {
			// Front
			0,1,3, 3,1,2,
			// Back
			4,5,7, 7,5,6,
			// Left
			8,9,11, 11,9,10,
			// Right
			12,13,15, 15,13,14,
			// Top
			16,17,19, 19,17,18,
			// Bottom
			20,21,23, 23,21,22
		};

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// EBO 객체 생성 및 바인드 후 indices 값 저장
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// VBO를 VAO의 Vertex Attributes로 연결
		// location = 0 -> pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// location = 1 -> color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// location = 2 -> texture
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// 텍스처 객체 생성 및 바인드
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// 이미지의 픽셀 데이터를 배열로 저장
		int width, height, nrChannels;
		unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data);

		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteProgram(rendering_program);
	}

	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);
		glEnable(GL_CULL_FACE);

		// 회전 메트릭스를 위한 작업
		float angle = currentTime * 100;
		vmath::mat4 rm = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);
		GLint rotMatLocation;

		// 이동 메트릭스를 위한 작업
		vmath::mat4 tm = vmath::translate((float)sin(currentTime), 0.0f, float(cos(currentTime)));
		GLint transMatLocation;

		// 뷰 메트릭스를 위한 작업
		vmath::vec3 eye(1.6, 1.3, 2.0);    // eye=카메라 위치, 위에서 내려다보기 -> y축 조절, 비스듬히 보기 -> x, z축 조절
		vmath::vec3 center(0.0, 0.0, 0.0); // center=바라보는 초점
		vmath::vec3 up(0.0, 1.0, 0.0);     // up=카메라의 정수리, 보통 (0,1,0)으로 고정
		vmath::mat4 vm = vmath::lookat(eye, center, up);
		GLint viewMatLocation;

		// 프로젝션 메트릭스를 위한 작업
		// 세로 화각=50도, 화면 비=가로/세로, Near, Far
		vmath::mat4 pm = vmath::perspective(50.0f, (float)(info.windowWidth) / (float)(info.windowHeight), 0.1f, 1000.0f);
		GLint projMatLocation;

		glUniform1i(glGetUniformLocation(rendering_program, "texIndex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);


		// ================= 정육면체 그리기 (rendering_program + VAO) =====================
		// 렌더링 위해 생성한 프로그램 객체를 사용
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

		// VAO 사용
		glBindVertexArray(VAO);
		// EBO를 활용해 사각형 그리기
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

private:
	GLuint rendering_program;
	GLuint VAO, VBO, EBO;
	GLuint texture;
};

DECLARE_MAIN(my_application)