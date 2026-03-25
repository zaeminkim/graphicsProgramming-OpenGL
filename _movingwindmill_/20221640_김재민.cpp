// sb7.h 헤더 파일을 포함시킨다.
#include <sb7.h>
#include <vmath.h>

// sb7::application을 상속받는다.
class my_application : public sb7::application
{
public:
	// 쉐이더 컴파일한다.
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;


		// 버텍스 쉐이더 소스 코드, 바람개비 그리기
		static const GLchar* vertex_shader_source[] =
		{
			"#version 430 core                                                   \n"
			"                                                                    \n"
			"layout (location = 0) in vec4 color;                                \n"
			"layout (location = 1) in float angle;                               \n"
			"out vec4 vs_color;                                                  \n"
			"                                                                    \n"
			"void main(void)                                                     \n"
			"{                                                                   \n"
			"	const vec4 vertices[12] = vec4[12](vec4(-0.5, 0.5, 0.5, 1.0),    \n"
			"								       vec4(0.0, 0.5, 0.5, 1.0),     \n"
			"								       vec4(0.0, 0.0, 0.5, 1.0),     \n"
			"                                                                    \n"
			"	   							       vec4(0.5 ,0.5, 0.5, 1.0),     \n"
			"		   						       vec4(0.5 ,0.0, 0.5, 1.0),     \n"
			"								       vec4(0.0 ,0.0, 0.5, 1.0),     \n"
			"                                                                    \n"
			"								       vec4(0.5, -0.5, 0.5, 1.0),    \n"
			"								       vec4(0.0, -0.5, 0.5, 1.0),    \n"
			"								       vec4(0.0, 0.0, 0.5, 1.0),     \n"
			"                                                                    \n"
			"								       vec4(-0.5, -0.5, 0.5, 1.0),   \n"
			"								       vec4(-0.5, 0.0, 0.5, 1.0),    \n"
			"								       vec4(0.0, 0.0, 0.5, 1.0));    \n"
			"	mat4 m1;                                                         \n"
			"	m1[0] = vec4(cos(angle), -sin(angle), 0, 0);                     \n"
			"	m1[1] = vec4(sin(angle), cos(angle), 0, 0);                      \n"
			"	m1[2] = vec4(0, 0, 1, 0);                                        \n"
			"	m1[3] = vec4(0, 0, 0, 1);                                        \n"

			"	mat4 m2;                                                         \n"
			"	m2[0] = vec4(1, 0, 0, sin(angle) * 0.5);                         \n"
			"	m2[1] = vec4(0, 1, 0, 0);                                        \n"
			"	m2[2] = vec4(0, 0, 1, 0);                                        \n"
			"	m2[3] = vec4(0, 0, 0, 1);                                        \n"

			"	vec4 pos = vertices[gl_VertexID];                                \n"

			"	float x = pos.x * m1[0][0] + pos.y * m1[0][1];                   \n"
			"	float y = pos.x * m1[1][0] + pos.y * m1[1][1];                   \n"

			"	gl_Position = vec4(x + m2[0][3], y, pos.z, pos.w);               \n"

			"	const vec4 colors[12] = vec4[12](vec4(1.0, 0.0, 0.0, 1.0),       \n"
			"							         vec4(0.0, 1.0, 0.0, 1.0),       \n"
			"							         vec4(0.0, 0.0, 1.0, 1.0),       \n"

			"                                    vec4(1.0, 0.0, 0.0, 1.0),       \n"
			"							         vec4(0.0, 1.0, 0.0, 1.0),       \n"
			"							         vec4(0.0, 0.0, 1.0, 1.0),       \n"

			"                                    vec4(1.0, 0.0, 0.0, 1.0),       \n"
			"							         vec4(0.0, 1.0, 0.0, 1.0),       \n"
			"							         vec4(0.0, 0.0, 1.0, 1.0),       \n"

			"                                    vec4(1.0, 0.0, 0.0, 1.0),       \n"
			"							         vec4(0.0, 1.0, 0.0, 1.0),       \n"
			"							         vec4(0.0, 0.0, 1.0, 1.0));      \n"


			"	vs_color = colors[gl_VertexID] + color;                          \n"
			"}															         \n"
		};

