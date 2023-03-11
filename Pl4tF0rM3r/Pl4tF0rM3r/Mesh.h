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

//classe permettant de creer un objet
class Mesh
{
	//ID des layout et uniformes poue le shader
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

	//ID des layout et uniformes poue le shader (light)
	GLuint normalBuffer;
	GLuint modelID;
	GLuint lightColorID;
	GLuint lightPosID;
	GLuint viewPosID;
	GLuint colorID;

	//variable pour les lumieres
	glm::mat4 model;
	glm::vec3 lightColor;
	glm::vec3 lightPos;

	//variable stockant le MVP
	glm::mat4 MVP;

	//variable stockant des valeurs pour les uniformes
	int m_texture;
	int m_cellshading;
	int m_vertexNumber;

	//variable de modification de l objet
	float scale;
	float degree;
	glm::vec3 rotation;
	float angle;
	glm::vec3 position;

	//indices pour l optimisation
	std::vector<unsigned short> indices;

	//fonction permettant le chargement des textures
	void loadTexture(GLuint programID, const char* pathTexture);

public:
	//constructeur et destructeur de mesh 
	Mesh(glm::vec3 position, GLuint programID, GLfloat* colorBufferData, int sizeColorBuffer, const char* pathTexture, const char* pathOBJ, bool is_teture, bool is_cellshading);
	~Mesh();

	//set du MVP
	inline void SetMVP(glm::mat4 view, glm::mat4 projection) { MVP = projection * view * model; };
	//get du MVP
	inline glm::mat4 GetMVP() { return MVP; };
	//set du scale
	inline void setScale(float _scale) { scale = _scale; model = glm::scale(model, glm::vec3(scale)); };
	//set de la rotation
	inline void setRotate(float x, float  y, float z, float _degree) { float degree = _degree; glm::vec3 rotation = glm::vec3(x, y, z); model = glm::rotate(model, glm::radians(degree), rotation); };
	//set de la postion
	inline void setPosition(glm::vec3 _position) { position = _position; model = glm::translate(model, position); };
	//fonction gerant l affichage des objets
	void DrawMesh(glm::vec3 viewPos, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 color);
};

