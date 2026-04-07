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
		rendering_program = compile_shaders();

		// VAO 객체 생성 및 바인드
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO); // 정육면체

		// 위치와 색상 값 배열 선언
		GLfloat vertices[] = {
			// 반시계 방향으로 버텍스 정의, 12개 삼각형 = 36개 버텍스
			// 삼각형 2개로 정육면체 한 면 만들기
			// 정육면체 뒷면은 다 똑같은데 시계방향으로 버텍스 정의하면 됨

			// =========== 정육면체 면1 =============
			-0.25f,0.25f,0.25f,1.0f,0.0f,0.0f,   // 1
			-0.25f,-0.25f,0.25f,1.0f,0.0f,0.0f,  // 2
			0.25f,0.25f,0.25f,1.0f,0.0f,0.0f,    // 3
			0.25f,0.25f,0.25f,1.0f,0.0f,0.0f,    // 4
			-0.25f,-0.25f,0.25f,1.0f,0.0f,0.0f,  // 5
			0.25f,-0.25f,0.25f,1.0f,0.0f,0.0f,   // 6

			// =========== 정육면체 면2 =============
			0.25f,0.25f,0.25f,0.0f,1.0f,0.0f,     // 1
			0.25f,-0.25f,0.25f,0.0f,1.0f,0.0f,    // 2
			0.25f,0.25f,-0.25f,0.0f,1.0f,0.0f,    // 3
			0.25f,0.25f,-0.25f,0.0f,1.0f,0.0f,    // 4
			0.25f,-0.25f,0.25f,0.0f,1.0f,0.0f,    // 5
			0.25f,-0.25f,-0.25f,0.0f,1.0f,0.0f,   // 6

			// =========== 정육면체 면3 =============
			-0.25f,0.25f,0.25f,0.0f,0.0f,1.0f,    // 1
			0.25f,0.25f,0.25f,0.0f,0.0f,1.0f,     // 2
			0.25f,0.25f,-0.25f,0.0f,0.0f,1.0f,    // 3
			0.25f,0.25f,-0.25f,0.0f,0.0f,1.0f,    // 4
			-0.25f,0.25f,-0.25f,0.0f,0.0f,1.0f,   // 5
			-0.25f,0.25f,0.25f,0.0f,0.0f,1.0f,    // 6

			// =========== 정육면체 면4 =============
			-0.25f,0.25f,-0.25f,1.0f,0.0f,0.0f,   // 1
			0.25f,0.25f,-0.25f,1.0f,0.0f,0.0f,    // 2
			-0.25f,-0.25f,-0.25f,1.0f,0.0f,0.0f,  // 3
			-0.25f,-0.25f,-0.25f,1.0f,0.0f,0.0f,  // 4
			0.25f,0.25f,-0.25f,1.0f,0.0f,0.0f,    // 5
			0.25f,-0.25f,-0.25f,1.0f,0.0f,0.0f,   // 6

			// =========== 정육면체 면5 =============
			-0.25f,0.25f,0.25f,0.0f,1.0f,0.0f,     // 1
			-0.25f,0.25f,-0.25f,0.0f,1.0f,0.0f,    // 2
			-0.25f,-0.25f,0.25f,0.0f,1.0f,0.0f,    // 3
			-0.25f,-0.25f,0.25f,0.0f,1.0f,0.0f,    // 4
			-0.25f,0.25f,-0.25f,0.0f,1.0f,0.0f,    // 5
			-0.25f,-0.25f,-0.25f,0.0f,1.0f,0.0f,   // 6

			// =========== 정육면체 면6 =============
			-0.25f,-0.25f,-0.25f,0.0f,0.0f,1.0f,   // 1
			0.25f,-0.25f,-0.25f,0.0f,0.0f,1.0f,    // 2
			0.25f,-0.25f,0.25f,0.0f,0.0f,1.0f,     // 3
			-0.25f,-0.25f,0.25f,0.0f,0.0f,1.0f,    // 4
			0.25f,-0.25f,-0.25f,0.0f,0.0f,1.0f,    // 5
			0.25f,-0.25f,0.25f,0.0f,0.0f,1.0f,     // 6
		};

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// VBO를 VAO의 Vertex Attributes로 연결
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

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
		// 그리기
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

private:
	GLuint rendering_program;
	GLuint VAO, VBO;
};

DECLARE_MAIN(my_application)