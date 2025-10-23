//Arely Olvera González
//Practica 9
//Fecha de entrega: 22/10/25
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

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

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
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),           // Point light 1 
	glm::vec3(1.0f, 0.35f, -2.2f),        // Lámpara de jardín 1 
	glm::vec3(1.0f, 0.35f, -8.5f),        // Lámpara de jardín 2 
	glm::vec3(0.0f,0.0f, 0.0f)            // Point light 4 
};
// Posiciones de los focos (postes de luz)
glm::vec3 spotLightPositions[] = {
	glm::vec3(-7.0f, 2.8f, 0.0f),    // Poste de luz 1
	glm::vec3(-7.0f, 2.8f, -9.5f)    // Poste de luz 2
};

// Direcciones de los focos (apuntando hacia abajo)
glm::vec3 spotLightDirections[] = {
	glm::vec3(0.0f, -1.0f, 0.0f),    
	glm::vec3(0.0f, -1.0f, 0.0f)     
};


float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9 Arely Olvera", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	
	/*CARGA DE MODELOS REFERENTES A LA PRACTICA 6 
	 (ESCENARIO JARDÍN)*/

	Model garden((char*)"Models/garden_Scene.obj");

	//Carga del modelo(perro)
	Model dog((char*)"Models/RedDog.obj");

	//Carga del modelo (gato)
	Model cat((char*)"Models/CAT_02.obj");

	//Carga del PRIMER modelo  (juego infantil)
	Model playground((char*)"Models/Playground.obj");

	//Carga del SEGUNDO modelo  (triciclo)
	Model tricycle((char*)"Models/Tricycle.obj");

	//Carga del TERCER modelo (alberca)
	Model pool((char*)"Models/Pool.obj");

	//Carga del CUARTO modelo (asador)
	Model grill((char*)"Models/Grill.obj");

	//Carga del QUINTO modelo (mesa de jardín)
	Model gardenTab((char*)"Models/Garden_Table.obj");

	// Carga del modelo del conejo
	Model conejo((char*)"Models/Conejo.obj");

	//CARGA DE MODELOS PARA LAS ILUMINACIONES

	//Modelo de poste de luz
	Model posLuz((char*)"Models/LuzPoste.obj");

	//Modelo de la lampara de jardín
	Model lampGarden((char*)"Models/LampGarden.obj");


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f);  // Cambiar a 0
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);   // Cambiar a 0
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);  // Cambiar a 0
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.0f);      // Cambiar a 0
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0f);   // Cambiar a 0

		// Point light 2 - Lámpara de jardín 1 (Color #7D0AFF: RGB normalizado 0.49, 0.039, 1.0)
