#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "C:/Users/swann/Documents/OpenGL/OpenGLProjet/external/stb-master/stb_image.h"

Mesh::Mesh(glm::vec3 position, GLuint programID, GLfloat* colorBufferData, int sizeColorBuffer, const char* pathTexture, const char* pathOBJ, bool is_teture, bool is_cellshading) : scale(1.0f) {
	matrixID = glGetUniformLocation(programID, "mvp");

	//light
	modelID = glGetUniformLocation(programID, "model");
	lightColorID = glGetUniformLocation(programID, "lightColor");
	lightPosID = glGetUniformLocation(programID, "lightPos");
	viewPosID = glGetUniformLocation(programID, "viewPos");

	m_texture = is_teture;
	if (is_teture) {
		loadTexture(programID, pathTexture);
	}
	isTextureID = glGetUniformLocation(programID, "isTexture");

	m_cellshading = is_cellshading;
	isCellShadingID = glGetUniformLocation(programID, "isCellShading");

	vertexArrayID;	// Vertex Array Object (VAO) / 1 par objet / stocke les VBO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// Lire le fichier .obj
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	bool res = loadOBJ(pathOBJ, vertices, uvs, normals);

	std::vector<glm::vec3> verticesIndex;
	std::vector<glm::vec2> uvsIndex;
	std::vector<glm::vec3> normalsIndex;

	indexVBO(vertices, uvs, normals, indices, verticesIndex, uvsIndex, normalsIndex);

	m_vertexNumber = vertices.size();

	// Génère un tampon pour les indices
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBuffer); // G�n�re un tampon et place l'identifiant dans 'vertexbuffer'
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
	glBufferData(GL_ARRAY_BUFFER, verticesIndex.size() * sizeof(glm::vec3), &verticesIndex[0], GL_STATIC_DRAW); // Fournit les sommets � OpenGL.

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeColorBuffer, colorBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvsIndex.size() * sizeof(glm::vec2), &uvsIndex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normalsIndex.size() * sizeof(glm::vec3), &normalsIndex[0], GL_STATIC_DRAW);



	// Matrice de projection : Champ de vision de 45� , ration 4:3, distance d'affichage : 0.1 unit�s <-> 100 unit�s 
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	model = glm::mat4(1.0f);
	setPosition(position);
}

Mesh::Mesh(glm::vec3 position, GLuint programID, GLfloat* colorBufferData, int sizeColorBuffer, const char* pathTexture, GLfloat* vertexBufferData, int sizeVertexBuffer, GLfloat* uv_data, int sizeUv_data, bool is_teture, bool is_cellshading) {	
	matrixID = glGetUniformLocation(programID, "mvp");

	//light
	modelID = glGetUniformLocation(programID, "model");
	lightColorID = glGetUniformLocation(programID, "lightColor");
	lightPosID = glGetUniformLocation(programID, "lightPos");
	viewPosID = glGetUniformLocation(programID, "viewPos");

	if (is_teture) {
		loadTexture(programID, pathTexture);
	}
	isTextureID = glGetUniformLocation(programID, "isTexture");

	m_cellshading = is_cellshading;
	isCellShadingID = glGetUniformLocation(programID, "isCellShading");

	// Cr�ation des normales
	std::vector<glm::vec3> normals;
	int i = 0;
	while (i < sizeVertexBuffer)
	{
		// R�cup�ration des points
		glm::vec3 p1 = glm::vec3(vertexBufferData[i], vertexBufferData[i + 1], vertexBufferData[i + 2]);
		glm::vec3 p2 = glm::vec3(vertexBufferData[i + 3], vertexBufferData[i + 4], vertexBufferData[i + 5]);
		glm::vec3 p3 = glm::vec3(vertexBufferData[i + 6], vertexBufferData[i + 7], vertexBufferData[i + 8]);

		// Cr�ation des faces
		glm::vec3 v1 = p2 - p1;
		glm::vec3 v2 = p3 - p1;

		// Calcul de la normale du triangle
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
		// On le met 3 fois pour le nombre de points
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);

		i += 9;
	}

	m_texture = is_teture;

	vertexArrayID;	// Vertex Array Object (VAO) / 1 par objet / stocke les VBO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	m_vertexNumber = sizeVertexBuffer;

	vertexBuffer; // Permet d'identifier notre tampon de sommets / Vertex Buffer Object (VBO)
	glGenBuffers(1, &vertexBuffer); // G�n�re un tampon et place l'identifiant dans 'vertexbuffer'
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
	glBufferData(GL_ARRAY_BUFFER, sizeVertexBuffer, vertexBufferData, GL_STATIC_DRAW); // Fournit les sommets � OpenGL.

	colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeColorBuffer, colorBufferData, GL_STATIC_DRAW);

	uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeUv_data, uv_data, GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	// Matrice de projection : Champ de vision de 45� , ration 4:3, distance d'affichage : 0.1 unit�s <-> 100 unit�s 
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	model = glm::mat4(1.0f);
	setPosition(position);
}

