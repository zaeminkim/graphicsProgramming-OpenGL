#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>

#include <sb7.h>
#include <vmath.h>

// Assimp 헤더 추가
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*
================================================================================
Assimp의 기본 계층 구조 (Hierarchy)
================================================================================
1. aiScene: 로드된 전체 3D 모델 데이터의 루트 컨테이너입니다.
   - mRootNode: 모델의 계층 구조를 담고 있는 루트 노드.
   - mMeshes: 모델에 포함된 모든 실제 기하학적 데이터(Mesh)의 배열.
   - mMaterials: 메쉬들이 사용하는 재질 정보 배열.

2. aiNode: 모델의 공간적 계층 구조(Tree)를 형성합니다.
   - mTransformation: 부모 노드에 대한 상대적인 변환 행렬.
   - mChildren: 자식 노드들.
   - mMeshes: 이 노드가 참조하고 있는 aiScene->mMeshes의 인덱스들.

3. aiMesh: 실제 정점(Vertex) 데이터가 들어있는 단위입니다.
   - mVertices: 정점 위치 정보.
   - mNormals: 법선 벡터 정보.
   - mTextureCoords: 텍스처 좌표 정보.
   - mFaces: 인덱스 버퍼 구성을 위한 면(삼각형 등) 정보.
================================================================================
*/

// 개별 메쉬를 관리하기 위한 구조체
struct Mesh {
    std::vector<vmath::vec3> vertices;
    std::vector<vmath::vec2> texCoords;
    std::vector<vmath::vec3> normals;
    std::vector<unsigned int> indices;

    GLuint VAO, VBO_pos, VBO_tex, VBO_norm, EBO;

    Mesh(std::vector<vmath::vec3> v, std::vector<vmath::vec2> t, std::vector<vmath::vec3> n, std::vector<unsigned int> i) 
        : vertices(v), texCoords(t), normals(n), indices(i) {
        setupMesh();
    }

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO_pos);
        glGenBuffers(1, &VBO_tex);
        glGenBuffers(1, &VBO_norm);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Position
        glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vmath::vec3), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // TexCoord
        if (!texCoords.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO_tex);
            glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(vmath::vec2), &texCoords[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        }

        // Normal
        if (!normals.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO_norm);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vmath::vec3), &normals[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        }

        // Indices
        if (!indices.empty()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        }

        glBindVertexArray(0);
    }

    void draw() {
        glBindVertexArray(VAO);
        if (indices.empty())
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        else
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void cleanup() {
        glDeleteBuffers(1, &VBO_pos);
        glDeleteBuffers(1, &VBO_tex);
        glDeleteBuffers(1, &VBO_norm);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
};

class Model {
public:
    // 기존 데이터 멤버 (하위 호환성을 위해 유지하되, Assimp 사용 시에는 meshes 벡터를 사용)
    /*
	std::vector<vmath::vec3> vPositions;
	std::vector<vmath::vec2> vTexCoords;
	std::vector<vmath::vec3> vNormals;
	std::vector<GLuint> vIndices;
    */

    std::vector<Mesh> meshes;
    std::string directory;

	GLuint diffuseMap, specularMap;
	float shininess;
	vmath::vec3 defaultAmbient, defaultDiffuse, defaultSpecular;

private:
	GLuint VAO;
	GLuint VBO_positions, VBO_texCoords, VBO_normals;
	GLuint EBO;
	bool useDiffuseMap, useSpecularMap;

public:
	Model() {
		shininess = 32.f;
		useDiffuseMap = false;
		useSpecularMap = false;

		defaultAmbient = vmath::vec3(1.0f, 1.0f, 1.0f);
		defaultDiffuse = vmath::vec3(1.0f, 1.0f, 1.0f);
		defaultSpecular = vmath::vec3(0.0f, 0.0f, 0.0f);
	} 

	~Model() {
		glDeleteTextures(1, &diffuseMap);
		glDeleteTextures(1, &specularMap);
		
        for(auto& mesh : meshes) {
            mesh.cleanup();
        }

        // 기존 cleanup 코드 주석 처리
        /*
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO_positions);
		glDeleteBuffers(1, &VBO_texCoords);
		glDeleteBuffers(1, &VBO_normals);
		glDeleteVertexArrays(1, &VAO);
        */
	}

	void init() {
        // 기존 init 코드 주석 처리 (Mesh 구조체에서 개별적으로 수행됨)
        /*
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_positions);
		glGenBuffers(1, &VBO_texCoords);
		glGenBuffers(1, &VBO_normals);
		glGenBuffers(1, &EBO);
        */
		glGenTextures(1, &diffuseMap);
		glGenTextures(1, &specularMap);
	}

    // Assimp를 사용한 모델 로딩 시작 함수
    bool loadModel(std::string path) {
        Assimp::Importer importer;
        // aiProcess_Triangulate: 모든 폴리곤을 삼각형으로 변환
        // aiProcess_FlipUVs: 텍스처 좌표의 y축을 뒤집음 (OpenGL 기준)
        // aiProcess_GenSmoothNormals: 법선 벡터가 없는 경우 자동으로 생성
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return false;
        }
        directory = path.substr(0, path.find_last_of('/'));

        // 루트 노드부터 재귀적으로 계층 구조를 탐색합니다.
        processNode(scene->mRootNode, scene);

        return true;
    }

private:
    // 노드 계층 구조를 탐색하는 재귀 함수
    void processNode(aiNode *node, const aiScene *scene) {
        // 1. 현재 노드에 포함된 모든 메쉬를 처리합니다.
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            // node->mMeshes는 aiScene->mMeshes의 인덱스를 가지고 있습니다.
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // 2. 자식 노드들이 있다면 각각에 대해 다시 processNode를 호출합니다. (재귀)
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    // aiMesh 데이터를 우리가 사용하는 Mesh 구조체로 변환하는 함수
    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<vmath::vec3> vertices;
        std::vector<vmath::vec2> texCoords;
        std::vector<vmath::vec3> normals;
        std::vector<unsigned int> indices;

        // 정점 데이터 추출
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            vmath::vec3 vector;
            vector[0] = mesh->mVertices[i].x;
            vector[1] = mesh->mVertices[i].y;
            vector[2] = mesh->mVertices[i].z;
            vertices.push_back(vector);

            if(mesh->HasNormals()) {
                vmath::vec3 normal;
                normal[0] = mesh->mNormals[i].x;
                normal[1] = mesh->mNormals[i].y;
                normal[2] = mesh->mNormals[i].z;
                normals.push_back(normal);
            }

            if(mesh->mTextureCoords[0]) { // 텍스처 좌표가 존재하는지 확인
                vmath::vec2 vec;
                vec[0] = mesh->mTextureCoords[0][i].x;
                vec[1] = mesh->mTextureCoords[0][i].y;
                texCoords.push_back(vec);
            } else {
                texCoords.push_back(vmath::vec2(0.0f, 0.0f));
            }
        }

        // 인덱스 데이터 추출
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return Mesh(vertices, texCoords, normals, indices);
    }

