#pragma once
//include
#include "Camera.h"

//constructeur de camera
Camera::Camera(glm::vec3 position) {
	//on set le temps
	currentTime = glfwGetTime();
	lastTime = currentTime;

	//on set les forces a appliquer
	speed = 10.f;
	jumpForce = 3.f;
	rotationSpeed = 0.005f;

	// Angle horizontal : vers -Z 
	horizontalAngle = 3.14f;
	// Angle vertical : 0, regarde l'horizon 
	verticalAngle = 0.0f;

	// Direction : une conversion de coordonn�es sph�riques vers coordonn�es cart�siennes
	m_direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Vecteur � vers la droite �
	m_right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Vecteur � haut � : perpendiculaire � la direction et le vecteur vers la droite
	m_up = glm::cross(m_right, m_direction);

	//Vecteur de position
	m_postion = position;

	//mise a jour de la matrice de vue
	view = glm::lookAt(
		m_postion,
		m_postion + m_direction,
		m_up
	);
}

//destructeur de camera
Camera::~Camera(){}

//fonction pour set la vue actuel;
void Camera::SetCurrentView() {
	// Direction : une conversion de coordonn�es sph�riques vers coordonn�es cart�siennes
	m_direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Vecteur � vers la droite �
	m_right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Vecteur � haut � : perpendiculaire � la direction et le vecteur vers la droite
	m_up = glm::cross(m_right, m_direction);

	view = glm::lookAt(
		m_postion,
		m_postion + m_direction,
		m_up
	);
}

//fonction du jump
void Camera::Jump() {
	//si on depasse le delay dde jump on arrete de sauter
	if (currentTime >=jumpDeltaTime) {
		isJumping = false;
		jumpForce = 3.f;
	}
	//sinon on continu de sauter
	else {
		//on augmente la vitesse progrssivement jusqu'a arrivé a un certain seuille
		if (jumpForce < 6.f)
			jumpForce += 0.01f;
		m_postion += m_up * deltaTime * jumpForce;
	}
}
//fonction gerant la gravite
void Camera::Gravity(bool isGround) {
	
	//si on est au sol on reinitialise le jump
	if (isGround) {
		jumpForce = 3.f;
		isGrounded = true;
	}
	//sinon on continue de tomber
	else {
		//on augmente la vitesse progrssivement jusqu'a arrivé a un certain seuille
		if (jumpForce < 6.f)
			jumpForce += 0.01f;
		m_postion -= m_up * deltaTime * (jumpForce*1.35f);
	}
}

//fonction gerant les input
void Camera::MoveCamera(GLFWwindow* window, double mouseX, double mouseY) {
	// Recupere le temps pour le deplacement
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;
	// Aller vers l'avant
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		m_postion += m_direction * deltaTime * speed;
	// Aller vers l'arri�re
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_postion -= m_direction * deltaTime * speed;
	// Pas a droite
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_postion += m_right * deltaTime * speed;
	// Pas a gauche
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		m_postion -= m_right * deltaTime * speed;
	// Restart
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		Restart();
	
	// Jump
	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !isJumping && isGrounded) {
		jumpDeltaTime = currentTime + 0.4f;
		isJumping = true;
		isGrounded = false;
	}

	//recuperation du mouvement de camera
	horizontalAngle = -(mouseX)*rotationSpeed;
	
	//on met a jour la camera
	SetCurrentView();
}