Mesh::~Mesh(){
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &isTextureID);
	glDeleteBuffers(1, &isCellShadingID);
	glDeleteBuffers(1, &textureID);
	glDeleteBuffers(1, &matrixID);
	glDeleteBuffers(1, &modelID);
	glDeleteBuffers(1, &lightColorID);
	glDeleteBuffers(1, &lightPosID);
	glDeleteBuffers(1, &viewPosID);
	glDeleteBuffers(1, &texture);
	glDeleteBuffers(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexArrayID);
	glDeleteBuffers(1, &elementBuffer);
}

void Mesh::loadTexture(GLuint programID, const char* pathTexture) {
	// Texture OpenGL
	texture;
	// Cr�e une texture OpenGL
	glGenTextures(1, &texture);
	// "Lie" la nouvelle texture cr��e : tous les appels suivants aux fonctions de texture vont modifier cette texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// R�p�te si ne prend pas toute la face.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Lorsque l'on r�tr�cit l'image, utiliser un fondu lin�aire entre deux MIP maps, chacune �tant aussi filtr�e lin�airement
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Lorsque l'on agrandit l'image (aucune MIP map plus grande n'est disponible), utiliser le filtrage LIN�AIRE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Identifiant pour notre variable uniforme "myTextureSampler"
	textureID;

	// Lit le fichier, appel glTexImage2D avec les bons param�tres
	int width, height, nrChannels;
	unsigned char* data = stbi_load(pathTexture, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// G�n�rer les MIP maps. 
		glGenerateMipmap(GL_TEXTURE_2D);

		// Initialise "myTextureSampler"
		textureID = glGetUniformLocation(programID, "myTextureSampler");
	}
	else
	{
		printf("Failed to load texture");
	}
	stbi_image_free(data);
}

void Mesh::DrawMesh(glm::vec3 viewPos, glm::vec3 lightColor, glm::vec3 lightPos) {
	//light
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniform3f(lightColorID, lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(lightPosID, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(viewPosID, viewPos.x, viewPos.y, viewPos.z);
	
	// Pour chaque modèle affiché parce que la MVP sera différente (au moins pour la partie M)
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

	//active texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);

	if (m_texture)
		glUniform1i(isTextureID, 1);
	else
		glUniform1i(isTextureID, 0);

	if (m_cellshading)
		glUniform1i(isCellShadingID, 1);
	else
		glUniform1i(isCellShadingID, 0);

	//Tampon Sommet
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,//layout
		3,//taille
		GL_FLOAT,//type
		GL_FALSE,//normalisé
		0,//nombre octet
		(void*)0//decolage tampon
	);

	//Tampon Couleur
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(
		1,//layout
		3,//taille
		GL_FLOAT,//type
		GL_FALSE,//normalisé
		0,//nombre octet
		(void*)0//decolage tampon
	);

	//Tampon UV
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		2,//layout
		2,//taille
		GL_FLOAT,//type
		GL_FALSE,//normalisé
		0,//nombre octet
		(void*)0//decolage tampon
	);

	//Tampon Normal
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		3,//layout
		3,//taille
		GL_FLOAT,//type
		GL_FALSE,//normalisé
		0,//nombre octet
		(void*)0//decolage tampon
	);

	if (scale != 1.0f)
	{
		setScale(scale);
		scale = 1.0f;
	}

	//draw Cube
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	// Dessine les triangles
	glDrawElements(
		GL_TRIANGLES, // mode
		indices.size(), // nombre
		GL_UNSIGNED_SHORT, // type
		(void*)0 // décalage du tableau de tampons
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}