public:
    // 기존의 수동 메쉬 설정 방식도 유지 (상자, 피라미드 등에 사용)
	void setupMesh(int _numVertices, GLfloat *_vPositions, GLfloat *_vTexCoords = NULL, GLfloat* _vNormals = NULL) {
		std::vector<vmath::vec3> vertices;
		std::vector<vmath::vec2> texCoords;
		std::vector<vmath::vec3> normals;
        std::vector<unsigned int> indices;

		for (int i = 0; i < _numVertices; i++) {
			vertices.push_back(vmath::vec3(_vPositions[i * 3], _vPositions[i * 3 + 1], _vPositions[i * 3 + 2]));
		}
		if (_vTexCoords) {
			for (int i = 0; i < _numVertices; i++) {
				texCoords.push_back(vmath::vec2(_vTexCoords[i * 2], _vTexCoords[i * 2 + 1]));
			}
		}
		if (_vNormals) {
			for (int i = 0; i < _numVertices; i++) {
				normals.push_back(vmath::vec3(_vNormals[i * 3], _vNormals[i * 3 + 1], _vNormals[i * 3 + 2]));
			}
		}
		
        meshes.push_back(Mesh(vertices, texCoords, normals, indices));
	}

	void setupIndices(int _numIndices, GLuint *_indices) {
        // 기존 방식은 meshes[0]이 존재한다고 가정
        if(meshes.empty()) return;

        for (int i = 0; i < _numIndices; i++) {
            meshes[0].indices.push_back(_indices[i]);
        }
        meshes[0].setupMesh(); // 인덱스 추가 후 다시 버퍼 설정
	}

	bool loadDiffuseMap(const char *_filepath) {
		if (loadTextureFile(diffuseMap, _filepath)) {
			useDiffuseMap = true;
			return true;
		}
		useDiffuseMap = false;
		return false;
	}

	bool loadSpecularMap(const char *_filepath) {
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

        // 첫 번째 메쉬의 정보를 기준으로 Normal 사용 여부 전달
		glUniform1i(glGetUniformLocation(_shaderID, "useNormal"), meshes.empty() ? 0 : (int)meshes[0].normals.size());
		glUniform1f(glGetUniformLocation(_shaderID, "material.shininess"), shininess);

		if(useDiffuseMap) {
			glUniform1i(glGetUniformLocation(_shaderID, "material.diffuse"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
		}

		if(useSpecularMap) {
			glUniform1i(glGetUniformLocation(_shaderID, "material.specular"), 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);
		}

        // 모든 메쉬를 순회하며 그립니다.
        for(auto& mesh : meshes) {
            mesh.draw();
        }
	}

    // 기존 loadOBJ 코드 주석 처리
    /*
	bool loadOBJ(const char *filepath) {
        // ... (생략된 기존 코드)
		return true;
	}
    */

private:
    // 기존 prepareBuffers는 Mesh::setupMesh로 이동됨
    /*
	void prepareBuffers() {
        // ... (생략된 기존 코드)
	}
    */

	bool loadTextureFile(GLuint textureID, const char* filepath) {
		int width, height, nrChannels;
		unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);

		if (data) {
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
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
