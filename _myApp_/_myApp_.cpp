#include <sb7.h>
#include <vmath.h>

class my_application : public sb7::application
{
public:
	// 쉐이더를 컴파일한다
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		// 버텍스 쉐이더 소스코드를 쉐이더 객체로 전달한다.
		static const GLchar* vertex_shader_source[] =
		{
			"#version 430 core                  \n"
			"                                   \n"
			"void main(void)                    \n"
			"{                                  \n"
			"gl_Position=vec4(0.0,0.0,0.5,1.0); \n"
			"}                                  \n"
		};
		// 프래그먼트 쉐이더 소스코드를 쉐이더 객체로 전달한다.
		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core                  \n"
			"                                   \n"
			"out vec4 color;                    \n"
			"void main(void)                    \n"
			"{                                  \n"
			"color=vec4(0.0, 0.8, 1.0, 1.0);    \n"
			"}                                  \n"
		};

		// 버텍스 쉐이더를 생성, 전달, 컴파일 한다.
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		// 프래그먼트 쉐이더를 생성, 전달, 컴파일 한다.
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		// 프로그램을 생성하고 쉐이더를 어태치시키고 링크한다.
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 쉐이더 객체를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	// 애플리케이션 초기화를 수행한다
	virtual void startup()
	{
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	// 애플리케이션이 종료될 때 호출된다
	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
	}

	// 렌더링 virtual 함수를 작성해서 오버라이딩한다
	virtual void render(double currentTime)
	{
		const GLfloat red[] = {
			(float)sin(currentTime) * 0.5f + 0.5f,
			(float)cos(currentTime) * 0.5f + 0.5f,
			0.0f,
			1.0f
		};
		glClearBufferfv(GL_COLOR, 0, red);

		// 렌더링을 위해 만든 프로그램을 사용하도록 한다
		glUseProgram(rendering_program);

		// 점을 하나 그린다
		glPointSize(80.0f);
		glDrawArrays(GL_POINTS, 0, 1);
	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;
};

DECLARE_MAIN(my_application)
