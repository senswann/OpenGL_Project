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
glm::mat4 projection = glm::perspective(45.f, width / height, 0.1f, 100.f);

//fonction utiliser pour changer de facon random la couleur du fond
void changeColor() {
	//on recup le temps actuel
	double currentTime = glfwGetTime();

	if (currentTime - lastColorChangeTime > 2) {
		//random rouge, vert, bleu
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;

		//on set la couleur
		glClearColor(r, g, b, 1.0f);

		// on met a jour la valeur du dernier changement
		lastColorChangeTime = currentTime;
	}
}

//poue check si nous somme sur un block (ne fonctionne pas)
void CheckColision(Camera cam, std::vector<glm::vec3> blockPosArray, std::vector<glm::vec3> blockSizeArray) {
	glm::vec3 cameraPos = cam.GetPosition();
	for (int i = 0; i < blockPosArray.size(); i++) {
		if (cameraPos.x <blockPosArray.at(i).x + blockSizeArray.at(i).x &&
			cameraPos.x + 1.f > blockPosArray.at(i).x &&
			cameraPos.y < blockPosArray.at(i).y + blockSizeArray.at(i).y &&
			cameraPos.y + 1.f > blockPosArray.at(i).y &&
			cameraPos.z < blockPosArray.at(i).z + blockSizeArray.at(i).z &&
			cameraPos.z + 1.f >blockPosArray.at(i).z) {
			// Collision detected
			std::cout << "ON A BLOCK" << std::endl;
			cam.Gravity(true);
			return;
		}
	}
	cam.Gravity(false);
}

//fonction permettant de rescale la fenetre tout en gardant la bonne vue
void framebuffer_size_callback(GLFWwindow* window, int _width, int _height) {
	width = _width;
	height = _height;
	glViewport(0, 0, _width, _height);
}

