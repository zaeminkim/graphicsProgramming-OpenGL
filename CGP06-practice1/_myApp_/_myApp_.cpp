#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include <sb7.h>
#include <vmath.h>
#include <shader.h>

class my_application : public sb7::application
{
public:
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = sb7::shader::load("pinwheel_vs.glsl", GL_VERTEX_SHADER);
		fragment_shader = sb7::shader::load("pinwheel_fs.glsl", GL_FRAGMENT_SHADER);


		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	GLuint compile_shaders2(void) {
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = sb7::shader::load("stick_vs.glsl", GL_VERTEX_SHADER);
		fragment_shader = sb7::shader::load("stick_fs.glsl", GL_FRAGMENT_SHADER);

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
		rendering_program2 = compile_shaders2();

		// VAO 객체 생성 및 바인드
		glGenVertexArrays(2, VAO);

		glBindVertexArray(VAO[0]); // 바람개비

		// 위치와 색상 값 배열 선언
		GLfloat vertices[] = {
			-0.5f,0.5f,0.5f,1.0f,0.0f,0.0f, // 1
			0.0f,0.5f,0.5f,0.0f,1.0f,0.0f,  // 2
			0.0f,0.0f,0.5f,0.0f,0.0f,1.0f,  // 3
			0.5f,0.5f,0.5f,1.0f,0.0f,0.0f,  // 4
			0.5f,0.0f,0.5f,0.0f,1.0f,0.0f,  // 5
			0.0f,0.0f,0.5f,0.0f,0.0f,1.0f,  // 6
			0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f, // 7
			0.0f,-0.5f,0.5f,0.0f,1.0f,0.0f, // 8
			0.0f,0.0f,0.5f,0.0f,0.0f,1.0f,  // 9
			-0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f,// 10
			-0.5f,0.0f,0.5f,0.0f,1.0f,0.0f, // 11
			0.0f,0.0f,0.5f,0.0f,0.0f,1.0f,  // 12
		};

		glGenBuffers(2, VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// VBO를 VAO의 Vertex Attributes로 연결
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(VAO[1]); // 막대기

		GLfloat vertices2[] = {
			-0.03, -0.8, 0.5,
			0.03, -0.8, 0.5,
			-0.03,  0.0, 0.5,
			0.03, -0.8, 0.5,
			0.03,  0.0, 0.5,
			-0.03,  0.0, 0.5,
		};

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, VAO);
		glDeleteProgram(rendering_program);
		glDeleteProgram(rendering_program2);
	}

	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);

		// ================= 막대기 그리기 (프로그램2 + VAO[1]) ======================
		// 렌더링 위해 생성한 프로그램 객체(막대기)를 사용
		glUseProgram(rendering_program2); // 막대기용 쉐이더 선택
		glBindVertexArray(VAO[1]);        // 막대기용 데이터 상자 선택
		glDrawArrays(GL_TRIANGLES, 0, 6); // 그리기


		// ================= 바람개비 그리기 (프로그램1 + VAO[1]) =====================
		// 렌더링 위해 생성한 프로그램 객체(바람개비)를 사용
		glUseProgram(rendering_program);

		float angle = currentTime * 50;
		vmath::mat4 rm = vmath::rotate(angle, 0.0f, 0.0f, 1.0f);
		GLint rotMatLocation = glGetUniformLocation(rendering_program, "rotMat");
		glUniformMatrix4fv(rotMatLocation, 1, GL_FALSE, rm);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 12);
	}

private:
	GLuint rendering_program;
	GLuint rendering_program2;
	GLuint VAO[2], VBO[2];
};

DECLARE_MAIN(my_application)