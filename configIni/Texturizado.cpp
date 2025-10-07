//Arely Olvera González
//Práctica 7
//Fecha de entrega:  06/10/25
//No. de cuenta: 319209608

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 7 Arely Olvera", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	//Build and compile our shader program
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Vértices del cubo basados en el archivo .obj de Blender
	// Normalizados de escala 2.0 a 1.0 (dividiendo entre 2)
	GLfloat vertices[] =
	{
		// Posiciones basadas en el .obj (v1-v8)
		// v1: 1.000000 1.000000 -1.000000
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.644774f, 0.757251f,  // 0
		 // v2: 1.000000 -1.000000 -1.000000
		  0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.970028f, 0.242020f,  // 1
		  // v3: 1.000000 1.000000 1.000000
		   0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.970028f, 0.242020f,  // 2
		   // v4: 1.000000 -1.000000 1.000000
			0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.320967f, 0.756132f,  // 3
			// v5: -1.000000 1.000000 -1.000000
			-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.644862f, 0.978140f,  // 4
			// v6: -1.000000 -1.000000 -1.000000
			-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.643176f, 0.268096f,  // 5
			// v7: -1.000000 1.000000 1.000000
			-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.352393f, 0.977118f,  // 6
			// v8: -1.000000 -1.000000 1.000000
			-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.320327f, 0.976799f,  // 7

			// Cara 1: Superior (Y positivo) - f 1/1/1 5/2/1 7/3/1 3/4/1
			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.644774f, 0.757251f,  // 8
			-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.644862f, 0.978140f,  // 9
			-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.352393f, 0.977118f,  // 10
			 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.352349f, 0.756987f,  // 11

			 // Cara 2: Frontal (Z positivo) - f 4/5/2 3/6/2 7/7/2 8/8/2
			  0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.320967f, 0.756132f,  // 12
			  0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.320327f, 0.976799f,  // 13
			 -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.029347f, 0.977840f,  // 14
			 -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.029312f, 0.756792f,  // 15

			 // Cara 3: Izquierda (X negativo) - f 8/9/3 7/10/3 5/11/3 6/12/3
			 -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.320321f, 0.511867f,  // 16
			 -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.322488f, 0.733351f,  // 17
			 -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.029730f, 0.733598f,  // 18
			 -0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.030858f, 0.511936f,  // 19

			 // Cara 4: Inferior (Y negativo) - f 6/13/4 2/14/4 4/15/4 8/16/4
			 -0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.643176f, 0.268096f,  // 20
			  0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.643900f, 0.489196f,  // 21
			  0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.351647f, 0.490129f,  // 22
			 -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.351500f, 0.268848f,  // 23

			 // Cara 5: Derecha (X positivo) - f 2/17/5 1/18/5 3/19/5 4/20/5
			  0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.968193f, 0.021733f,  // 24
			  0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.970028f, 0.242020f,  // 25
			  0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.676779f, 0.242651f,  // 26
			  0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.678942f, 0.021608f,  // 27

			  // Cara 6: Trasera (Z negativo) - f 6/21/6 5/22/6 1/23/6 2/24/6
			  -0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.322456f, 0.267783f,  // 28
			  -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.322750f, 0.488839f,  // 29
			   0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.031485f, 0.489056f,  // 30
			   0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.030202f, 0.268765f   // 31
	};

	GLuint indices[] =
	{
		// Cara 1: Superior (Y positivo)
		8, 9, 10, //primer triángulo
		10, 11, 8, //segundo triángulo

		// Cara 2: Frontal (Z positivo)
		12, 13, 14,
		14, 15, 12,

		// Cara 3: Izquierda (X negativo)
		16, 17, 18,
		18, 19, 16,

		// Cara 4: Inferior (Y negativo)
		20, 21, 22,
		22, 23, 20,

		// Cara 5: Derecha (X positivo)
		24, 25, 26,
		26, 27, 24,

		// Cara 6: Trasera (Z negativo)
		28, 29, 30,
		30, 31, 28
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Cargar textura
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Cargar imagen (textura sin transparencia)
	unsigned char* image = stbi_load("images/zodiaco.png", &textureWidth, &textureHeight, &nrChannels, 0);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lampShader.Use();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 model(1);

		// Rotación adicional para ver el cubo girar y poder visualizar todas sus caras con las diferentes texturas 
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.5f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}

void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}