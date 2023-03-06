#pragma once
//include
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/glm.hpp"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/ext/matrix_clip_space.hpp"
#include "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/external/glm-master/glm/ext/matrix_transform.hpp"
#include <iostream>
#include <vector>

class Camera
{
private:
	glm::vec3 m_postion, m_direction, m_right, m_up;
	float horizontalAngle;
	float verticalAngle;
	glm::mat4 view;
	double lastTime, currentTime;
	float speed, rotationSpeed;
	float jumpForce;
	int jumpCount=0;

public:
	Camera(glm::vec3 position);
	~Camera();

	bool isJumping = false;
	bool isGrounded = true;
	inline glm::mat4 GetView() { return view;}
	inline glm::vec3 GetPosition() { return m_postion; }
	void MoveCamera(GLFWwindow* window);
	void Jump();
	void Gravity();
	void SetCurrentView();
};

