//Práctica 3                            Martínez Cano Tania
//Fecha de Entrega: 3 de Septiembre     320028603

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica3_Tania_Martinez", nullptr, nullptr);

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
	// use with Orthographic Projection
	GLfloat vertices[] = {
	   -0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f, 0.0f, // Front
		0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f, 0.0f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f, 0.0f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f, 0.0f,
	   -0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f, 0.0f,
	   -0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f, 0.0f,

	   -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 0.0f, // Back
		0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 0.0f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 0.0f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 0.0f,
	   -0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 0.0f,
	   -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 0.0f,

		0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f, 1.0f,
		0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f, 1.0f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 0.0f, 1.0f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 0.0f, 1.0f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 0.0f, 1.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f, 1.0f,

	   -0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f, 0.0f,
	   -0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f, 0.0f,
	   -0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f, 0.0f,
	   -0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f, 0.0f,
	   -0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 1.0f, 0.0f,
	   -0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f, 0.0f,

	   -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 1.0f,
		0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 1.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f, 1.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f, 1.0f,
	   -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f, 1.0f,
	   -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f, 1.0f,

	   -0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 0.2f, 0.5f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 0.2f, 0.5f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f, 0.5f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f, 0.5f,
	   -0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f, 0.5f,
	   -0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 0.2f, 0.5f,
	};

	// use with Perspective Projection
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
	//	0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
	//	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,
	//	
	//    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
	//	 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//    -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
 //     
	//	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
	//	-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
	//	
	//	-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
	//	-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
	//	-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	//};


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	// Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Matriz de proyeccion ortogonal
	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);

	// Declaración de variables para transformaciones
	glm::mat4 model;
	glm::mat4 view;

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

		// Configurar vista y proyección
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(screenWidth / 2.0f, screenHeight / 2.0f, -500.0f));
		view = glm::rotate(view, 0.35f, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, 0.17f, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);

		// Cubo #1 (Base - Más grande)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, -180.0f, 0.0f));
		model = glm::rotate(model, 2.35f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cubo #2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0f, -85.0f, 0.0f));
		model = glm::rotate(model, -1.20f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cubo #3 (Mediano)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cubo #4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 75.0f, 0.0f));
		model = glm::rotate(model, -2.50f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cubo #5 (Cúspide - Más pequeño)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 140.0f, 0.0f));
		model = glm::rotate(model, 1.10f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}