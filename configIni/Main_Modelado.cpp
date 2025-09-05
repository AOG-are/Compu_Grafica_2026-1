//Arely Olvera González
//Práctica 4 
//Fecha de entrega: 04/09/25
//No. de cuenta: 319209608

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f; 
float movZ = -5.0f;
float rot = 0.0f;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 Arely Olvera", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	// Colores blancos para el cuerpo del caballo
	float vertices[] = {
		// Cara frontal - blanco
		-0.5f, -0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.5f, -0.5f, 0.5f,  0.95f, 0.95f, 0.95f,
		0.5f,  0.5f, 0.5f,  0.95f, 0.95f, 0.95f,
		0.5f,  0.5f, 0.5f,  0.95f, 0.95f, 0.95f,
		-0.5f,  0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f, -0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

		// Cara trasera - blanco con sombra ligera
		-0.5f, -0.5f,-0.5f, 0.85f, 0.85f, 0.85f,
		 0.5f, -0.5f,-0.5f, 0.85f, 0.85f, 0.85f,
		 0.5f,  0.5f,-0.5f, 0.85f, 0.85f, 0.85f,
		 0.5f,  0.5f,-0.5f, 0.85f, 0.85f, 0.85f,
		-0.5f,  0.5f,-0.5f, 0.85f, 0.85f, 0.85f,
		-0.5f, -0.5f,-0.5f, 0.85f, 0.85f, 0.85f,

		// Cara derecha - blanco con sombra
		 0.5f, -0.5f,  0.5f, 0.80f, 0.80f, 0.80f,
		 0.5f, -0.5f, -0.5f, 0.80f, 0.80f, 0.80f,
		 0.5f,  0.5f, -0.5f, 0.80f, 0.80f, 0.80f,
		 0.5f,  0.5f, -0.5f, 0.80f, 0.80f, 0.80f,
		 0.5f,  0.5f,  0.5f, 0.80f, 0.80f, 0.80f,
		 0.5f,  -0.5f, 0.5f, 0.80f, 0.80f, 0.80f,

		 // Cara izquierda - blanco con sombra
		 -0.5f,  0.5f,  0.5f, 0.80f, 0.80f, 0.80f,
		 -0.5f,  0.5f, -0.5f, 0.80f, 0.80f, 0.80f,
		 -0.5f, -0.5f, -0.5f, 0.80f, 0.80f, 0.80f,
		 -0.5f, -0.5f, -0.5f, 0.80f, 0.80f, 0.80f,
		 -0.5f, -0.5f,  0.5f, 0.80f, 0.80f, 0.80f,
		 -0.5f,  0.5f,  0.5f, 0.80f, 0.80f, 0.80f,

		 // Cara inferior - blanco con sombra fuerte
		 -0.5f, -0.5f, -0.5f, 0.75f, 0.75f, 0.75f,
		 0.5f, -0.5f, -0.5f,  0.75f, 0.75f, 0.75f,
		 0.5f, -0.5f,  0.5f,  0.75f, 0.75f, 0.75f,
		 0.5f, -0.5f,  0.5f,  0.75f, 0.75f, 0.75f,
		 -0.5f, -0.5f,  0.5f, 0.75f, 0.75f, 0.75f,
		 -0.5f, -0.5f, -0.5f, 0.75f, 0.75f, 0.75f,

		 // Cara superior - blanco brillante
		 -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	};

	// Vértices para la crin y cola café caramelo
	float maneVertices[] = {
		// Color café caramelo
		-0.5f, -0.5f, 0.5f, 0.82f, 0.58f, 0.35f,
		0.5f, -0.5f, 0.5f,  0.82f, 0.58f, 0.35f,
		0.5f,  0.5f, 0.5f,  0.82f, 0.58f, 0.35f,
		0.5f,  0.5f, 0.5f,  0.82f, 0.58f, 0.35f,
		-0.5f,  0.5f, 0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f, -0.5f, 0.5f, 0.82f, 0.58f, 0.35f,

		-0.5f, -0.5f,-0.5f, 0.82f, 0.58f, 0.35f,
		 0.5f, -0.5f,-0.5f, 0.82f, 0.58f, 0.35f,
		 0.5f,  0.5f,-0.5f, 0.82f, 0.58f, 0.35f,
		 0.5f,  0.5f,-0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f,  0.5f,-0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f, -0.5f,-0.5f, 0.82f, 0.58f, 0.35f,

		 0.5f, -0.5f,  0.5f, 0.82f, 0.58f, 0.35f,
		 0.5f, -0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
		 0.5f,  0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
		 0.5f,  0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
		 0.5f,  0.5f,  0.5f, 0.82f, 0.58f, 0.35f,
		 0.5f,  -0.5f, 0.5f, 0.82f, 0.58f, 0.35f,

		-0.5f,  0.5f,  0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f,  0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f, -0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f, -0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f, -0.5f,  0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f,  0.5f,  0.5f, 0.82f, 0.58f, 0.35f,

		-0.5f, -0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
		0.5f, -0.5f, -0.5f,  0.82f, 0.58f, 0.35f,
		0.5f, -0.5f,  0.5f,  0.82f, 0.58f, 0.35f,
		0.5f, -0.5f,  0.5f,  0.82f, 0.58f, 0.35f,
		-0.5f, -0.5f,  0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f, -0.5f, -0.5f, 0.82f, 0.58f, 0.35f,

		-0.5f,  0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
		0.5f,  0.5f, -0.5f,  0.82f, 0.58f, 0.35f,
		0.5f,  0.5f,  0.5f,  0.82f, 0.58f, 0.35f,
		0.5f,  0.5f,  0.5f,  0.82f, 0.58f, 0.35f,
		-0.5f,  0.5f,  0.5f, 0.82f, 0.58f, 0.35f,
		-0.5f,  0.5f, -0.5f, 0.82f, 0.58f, 0.35f,
	};

	// Vértices para las puntas de las patas (café oscuro)
	float darkVertices[] = {
		// Colores café oscuro para cascos
		-0.5f, -0.5f, 0.5f, 0.45f, 0.30f, 0.20f,
		0.5f, -0.5f, 0.5f,  0.45f, 0.30f, 0.20f,
		0.5f,  0.5f, 0.5f,  0.45f, 0.30f, 0.20f,
		0.5f,  0.5f, 0.5f,  0.45f, 0.30f, 0.20f,
		-0.5f,  0.5f, 0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f, -0.5f, 0.5f, 0.45f, 0.30f, 0.20f,

		-0.5f, -0.5f,-0.5f, 0.45f, 0.30f, 0.20f,
		 0.5f, -0.5f,-0.5f, 0.45f, 0.30f, 0.20f,
		 0.5f,  0.5f,-0.5f, 0.45f, 0.30f, 0.20f,
		 0.5f,  0.5f,-0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f,  0.5f,-0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f, -0.5f,-0.5f, 0.45f, 0.30f, 0.20f,

		 0.5f, -0.5f,  0.5f, 0.45f, 0.30f, 0.20f,
		 0.5f, -0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
		 0.5f,  0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
		 0.5f,  0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
		 0.5f,  0.5f,  0.5f, 0.45f, 0.30f, 0.20f,
		 0.5f,  -0.5f, 0.5f, 0.45f, 0.30f, 0.20f,

		-0.5f,  0.5f,  0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f,  0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f, -0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f, -0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f, -0.5f,  0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f,  0.5f,  0.5f, 0.45f, 0.30f, 0.20f,

		-0.5f, -0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
		0.5f, -0.5f, -0.5f,  0.45f, 0.30f, 0.20f,
		0.5f, -0.5f,  0.5f,  0.45f, 0.30f, 0.20f,
		0.5f, -0.5f,  0.5f,  0.45f, 0.30f, 0.20f,
		-0.5f, -0.5f,  0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f, -0.5f, -0.5f, 0.45f, 0.30f, 0.20f,

		-0.5f,  0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
		0.5f,  0.5f, -0.5f,  0.45f, 0.30f, 0.20f,
		0.5f,  0.5f,  0.5f,  0.45f, 0.30f, 0.20f,
		0.5f,  0.5f,  0.5f,  0.45f, 0.30f, 0.20f,
		-0.5f,  0.5f,  0.5f, 0.45f, 0.30f, 0.20f,
		-0.5f,  0.5f, -0.5f, 0.45f, 0.30f, 0.20f,
	};

	// Vértices blancos para los ojos 
	float whiteVertices[] = {
		// Color blanco
		-0.5f, -0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.5f, -0.5f, 0.5f,  0.95f, 0.95f, 0.95f,
		0.5f,  0.5f, 0.5f,  0.95f, 0.95f, 0.95f,
		0.5f,  0.5f, 0.5f,  0.95f, 0.95f, 0.95f,
		-0.5f,  0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f, -0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

		-0.5f, -0.5f,-0.5f, 0.95f, 0.95f, 0.95f,
		 0.5f, -0.5f,-0.5f, 0.95f, 0.95f, 0.95f,
		 0.5f,  0.5f,-0.5f, 0.95f, 0.95f, 0.95f,
		 0.5f,  0.5f,-0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f,  0.5f,-0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f, -0.5f,-0.5f, 0.95f, 0.95f, 0.95f,

		 0.5f, -0.5f,  0.5f, 0.95f, 0.95f, 0.95f,
		 0.5f, -0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
		 0.5f,  0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
		 0.5f,  0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
		 0.5f,  0.5f,  0.5f, 0.95f, 0.95f, 0.95f,
		 0.5f,  -0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

		-0.5f,  0.5f,  0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f,  0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f, -0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f, -0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f, -0.5f,  0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f,  0.5f,  0.5f, 0.95f, 0.95f, 0.95f,

		-0.5f, -0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
		0.5f, -0.5f, -0.5f,  0.95f, 0.95f, 0.95f,
		0.5f, -0.5f,  0.5f,  0.95f, 0.95f, 0.95f,
		0.5f, -0.5f,  0.5f,  0.95f, 0.95f, 0.95f,
		-0.5f, -0.5f,  0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f, -0.5f, -0.5f, 0.95f, 0.95f, 0.95f,

		-0.5f,  0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
		0.5f,  0.5f, -0.5f,  0.95f, 0.95f, 0.95f,
		0.5f,  0.5f,  0.5f,  0.95f, 0.95f, 0.95f,
		0.5f,  0.5f,  0.5f,  0.95f, 0.95f, 0.95f,
		-0.5f,  0.5f,  0.5f, 0.95f, 0.95f, 0.95f,
		-0.5f,  0.5f, -0.5f, 0.95f, 0.95f, 0.95f,
	};

	// Vértices para las pupilas azules 
	float blueVertices[] = {
		// Color azul
		-0.5f, -0.5f, 0.5f, 0.2f, 0.3f, 0.8f,
		0.5f, -0.5f, 0.5f,  0.2f, 0.3f, 0.8f,
		0.5f,  0.5f, 0.5f,  0.2f, 0.3f, 0.8f,
		0.5f,  0.5f, 0.5f,  0.2f, 0.3f, 0.8f,
		-0.5f,  0.5f, 0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f, -0.5f, 0.5f, 0.2f, 0.3f, 0.8f,

		-0.5f, -0.5f,-0.5f, 0.2f, 0.3f, 0.8f,
		 0.5f, -0.5f,-0.5f, 0.2f, 0.3f, 0.8f,
		 0.5f,  0.5f,-0.5f, 0.2f, 0.3f, 0.8f,
		 0.5f,  0.5f,-0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f,  0.5f,-0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f, -0.5f,-0.5f, 0.2f, 0.3f, 0.8f,

		 0.5f, -0.5f,  0.5f, 0.2f, 0.3f, 0.8f,
		 0.5f, -0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
		 0.5f,  0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
		 0.5f,  0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
		 0.5f,  0.5f,  0.5f, 0.2f, 0.3f, 0.8f,
		 0.5f,  -0.5f, 0.5f, 0.2f, 0.3f, 0.8f,

		-0.5f,  0.5f,  0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f,  0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f, -0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f, -0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f, -0.5f,  0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f,  0.5f,  0.5f, 0.2f, 0.3f, 0.8f,

		-0.5f, -0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
		0.5f, -0.5f, -0.5f,  0.2f, 0.3f, 0.8f,
		0.5f, -0.5f,  0.5f,  0.2f, 0.3f, 0.8f,
		0.5f, -0.5f,  0.5f,  0.2f, 0.3f, 0.8f,
		-0.5f, -0.5f,  0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f, -0.5f, -0.5f, 0.2f, 0.3f, 0.8f,

		-0.5f,  0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
		0.5f,  0.5f, -0.5f,  0.2f, 0.3f, 0.8f,
		0.5f,  0.5f,  0.5f,  0.2f, 0.3f, 0.8f,
		0.5f,  0.5f,  0.5f,  0.2f, 0.3f, 0.8f,
		-0.5f,  0.5f,  0.5f, 0.2f, 0.3f, 0.8f,
		-0.5f,  0.5f, -0.5f, 0.2f, 0.3f, 0.8f,
	};

	// Vértices para el beige claro del hocico
	float beigeVertices[] = {
		// Color beige claro
		-0.5f, -0.5f, 0.5f, 0.96f, 0.93f, 0.82f,
		0.5f, -0.5f, 0.5f,  0.96f, 0.93f, 0.82f,
		0.5f,  0.5f, 0.5f,  0.96f, 0.93f, 0.82f,
		0.5f,  0.5f, 0.5f,  0.96f, 0.93f, 0.82f,
		-0.5f,  0.5f, 0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f, 0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f,-0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f, -0.5f,-0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f,  0.5f,-0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f,  0.5f,-0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f,  0.5f,-0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f,-0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f, -0.5f,  0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f, -0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f,  0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f,  0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f,  0.5f,  0.5f, 0.96f, 0.93f, 0.82f,
		 0.5f,  -0.5f, 0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f,  0.5f,  0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f,  0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f,  0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f,  0.5f,  0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		0.5f, -0.5f, -0.5f,  0.96f, 0.93f, 0.82f,
		0.5f, -0.5f,  0.5f,  0.96f, 0.93f, 0.82f,
		0.5f, -0.5f,  0.5f,  0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f,  0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f, -0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f,  0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
		0.5f,  0.5f, -0.5f,  0.96f, 0.93f, 0.82f,
		0.5f,  0.5f,  0.5f,  0.96f, 0.93f, 0.82f,
		0.5f,  0.5f,  0.5f,  0.96f, 0.93f, 0.82f,
		-0.5f,  0.5f,  0.5f, 0.96f, 0.93f, 0.82f,
		-0.5f,  0.5f, -0.5f, 0.96f, 0.93f, 0.82f,
	};

	GLuint VBO, VAO, VBO2, VAO2, VBO3, VAO3, VBO4, VAO4, VBO5, VAO5, VBO6, VAO6;

	// VAO para partes blancas del caballo
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// VAO para la crin y cola café caramelo
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(maneVertices), maneVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// VAO para puntas de patas café oscuro
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(darkVertices), darkVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// VAO para ojos blancos
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(whiteVertices), whiteVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// VAO para pupilas azules
	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glBindVertexArray(VAO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blueVertices), blueVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// VAO para el hocico beige claro
	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glBindVertexArray(VAO6);
	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(beigeVertices), beigeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		Inputs(window);
		glfwPollEvents();

		// Render
		glClearColor(0.5f, 0.7f, 0.8f, 1.0f); // Fondo azul claro
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);

		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// ========== PARTES BLANCAS DEL CABALLO ==========
		glBindVertexArray(VAO);

		// CUERPO PRINCIPAL (blanco)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.0f, 2.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// CABEZA (blanca)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.85f, 1.6f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// HOCICO BASE (blanco)

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.55f, 2.4f));
		model = glm::scale(model, glm::vec3(0.7f, 0.5f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// PARTE SUPERIOR DEL HOCICO (blanca)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.55f, 2.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// CUELLO
		// Parte baja del cuello
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.9f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Parte media del cuello
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 1.2f));
		model = glm::scale(model, glm::vec3(0.75f, 0.7f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Parte alta del cuello (conexión con la cabeza)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.45f, 1.4f));
		model = glm::scale(model, glm::vec3(0.7f, 0.6f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// OREJAS (blancas)
		// Oreja izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.35f, 1.45f, 1.6f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Oreja derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, 1.45f, 1.6f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// PATAS BLANCAS (parte superior)
		// Pata delantera izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.8f, 0.8f));
		model = glm::scale(model, glm::vec3(0.3f, 2.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata delantera derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.8f, 0.8f));
		model = glm::scale(model, glm::vec3(0.3f, 2.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata trasera izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.8f, -0.8f));
		model = glm::scale(model, glm::vec3(0.3f, 2.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata trasera derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.8f, -0.8f));
		model = glm::scale(model, glm::vec3(0.3f, 2.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//  CRIN Y COLA CAFÉ CARAMELO
		glBindVertexArray(VAO2);  
		// CRIN PRINCIPAL (café caramelo) 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.25f, 1.4f));
		model = glm::scale(model, glm::vec3(0.2f, 0.6f, -0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MECHÓN IZQUIERDO (café caramelo) 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.12f, 1.38f, 1.26f));
		model = glm::scale(model, glm::vec3(0.15f, 0.4f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MECHÓN DERECHO (café caramelo) 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.12f, 1.38f, 1.26f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MECHÓN CENTRAL (café caramelo) 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.35f, 1.2f));
		model = glm::scale(model, glm::vec3(0.12f, 0.3f, 0.12f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//  MELENA LATERAL DERECHA (MECHONES DESFASADOS) 
		// Mechón lateral 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.52f, 1.096f, 1.5f));
		model = glm::scale(model, glm::vec3(0.08f, 0.5f, 0.12f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mechón lateral 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.52f, 0.95f, 1.33f));
		model = glm::scale(model, glm::vec3(0.09f, 0.8f, 0.11f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mechón lateral 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.52f, 0.95f, 1.15f));
		model = glm::scale(model, glm::vec3(0.07f, 0.8f, 0.13f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mechón lateral 4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.52f, 0.85f, 1.01f));
		model = glm::scale(model, glm::vec3(0.08f, 1.0f, 0.10f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MECHONES DEL CUELLO
		glBindVertexArray(VAO2);  
		// Mechón del cuello superior 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.9f));
		model = glm::scale(model, glm::vec3(0.25f, 0.8f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mechón del cuello medio-alto
		glBindVertexArray(VAO2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, 0.7f));
		model = glm::scale(model, glm::vec3(0.23f, 0.7f, 0.23f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mechón del cuello medio
		glBindVertexArray(VAO2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.5f));
		model = glm::scale(model, glm::vec3(0.21f, 0.6f, 0.21f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Mechón del cuello medio-bajo
		glBindVertexArray(VAO2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.3f));
		model = glm::scale(model, glm::vec3(0.19f, 0.5f, 0.19f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mechón del cuello base
		glBindVertexArray(VAO2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.1f));
		model = glm::scale(model, glm::vec3(0.17f, 0.4f, 0.17f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//  COLA CAFÉ CARAMELO 
		glBindVertexArray(VAO2);

		// COLA SUPERIOR (la más larga)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -1.2f));
		model = glm::scale(model, glm::vec3(0.2f, 1.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// COLA MEDIA IZQUIERDA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.12f, -0.5f, -1.3f));
		model = glm::scale(model, glm::vec3(0.15f, 0.8f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// COLA MEDIA DERECHA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.12f, -0.5f, -1.3f));
		model = glm::scale(model, glm::vec3(0.15f, 0.8f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// COLA CORTA CENTRAL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, -1.3f));
		model = glm::scale(model, glm::vec3(0.12f, 0.4f, 0.12f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MECHONES ADICIONALES DE LA COLA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.08f, -0.9f, -1.2f));
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.08f, -0.9f, -1.2f));
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//  PUNTAS DE PATAS CAFÉ OSCURO
		glBindVertexArray(VAO3);

		// PARTE BAJA DE LAS PATAS (café oscuro)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -1.75f, 0.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.5f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -1.75f, 0.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.5f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -1.75f, -0.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.5f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -1.75f, -0.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.5f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// CASCOS (café oscuro)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -1.55f, 0.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.2f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -1.55f, 0.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.2f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -1.55f, -0.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.2f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -1.55f, -0.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.2f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// OJOS BLANCOS
		glBindVertexArray(VAO4);

		// Ojo izquierdo (base blanca)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, 1.0f, 2.25f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Ojo derecho (base blanca)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, 1.0f, 2.25f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// PUPILAS AZULES 
		glBindVertexArray(VAO5);

		// Pupila izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, 1.0f, 2.27f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pupila derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, 1.0f, 2.27f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// HOCICO 
		glBindVertexArray(VAO6);

		// Parte frontal del hocico
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.50f, 2.82f));
		model = glm::scale(model, glm::vec3(0.3f, 0.25f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// FOSAS NASALES (pequeños cubitos oscuros)
		glBindVertexArray(VAO3);

		// Fosa nasal izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.08f, 0.56f, 2.84f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Fosa nasal derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.08f, 0.56f, 2.84f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// BOCA (parte inferior)
		glBindVertexArray(VAO6);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 2.82f));
		model = glm::scale(model, glm::vec3(0.25f, 0.1f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);
	glDeleteVertexArrays(1, &VAO6);
	glDeleteBuffers(1, &VBO6);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.008f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.008f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //Cambio de PAGE_UP por "KEY_UP"
		movY += 0.008f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //Cambio de PAGE_DOWN por "KEY_DOWN"
		movY -= 0.008f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.008f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.008f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.04f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.04f;
}