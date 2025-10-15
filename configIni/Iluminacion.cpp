//Arely Olvera González
//Práctica 8
//Fecha de entrega: 15/10/25
//No. de cuenta: 319209608

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes- Sol (día)
glm::vec3 lightPosSun(0.0f, 5.0f, 0.0f);
float angleSun = 0.0f;
float radiusSun = 30.0f;
bool animateSun = false;

// Light attributes- Luna (noche)
glm::vec3 lightPosMoon(0.0f, 5.0f, 0.0f);
float angleMoon = 180.0f; // Inicia en posición opuesta al sol
float radiusMoon = 30.0f;
bool animateMoon = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 Arely Olvera", nullptr, nullptr);

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

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    //CARGA DE MODELOS REFERENTES A LA PRACTICA 6 
    // (ESCENARIO JARDÍN)

    Model garden((char*)"Models/garden_Scene.obj");

    //Carga del modelo(perro)
    Model dog((char*)"Models/RedDog.obj");

    //Carga del modelo (gato)
    Model cat((char*)"Models/CAT_02.obj");

    //Carga del PRIMER modelo  (juego infantil)
    Model playground((char*)"Models/Playground.obj");

    //Carga del SEGUNDO modelo  (triciclo)
    Model tricycle((char*)"Models/Tricycle.obj");

    //Craga del TERCER modelo (alberca)
    Model pool((char*)"Models/Pool.obj");

    //Carga del CUARTO modelo (asador)
    Model grill((char*)"Models/Grill.obj");

    //Carga del QUINTO modelo (mesa de jardín)
    Model gardenTab((char*)"Models/Garden_Table.obj");

    //CARGA DE MODELOS PARA LAS DOS ILUMINACIONES
    //Carga del modelo (sol)
    Model sol((char*)"Models/Sun.obj");

    //Carga del modelo (luna)
    Model luna((char*)"Models/Moon.obj");

    // Carga del modelo del conejo
    Model conejo((char*)"Models/Conejo.obj");



    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

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

    // Load textures
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;

    // textura del perro
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);


    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
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
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Posiciones de las luces con trayectorias circulares
        if (animateSun)
        {
            angleSun += 30.0f * deltaTime;
            if (angleSun >= 360.0f) angleSun -= 360.0f;
        }

        if (animateMoon)
        {
            angleMoon += 25.0f * deltaTime;
            if (angleMoon >= 360.0f) angleMoon -= 360.0f;
        }
        // Calcular posiciones en trayectorias semicirculares (solo para visualizar en la parte superior)
        lightPosSun.x = radiusSun * cos(glm::radians(angleSun));
        lightPosSun.y = abs(radiusSun * sin(glm::radians(angleSun))); // abs() mantiene Y positivo
        lightPosSun.z = -5.0f;

        // Trayectoria de la Luna
        lightPosMoon.x = radiusMoon * cos(glm::radians(angleMoon));
        lightPosMoon.y = abs(radiusMoon * sin(glm::radians(angleMoon))); // abs() mantiene Y positivo
        lightPosMoon.z = 5.0f;

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calcular intensidad según posición Y
        float sunIntensity = glm::clamp(lightPosSun.y / radiusSun, 0.0f, 1.0f);
        float moonIntensity = glm::clamp(lightPosMoon.y / radiusMoon, 0.0f, 1.0f);

        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

        // Determinar cuál luz es dominante
        glm::vec3 dominantLightPos;
        glm::vec3 ambientColor, diffuseColor;

        if (sunIntensity > moonIntensity)
        {
            // Día - luz del sol
            dominantLightPos = lightPosSun;
            ambientColor = glm::vec3(0.6f, 0.6f, 0.5f) * sunIntensity;
            diffuseColor = glm::vec3(1.0f, 0.95f, 0.8f) * sunIntensity;
        }
        else
        {
            // Noche - luz de la luna
            dominantLightPos = lightPosMoon;
            ambientColor = glm::vec3(0.3f, 0.3f, 0.25f) * moonIntensity;
            diffuseColor = glm::vec3(0.3f, 0.3f, 0.5f) * moonIntensity;
        }

        glUniform3f(lightPosLoc, dominantLightPos.x, dominantLightPos.y, dominantLightPos.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        //DIBUJO DE LOS MODELOS

        //Dibujo del jardín (escenario)
        glm::mat4 modelGarden(1);

        modelGarden = glm::translate(modelGarden, glm::vec3(-3.0f, 0.0f, 0.0f));
        modelGarden = glm::rotate(modelGarden, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGarden));
        garden.Draw(lightingShader);

        //Dibujo de juego infantil
        glm::mat4 modelPlayground(1);

        modelPlayground = glm::translate(modelPlayground, glm::vec3(-1.0f, 0.1f, 3.0));
        modelPlayground = glm::rotate(modelPlayground, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPlayground));
        playground.Draw(lightingShader);

        //Dibujo de un triciclo
        glm::mat4 modelTricycle(1);

        modelTricycle = glm::translate(modelTricycle, glm::vec3(-1.0f, 0.16f, 0.0f));
        modelTricycle = glm::rotate(modelTricycle, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelTricycle = glm::scale(modelTricycle, glm::vec3(0.01f, 0.01f, 0.01f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTricycle));
        tricycle.Draw(lightingShader);

        //Dibujo de una alberca
        glm::mat4 modelPool(1);

        modelPool = glm::translate(modelPool, glm::vec3(5.0f, 0.39f, -9.5f));
        modelPool = glm::rotate(modelPool, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelPool = glm::scale(modelPool, glm::vec3(0.2f, 0.11f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPool));
        pool.Draw(lightingShader);

        //Dibujo de un asador
        glm::mat4 modelGrill(1);

        modelGrill = glm::translate(modelGrill, glm::vec3(8.0f, 0.11f, -9.5f));
        modelGrill = glm::rotate(modelGrill, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelGrill = glm::scale(modelGrill, glm::vec3(6.0f, 6.0f, 6.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGrill));
        grill.Draw(lightingShader);

        //Dibujo de una mesa de jardín
        glm::mat4 modelGardenTab(1);

        modelGardenTab = glm::translate(modelGardenTab, glm::vec3(10.0f, 0.1f, -9.4f));
        modelGardenTab = glm::rotate(modelGardenTab, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelGardenTab = glm::scale(modelGardenTab, glm::vec3(0.15f, 0.15f, 0.15f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGardenTab));
        gardenTab.Draw(lightingShader);

        // Diujo del perro 
        glm::mat4 model(1);

        model = glm::translate(model, glm::vec3(3.0f, 0.33f, -5.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        //Dibujamos gato
        glm::mat4 modelCat(1);

        modelCat = glm::translate(modelCat, glm::vec3(-3.0f, 0.1f, 0.0f));
        modelCat = glm::rotate(modelCat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelCat = glm::scale(modelCat, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCat));
        cat.Draw(lightingShader);

        //Dibujamos conejo
        glm::mat4 modelCon(1);

        modelCon = glm::mat4(1);
        modelCon = glm::translate(modelCon, glm::vec3(3.0f, 0.1f, 0.0f)); // Mover a la derecha
        modelCon = glm::scale(modelCon, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCon));
        conejo.Draw(lightingShader);

        //DIBUJO DE MODELOS PARA LAS DOS ILUMINACIONES
        //Dibujo del sol
        glm::mat4 modelSun(1);

        modelSun = glm::translate(modelSun, lightPosSun);
        modelSun = glm::scale(modelSun, glm::vec3(1.3f, 1.3f, 1.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSun));
        sol.Draw(lightingShader);

        //Dibujo de la luna 

        glm::mat4 modelMoon(1);
        modelMoon = glm::translate(modelMoon, lightPosMoon);
        modelMoon = glm::scale(modelMoon, glm::vec3(1.3f, 1.3f, 1.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMoon));
        luna.Draw(lightingShader);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
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
    // Tecla O: Activar/desactivar animación del sol
    if (keys[GLFW_KEY_O] && action == GLFW_PRESS)
    {
        animateSun = !animateSun;
    }
    // Tecla L: Activar/desactivar animación de la luna
    if (keys[GLFW_KEY_L] && action == GLFW_PRESS)
    {
        animateMoon = !animateMoon;
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


