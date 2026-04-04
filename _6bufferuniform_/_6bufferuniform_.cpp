#include <sb7.h>
#include <vmath.h>

class my_application : public sb7::application
{
public:
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		static const GLchar* vertex_shader_source[] =
		{
			"#version 430 core											   \n"
			"															   \n"
			"layout (location = 0) in vec3 pos;                            \n"
			"layout (location = 1) in vec3 color;                          \n"

			// uniform 전역 변수: 모든 쉐이더 스테이지에서 접근 가능
			"uniform mat4 rotMat;									       \n"

			"out vec3 vsColor;                                             \n"

			"layout (location = 2) in float time;                          \n"

			"void main(void)											   \n"
			"{															   \n"
			"gl_Position = rotMat * vec4(pos.x, pos.y, pos.z, 1.0);        \n"
			"vsColor = color;                                              \n"
			"}															   \n"
		};

		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core                  \n"
			"in vec3 vsColor;                   \n"
			"out vec4 fragColor;                \n"

			"void main(void)                    \n"
			"{                                  \n"
			"	fragColor = vec4(vsColor, 1.0); \n"
			"}                                  \n"
		};


		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	// 애플리케이션 초기화를 수행한다 (초기 한 번만 실행)
	virtual void startup()
	{
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// 삼각형 세 점 위치와 색상을 한 번에 정의
		GLfloat vertices[] = {
			0.25f,-0.25f,0.5f,1.0f,0.0f,0.0f,  // 버텍스1
			-0.25f,0.25f,0.5f,0.0f,1.0f,0.0f,  // 버텍스2
			0.25f,0.25f,0.5f,0.0f,0.0f,1.0f    // 버텍스3
		};
		
		// VBO를 생성하여 vertices 값들을 복사
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// VBO를 쪼개서 각각 버텍스 속성으로 연결
		// 위치 (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 컬러 (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// 속성값 = 0으로 언바인딩
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteProgram(rendering_program);
	}

	// virtual render() 함수를 작성해서 오버라이딩
	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);

		// 렌더링 위해 생성한 프로그램 객체를 사용
		glUseProgram(rendering_program);

		// z축에 대한 회전행렬 생성 (vmath 이용)
		float angle = currentTime * 50;
		vmath::mat4 rm = vmath::rotate(angle, 0.0f, 0.0f, 1.0f);

		// uniform location 저장
		GLint rotMatLocation = glGetUniformLocation(rendering_program, "rotMat");
		// location에 값을 넘길 때 사용하는 함수
		glUniformMatrix4fv(rotMatLocation, 1, GL_FALSE, rm);

		// 미리 설정한 VAO를 그리기 위해 바인드
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLuint rendering_program;
	GLuint VAO;
};

DECLARE_MAIN(my_application)
