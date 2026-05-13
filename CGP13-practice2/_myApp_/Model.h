#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>

#include <sb7.h>
#include <vmath.h>

class Model {
public:
	// Ŭ���� ����
	std::vector<vmath::vec3> vPositions;
	std::vector<vmath::vec2> vTexCoords;
	std::vector<vmath::vec3> vNormals;
	std::vector<GLuint> vIndices;

	GLuint diffuseMap, specularMap;
	float shininess;
	vmath::vec3 defaultAmbient, defaultDiffuse, defaultSpecular;

private:
	GLuint VAO;
	GLuint VBO_positions, VBO_texCoords, VBO_normals;
	GLuint EBO;
	bool useDiffuseMap, useSpecularMap;

public:
	// ������
	Model() {
		shininess = 32.f;
		useDiffuseMap = false;
		useSpecularMap = false;

		defaultAmbient = vmath::vec3(1.0f, 1.0f, 1.0f);
		defaultDiffuse = vmath::vec3(1.0f, 1.0f, 1.0f);
		defaultSpecular = vmath::vec3(0.0f, 0.0f, 0.0f);
	}
	// �Ҹ���
	~Model() {
		glDeleteTextures(1, &diffuseMap);
		glDeleteTextures(1, &specularMap);

		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO_positions);
		glDeleteBuffers(1, &VBO_texCoords);
		glDeleteBuffers(1, &VBO_normals);
		glDeleteVertexArrays(1, &VAO);
	}
	// OpenGL ������Ʈ ����
	void init() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_positions);
		glGenBuffers(1, &VBO_texCoords);
		glGenBuffers(1, &VBO_normals);
		glGenBuffers(1, &EBO);

		glGenTextures(1, &diffuseMap);
		glGenTextures(1, &specularMap);
	}
	// �迭�� �Է����� ����, �迭�� �ִ� �����͵��� Ŭ���� ������ �̵���Ŵ
	void setupMesh(int _numVertices, GLfloat* _vPositions, GLfloat* _vTexCoords = NULL, GLfloat* _vNormals = NULL) {
		// 1. copy data from arrays
		for (int i = 0; i < _numVertices; i++) {
			vmath::vec3 position;
			position[0] = _vPositions[i * 3 + 0];
			position[1] = _vPositions[i * 3 + 1];
			position[2] = _vPositions[i * 3 + 2];
			this->vPositions.push_back(position); // ���� Ŭ������ vPosition�� �������� �߰��ϱ�
		}

		if (_vTexCoords) { // NULL�� �ƴ� ������ ����
			for (int i = 0; i < _numVertices; i++) {
				vmath::vec2 texCoords;
				texCoords[0] = _vTexCoords[i * 2 + 0];
				texCoords[1] = _vTexCoords[i * 2 + 1];
				this->vTexCoords.push_back(texCoords);
			}
		}

		if (_vNormals) { // NULL�� �ƴ� ������ ����
			for (int i = 0; i < _numVertices; i++) {
				vmath::vec3 normal;
				normal[0] = _vNormals[i * 3 + 0];
				normal[1] = _vNormals[i * 3 + 1];
				normal[2] = _vNormals[i * 3 + 2];
				this->vNormals.push_back(normal);
			}
		}

		// 2. create buffers
		// OpenGL ���ۿ� ������ ������ �����͵�� ä���ֱ�
		prepareBuffers();

	}

	void setupIndices(int _numIndices, GLuint* _indices) {
		for (int i = 0; i < _numIndices; i++) {
			vIndices.push_back(_indices[i]); // �Է����� ���� �迭�� vIndices�� �߰��ϱ�
		}
		// prepareBuffers()�� ���� ������ ����
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(int), &vIndices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
	// bool��
	bool loadDiffuseMap(const char* _filepath) {
		if (loadTextureFile(diffuseMap, _filepath)) {
			useDiffuseMap = true;
			return true;
		}

		useDiffuseMap = false;
		return false;
	}

	bool loadSpecularMap(const char* _filepath) {
		if (loadTextureFile(specularMap, _filepath)) {
			useSpecularMap = true;
			return true;
		}

		useSpecularMap = false;
		return false;
	}

	void draw(GLuint _shaderID) {
		glUniform3fv(glGetUniformLocation(_shaderID, "material.defaultAmbient"), 1, defaultAmbient);
		glUniform3fv(glGetUniformLocation(_shaderID, "material.defaultDiffuse"), 1, defaultDiffuse);
		glUniform3fv(glGetUniformLocation(_shaderID, "material.defaultSpecular"), 1, defaultSpecular);
		glUniform1i(glGetUniformLocation(_shaderID, "material.useDiffuseMap"), (int)useDiffuseMap);
		glUniform1i(glGetUniformLocation(_shaderID, "material.useSpecularMap"), (int)useSpecularMap);

		glUniform1i(glGetUniformLocation(_shaderID, "useNormal"), (int)vNormals.size()); // ��ֺ��Ͱ� ���� ��� ������ ������ ���� ����
		glUniform1f(glGetUniformLocation(_shaderID, "material.shininess"), shininess);

		if (useDiffuseMap) {
			glUniform1i(glGetUniformLocation(_shaderID, "material.diffuse"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
		}

		if (useSpecularMap) {
			glUniform1i(glGetUniformLocation(_shaderID, "material.specular"), 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);
		}

		glBindVertexArray(VAO);
		if (vIndices.empty())
			glDrawArrays(GL_TRIANGLES, 0, vPositions.size());
		else
			glDrawElements(GL_TRIANGLES, vIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	bool loadOBJ(const char* filepath) { // obj ������ ��θ� �Է����� ����
		FILE* file = fopen(filepath, "r");
		if (file == NULL)
			return false;

		bool readVT = false, readVN = false;
		// ���� Ŭ���� �����鿡 obj ������ �� ����
		std::vector<vmath::vec3> tmpPositions;
		std::vector<vmath::vec2> tmpTexCoords;
		std::vector<vmath::vec3> tmpNormals;
		std::vector<GLuint> tmpVIndices, tmpVtIndices, tmpVnIndices;

		while (true) {
			char lineBuffer[512];

			// ó�� �� �ܾ� �о����
			int res = fscanf(file, "%s", lineBuffer);
			// ���� ���̸� �б� ����
			if (res == EOF)
				break;

			// ���ؽ� 3���� ������ �о����
			if (strcmp(lineBuffer, "v") == 0) {
				vmath::vec3 v;
				fscanf(file, "%f %f %f\n", &v[0], &v[1], &v[2]);
				tmpPositions.push_back(v);
			}
			// ���ؽ� �ؽ�ó ��ǥ �о����
			else if (strcmp(lineBuffer, "vt") == 0) {
				vmath::vec2 vt;
				fscanf(file, "%f %f\n", &vt[0], &vt[1]);
				tmpTexCoords.push_back(vt);
				readVT = true;
			}
			// ���ؽ� �븻 �о����
			else if (strcmp(lineBuffer, "vn") == 0) {
				vmath::vec3 vn;
				fscanf(file, "%f %f %f\n", &vn[0], &vn[1], &vn[2]);
				tmpNormals.push_back(vn);
				readVN = true;
			}
			// ���̽� �ε����� �о����
			else if (strcmp(lineBuffer, "f") == 0) {
				GLuint vIndex[3], vtIndex[3], vnIndex[3];

				// �����Ǹ� �ִ� ���
				if (!readVT && !readVN) {
					fscanf(file, "%d %d %d\n", &vIndex[0], &vIndex[1], &vIndex[2]);
				}
				// ������/�ؽ�����ǥ �ִ� ���
				else if (readVT && !readVN) {
					fscanf(file, "%d/%d %d/%d %d/%d\n", &vIndex[0], &vtIndex[0], &vIndex[1], &vtIndex[1], &vIndex[2], &vtIndex[2]);
				}
				// ������/��� �ִ� ���
				else if (!readVT && readVN) {
					fscanf(file, "%d//%d %d//%d %d//%d\n", &vIndex[0], &vnIndex[0], &vIndex[1], &vnIndex[1], &vIndex[2], &vnIndex[2]);
				}
				// ������/�ؽ�����ǥ/��� �ִ� ���
				else if (readVT && readVN) {
					fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &vtIndex[0], &vnIndex[0], &vIndex[1], &vtIndex[1], &vnIndex[1], &vIndex[2], &vtIndex[2], &vnIndex[2]);
				}

				for (int i = 0; i < 3; i++) {
					tmpVIndices.push_back(vIndex[i]);
					if (readVT)
						tmpVtIndices.push_back(vtIndex[i]);
					if (readVN)
						tmpVnIndices.push_back(vnIndex[i]);
				}
			}
			else {
				char trashBuffer[1000];
				fgets(trashBuffer, 1000, file);
			}
		}
		// Ŭ���� ������ �ӽ� ������ �����͸� �Űܾ� �ϹǷ� ������ �����͵��� ����
		vPositions.clear();
		vTexCoords.clear();
		vNormals.clear();
		vIndices.clear();
		// �ε����� ��������� �ʰ� �׳� ��ø�ؼ� ������ ���� (OpenGL EBO�� ���� �ε����ۿ� �������� �����Ƿ�...)
		for (int i = 0; i < tmpVIndices.size(); i++) {
			vmath::vec3 position = tmpPositions[tmpVIndices[i] - 1];
			vPositions.push_back(position);
		}
		for (int i = 0; i < tmpVtIndices.size(); i++) {
			vmath::vec2 texCoords = tmpTexCoords[tmpVtIndices[i] - 1];
			vTexCoords.push_back(texCoords);
		}
		for (int i = 0; i < tmpVnIndices.size(); i++) {
			vmath::vec3 normal = tmpNormals[tmpVnIndices[i] - 1];
			vNormals.push_back(normal);
		}

		prepareBuffers();

		return true;
	}

private:
	void prepareBuffers() {
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_positions); // vPositions�� ������ VBO_positions�� �ű��
		glBufferData(GL_ARRAY_BUFFER, vPositions.size() * sizeof(vmath::vec3), &vPositions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		if (!vTexCoords.empty()) { // vTexCoords ���� Ŭ������ ������� �ʴٸ�
			glBindBuffer(GL_ARRAY_BUFFER, VBO_texCoords);
			glBufferData(GL_ARRAY_BUFFER, vTexCoords.size() * sizeof(vmath::vec2), &vTexCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		}

		if (!vNormals.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
			glBufferData(GL_ARRAY_BUFFER, vNormals.size() * sizeof(vmath::vec3), &vNormals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	bool loadTextureFile(GLuint textureID, const char* filepath) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

		if (data) {
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3) // R, G, B (ex. jpg)
				format = GL_RGB;
			else if (nrChannels == 4) // PNG
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			return true;
		}

		stbi_image_free(data);
		return false;
	}
};

#endif 