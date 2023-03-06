#include "Camera.h"

Camera::Camera(glm::vec3 position) {
	currentTime = glfwGetTime();
	lastTime = currentTime;

	speed = 3.0f;
	rotationSpeed = 1.0f;

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

	view = glm::lookAt(
		m_postion,
		m_postion + m_direction,
		m_up
	);
}

Camera::~Camera(){}


void Camera::MoveCamera(GLFWwindow* window) {
	// R�cup�re le temps pour le d�placement
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;
	// Aller vers l'avant
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		m_postion += m_direction * deltaTime * speed;
	// Aller vers l'arri�re
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_postion -= m_direction * deltaTime * speed;
	// Pas � droite
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_postion += m_right * deltaTime * speed;
	// Pas � gauche
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		m_postion -= m_right * deltaTime * speed;
	// Pas � haut
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_postion += m_up * deltaTime * speed;
	// Pas � bas
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_postion -= m_up * deltaTime * speed;
	// Tourne � gauche
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		horizontalAngle += rotationSpeed * deltaTime;
	// Tourne � droite
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		horizontalAngle -= rotationSpeed * deltaTime;

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