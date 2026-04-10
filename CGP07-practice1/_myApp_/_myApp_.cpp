#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class my_application : public sb7::application
{
public:
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = sb7::shader::load("square_vs.glsl", GL_VERTEX_SHADER);
		fragment_shader = sb7::shader::load("square_fs.glsl", GL_FRAGMENT_SHADER);


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
		rendering_program = compile_shaders();

		// VAO 객체 생성 및 바인드
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// 사각형 네 점의 위치와 색상 값 배열 선언 + 텍스처 좌표도 추가
		GLfloat vertices[] = {
			0.25f, 0.25f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // 우측 상단
			-0.25f, 0.25f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // 좌측 상단
			-0.25f, -0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 좌측 하단
			0.25f, -0.25f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f    // 우측 하단
		};
		// 삼각형으로 그릴 인덱스값 배열 정의 (인덱스의 순서)
		GLuint indices[] = {
			0, 1, 2,    // 첫번째 삼각형
			0, 2, 3     // 두번째 삼각형
		};

		// VBO를 생성 및 바인딩 후 vertices 값들을 복사 //
		// VBO는 정점의 위치나 색상 같은 실제 데이터를 배열로 담는다
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// EBO를 생성 및 바인딩 후 indices 값들을 복사 //
		// EBO는 VBO에 있는 데이터의 인덱스 순서를 배열로 담는다
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// VBO를 VAO의 Vertex Attributes로 연결 //
		// glVertexAttribPointer()는 바인딩된 VBO가 있어야만 사용 가능
		// 위치 속성 (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 컬러 속성 (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);


		// 텍스처 좌표 속성 (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);


		// 텍스처 객체 생성 및 바인딩 //
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// 텍스처 이미지 로드 //
		int width, height, nrChannels;
		unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

		if (data) {
			// glTexImage2D(): 텍스처 객체에 이미지 데이터 복사해서 넣기
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 생성
		}
		stbi_image_free(data);    // 텍스처 다 만들고 난 뒤엔 반드시 메모리 해제

		// 텍스처 샘플링/필터링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// 언바인드 안해도 됨
		/*glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);*/
	}

	virtual void shutdown()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteTextures(1, &texture);
		glDeleteVertexArrays(1, &VAO);
		glDeleteProgram(rendering_program);
	}

	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);

		// 그리기 전에 사용하고자 하는 텍스처 바인딩하기
		glUniform1i(glGetUniformLocation(rendering_program, "texture1"), 0); // (texture1의 location, 0)
																			 // texture1의 인덱스를 0으로 지정한다
		glActiveTexture(GL_TEXTURE0);                                        // 텍스처 샘플러의 위치 0 활성화
		glBindTexture(GL_TEXTURE_2D, texture);

		// ================= 오브젝트 그리기 (프로그램 + VAO) =====================
		// 렌더링 위해 생성한 프로그램 객체를 사용
		glUseProgram(rendering_program);
		// VAO 바인딩
		glBindVertexArray(VAO);
		// EBO를 활용해 사각형을 그린다. (삼각형 2개)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	GLuint rendering_program;
	GLuint VAO, VBO, EBO, texture;
};

DECLARE_MAIN(my_application)