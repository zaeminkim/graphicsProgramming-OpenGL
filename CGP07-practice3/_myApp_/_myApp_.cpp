#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class my_application : public sb7::application
{
public:
	// 배경 (모래바닥 + 하늘 + 물줄기)
	GLuint compile_shaders1(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = sb7::shader::load("bg_vs.glsl", GL_VERTEX_SHADER);
		fragment_shader = sb7::shader::load("bg_fs.glsl", GL_FRAGMENT_SHADER);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}
	// 모래성 (벽)
	GLuint compile_shaders2(void)
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
	// 모래성 (지붕)
	GLuint compile_shaders3(void)
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
		glGenTextures(3, texture_bg);
		glGenTextures(2, texture_wall);
		glGenTextures(1, &texture_roof);


		// -------------------------------------------- 배경 만들기 -------------------------------------------------- //
		// ================================= VAO[0] 바인드 ======================================= //
		glBindVertexArray(VAO[0]); // 정육면체

		// 버텍스 + 텍스처 좌표
		GLfloat vertices1[] = {
			// bottom (0~3) => 텍스처 "sand.jpg"
			-2.0f, -0.25f, 2.0f,  0.0f, 0.0f,     0.6f, 0.9f, 1.5f,
			2.0f, -0.25f, 2.0f,	  1.0f, 0.0f,     0.6f, 0.9f, 1.5f,
			2.0f, -0.25f, -2.0f,  1.0f, 1.0f,     1.0f, 1.0f, 1.0f,
			-2.0f, -0.25f, -2.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f,

			// top (4~7) => 텍스처 "sky.jpg"		  			     
			-2.0f, 3.75f, 2.0f,	  0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			2.0f, 3.75f, 2.0f,	  1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			2.0f, 3.75f, -2.0f,	  1.0f, 1.0f,     1.0f, 1.0f, 1.0f,
			-2.0f, 3.75f, -2.0f,  0.0f, 1.0f,     1.0f, 1.0f, 1.0f,

			// near (8~11) => 텍스처 "sky.jpg"		 			     
			-2.0f, -0.25f, 2.0f,  1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			2.0f, -0.25f, 2.0f,	  0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			2.0f, 3.75f, 2.0f,	  0.0f, 1.0f,     1.0f, 1.0f, 1.0f,
			-2.0f, 3.75f, 2.0f,	  1.0f, 1.0f,     1.0f, 1.0f, 1.0f,

			// far (12~15) => 텍스처 "sky.jpg"			  			     
			-2.0f, -0.25f, -2.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			2.0f, -0.25f, -2.0f,  1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			2.0f, 3.75f, -2.0f,	  1.0f, 1.0f,     1.0f, 1.0f, 1.0f,
			-2.0f, 3.75f, -2.0f,  0.0f, 1.0f,     1.0f, 1.0f, 1.0f,

			// right (16~19) => 텍스처 "sky.jpg"		  			     
			2.0f, -0.25f, 2.0f,	  1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			2.0f, -0.25f, -2.0f,  0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			2.0f, 3.75f, -2.0f,	  0.0f, 1.0f,     1.0f, 1.0f, 1.0f,
			2.0f, 3.75f, 2.0f,	  1.0f, 1.0f,     1.0f, 1.0f, 1.0f,

			// left (20~23) => 텍스처 "sky.jpg"			  			     
			-2.0f, -0.25f, 2.0f,  0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			-2.0f, -0.25f, -2.0f, 1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
			-2.0f, 3.75f, -2.0f,  1.0f, 1.0f,     1.0f, 1.0f, 1.0f,
			-2.0f, 3.75f, 2.0f,	  0.0f, 1.0f,     1.0f, 1.0f, 1.0f
		};
		GLuint indices1[] = {
			// bottom
			0,1,2,0,2,3,

			// top
			4,6,5,4,7,6,

			// near
			8,10,9,8,11,10,

			// far
			12,13,14,12,14,15,

			// right
			16,18,17,16,19,18,

			//left
			20,21,22,20,22,23
		};

		// ====================================== VBO [0] 바인드 ============================================ //
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

		// VBO를 VAO의 Vertex Attributes로 연결
		// location = 0 -> pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// location = 2 -> color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);


		// ====================================== EBO [0] 바인드 ============================================ //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

		// ====================================== texture_bg 바인드 ============================================ //
		// 배열 및 반복문 이용
		const char* texBgData[] = { "sand.jpg","sky.jpg","water.jpg" }; // 0, 1, 2
		int texBgNum = sizeof(texBgData) / sizeof(texBgData[0]);

		for (int i = 0; i < texBgNum; i++) {
			glBindTexture(GL_TEXTURE_2D, texture_bg[i]);
			// texture_bg[0] = texBgData[0], ...
			int width, height, nrChannels;
			unsigned char* data = stbi_load(texBgData[i], &width, &height, &nrChannels, 0);

			if (data) {
				// jpg와 png가 섞여 있는 경우
				GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			stbi_image_free(data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		// location = 1 -> texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);



		// -------------------------------------------- 모래성 (벽) 만들기 -------------------------------------------------- //
		// ================================= VAO[1] 바인드 ======================================= //
		glBindVertexArray(VAO[1]); // 직육면체

		GLfloat vertices2[] = {
			// near
			-0.25f,  0.25f,  0.25f,  -0.08f, 1.08f, // 0 
			-0.25f, -0.25f,  0.25f,  -0.08f, -0.08f, // 1 
			 0.25f, -0.25f,  0.25f,  1.08f, -0.08f, // 2 
			 0.25f,  0.25f,  0.25f,  1.08f, 1.08f,  // 3 

			 // far
			  0.25f,  0.25f, -0.25f, -0.08f, 1.08f,  // 4 
			  0.25f, -0.25f, -0.25f, -0.08f, -0.08f, // 5 
			 -0.25f, -0.25f, -0.25f, 1.08f, -0.08f,  // 6 
			 -0.25f,  0.25f, -0.25f, 1.08f, 1.08f,   // 7 

			 // left
			 -0.25f,  0.25f, -0.25f, -0.08f, 1.08f,  // 8 
			 -0.25f, -0.25f, -0.25f, -0.08f, -0.08f, // 9 
			 -0.25f, -0.25f,  0.25f, 1.08f, -0.08f,  //10 
			 -0.25f,  0.25f,  0.25f, 1.08f, 1.08f,   //11 

			 // right
			 0.25f,  0.25f,  0.25f,  -0.08f, 1.08f,  //12 
			 0.25f, -0.25f,  0.25f,  -0.08f, -0.08f, //13 
			 0.25f, -0.25f, -0.25f,  1.08f, -0.08f,  //14 
			 0.25f,  0.25f, -0.25f,  1.08f, 1.08f,   //15 

			 // top
			 -0.25f,  0.25f, -0.25f,  -0.08f, 1.08f,  //16 
			 -0.25f,  0.25f,  0.25f,  -0.08f, -0.08f, //17 
			  0.25f,  0.25f,  0.25f,  1.08f, -0.08f,  //18 
			  0.25f,  0.25f, -0.25f,  1.08f, 1.08f,   //19 

			  // bottom
			  -0.25f, -0.25f,  0.25f, -0.08f, 1.08f, //20 
			  -0.25f, -0.25f, -0.25f, -0.08f, -0.08f, //21 
			  0.25f, -0.25f, -0.25f,  1.08f, -0.08f, //22 
			  0.25f, -0.25f,  0.25f,  1.08f, 1.08f  //23 

			  // 텍스처에 border(0.08) 색깔 추가하기
		};

		GLuint indices2[] = {
			// Near
			0,1,3, 3,1,2,
			// Far
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

		// ====================================== VBO [1] 바인드 ============================================ //
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		// location = 0 -> pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// ====================================== EBO [1] 바인드 ============================================ //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);


		// ====================================== texture_wall 바인드 ============================================ //
		// 배열 및 반복문 이용
		const char* texWlData[] = { "sand_front.jpg","sand_wall.jpg" };
		int texWlNum = sizeof(texWlData) / sizeof(texWlData[0]);

		for (int i = 0; i < texWlNum; i++) {
			glBindTexture(GL_TEXTURE_2D, texture_wall[i]);

			int width, height, nrChannels;
			unsigned char* data = stbi_load(texWlData[i], &width, &height, &nrChannels, 0);

			if (data) {
				// jpg와 png가 섞여 있는 경우
				GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			stbi_image_free(data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			vmath::vec4 borderColor = vmath::vec4(0.5, 0.4, 0.3, 1.0);
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		// location = 1 -> texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);


		// -------------------------------------------- 모래성 (지붕) 만들기 -------------------------------------------------- //
		// ================================= VAO[2] 바인드 ======================================= //
		glBindVertexArray(VAO[2]); // 사각뿔

		GLfloat vertices3[] = {
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
			-0.25f,0.25f,-0.25f, 0.0f,0.0f,    //10
			-0.25f,0.25f,0.25f,  1.0f,0.0f,    //11

			// 밑 면 (사각형)
			-0.25f,0.25f,0.25f,  0.0f,0.0f,   //12
			0.25f,0.25f,0.25f,   1.0f,0.0f,   //13
			0.25f,0.25f,-0.25f,  1.0f,1.0f,   //14
			-0.25f,0.25f,-0.25f, 0.0f,1.0f    //15
		};

		GLuint indices3[] = {
			0,1,2,
			3,4,5,
			6,7,8,
			9,10,11,
			12,13,14,  15,12,14
		};

		// ====================================== VBO [2] 바인드 ============================================ //
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// ====================================== EBO [2] 바인드 ============================================ //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);


		// ====================================== texture_roof 바인드 ============================================ //
		glBindTexture(GL_TEXTURE_2D, texture_roof);

		int width, height, nrChannels;
		unsigned char* data = stbi_load("sand_roof.jpg", &width, &height, &nrChannels, 0);

		if (data) {
			// jpg와 png가 섞여 있는 경우
			GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// location = 1 -> texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	virtual void shutdown()
	{
		glDeleteProgram(rendering_program1);
		glDeleteProgram(rendering_program2);
		glDeleteProgram(rendering_program3);
		glDeleteVertexArrays(3, VAO);
		glDeleteBuffers(3, VBO);
		glDeleteBuffers(3, EBO);
		glDeleteTextures(3, texture_bg);
		glDeleteTextures(2, texture_wall);
		glDeleteTextures(1, &texture_roof);
	}

	virtual void render(double currentTime)
	{
		// 검정색 바탕
		const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, background);
		// 여러 객체 사이의 가림처리를 위해서
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// 이동, 회전, 스케일링 메트릭스를 위한 작업
		vmath::mat4 tm1 = vmath::translate(1.2f, 0.0f, -0.2f);
		vmath::mat4 rm1 = vmath::rotate(30.0f, 0.0f, 1.0f, 0.0f);
		vmath::mat4 sm1 = vmath::scale(1.0f, 1.0f, 1.0f);

		// 뷰 메트릭스를 위한 작업
		// eye=카메라 위치, 위에서 내려다보기 -> y축 조절, 비스듬히 보기 -> x, z축 조절
		// center=바라보는 초점
		// up=카메라의 정수리, 보통 (0,1,0)으로 고정
		vmath::vec3 eye((float)sin(currentTime * 0.5) * 2, 1.0, (float)cos(currentTime * 0.5) * 2 + 0.1);
		vmath::vec3 center(0.0, 0.0, 0.1);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 vm = vmath::lookat(eye, center, up);

		// 프로젝션 메트릭스를 위한 작업
		// 세로 화각=50도, 화면 비=가로/세로, Near, Far
		vmath::mat4 pm = vmath::perspective(50.0f, (float)(info.windowWidth) / (float)(info.windowHeight), 0.1f, 1000.0f);


		// -------------------- 배경 그리기 = rendering_program1 + VAO[0](VBO[0], EBO[0]) + texture_bg[3] ------------------------
		// ============================================ 프로그램 객체 사용 ============================================== //
		glUseProgram(rendering_program1);

		// 뷰 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "viewMat"), 1, GL_FALSE, vm);
		// 프로젝션 메트릭스
		glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "projMat"), 1, GL_FALSE, pm);
		// 물줄기 애니메이션을 위해 currentTime을 쉐이더로 전달
		glUniform1f(glGetUniformLocation(rendering_program1, "time"), (float)currentTime);

		// ================================================ VAO 바인드 ================================================= //
		glBindVertexArray(VAO[0]);

		// ============================================== 텍스처 바인드 ================================================ //
		// 0번, 1번 슬롯 세팅
		glUniform1i(glGetUniformLocation(rendering_program1, "texIndex"), 0);
		glUniform1i(glGetUniformLocation(rendering_program1, "texIndex2"), 1);

		glActiveTexture(GL_TEXTURE1); // 1번 위치 활성화
		glBindTexture(GL_TEXTURE_2D, texture_bg[2]);

		// bottom(texture_bg[0]), top(texture_bg[1]), near(texture_bg[1]), far(texture_bg[1]), right(texture_bg[1]), left(texture_bg[1])
		int texBgIndex[] = { 0,1,1,1,1,1 }; // EBO로 그리는 순서대로 텍스처 인덱스를 배치
		for (int i = 0; i < 6; i++) {
			// 0번 위치 활성화
			glActiveTexture(GL_TEXTURE0);
			// 배열에서 이번 면에 맞는 텍스처를 갈아 끼워서 바인딩
			glBindTexture(GL_TEXTURE_2D, texture_bg[texBgIndex[i]]);

			if (i == 0) { // bottom일 때
				glUniform1i(glGetUniformLocation(rendering_program1, "texBottom"), 1); // texBottom에 1 대입
			}
			else { // bottom이 아닐 때
				glUniform1i(glGetUniformLocation(rendering_program1, "texBottom"), 0); // texBottom에 0 대입
			}
			// i가 0일 땐 offset 0, i가 1일 땐 offset 6, i가 2일 땐 offset 12...
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
		}










		// ---------------------------- 3개의 모래성에 적용할 변환 행렬 배열 세팅 -------------------------------- //
		// ============== 각각의 모래성이 배치될 위치, 회전 각도, 크기를 배열로 미리 선언 ================== //
		vmath::mat4 tm_castles[3] = {
			vmath::translate(-0.5f, 0.0f + 0.25f * (1.0f - 1.0f), -0.2f),   // 1번 모래성
			vmath::translate(1.0f, 0.0f + 0.25f * (0.4f - 1.0f), 1.2f),     // 2번 모래성
			vmath::translate(1.0f, 0.0f + 0.25f * (0.6f - 1.0f), -1.4f)     // 3번 모래성
			// Y축 보정값 = 0.25f * (스케일 값 - 1.0f)
		};
		vmath::mat4 rm_castles[3] = {
			vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f),  // 1번 회전
			vmath::rotate(-25.0f, 0.0f, 1.0f, 0.0f), // 2번 회전
			vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f)   // 3번 회전
		};
		vmath::mat4 sm_castles[3] = {
			vmath::scale(1.0f, 1.0f, 1.0f),          // 1번: 원본
			vmath::scale(0.4f, 0.4f, 0.4f),          // 2번: 작게
			vmath::scale(0.6f, 0.6f, 0.6f)           // 3번: 중간 크기
		};


		// --------------------------- 모래성 (벽) 그리기 --------------------------------
		glUseProgram(rendering_program2);
		glBindVertexArray(VAO[1]);

		glUniform1i(glGetUniformLocation(rendering_program2, "texIndex"), 0);
		glActiveTexture(GL_TEXTURE0);

		// 카메라(View)와 화면(Proj) 행렬은 3개 모래성 모두 공통이므로 반복문 밖에서 한 번만
		glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "viewMat"), 1, GL_FALSE, vm);
		glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "projMat"), 1, GL_FALSE, pm);

		int texWlIndex[] = { 1,1,0,1,1,1 }; // 벽면 텍스처 인덱스

		// 모래성 개수(3개)만큼 반복해서 벽 그리기
		for (int i = 0; i < 3; i++) {
			// 이번에 그릴 모래성의 위치, 회전, 크기 행렬을 쉐이더로 전송
			glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "transMat"), 1, GL_FALSE, tm_castles[i]);
			glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "rotMat"), 1, GL_FALSE, rm_castles[i]);
			glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "scaleMat"), 1, GL_FALSE, sm_castles[i]);

			// 6개의 면 그리기
			for (int i = 0; i < 6; i++) {
				glBindTexture(GL_TEXTURE_2D, texture_wall[texWlIndex[i]]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
			}
		}


		// --------------------------- 모래성 (지붕) 그리기 --------------------------------
		glUseProgram(rendering_program3);
		glBindVertexArray(VAO[2]);

		glUniform1i(glGetUniformLocation(rendering_program3, "texIndex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_roof); // 지붕 텍스처는 1개뿐이므로 미리 꽂아둠

		// 공통 카메라 행렬 전송
		glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "viewMat"), 1, GL_FALSE, vm);
		glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "projMat"), 1, GL_FALSE, pm);

		// 지붕도 마찬가지로 3번 반복해서 그리기
		for (int i = 0; i < 3; i++) {
			// 지붕도 벽과 완전히 동일한 위치, 회전, 크기 행렬을 적용
			glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "transMat"), 1, GL_FALSE, tm_castles[i]);
			glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "rotMat"), 1, GL_FALSE, rm_castles[i]);
			glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "scaleMat"), 1, GL_FALSE, sm_castles[i]);

			// 사각뿔(삼각형 4개 + 사각형 1개 = 인덱스 18개) 한 번에 그리기
			glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
		}




















		//// ----------------------- 모래성 (벽) 그리기 = rendering_program2 + VAO[1](VBO[1], EBO[1]) + texture_wall[2] --------------------------------
		//// ============================================ 프로그램 객체 사용 ============================================== //
		//glUseProgram(rendering_program2);

		//// 이동 메트릭스
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "transMat"), 1, GL_FALSE, tm1);
		//// 회전 메트릭스
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "rotMat"), 1, GL_FALSE, rm1);
		//// 스케일 메트릭스
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "scaleMat"), 1, GL_FALSE, sm1);
		//// 뷰 메트릭스
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "viewMat"), 1, GL_FALSE, vm);
		//// 프로젝션 메트릭스
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program2, "projMat"), 1, GL_FALSE, pm);

		//// ================================================ VAO 바인드 ================================================= //
		//glBindVertexArray(VAO[1]);

		//// ============================================== 텍스처 바인드 ================================================ //
		//glUniform1i(glGetUniformLocation(rendering_program1, "texIndex"), 0);
		//glActiveTexture(GL_TEXTURE0);

		//// bottom(1), near(0), far(1), right(1), left(1), top(1)
		//int texWlIndex[] = { 1,1,0,1,1,1 };
		//for (int i = 0; i < 6; i++) {
		//	// 배열에서 이번 면에 맞는 텍스처 번호를 꺼내어 바인딩
		//	glBindTexture(GL_TEXTURE_2D, texture_wall[texWlIndex[i]]);
		//	// i가 0일 땐 offset 0, i가 1일 땐 offset 6, i가 2일 땐 offset 12...
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
		//}



		//// ------------------------ 모래성 (지붕) 그리기 = rendering_program3 + VAO[2](VBO[2], EBO[2]) + texture_roof --------------------------------
		//// ============================================ 프로그램 객체 사용 ============================================== //
		//glUseProgram(rendering_program3);

		//// ================================================ VAO 바인드 ================================================= //
		//glBindVertexArray(VAO[2]);

		//// ============================================== 텍스처 바인드 ================================================ //
		//glUniform1i(glGetUniformLocation(rendering_program1, "texIndex"), 0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture_roof);

		//glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "transMat"), 1, GL_FALSE, tm1);
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "rotMat"), 1, GL_FALSE, rm1);
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "scaleMat"), 1, GL_FALSE, sm1);
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "viewMat"), 1, GL_FALSE, vm);
		//glUniformMatrix4fv(glGetUniformLocation(rendering_program3, "projMat"), 1, GL_FALSE, pm);

		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	}

private:
	GLuint rendering_program1, rendering_program2, rendering_program3;
	GLuint VAO[3], VBO[3], EBO[3];
	GLuint texture_bg[3];
	GLuint texture_wall[2];
	GLuint texture_roof;
};

DECLARE_MAIN(my_application)