int main() {
	//on initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	//on creer et initialise la fenetre
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "Pl4tF0rM3r", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//on initialise le viewport
	glViewport(0, 0, (int)width, (int)height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//on check que GLEW fonctionne
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

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

	//light color
	glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);

	//light position
	std::vector<glm::vec3> lightPos;
	lightPos.push_back(glm::vec3(5.f, 4.f, 0.f));
	lightPos.push_back(glm::vec3(25.f, 4.f, 0.f));
	lightPos.push_back(glm::vec3(40.f, 4.f, 0.f));

	//couleur
	glm::vec3 violet = glm::vec3(0.59, 0.2, 0.86);
	glm::vec3 vert = glm::vec3(0.1, 0.57, 0.19);
	glm::vec3 gris = glm::vec3(0.5, 0.58, 0.87);
	glm::vec3 bleu = glm::vec3(0.09, 0.9, 0.8);
	glm::vec3 ore = glm::vec3(1., 0.76, 0.);

	//couleur du fond
	glClearColor(bleu.x, bleu.y, bleu.z,1.0f);

	//on initialise les shadder utiliser
	GLuint programID = LoadShaders("C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Shader/VertexShader.vert", "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Shader/FragmentShader.frag");
	
	//on creer la camera
	Camera* cam = new Camera(glm::vec3(0.f, 1.f, 0.f));

	//creation des props
	Mesh* cube = new Mesh(glm::vec3(3, 1, 3), programID, g_color_buffer_data_cube,sizeof(g_color_buffer_data_cube), "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Shader/container.jpg", "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Model/cube.obj", true, false);
	cube->SetMVP(cam->GetView(), projection);
	cube->setScale(0.75);

	Mesh* neko = new Mesh(glm::vec3(45, 1, 0), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Model/nekogirl.obj", false, true);
	neko->SetMVP(cam->GetView(), projection);
	neko->setRotate(0, 0.5, 0., -90.f);

	//initialisation des positions et taille des blocks
	std::vector<glm::vec3> blockPosArray;
	std::vector<glm::vec3> blockSizeArray;

	blockPosArray.push_back(glm::vec3(0, -1, 0));
	blockPosArray.push_back(glm::vec3(10, -1, 0));
	blockPosArray.push_back(glm::vec3(25, -1, 0));
	blockPosArray.push_back(glm::vec3(35, -1, 0));
	blockPosArray.push_back(glm::vec3(42, 0, 0));

	blockSizeArray.push_back(glm::vec3(5, 5, 1));
	blockSizeArray.push_back(glm::vec3(5, 5, 1));
	blockSizeArray.push_back(glm::vec3(5, 2, 1));
	blockSizeArray.push_back(glm::vec3(2, 2, 2));
	blockSizeArray.push_back(glm::vec3(5, 5, 1));

	//creation des platformes
	Mesh* platform = new Mesh(blockPosArray.at(0), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Model/platform.obj", false, true);
	platform->SetMVP(cam->GetView(), projection);

	Mesh* platform2 = new Mesh(blockPosArray.at(1), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Model/platform.obj", false, true);
	platform2->SetMVP(cam->GetView(), projection);

	Mesh* platform3 = new Mesh(blockPosArray.at(2), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Model/platform2.obj", false, true);
	platform3->SetMVP(cam->GetView(), projection);

	Mesh* platform4 = new Mesh(blockPosArray.at(3), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Model/cube.obj", false, true);
	platform4->SetMVP(cam->GetView(), projection);

	Mesh* platform5 = new Mesh(blockPosArray.at(4), programID, g_color_buffer_data_cube, sizeof(g_color_buffer_data_cube), nullptr, "C:/Users/swann/Documents/github/OpenGL_Project/Pl4tF0rM3r/Model/platform.obj", false, true);
	platform5->SetMVP(cam->GetView(), projection);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//variable de la camera
	double mouseX, mouseY;

	//limitCam (hauteur de la camera
	float limitY = 1.f;

	//variable stockant la position de la camera
	glm::vec3 camPos;

	//boucle de rendu
	do {
		camPos = cam->GetPosition();

		//check limit cam
		if (limitY == 1.f && camPos.x > 33.f)
			limitY =2.f;
		else if (limitY == 2.f && camPos.x < 33.f)
			limitY = 1.f;

		//recuperation de la positon de la souris
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//input de la camera
		cam->MoveCamera(window, mouseX, mouseY);

		//check pour savoir si le joueur peux sauter
		if (cam->GetIsJumping())
			cam->Jump();

		//retomber apres le jump
		else if(camPos.y>limitY)
			cam->Gravity(false);
		//check les bordures sur l'axe X
		else if (camPos.x < -5.f || (15.f < camPos.x && camPos.x < 20.f) || (30.f < camPos.x && camPos.x < 33.f) || camPos.x > 48.f)
			cam->Gravity(false);
		//check les bordures sur l'axe Z
		else if (camPos.z < -5.f || camPos.z > 5.f || ((camPos.z < -2.5f|| camPos.z > 2.5f) && (15.f < camPos.x && camPos.x < 38.f)))
			cam->Gravity(false);
		//sinon on le permet de resauter
		else if (!cam->GetIsJumping())
			cam->Gravity(true);

		//set du MVP pour tous les objets de la scene
		cube->SetMVP(cam->GetView(), projection);
		neko->SetMVP(cam->GetView(), projection);
		platform->SetMVP(cam->GetView(), projection);
		platform2->SetMVP(cam->GetView(), projection);
		platform3->SetMVP(cam->GetView(), projection);
		platform4->SetMVP(cam->GetView(), projection);
		platform5->SetMVP(cam->GetView(), projection);

		//affiche
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		//changeColor();

		//dessine les props
		cube->DrawMesh(camPos, lightColor, lightPos.at(0), vert);
		neko->DrawMesh(camPos, lightColor, lightPos.at(2), ore);

		//dessine les platformes
		platform->DrawMesh(camPos, lightColor, lightPos.at(0),vert);
		platform2->DrawMesh(camPos, lightColor, lightPos.at(0),vert);
		platform3->DrawMesh(camPos, lightColor, lightPos.at(1), gris);
		platform4->DrawMesh(camPos, lightColor, lightPos.at(2), violet);
		platform5->DrawMesh(camPos, lightColor, lightPos.at(2), violet);

		glfwSwapBuffers(window);
		glfwPollEvents();


	} while (!glfwWindowShouldClose(window));

	glDeleteProgram(programID);
	glfwTerminate();
	return 0;
}

