#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	if (window == NULL)
	{
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

	// Vértices del Koala en Origami (Posición X, Y, Z + Color R, G, B)
	float vertices[] = {
		// ==========================================
		// 1. OREJA IZQUIERDA (Gris Oscuro - 2 Triángulos)
		// ==========================================
		-0.30f,  0.35f, 0.0f,   0.55f, 0.57f, 0.60f,
		-0.75f,  0.65f, 0.0f,   0.55f, 0.57f, 0.60f,
		-0.30f,  0.65f, 0.0f,   0.55f, 0.57f, 0.60f,

		-0.30f,  0.35f, 0.0f,   0.55f, 0.57f, 0.60f,
		-0.80f,  0.20f, 0.0f,   0.55f, 0.57f, 0.60f,
		-0.75f,  0.65f, 0.0f,   0.55f, 0.57f, 0.60f,

		// ==========================================
		// 2. OREJA DERECHA (Gris Oscuro - 2 Triángulos)
		// ==========================================
		 0.30f,  0.35f, 0.0f,   0.55f, 0.57f, 0.60f,
		 0.30f,  0.65f, 0.0f,   0.55f, 0.57f, 0.60f,
		 0.75f,  0.65f, 0.0f,   0.55f, 0.57f, 0.60f,

		 0.30f,  0.35f, 0.0f,   0.55f, 0.57f, 0.60f,
		 0.75f,  0.65f, 0.0f,   0.55f, 0.57f, 0.60f,
		 0.80f,  0.20f, 0.0f,   0.55f, 0.57f, 0.60f,

		 // ==========================================
		 // 3. CARA BASE (Gris Claro - 5 Triángulos)
		 // ==========================================
		 // Triángulo Central Principal
		 -0.30f,  0.35f, 0.0f,   0.70f, 0.72f, 0.75f,
		  0.30f,  0.35f, 0.0f,   0.70f, 0.72f, 0.75f,
		  0.00f, -0.45f, 0.0f,   0.70f, 0.72f, 0.75f,

		  // Lateral Izquierdo Superior
		  -0.30f,  0.35f, 0.0f,   0.70f, 0.72f, 0.75f,
		   0.00f, -0.45f, 0.0f,   0.70f, 0.72f, 0.75f,
		  -0.80f, -0.10f, 0.0f,   0.70f, 0.72f, 0.75f,

		  // Lateral Derecho Superior
		   0.30f,  0.35f, 0.0f,   0.70f, 0.72f, 0.75f,
		   0.80f, -0.10f, 0.0f,   0.70f, 0.72f, 0.75f,
		   0.00f, -0.45f, 0.0f,   0.70f, 0.72f, 0.75f,

		   // Barbilla Izquierda
		   -0.80f, -0.10f, 0.0f,   0.70f, 0.72f, 0.75f,
			0.00f, -0.45f, 0.0f,   0.70f, 0.72f, 0.75f,
		   -0.40f, -0.45f, 0.0f,   0.70f, 0.72f, 0.75f,

		   // Barbilla Derecha
			0.80f, -0.10f, 0.0f,   0.70f, 0.72f, 0.75f,
			0.40f, -0.45f, 0.0f,   0.70f, 0.72f, 0.75f,
			0.00f, -0.45f, 0.0f,   0.70f, 0.72f, 0.75f,

			// ==========================================
			// 4. NARIZ RECTANGULAR (Negro - 2 Triángulos)
			// ==========================================
			-0.08f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			 0.08f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			 0.08f, -0.42f, 0.0f,   0.10f, 0.10f, 0.10f,

			-0.08f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			 0.08f, -0.42f, 0.0f,   0.10f, 0.10f, 0.10f,
			-0.08f, -0.42f, 0.0f,   0.10f, 0.10f, 0.10f,

			// ==========================================
			// 5. OJO IZQUIERDO CUADRADO (Negro - 2 Triángulos)
			// ==========================================
			-0.45f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			-0.23f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			-0.23f, -0.27f, 0.0f,   0.10f, 0.10f, 0.10f,

			-0.45f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			-0.23f, -0.27f, 0.0f,   0.10f, 0.10f, 0.10f,
			-0.45f, -0.27f, 0.0f,   0.10f, 0.10f, 0.10f,

			// ==========================================
			// 6. OJO DERECHO CUADRADO (Negro - 2 Triángulos)
			// ==========================================
			 0.23f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			 0.45f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			 0.45f, -0.27f, 0.0f,   0.10f, 0.10f, 0.10f,

			 0.23f, -0.05f, 0.0f,   0.10f, 0.10f, 0.10f,
			 0.45f, -0.27f, 0.0f,   0.10f, 0.10f, 0.10f,
			 0.23f, -0.27f, 0.0f,   0.10f, 0.10f, 0.10f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Fondo original de la plantilla
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();
		glBindVertexArray(VAO);

		// 15 triángulos = 45 vértices en total
		glDrawArrays(GL_TRIANGLES, 0, 45);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}