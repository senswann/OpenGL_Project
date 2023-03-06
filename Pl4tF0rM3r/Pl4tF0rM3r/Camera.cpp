#include "Camera.h"

Camera::Camera(glm::vec3 position) {
	currentTime = glfwGetTime();
	lastTime = currentTime;

	speed = 10.f;
	jumpForce = 6.f;
	rotationSpeed = 1.f;

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
	if (currentTime >=jumpDeltaTime) {
		isJumping = false;
		jumpCount = 0;
	}
	else {
		m_postion += m_up * deltaTime * jumpForce;
		jumpCount++;
	}
}

void Camera::Gravity() {
	m_postion -= m_up * deltaTime * (jumpForce*1.35f);
	if (m_postion.y <= 1)
		isGrounded = true;
}

void Camera::MoveCamera(GLFWwindow* window) {
	// R�cup�re le temps pour le d�placement
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);
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
	
	// Jump
	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !isJumping && isGrounded) {
		jumpDeltaTime = currentTime + 0.4f;
		isJumping = true;
		isGrounded = false;
	}

	// Tourne � gauche
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		horizontalAngle += rotationSpeed * deltaTime;
	// Tourne � droite
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		horizontalAngle -= rotationSpeed * deltaTime;
	
	SetCurrentView();
}