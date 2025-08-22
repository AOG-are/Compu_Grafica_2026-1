//Arely Olvera González
//Práctica 2
//Fecha de entrega: 20/08/25
//No. de cuenta: 319209608

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <vector>

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 Arely Olvera", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, resize);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Patrón del logo de Batman (1 = amarillo, 0 = negro)
    
    int pattern[24][25] = {
        {0,0,0,0,0,0,0,0,0,1,1,1,1, 1, 1, 1, 0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,0,0,0,0,0,0},
        {0,0,0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,0,0,0,0},
        {0,0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0,0,0,0},
        {0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0,0,0},
        { 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0,0},
        { 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1,0},
        { 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1,0},
        { 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,1},
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,1},
        {1,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,1},
        {1,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,1},
        {1,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,1},
        {1,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1,0,0,0,0,1,1},
        {0,1,0,0,0,1,1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,1,1,0,0,0,1,0},
        {0,1,1,0,0,1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,1,1,0,0,1,1,0},
        {0,0,1,1,0,0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1,1,0,0,1,1,0,0},
        {0,0,0,1,1,0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1,0,0,1,1,0,0,0},
        {0,0,0,0,1,1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0,0,1,1,0,0,0,0},
        {0,0,0,0,0,1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,1,1,0,0,0,0,0 },
        {0,0,0,0,0,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0, 0, 1, 1, 1, 1, 1, 1, 1,0,0,0,0,0,0,0,0,0}
    };

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generar vértices e índices para cada píxel
    float pixelSize = 0.06f;  // Tamaño de cada píxel
    float startX = -0.72f;    // Posición inicial X (centrado)
    float startY = 0.72f;     // Posición inicial Y (centrado)

    unsigned int vertexIndex = 0;

    for (int row = 0; row < 24; row++) {
        for (int col = 0; col < 25; col++) {
            if (pattern[row][col] != -1) { // solo dibuja píxeles visibles
                float x = startX + col * pixelSize;
                float y = startY - row * pixelSize;

                // Determinamos color basado en el patrón
                float r, g, b;
                if (pattern[row][col] == 1) { // Amarillo
                    r = 1.0f; g = 0.84f; b = 0.0f;
                }
                else { // Negro
                    r = 0.0f; g = 0.0f; b = 0.0f;
                }

                // Crear un cuadrado (2 triángulos) para cada píxel
                // Vértice inferior izquierdo
                vertices.push_back(x); vertices.push_back(y - pixelSize); vertices.push_back(0.0f);
                vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);

                // Vértice inferior derecho
                vertices.push_back(x + pixelSize); vertices.push_back(y - pixelSize); vertices.push_back(0.0f);
                vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);

                // Vértice superior derecho
                vertices.push_back(x + pixelSize); vertices.push_back(y); vertices.push_back(0.0f);
                vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);

                // Vértice superior izquierdo
                vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0f);
                vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);

                // Índices para dos triángulos que forman un cuadrado
                // Primer triángulo
                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex + 1);
                indices.push_back(vertexIndex + 2);

                // Segundo triángulo
                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex + 2);
                indices.push_back(vertexIndex + 3);

                vertexIndex += 4;
            }
        }
    }

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Cargar datos de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Cargar datos de índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Configurar atributos de vértices
    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Fondo de la cuadrícula
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar 
        ourShader.Use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}