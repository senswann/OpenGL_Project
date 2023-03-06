#pragma once
//include
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "C:/Users/swann/Documents/OpenGL/OpenGLProjet/external/glm-master/glm/glm.hpp"
#include "C:/Users/swann/Documents/OpenGL/OpenGLProjet/external/glm-master/glm/ext/matrix_clip_space.hpp"
#include "C:/Users/swann/Documents/OpenGL/OpenGLProjet/external/glm-master/glm/ext/matrix_transform.hpp"
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

public:
	Camera(glm::vec3 position);
	~Camera();

	inline glm::mat4 GetView() { return view;}
	inline glm::vec3 GetPosition() { return m_postion; }
	void MoveCamera(GLFWwindow* window);
};

