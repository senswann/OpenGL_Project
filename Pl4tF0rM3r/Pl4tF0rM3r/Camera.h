#pragma once
//include
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/glm.hpp"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/ext/matrix_clip_space.hpp"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/ext/matrix_transform.hpp"
#include <iostream>
#include <vector>

//classe gerant la camera
class Camera
{
private:
	//variable servant pour la matrice de vue
	glm::vec3 m_postion, m_direction, m_right, m_up;
	float horizontalAngle;
	float verticalAngle;
	glm::mat4 view;

	//variable stockant le temps
	double lastTime, currentTime;
	float deltaTime=0.f;
	float jumpDeltaTime = 0.f;
	
	//variable servant pour les forces a appliquer
	float speed, rotationSpeed;
	float jumpForce;

	//variable de check
	bool isJumping = false;
	bool isGrounded = true;

public:
	//constructeur et destructeur de camera
	Camera(glm::vec3 position);
	~Camera();

	//get de la matrice de vue
	inline glm::mat4 GetView() { return view;}
	//get de la position
	inline glm::vec3 GetPosition() { return m_postion; }
	//fonction gerant les input
	void MoveCamera(GLFWwindow* window, double mouseX, double mouseY);
	//fonction du jump
	void Jump();
	//fonction gerant la gravite
	void Gravity(bool isGround);
	//fonction mettant a jour le matrice de vue
	void SetCurrentView();
	//get pour savoir si le joueur saute
	inline bool GetIsJumping() { return isJumping; }
	//fonction permettant de recommencer facilement
	inline void Restart() { m_postion = glm::vec3(0.f, 1.f, 0.f); }
};