		// 프래그먼트 쉐이더 소스 코드
		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core                  \n"
			"                                   \n"
			"in vec4 vs_color;                  \n"
			"out vec4 color;                    \n"
			"                                   \n"
			"void main(void)                    \n"
			"{                                  \n"
			"	color = vs_color;               \n"
			"}                                  \n"
		};

		// 버텍스 쉐이더를 생성하고 컴파일한다.
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		// 프로그램을 생성하고 쉐이더를 Attach 시키고 링크한다.
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	GLuint compile_shaders2(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;


		// 버텍스 쉐이더 소스 코드, 막대기 그리기
		static const GLchar* vertex_shader_source[] =
		{
			"#version 430 core                                                    \n"
			"                                                                     \n"
			"layout (location = 1) in float angle;"

			"void main(void)                                                      \n"
			"{                                                                    \n"
			"	const vec4 vertices[6] = vec4[6](vec4(-0.02, 0.0, 0.5, 1.0),      \n"
			"									 vec4(-0.02, -1.0, 0.5, 1.0),     \n"
			"                                    vec4(0.02, 0.0, 0.5, 1.0),       \n"
			"									 vec4(0.02, -1.0, 0.5, 1.0),      \n"
			"								     vec4(0.02, 0.0, 0.5, 1.0),       \n"
			"                                    vec4(-0.02, -1.0, 0.5, 1.0));    \n"

			"	mat4 m2;                                                          \n"
			"	m2[0] = vec4(1, 0, 0, sin(angle) * 0.5);                          \n"
			"	m2[1] = vec4(0, 1, 0, 0);                                         \n"
			"	m2[2] = vec4(0, 0, 1, 0);                                         \n"
			"	m2[3] = vec4(0, 0, 0, 1);                                         \n"

			"	vec4 pos = vertices[gl_VertexID];                                 \n"

			"	float x = pos.x + m2[0][3];                                       \n"

			"	gl_Position = vec4(x, pos.y, pos.z, pos.w);                       \n"
			"}                                                                    \n"
		};

		// 프래그먼트 쉐이더 소스 코드
		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core                      \n"
			"                                       \n"
			"out vec4 color;                        \n"
			"                                       \n"
			"void main(void)                        \n"
			"{                                      \n"
			"	color = vec4(1.0, 1.0, 0.5, 1.0);   \n"
			"}                                      \n"
		};

		// 버텍스 쉐이더를 생성하고 컴파일한다.
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		// 프로그램을 생성하고 쉐이더를 Attach 시키고 링크한다.
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	// 애플리케이션 초기화를 수행한다.
	virtual void startup()
	{
		rendering_program = compile_shaders();
		rendering_program2 = compile_shaders2();
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	// 애플리케이션 끝날 때 호출된다.
	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
		glDeleteProgram(rendering_program2);
	}

	// 렌더링 virtual 함수를 작성해서 오버라이딩한다.
	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);

		glUseProgram(rendering_program2);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		glUseProgram(rendering_program);

		glVertexAttrib1f(1, (float)currentTime);

		GLfloat color_change[] = { (float)sin(currentTime) * 0.5f + 0.5f,
								   (float)cos(currentTime) * 0.5f + 0.5f,
								   (float)sin(currentTime) * 0.5f + 0.5f,
								   1.0f };

		glVertexAttrib4fv(0, color_change);

		glDrawArrays(GL_TRIANGLES, 0, 12);
	}

private:
	GLuint rendering_program;
	GLuint rendering_program2;
	GLuint vertex_array_object;
};

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)

// ######################################################## 내가 작성한 코드 아님 ################################################################