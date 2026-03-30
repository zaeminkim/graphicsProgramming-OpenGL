#include <sb7.h>

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
			"#version 430 core                  \n"
			"                                   \n"
			"layout (location = 0) in vec4 color;                          \n"
			"layout (location = 3) in float time;                          \n"
			"out vec4 vs_color; \n"
			"void main(void)                    \n"
			"{                                  \n"
			"const vec4 vertices[12] = vec4[12](vec4(-0.5,0.5,0.5,1.0),    \n"
			"								    vec4(0.0,0.5,0.5,1.0),     \n"
			"								    vec4(0.0,0.0,0.5,1.0),     \n"
			"                                                              \n"
			"								    vec4(0.5,0.5,0.5,1.0),     \n"
			"								    vec4(0.5,0.0,0.5,1.0),     \n"
			"								    vec4(0.0,0.0,0.5,1.0),     \n"
			"                                                              \n"
			"								    vec4(0.5,-0.5,0.5,1.0),    \n"
			"								    vec4(0.0,-0.5,0.5,1.0),    \n"
			"								    vec4(0.0,0.0,0.5,1.0),     \n"
			"                                                              \n"
			"								    vec4(-0.5,-0.5,0.5,1.0),   \n"
			"								    vec4(-0.5,0.0,0.5,1.0),    \n"
			"								    vec4(0.0,0.0,0.5,1.0));    \n"

			// 쉐이더에서 행렬을 정의할 때 열단위로 한 줄씩 정의한다
			// 바람개비 회전행렬
			"mat4 rot_mat;\n"
			"rot_mat[0]=vec4(cos(time), sin(time), 0, 0);\n"
			"rot_mat[1]=vec4(-sin(time), cos(time), 0, 0);\n"
			"rot_mat[2]=vec4(0, 0, 1, 0);\n"
			"rot_mat[3]=vec4(0, 0, 0, 1);\n"

			// 바람개비 이동행렬
			"mat4 trans_mat;\n"
			"trans_mat[0]=vec4(1,0,0,0);\n"
			"trans_mat[1]=vec4(0,1,0,0);\n"
			"trans_mat[2]=vec4(0,0,1,0);\n"
			"trans_mat[3]=vec4(sin(time),0,0,1);\n"

			"gl_Position = trans_mat * rot_mat * vertices[gl_VertexID];                             \n"
			// 행렬은 오른쪽에서 왼쪽 방향으로 계산됨.
			// Scaling -> Rotation -> Translation 순서로 계산해야 함.
			"	const vec4 colors[12] = vec4[12](                          \n"
			"                           vec4(1.0,0.0,0.0,1.0),            \n"
			"							vec4(0.0,1.0,0.0,1.0),            \n"
			"							vec4(0.0,0.0,1.0,1.0),            \n"
			"                           vec4(1.0,0.0,0.0,1.0),            \n"
			"							vec4(0.0,1.0,0.0,1.0),            \n"
			"							vec4(0.0,0.0,1.0,1.0),            \n"
			"                           vec4(1.0,0.0,0.0,1.0),            \n"
			"							vec4(0.0,1.0,0.0,1.0),            \n"
			"							vec4(0.0,0.0,1.0,1.0),            \n"
			"                           vec4(1.0,0.0,0.0,1.0),            \n"
			"							vec4(0.0,1.0,0.0,1.0),            \n"
			"							vec4(0.0,0.0,1.0,1.0));            \n"


			"	vs_color=colors[gl_VertexID] + color;                      \n"
			"}															   \n"
		};

		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core                  \n"
			"in vec4 vs_color;                  \n"
			"out vec4 color;                    \n"
			"void main(void)                    \n"
			"{                                  \n"
			"	color=vs_color;                 \n"
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

	GLuint compile_shaders2(void) {
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		static const GLchar* vertex_shader_source[] =
		{
			"#version 430 core                  \n"
			"                                   \n"
			"layout (location = 0) in vec4 color;                          \n"
			"layout (location = 3) in float time; \n"
			"out vec4 vs_color; \n"
			"void main(void)                    \n"
			"{                                  \n"
			"const vec4 vertices[6] = vec4[6](    \n"
            "        vec4(-0.03, -0.8, 0.9, 1.0),         \n"
			"        vec4( 0.03, -0.8, 0.9, 1.0),         \n"
			"        vec4(-0.03,  0.0, 0.9, 1.0),         \n"

			"        vec4( 0.03, -0.8, 0.9, 1.0),         \n"
			"        vec4( 0.03,  0.0, 0.9, 1.0),         \n"
			"        vec4(-0.03,  0.0, 0.9, 1.0));        \n"

			// 막대기 이동 행렬
			"mat4 trans_mat; \n"
			"trans_mat[0]=vec4(1,0,0,0);\n"
			"trans_mat[1]=vec4(0,1,0,0);\n"
			"trans_mat[2]=vec4(0,0,1,0);\n"
			"trans_mat[3]=vec4(sin(time),0,0,1);\n"

			"gl_Position = trans_mat * vertices[gl_VertexID];    \n"
			// 행렬은 오른쪽에서 왼쪽 방향으로 계산됨.
			// Scaling -> Rotation -> Translation 순서로 계산해야 함.
			"}															   \n"
		};

		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core                  \n"
			"in vec4 vs_color;                                   \n"
			"out vec4 color;                    \n"
			"void main(void)                    \n"
			"{                                  \n"
			"	color=vec4(0.0, 0.8, 1.0, 1.0);              \n"
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


	virtual void startup()
	{
		rendering_program = compile_shaders();
		rendering_program2 = compile_shaders2();
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
		glDeleteProgram(rendering_program2);
	}

	virtual void render(double currentTime)
	{
		const GLfloat background[] = {
			0.0f,
			0.0f,
			0.0f,
			1.0f
		};
		glClearBufferfv(GL_COLOR, 0, background);

		glUseProgram(rendering_program2);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUseProgram(rendering_program);

		glVertexAttrib1f(3, (float)currentTime);

		GLfloat color_change[] = { 
			(float)sin(currentTime) * 0.5f + 0.5f,
			(float)cos(currentTime) * 0.5f + 0.5f,
			0.0f,
			1.0f 
		};

		glVertexAttrib4fv(0, color_change);

		glDrawArrays(GL_TRIANGLES, 0, 12);

	}

private:
	GLuint rendering_program;
	GLuint rendering_program2;
	GLuint vertex_array_object;
};

DECLARE_MAIN(my_application)
