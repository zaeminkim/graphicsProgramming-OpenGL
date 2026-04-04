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
			"#version 430 core										  \n"
			"														  \n"
			"void main(void)										  \n"
			"{														  \n"
			"const vec4 vertices[5] = vec4[5](vec4(0.6,0.6,0.5,1.0),  \n"
			"						          vec4(-0.6,0.6,0.5,1.0), \n"
			"								  vec4(0.0,-0.6,0.5,1.0), \n"
			"								  vec4(-0.4,0.0,0.5,1.0), \n"
			"								  vec4(0.4,0.0,0.5,1.0)); \n"
			"	gl_Position=vertices[gl_VertexID];                    \n" 
			"}														  \n"
		};
		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core                  \n"
			"                                   \n"
			"out vec4 color;                    \n"
			"void main(void)                    \n"
			"{                                  \n"
			"color=vec4(1.0, 1.0, 0.0, 1.0);    \n"
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
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
	}

	virtual void render(double currentTime)
	{
		const GLfloat blue[] = { 0.0f, 0.2f, 0.6f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, blue);

		glUseProgram(rendering_program);
		
		glPointSize(80.0f);
		glDrawArrays(GL_POINTS, 0, 2);

		glDrawArrays(GL_TRIANGLES, 2, 3);

	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;
};

DECLARE_MAIN(my_application)
