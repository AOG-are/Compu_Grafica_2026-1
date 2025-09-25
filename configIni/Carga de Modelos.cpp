//Arely Olvera González
//Práctica 6
//Fecha de entrega:  24/09/25
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
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Practica 6 Arely Olvera", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    Model dog((char*)"Models/RedDog.obj");

    //Carga del nuevo modelo (gato)
    Model cat((char*)"Models/CAT_02.obj");

    //Carga del nuevo modelo  (escenario jardín)
    Model garden((char*)"Models/garden_Scene.obj");

    //---------------5 MODELOS ADICIONALES--------------

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

    

    //Matriz de proyección
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    
   

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

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Dibujo del perro
        glm::mat4 model(1);
       
        model = glm::translate(model, glm::vec3(3.0f, 0.33f, -5.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        //Dibujamos gato
        glm::mat4 modelCat(1);
        modelCat = glm::translate(modelCat, glm::vec3(-3.0f, 0.1f, 0.0f));
        modelCat = glm::rotate(modelCat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelCat = glm::scale(modelCat, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCat));
        cat.Draw(shader);

       //Dibujo del jardín (escenario)
        glm::mat4 modelGarden(1);
        modelGarden = glm::translate(modelGarden, glm::vec3(-3.0f, 0.0f, 0.0f));
        modelGarden = glm::rotate(modelGarden, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGarden));
        garden.Draw(shader);

        //---------5 MODELOS ADICIONALES-------
        
        //Dibujo de juego infantil
        glm::mat4 modelPlayground(1);
        modelPlayground = glm::translate(modelPlayground, glm::vec3(-1.0f, 0.1f, 3.0));
        modelPlayground = glm::rotate(modelPlayground, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPlayground));
        playground.Draw(shader);

        //Dibujo de un triciclo
        glm::mat4 modelTricycle(1);
        modelTricycle = glm::translate(modelTricycle, glm::vec3(-1.0f, 0.16f, 0.0f));
        modelTricycle = glm::rotate(modelTricycle, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelTricycle = glm::scale(modelTricycle, glm::vec3(0.01f, 0.01f, 0.01f));
        
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTricycle));
        tricycle.Draw(shader);

        //Dibujo de una alberca
        glm::mat4 modelPool(1);
        modelPool = glm::translate(modelPool, glm::vec3(5.0f, 0.39f, -9.5f));
        modelPool = glm::rotate(modelPool, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelPool = glm::scale(modelPool, glm::vec3(0.2f, 0.11f, 0.2f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPool));
        pool.Draw(shader);

        //Dibujo de un asador

        glm::mat4 modelGrill(1);
        modelGrill = glm::translate(modelGrill, glm::vec3(8.0f, 0.11f, -9.5f));
        modelGrill = glm::rotate(modelGrill, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelGrill = glm::scale(modelGrill, glm::vec3(6.0f, 6.0f, 6.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGrill));
        grill.Draw(shader);
        
        //Dibujo de una mesa de jardín
        glm::mat4 modelGardenTab(1);
        modelGardenTab = glm::translate(modelGardenTab, glm::vec3(10.0f, 0.1f, -9.4f));
        modelGardenTab = glm::rotate(modelGardenTab, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelGardenTab = glm::scale(modelGardenTab, glm::vec3(0.15f, 0.15f, 0.15f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGardenTab));
        gardenTab.Draw(shader);

        

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

