#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "EBO.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"

GLfloat vertices[] = {
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower right corner
	0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // upper corner
	-0.5f / 2,  0.5f * float(sqrt(3)) / 6, 0.0f, // inner left
	0.5f / 2,  0.5f * float(sqrt(3)) / 6, 0.0f, // inner right
	0.0f,  -0.5f * float(sqrt(3)) / 3, 0.0f, // inner down
};

GLuint indices[] = {
	0, 3, 5, // lower left triangle
	3, 2, 4, // lower right triangle
	5, 4, 1 // upper triangle
};

int main()
{
	glfwInit();

	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile, so no deprecated functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Game Engine", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();
	// Tell opengl the size of the rendering window
	glViewport(0, 0, 800, 800);


	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();

		VAO1.Bind();

		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}