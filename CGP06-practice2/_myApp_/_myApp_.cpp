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
		// =============================== 바람개비 ===================================
		glBindVertexArray(VAO[0]); // VAO[0] 선택하기 //

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
		// VBO 객체 생성 및 바인드
		glGenBuffers(2, VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // VBO[0] 선택하기 //
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // VBO에 vertices 복사해서 저장하기 //

		// VBO를 VAO의 Vertex Attributes로 연결
		// VAO[0]과 VBO[0]을 연결하기, 어떻게 나누는지 알려주는 것//
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 위치
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // 색상
		glEnableVertexAttribArray(1);

		// ================================= 막대기 ======================================
		glBindVertexArray(VAO[1]);

		// 위치 배열 선언
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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 언바인드
		glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO 언바인드
		glBindVertexArray(0);             //VAO 언바인드
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(2, VBO);

		glDeleteProgram(rendering_program);
		glDeleteProgram(rendering_program2);
	}

	virtual void render(double currentTime)
	{
		// ================= 검정색 바탕화면 그리기 ============================
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);


		// 이동 메트릭스 사용을 위한 작업
		vmath::mat4 tm = vmath::translate(float(sin(currentTime)), 0.0f, 0.0f);
		GLint transMatLocation;
		// 뷰 메트릭스 사용을 위한 작업
		vmath::vec3 eye(0.2, 0.3, 0.8);
		vmath::vec3 center(0.0, 0.0, 0.5);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 vm = vmath::lookat(eye, center, up);
		GLint viewMatLocation;
		// 프로젝션 메트릭스 사용을 위한 작업
		float left = -0.1;
		float right = 0.1;
		float top = 0.07;
		float bottom = -0.07;
		float znear = 0.1;
		float zfar = 10;
		vmath::mat4 pm = vmath::frustum(left, right, bottom, top, znear, zfar);
		GLint projMatLocation;


		GLfloat colorChange[] = { 
			(float)sin(currentTime) * 0.5f + 0.5f,
			(float)cos(currentTime) * 0.5f + 0.5f,
			0.0f,
			1.0f 
		};
		glVertexAttrib4fv(2, colorChange);


		// ================= 막대기 그리기 (프로그램2 + VAO[1]) ======================
		// 렌더링 위해 생성한 프로그램 객체(막대기)를 사용
		glUseProgram(rendering_program2); // 막대기용 쉐이더 선택

		// 이동 메트릭스
		transMatLocation = glGetUniformLocation(rendering_program2, "transMat");
		glUniformMatrix4fv(transMatLocation, 1, GL_FALSE, tm);

		// 뷰 메트릭스
		viewMatLocation = glGetUniformLocation(rendering_program2, "viewMat");
		glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, vm);

		// 프로젝션 메트릭스
		projMatLocation = glGetUniformLocation(rendering_program2, "projMat");
		glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, pm);

		glBindVertexArray(VAO[1]);        // 막대기용 데이터 상자 선택
		glDrawArrays(GL_TRIANGLES, 0, 6); // 그리기



		// ================= 바람개비 그리기 (프로그램1 + VAO[1]) =====================
		// 렌더링 위해 생성한 프로그램 객체(바람개비)를 사용
		glUseProgram(rendering_program);

		// 회전 메트릭스
		float angle = currentTime * 50;
		vmath::mat4 rm = vmath::rotate(angle, 0.0f, 0.0f, 1.0f);
		GLint rotMatLocation = glGetUniformLocation(rendering_program, "rotMat");
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

		glBindVertexArray(VAO[0]);         // 바람개비용 데이터 상자 선택
		glDrawArrays(GL_TRIANGLES, 0, 12); // 그리기
	}

private:
	GLuint rendering_program;
	GLuint rendering_program2;
	GLuint VAO[2], VBO[2];
};

DECLARE_MAIN(my_application)
