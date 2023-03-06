#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "stdio.h"
#include "stdlib.h"
#include "Includes/shader.hpp"
#include <iostream>
#include <vector>
#include "Pl4tF0rM3r/Camera.h"
#include "Pl4tF0rM3r/Mesh.h"

//Variables global
double lastColorChangeTime = 0.0;
float width = 1280;
float height = 720;

float scale = 1.0f;
glm::mat4 projection = glm::perspective(45.f, width / height, 0.1f, 100.f);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scale += yoffset * 0.1f;
}

void InputManager(GLFWwindow* _window) {
	//input scale
	glfwSetScrollCallback(_window, scroll_callback);
}

void changeColor() {
	// get the current time
	double currentTime = glfwGetTime();

	if (currentTime - lastColorChangeTime > 2) {
		// generate random red, green, and blue values between 0 and 1
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;

		// set the clear color to the random color
		glClearColor(r, g, b, 1.0f);

		// update the time of the last color change
		lastColorChangeTime = currentTime;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height) {
	width = _width;
	height = _height;
	glViewport(0, 0, _width, _height);
}

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "Sussy", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glViewport(0, 0, (int)width, (int)height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	glClearColor(211.f/ 255.f, 95.f/ 255.f, 255.f/ 255.f,255.f/ 255.f);

	// Les sommets. Trois floats cons�cutifs donnent un sommet 3D ; trois sommets cons�cutifs donnent un triangle
	// Un cube poss�de six faces avec deux triangles pour chaque, donc cela fait 6*2=12 triangles et 12*3=36 sommets 
	GLfloat g_vertex_buffer_data_cube[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : d�but
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : fin 
	1.0f, 1.0f,-1.0f, // triangle 2 : d�but 
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : fin 
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
	};
	// Une couleur pour chaque sommet. Elles ont �t� g�n�r�es al�atoirement. 
	GLfloat g_color_buffer_data_cube[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
	};
	// Deux coordonn�es UV pour chaque sommet. Les coordonn�es ont �t� cr��es avec Blender.
	GLfloat g_uv_buffer_data_cube[] = {
	0.000059f, 1.0f - 0.000004f,
	0.000103f, 1.0f - 0.336048f,
	0.335973f, 1.0f - 0.335903f,
	1.000023f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.336024f, 1.0f - 0.671877f,
	0.667969f, 1.0f - 0.671889f,
	1.000023f, 1.0f - 0.000013f,
	0.668104f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.000059f, 1.0f - 0.000004f,
	0.335973f, 1.0f - 0.335903f,
	0.336098f, 1.0f - 0.000071f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.336024f, 1.0f - 0.671877f,
	1.000004f, 1.0f - 0.671847f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.668104f, 1.0f - 0.000013f,
	0.335973f, 1.0f - 0.335903f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.668104f, 1.0f - 0.000013f,
	0.336098f, 1.0f - 0.000071f,
	0.000103f, 1.0f - 0.336048f,
	0.000004f, 1.0f - 0.671870f,
	0.336024f, 1.0f - 0.671877f,
	0.000103f, 1.0f - 0.336048f,
	0.336024f, 1.0f - 0.671877f,
	0.335973f, 1.0f - 0.335903f,
	0.667969f, 1.0f - 0.671889f,
	1.000004f, 1.0f - 0.671847f,
	0.667979f, 1.0f - 0.335851f
	};


	//light color
	glm::vec3 lightColor = glm::vec3(0.75f, 0.75f, 0.75f);
	//light pos
	glm::vec3 lightPos = glm::vec3(2.0f, 1.5f, 2.0f);

	GLuint programID = LoadShaders("C:/Users/swann/Documents/OpenGL/OpenGLProjet/Shader/VertexShader.vert", "C:/Users/swann/Documents/OpenGL/OpenGLProjet/Shader/FragmentShader.frag");
	Camera* cam = new Camera(glm::vec3(0.f, 0.f, 7.f));
	Mesh* cube = new Mesh(glm::vec3(0, 0, 0), programID, g_color_buffer_data_cube,sizeof(g_color_buffer_data_cube), "C:/Users/swann/Documents/OpenGL/OpenGLProjet/Shader/container.jpg", "C:/Users/swann/Documents/OpenGL/OpenGLProjet/Model/cube.obj", true, false);
	cube->SetMVP(cam->GetView(), projection);
	Mesh* cube2 = new Mesh(glm::vec3(3, 0, 3), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, "C:/Users/swann/Documents/OpenGL/OpenGLProjet/Model/untitled.obj",false, true);
	cube2->SetMVP(cam->GetView(), projection);
	Mesh* cube3 = new Mesh(glm::vec3(0, 2, 0), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, "C:/Users/swann/Documents/OpenGL/OpenGLProjet/Model/cube.obj", false, true);
	cube2->SetMVP(cam->GetView(), projection);
	Mesh* cube4 = new Mesh(glm::vec3(0,4,0), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, g_vertex_buffer_data_cube, sizeof(g_vertex_buffer_data_cube)/ sizeof(g_vertex_buffer_data_cube[0]), g_uv_buffer_data_cube, sizeof(g_uv_buffer_data_cube)/ sizeof(g_uv_buffer_data_cube[0]), false, false);
	cube4->SetMVP(cam->GetView(), projection);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	do {
		//input camera
		cam->MoveCamera(window);
		InputManager(window);

		cube->SetMVP(cam->GetView(), projection);
		cube2->SetMVP(cam->GetView(), projection);
		cube3->SetMVP(cam->GetView(), projection);
		cube4->SetMVP(cam->GetView(), projection);

		//affiche
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		changeColor();

		cube->DrawMesh(cam->GetPosition(), lightColor, cam->GetPosition());
		cube2->DrawMesh(cam->GetPosition(), lightColor, cam->GetPosition());
		cube3->DrawMesh(cam->GetPosition(), lightColor, cam->GetPosition());
		cube4->DrawMesh(cam->GetPosition(), lightColor, cam->GetPosition());

		glfwSwapBuffers(window);
		glfwPollEvents();


	} while (!glfwWindowShouldClose(window));

	glDeleteProgram(programID);
	glfwTerminate();
	return 0;
}

