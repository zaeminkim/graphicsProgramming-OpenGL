#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include <sb7.h>
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