// Intensidad aumentada
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.49f * 0.1f, 0.039f * 0.1f, 1.0f * 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.49f * 0.7f, 0.039f * 0.7f, 1.0f * 0.7f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.49f * 0.5f, 0.039f * 0.5f, 1.0f * 0.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3 - Lámpara de jardín 2 (Color #7D0AFF)
		// Intensidad aumentada
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.49f * 0.1f, 0.039f * 0.1f, 1.0f * 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.49f * 0.7f, 0.039f * 0.7f, 1.0f * 0.7f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.49f * 0.5f, 0.039f * 0.5f, 1.0f * 0.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// SpotLight 1 - Poste de luz 1 (Color #E3FFBA)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].position"), spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].direction"), spotLightDirections[0].x, spotLightDirections[0].y, spotLightDirections[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].ambient"), 0.89f * 0.1f, 1.0f * 0.1f, 0.73f * 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].diffuse"), 0.89f, 1.0f, 0.73f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].specular"), 0.89f, 1.0f, 0.73f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].linear"), 0.0014f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].quadratic"), 0.000007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].cutOff"), glm::cos(glm::radians(10.0f)));        
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].outerCutOff"), glm::cos(glm::radians(16.0f)));  

		// SpotLight 2 - Poste de luz 2 (Color #E3FFBA)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].position"), spotLightPositions[1].x, spotLightPositions[1].y, spotLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].direction"), spotLightDirections[1].x, spotLightDirections[1].y, spotLightDirections[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].ambient"), 0.89f * 0.1f, 1.0f * 0.1f, 0.73f * 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].diffuse"), 0.89f, 1.0f, 0.73f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].specular"), 0.89f, 1.0f, 0.73f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].linear"), 0.0014f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].quadratic"), 0.000007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].cutOff"), glm::cos(glm::radians(10.0f)));        
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].outerCutOff"), glm::cos(glm::radians(16.0f)));  


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		
		////DIBUJO DE LOS MODELOS

		/*Dibujo del jardín (escenario)*/
		glm::mat4 modelGarden(1);

		modelGarden = glm::translate(modelGarden, glm::vec3(-3.0f, 0.0f, 0.0f));
		modelGarden = glm::rotate(modelGarden, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGarden));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		garden.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		//Dibujo de juego infantil
		glm::mat4 modelPlayground(1);

		modelPlayground = glm::translate(modelPlayground, glm::vec3(-1.0f, 0.1f, 3.0));
		modelPlayground = glm::rotate(modelPlayground, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPlayground));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		playground.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		//Dibujo de un triciclo
		glm::mat4 modelTricycle(1);

		modelTricycle = glm::translate(modelTricycle, glm::vec3(-1.0f, 0.16f, 0.0f));
		modelTricycle = glm::rotate(modelTricycle, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTricycle = glm::scale(modelTricycle, glm::vec3(0.01f, 0.01f, 0.01f));

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTricycle));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		tricycle.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//Dibujo de una alberca
		glm::mat4 modelPool(1);

		modelPool = glm::translate(modelPool, glm::vec3(5.0f, 0.39f, -9.5f));
		modelPool = glm::rotate(modelPool, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPool = glm::scale(modelPool, glm::vec3(0.2f, 0.11f, 0.2f));

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPool));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		pool.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//Dibujo de un asador
		glm::mat4 modelGrill(1);

		modelGrill = glm::translate(modelGrill, glm::vec3(8.0f, 0.11f, -9.5f));
		modelGrill = glm::rotate(modelGrill, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGrill = glm::scale(modelGrill, glm::vec3(6.0f, 6.0f, 6.0f));
		
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGrill));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		grill.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//Dibujo de una mesa de jardín
		glm::mat4 modelGardenTab(1);

		modelGardenTab = glm::translate(modelGardenTab, glm::vec3(10.0f, 0.1f, -9.4f));
		modelGardenTab = glm::rotate(modelGardenTab, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGardenTab = glm::scale(modelGardenTab, glm::vec3(0.15f, 0.15f, 0.15f));
		
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGardenTab));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		gardenTab.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		// Diujo del perro 
		glm::mat4 modelDog(1);

		modelDog = glm::translate(modelDog, glm::vec3(3.0f, 0.33f, -5.0f));
		modelDog = glm::rotate(modelDog, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelDog = glm::scale(modelDog, glm::vec3(0.5f, 0.5f, 0.5f));
		
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		dog.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//Dibujamos gato
		glm::mat4 modelCat(1);

		modelCat = glm::translate(modelCat, glm::vec3(-3.0f, 0.1f, 0.0f));
		modelCat = glm::rotate(modelCat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCat = glm::scale(modelCat, glm::vec3(0.3f, 0.3f, 0.3f));

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCat));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		cat.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//Dibujamos conejo
		glm::mat4 modelCon(1);

		modelCon = glm::mat4(1);
		modelCon = glm::translate(modelCon, glm::vec3(3.0f, 0.1f, 0.0f)); // Mover a la derecha
		modelCon = glm::scale(modelCon, glm::vec3(1.0f, 1.0f, 1.0f));

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCon));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		conejo.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//DIBUJO DE LOS MODELOS PARA LAS ILUMINACIONES

		//Dibujo del poste de luz 1
		glm::mat4 modelPoste(1);
		
		modelPoste = glm::mat4(1);
		
		modelPoste = glm::mat4(1);
		modelPoste = glm::translate(modelPoste, glm::vec3(-7.0f, 0.56f, 0.0f)); 
		modelPoste= glm::rotate(modelPoste, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPoste = glm::scale(modelPoste, glm::vec3(1.0f, 1.0f, 1.0f));

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPoste));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		posLuz.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//Dibujo del poste de luz 2
		glm::mat4 modelPoste2(1);

		modelPoste2 = glm::mat4(1);
		modelPoste2 = glm::translate(modelPoste2, glm::vec3(-7.0f, 0.56f, -9.5f));
		modelPoste2 = glm::rotate(modelPoste2, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPoste2 = glm::scale(modelPoste2, glm::vec3(1.0f, 1.0f, 1.0f));

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPoste2));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		posLuz.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//Dibujo de la lampara de jardín 1
		glm::mat4 modelLampG(1);

		modelLampG = glm::mat4(1);
		modelLampG = glm::translate(modelLampG, glm::vec3(1.0f, 0.1f, -2.2f)); 
		modelLampG = glm::scale(modelLampG, glm::vec3(0.5f, 0.5f, 0.5f));

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampG));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		lampGarden.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//Dibujo de la lampara de jardín 2
		glm::mat4 modelLampG2(1);

		modelLampG2 = glm::mat4(1);
		modelLampG2 = glm::translate(modelLampG2, glm::vec3(1.0f, 0.1f, -8.5f)); 
		modelLampG2 = glm::scale(modelLampG2, glm::vec3(0.5f, 0.5f, 0.5f));

		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampG2));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		lampGarden.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		/*for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[0]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
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

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.001f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.001f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.001f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.001f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.001f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.001f;
	}

}

// Is called whenever a key is pressed/released via GLFW
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

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
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
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}