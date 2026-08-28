#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D", NULL, NULL);
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

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Vértices de la cara (con Barbilla en Triángulo) + Ojos + Nariz
    float vertices[] = {
        // Base de la Cabeza y Orejas
        -0.8f,  0.7f, 0.0f,      0.75f, 0.75f, 0.75f, // V0
        -0.5f,  0.8f, 0.0f,      0.75f, 0.75f, 0.75f, // V1
        -0.3f,  0.6f, 0.0f,      0.75f, 0.75f, 0.75f, // V2
         0.3f,  0.6f, 0.0f,      0.75f, 0.75f, 0.75f, // V3
         0.5f,  0.8f, 0.0f,      0.75f, 0.75f, 0.75f, // V4
         0.8f,  0.7f, 0.0f,      0.75f, 0.75f, 0.75f, // V5
         0.8f,  0.0f, 0.0f,      0.75f, 0.75f, 0.75f, // V6
         0.5f, -0.6f, 0.0f,      0.75f, 0.75f, 0.75f, // V7
         0.0f, -0.3f, 0.0f,      0.75f, 0.75f, 0.75f, // V8
        -0.5f, -0.6f, 0.0f,      0.75f, 0.75f, 0.75f, // V9
        -0.8f,  0.0f, 0.0f,      0.75f, 0.75f, 0.75f, // V10

        // Ojo Izquierdo
        -0.45f,  0.3f, 0.0f,     0.10f, 0.10f, 0.10f, // V11
        -0.25f,  0.3f, 0.0f,     0.10f, 0.10f, 0.10f, // V12
        -0.35f,  0.1f, 0.0f,     0.10f, 0.10f, 0.10f, // V13

        // Ojo Derecho
         0.25f,  0.3f, 0.0f,     0.10f, 0.10f, 0.10f, // V14
         0.45f,  0.3f, 0.0f,     0.10f, 0.10f, 0.10f, // V15
         0.35f,  0.1f, 0.0f,     0.10f, 0.10f, 0.10f, // V16

         // Nariz Hexagonal
         -0.12f,  0.10f, 0.0f,    0.20f, 0.20f, 0.20f, // V17
          0.12f,  0.10f, 0.0f,    0.20f, 0.20f, 0.20f, // V18
          0.18f, -0.05f, 0.0f,    0.20f, 0.20f, 0.20f, // V19
          0.00f, -0.22f, 0.0f,    0.20f, 0.20f, 0.20f, // V20
         -0.18f, -0.05f, 0.0f,    0.20f, 0.20f, 0.20f  // V21
    };

    // Índices para armar los triángulos
    unsigned int indices[] = {
        // --- CABEZA Y OREJAS ---
        0, 10, 1,
        1, 10, 2,
        3, 6, 4,
        4, 6, 5,
        2, 3, 7,
        2, 7, 9,
        7, 8, 9,
        9, 10, 2,
        3, 6, 7,

        // --- OJOS ---
        11, 12, 13,
        14, 15, 16,

        // --- NARIZ ---
        17, 18, 20,
        17, 20, 21,
        18, 19, 20
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

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);

        // 14 triángulos (42 índices)
        glDrawElements(GL_TRIANGLES, 42, GL_UNSIGNED_INT, 0);

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