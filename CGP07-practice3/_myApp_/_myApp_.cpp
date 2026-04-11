#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class my_application : public sb7::application
{
public:
	GLuint compile_shaders1(void)
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

	GLuint compile_shaders2(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = sb7::shader::load("pyramid_vs.glsl", GL_VERTEX_SHADER);
		fragment_shader = sb7::shader::load("pyramid_fs.glsl", GL_FRAGMENT_SHADER);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	GLuint compile_shaders3(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = sb7::shader::load("floor_vs.glsl", GL_VERTEX_SHADER);
		fragment_shader = sb7::shader::load("floor_fs.glsl", GL_FRAGMENT_SHADER);

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
		stbi_set_flip_vertically_on_load(true); // 텍스처 이미지의 상하 맞추기

		rendering_program1 = compile_shaders1();
		rendering_program2 = compile_shaders2();
		rendering_program3 = compile_shaders3();

		// VAO 객체 생성 및 바인드 //
		glGenVertexArrays(3, VAO);
		// VBO 객체 생성 및 바인드 //
		glGenBuffers(3, VBO);
		// EBO 객체 생성 및 바인드 //
		glGenBuffers(3, EBO);
		// 텍스처 객체 생성 및 바인드 //
		glGenTextures(3, texture);
		
		// -------------------------------------------- 집 몸통 만들기 -------------------------------------------------- //
		// ================================= VAO[0] 바인드 ======================================= //
		glBindVertexArray(VAO[0]); // 정육면체

		// 위치와 색상 값 배열 선언 (24개 정점: 면당 4개)
		// 각 정점: pos(3), color(3), tex(2) -> stride = 8 floats
		GLfloat vertices[] = {
			// -- 앞 면 (z = +0.25) --
			-0.25f,  0.25f,  0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 0 
			-0.25f, -0.25f,  0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 1 
			 0.25f, -0.25f,  0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 2 
			 0.25f,  0.25f,  0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 3 

			// -- 뒷 면 (z = -0.25) --
			 0.25f,  0.25f, -0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 4 
			 0.25f, -0.25f, -0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 5 
			-0.25f, -0.25f, -0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 6 
			-0.25f,  0.25f, -0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 7 

			// -- 왼쪽 면 (x = -0.25) --
			-0.25f,  0.25f, -0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 8 
			-0.25f, -0.25f, -0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 9 
			-0.25f, -0.25f,  0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //10 
			-0.25f,  0.25f,  0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, //11 

			// -- 오른쪽 면 (x = +0.25) --
			 0.25f,  0.25f,  0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //12 
			 0.25f, -0.25f,  0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //13 
			 0.25f, -0.25f, -0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //14 
			 0.25f,  0.25f, -0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, //15 

			// -- 윗 면 (y = +0.25) --
			-0.25f,  0.25f, -0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //16 
			-0.25f,  0.25f,  0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //17 
			 0.25f,  0.25f,  0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //18 
			 0.25f,  0.25f, -0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, //19 

			// -- 아랫 면 (y = -0.25) --
			-0.25f, -0.25f,  0.25f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //20 
			-0.25f, -0.25f, -0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //21 
			 0.25f, -0.25f, -0.25f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //22 
			 0.25f, -0.25f,  0.25f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f  //23 
		};
		// 인덱스: 면당 2삼각형 (총 36개)
		GLuint indices[] = {
			// Front
			0,1,3, 3,1,2,
			// Back
			4,5,7, 7,5,6,
			// Left
			8,9,11, 11,9,10,
			// Right
			12,13,15, 15,13,14,
			// Top
			16,17,19, 19,17,18,
			// Bottom
			20,21,23, 23,21,22
		};

		// ====================================== VBO [0] 바인드 ============================================ //
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// VBO를 VAO의 Vertex Attributes로 연결
		// location = 0 -> pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// location = 1 -> color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// ====================================== EBO [0] 바인드 ============================================ //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// ====================================== texture [0] 바인드 ============================================ //
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	
		// 이미지의 픽셀 데이터를 배열로 저장
		int width, height, nrChannels;
		unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data);
		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// location = 2 -> texture
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);



		// -------------------------------------------- 집 지붕 만들기 -------------------------------------------------- //
		// ================================= VAO[1] 바인드 ======================================= //
		glBindVertexArray(VAO[1]); // 사각뿔

		GLfloat vertices2[] = {
			// 옆 면 (삼각형)
			0.0f, 0.45f, 0.0f,   0.5f, 1.0f,   // 0
			-0.25f,0.25f,0.25f,  0.0f, 0.0f,   // 1
			0.25f,0.25f,0.25f,   1.0f, 0.0f,   // 2
							     
			0.0f,0.45f,0.0f,     0.5f, 1.0f,   // 3 
			0.25f,0.25f,0.25f,   0.0f, 0.0f,   // 4
			0.25f,0.25f,-0.25f,  1.0f, 0.0f,   // 5

			0.0f,0.45f,0.0f,     0.5f, 1.0f,   // 6
			0.25f,0.25f,-0.25f,  0.0f,0.0f,    // 7
			-0.25f,0.25f,-0.25f, 1.0f,0.0f,    // 8

			0.0f,0.45f,0.0f,     0.5f, 1.0f,   // 9
			-0.25f,0.25f,-0.25f, 0.0f,0.0f,   // 10
			-0.25f,0.25f,0.25f,  1.0f,0.0f,   // 11

			// 밑 면 (사각형)
			-0.25f,0.25f,0.25f,  0.0f,0.0f,   // 12
			0.25f,0.25f,0.25f,   1.0f,0.0f,   // 13
			0.25f,0.25f,-0.25f,  1.0f,1.0f,   // 14
			-0.25f,0.25f,-0.25f, 0.0f,1.0f    // 15
		};
		
		GLuint indices2[] = {
			0,1,2,

			3,4,5,

			6,7,8,

			9,10,11,

			12,13,14,15,12,13
		};

		// ====================================== VBO [1] 바인드 ============================================ //
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		// location = 0 -> pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// ====================================== EBO [1] 바인드 ============================================ //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

		// ====================================== texture [1] 바인드 ============================================ //
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		// 이미지의 픽셀 데이터를 배열로 저장
		int width2, height2, nrChannels2;
		unsigned char* data2 = stbi_load("container2.png", &width2, &height2, &nrChannels2, 0);
		// 이미지 데이터를 복사해서 텍스처 만들기
		if (data2) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2); // png는 GL_RGBA
			glGenerateMipmap(GL_TEXTURE_2D); // 밉맵 만들기
		}
		// 텍스처 만들고 난 후 반드시 메모리 해제
		stbi_image_free(data2);
		// 텍스처 래핑, 샘플링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// location = 2 -> texture
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);



		// -------------------------------------------- 배경 만들기 -------------------------------------------------- //
		glBindVertexArray(VAO[2]);
		GLfloat vertices3[] = {
			-2.0f, -0.25f, 2.0f,
			2.0f, -0.25f, 2.0f,
			2.0f, -0.25f, -2.0f,
			-2.0f, -0.25f, -2.0f
		};
		GLuint indices3[] = {
			0,1,2,0,2,3
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		// 텍스처 좌표를 기존의 vertices에 추가하지 않고 새로운 배열을 만들고 VBO를 하나 더 추가하는 방식으로
		GLfloat verticesTex[] = {
			0.0f,0.0f,
			4.0f,0.0f,
			4.0f,4.0f,
			0.0f,4.0f
		};
		glGenBuffers(1, &VBO_Tex);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Tex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTex), verticesTex, GL_STATIC_DRAW);

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		int width3, height3, nrChannels3;
		unsigned char* data3 = stbi_load("wall.jpg", &width3, &height3, &nrChannels3, 0);
		if (data3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data3);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(2, VBO);
		glDeleteBuffers(2, EBO);
		glDeleteTextures(2, texture);
		glDeleteProgram(rendering_program1);
		glDeleteProgram(rendering_program2);
	}

	virtual void render(double currentTime)
	{
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);
		// 여러 객체 사이의 가림처리를 위해서
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	
		// 이동 메트릭스를 위한 작업
		vmath::mat4 tm = vmath::translate(0.4f, 0.0f, -1.0f);
		GLint transMatLocation;

		// 뷰 메트릭스를 위한 작업
		vmath::vec3 eye((float)sin(currentTime*0.4), 1.3, (float)cos(currentTime*0.4));	   // eye=카메라 위치, 위에서 내려다보기 -> y축 조절, 비스듬히 보기 -> x, z축 조절
		vmath::vec3 center(0.0, 0.0, 0.0);										           // center=바라보는 초점
		vmath::vec3 up(0.0, 1.0, 0.0);											           // up=카메라의 정수리, 보통 (0,1,0)으로 고정
		vmath::mat4 vm = vmath::lookat(eye, center, up);
		GLint viewMatLocation;

		// 프로젝션 메트릭스를 위한 작업
		// 세로 화각=50도, 화면 비=가로/세로, Near, Far
		vmath::mat4 pm = vmath::perspective(50.0f, (float)(info.windowWidth) / (float)(info.windowHeight), 0.1f, 1000.0f);
		GLint projMatLocation;


		// ================= 정육면체 그리기 (rendering_program1 + VAO[0](VBO[0], EBO[0]) =====================
		// 렌더링 위해 생성한 프로그램 객체를 사용
		glUseProgram(rendering_program1);

		// 그리기 전에 사용하고자 하는 텍스처 바인드하기
		glUniform1i(glGetUniformLocation(rendering_program1, "texIndex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		// 이동 메트릭스
		transMatLocation = glGetUniformLocation(rendering_program1, "transMat");
		glUniformMatrix4fv(transMatLocation, 1, GL_FALSE, tm);

		// 뷰 메트릭스
		viewMatLocation = glGetUniformLocation(rendering_program1, "viewMat");
		glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, vm);

		// 프로젝션 메트릭스
		projMatLocation = glGetUniformLocation(rendering_program1, "projMat");
		glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, pm);

		// VAO 바인드
		glBindVertexArray(VAO[0]);
		// EBO를 활용해 정육면체 그리기
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));
		// 만약에 면마다 다른 텍스처를 적용하려면 인덱스0 ~ 인덱스5, 인덱스6 ~ 인덱스 11, ... 끊어서 DrawElements() 하면 됨


		// ======================== 사각뿔 그리기 (rendering_program2 + VAO[1](VBO[1], EBO[1]) ============================
		glUseProgram(rendering_program2);

		// 그리기 전에 사용하고자 하는 텍스처 바인드하기
		glUniform1i(glGetUniformLocation(rendering_program2, "texIndex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		
		//transMatLocation = glGetUniformLocation(rendering_program2, "transMat");
		//viewMatLocation = glGetUniformLocation(rendering_program2, "viewMat");
		//projMatLocation = glGetUniformLocation(rendering_program2, "projMat");

		glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "transMat"), 1, GL_FALSE, tm);
		glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "viewMat"), 1, GL_FALSE, vm);
		glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "projMat"), 1, GL_FALSE, pm);

		// VAO 바인드
		glBindVertexArray(VAO[1]);
		// EBO를 활용해 사각뿔 그리기
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);


		// ======================== 배경 그리기 (rendering_program3 + VAO[2](VBO[2], EBO[2] + VBO_Tex) ============================
		glUseProgram(rendering_program3);
		glUniform1i(glGetUniformLocation(rendering_program3, "texIndex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "transMat"), 1, GL_FALSE, tm);
		glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "viewMat"), 1, GL_FALSE, vm);
		glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "projMat"), 1, GL_FALSE, pm);

		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	GLuint rendering_program1, rendering_program2, rendering_program3;
	GLuint VAO[3], VBO[3], EBO[3];
	GLuint VBO_Tex;
	GLuint texture[3];
};

DECLARE_MAIN(my_application)