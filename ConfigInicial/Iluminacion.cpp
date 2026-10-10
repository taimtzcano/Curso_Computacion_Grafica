// Practica 8                                Martinez Cano Tania
// Fecha de Entrega: 9 de Octubre            320028603

// Std. Includes
#include <string>
#include <iostream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
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
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Variables para el movimiento del Sol y Luna
float sunAngle = 1.57f;  // Ángulo inicial (Sol arriba)
float sunRadius = 34.0f; // Distancia del recorrido

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 Martinez Cano Tania", nullptr, nullptr);

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

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Setup shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model gato((char*)"Models/CAT+02.obj");
    Model pollo((char*)"Models/chicken_001.obj");
    Model granja((char*)"Models/LowPoly_FarmReady_blenderobj.obj");
    Model peach((char*)"Models/PeachOBJ.obj");
    Model caballo((char*)"Models/horse_001.obj");
    Model gatito((char*)"Models/kitty_001.obj");
    Model sol((char*)"Models/Style+Sun_v1_001.obj");
    Model luna((char*)"Models/MOON05.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // Fondo gris estático
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // CALCULO DE POSICIÓN DEL SOL Y LA LUNA
        glm::vec3 farmCenter = glm::vec3(-1.5f, -0.45f, -1.0f);

        // Posición del Sol
        glm::vec3 sunPos;
        sunPos.x = farmCenter.x;
        sunPos.y = farmCenter.y + sin(sunAngle) * sunRadius;
        sunPos.z = farmCenter.z + cos(sunAngle) * sunRadius;

        // Posición de la Luna (Opuesta a 180° del Sol)
        float moonAngle = sunAngle + 3.14159f;
        glm::vec3 moonPos;
        moonPos.x = farmCenter.x;
        moonPos.y = farmCenter.y + sin(moonAngle) * sunRadius;
        moonPos.z = farmCenter.z + cos(moonAngle) * sunRadius;

        // Determinar si el Sol está arriba
        bool isDay = (sunPos.y > farmCenter.y);

        glm::mat4 view = camera.GetViewMatrix();

        // 1. DIBUJAR MODELOS CON ILUMINACIÓN
        lightingShader.Use();

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "viewPos"), 1, glm::value_ptr(camera.GetPosition()));

        // Material
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // CONFIGURACIÓN DE LUZ SEGÚN EL DÍA O LA NOCHE
        if (isDay)
        {
            // Luz Principal (Sol - Intensa y Cálida)
            glUniform3fv(glGetUniformLocation(lightingShader.Program, "light.position"), 1, glm::value_ptr(sunPos));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 0.95f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
        }
        else
        {
            // Luz Principal (Luna - Iluminación azulada ligeramente más clara)
            glUniform3fv(glGetUniformLocation(lightingShader.Program, "light.position"), 1, glm::value_ptr(moonPos));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.12f, 0.12f, 0.18f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.35f, 0.38f, 0.45f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.25f, 0.25f, 0.35f);
        }

        // Luz 2 (Luz secundaria de relleno)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), 0.0f, 2.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.1f, 0.1f, 0.1f);

        // Perro
        glm::mat4 model(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // Gato
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 1.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        gato.Draw(lightingShader);

        // Pollo 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pollo.Draw(lightingShader);

        // Caballo
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, -0.5f, 3.0f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        caballo.Draw(lightingShader);

        // Gatito
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -0.4f, 2.5f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        gatito.Draw(lightingShader);

        // Pollo 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, -0.5f, 1.5f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pollo.Draw(lightingShader);

        // Pollo 3
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 3.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pollo.Draw(lightingShader);

        // Pollo 4
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, -0.5f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pollo.Draw(lightingShader);

        // Pollo 5
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pollo.Draw(lightingShader);

        // Peach 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -0.5f, 2.0f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        peach.Draw(lightingShader);

        // Peach 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, -0.5f, -2.0f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        peach.Draw(lightingShader);

        // Granja
        model = glm::mat4(1.0f);
        model = glm::translate(model, farmCenter);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        granja.Draw(lightingShader);

 
        lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Dibujar Sol
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 0.9f, 0.3f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, sunPos);
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sol.Draw(lampShader);

        // Dibujar Luna
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 0.85f, 0.88f, 1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, moonPos);
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        luna.Draw(lampShader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    // Movimiento de Cámara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    // Teclas J y L para mover el Sol/Luna
    float sunSpeed = 1.2f * deltaTime;
    if (keys[GLFW_KEY_J]) sunAngle -= sunSpeed;
    if (keys[GLFW_KEY_L]) sunAngle += sunSpeed;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
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