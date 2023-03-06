#pragma once
//include
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/glm.hpp"
#include "stdio.h"
#include "stdlib.h"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/ext/matrix_clip_space.hpp"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/ext/matrix_transform.hpp"
#include <iostream>
#include <vector>
#include "../Includes/objloader.hpp"
#include "../Includes/vboindexer.hpp"

class Mesh
{
	GLuint matrixID;
	GLuint texture;
	GLuint textureID;
	GLuint vertexArrayID;
	GLuint isTextureID;
	GLuint vertexBuffer;
	GLuint colorBuffer;
	GLuint uvBuffer;
	GLuint elementBuffer;
	GLuint isCellShadingID;

	//light
	GLuint normalBuffer;
	GLuint modelID;
	GLuint lightColorID;
	GLuint lightPosID;
	GLuint viewPosID;
	glm::mat4 model;
	glm::vec3 lightColor;
	glm::vec3 lightPos;

	glm::mat4 MVP;
	int m_texture;
	int m_cellshading;
	int m_vertexNumber;
	float scale;
	glm::vec3 position;
	std::vector<unsigned short> indices;

	void loadTexture(GLuint programID, const char* pathTexture);

public:
	Mesh(glm::vec3 position, GLuint programID, GLfloat* colorBufferData, int sizeColorBuffer, const char* pathTexture, const char* pathOBJ, bool is_teture, bool is_cellshading);
	Mesh(glm::vec3 position, GLuint programID, GLfloat* colorBufferData, int sizeColorBuffer, const char* pathTexture, GLfloat* vertexBufferData, int sizeVertexBuffer, GLfloat* uv_data, int sizeUv_data, bool is_teture, bool is_cellshading);
	~Mesh();

	inline void SetMVP(glm::mat4 view, glm::mat4 projection) { MVP = projection * view * model; };
	inline glm::mat4 GetMVP() { return MVP; };
	inline void setScale(float _scale) { scale=_scale; model = glm::scale(model, glm::vec3(scale));};
	inline void setPosition(glm::vec3 _position) { position = _position; model = glm::translate(model, position); };
	void DrawMesh(glm::vec3 viewPos, glm::vec3 lightColor, glm::vec3 lightPos);
};

