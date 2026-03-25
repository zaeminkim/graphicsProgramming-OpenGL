#include <sb7.h>

class my_application : public sb7::application
{
public:
	// 쉐이더를 컴파일한다
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		GLuint tcs, tes, gs;

		// 버텍스 쉐이더 소스코드를 쉐이더 객체로 전달한다.
		static const GLchar* vertex_shader_source[] =
		{
			"#version 430 core                  \n"
			"layout (location = 0) in vec4 offset;                         \n"
			"layout (location = 1) in vec4 color;                          \n"
			"out VS_OUT {                                            \n"
			"	vec4 color; \n"
			"} vs_out;      \n"

			"void main(void)                    \n"
			"{                                  \n"
			"	const vec4 vertices[3] = vec4[3](vec4(0.25,-0.25,0.5,1.0), \n"
			"								  vec4(-0.25,0.25,0.5,1.0),    \n"
			"								  vec4(0.25,0.25,0.5,1.0));    \n"
			"	gl_Position=vertices[gl_VertexID] + offset; \n"
			"	vs_out.color = color;                       \n"
			"}                                  \n"
		};

		// TCS 소스 코드
		static const GLchar* tcs_source[] =
		{
			"#version 430 core                  \n"
			"layout (vertices = 3) out;                         \n"
			"                                        \n"
			"void main(void)                    \n"
			"{                                  \n"
			"	if (gl_InvocationID == 0 ) {    \n"
			"		gl_TessLevelInner[0] = 5.0;    \n"
			"		gl_TessLevelOuter[0] = 5.0;    \n"
			"		gl_TessLevelOuter[1] = 5.0;    \n"
			"		gl_TessLevelOuter[2] = 5.0;    \n"
			"	}                                  \n"
			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;          \n"
			"}            \n"
		};

		// TES 소스 코드
		static const GLchar* tes_source[] =
		{
			"#version 430 core                  \n"
			"layout (triangles, equal_spacing, cw) in;                         \n"
			"                                        \n"
			"void main(void)                    \n"
			"{                                  \n"
			"	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +    \n"
			"		gl_TessCoord.y * gl_in[1].gl_Position +   \n"
			"		gl_TessCoord.z * gl_in[2].gl_Position);   \n"
			" } \n"
		};

		// GS 소스 코드
		static const GLchar* gs_source[] =
		{
			"#version 430 core                  \n"
			"layout (triangles) in;                         \n"
			"layout (points, max_vertices = 3) out; \n"
			"                                        \n"
			"void main(void)                    \n"
			"{                                  \n"
			"	int i;                  \n"
			"	for (int i=0; i<gl_in.length(); i++) { \n"
			"		gl_Position = gl_in[i].gl_Position;    \n"
			"		EmitVertex(); \n"
			"		gl_Position[0] *= -1; \n"
			"		gl_Position[1] *= -1; \n"
			"		EmitVertex(); \n"
			"	}             \n"
			"}                \n"
		};

		// 프래그먼트 쉐이더 소스코드를 쉐이더 객체로 전달한다.
		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core                  \n"
			"                                   \n"
			"in VS_OUT {                  \n"
			"	vec4 color;                    \n"
			"} fs_in;                   \n"
			"out vec4 color;                                  \n"
			"void main(void)                \n"
			"{                              \n"
			"	color=vec4(0.0,0.8,1.0,1.0); //fs_in.color;            \n"
			"}                                  \n"
		};

		// 버텍스 쉐이더를 생성, 전달, 컴파일 한다.
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		// TCS를 생성하고 컴파일한다.
		tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tcs, 1, tcs_source, NULL);
		glCompileShader(tcs);

		// TES를 생성하고 컴파일한다.
		tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tes, 1, tes_source, NULL);
		glCompileShader(tes);

		// GS를 생성하고 컴파일한다.
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gs, 1, gs_source, NULL);
		glCompileShader(gs);

		// 프래그먼트 쉐이더를 생성, 전달, 컴파일 한다.
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		// 프로그램을 생성하고 쉐이더를 어태치시키고 링크한다.
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, tcs);
		glAttachShader(program, tes);
		glAttachShader(program, gs);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 쉐이더 객체를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(tcs);
		glDeleteShader(tes);
		glDeleteShader(gs);
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

	// 렌더링 virtual 함수를 작성해서 오버라이딩한다.
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

		GLfloat attrib[] = { (float)sin(currentTime) * 0.5f, (float)cos(currentTime) * 0.5f, 0.0f, 0.0f };

		// 입력 속성 0의 값을 갱신한다 => 삼각형 위치
		glVertexAttrib4fv(0, attrib);

		GLfloat color[] = { 1.0f,0.0f,0.0f,0.0f };

		// 입력 속성 1의 값을 갱신한다 => 삼각형 색상
		glVertexAttrib4fv(1, color);

		glPointSize(5.0f);
		glPatchParameteri(GL_PATCH_VERTICES, 3);

		// 폴리곤 그리는 모드를 와이어프레임으로 변경한다
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// 삼각형을 그린다
		glDrawArrays(GL_PATCHES, 0, 3);
	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;
};

DECLARE_MAIN(my_application)
