#include "Engine.h"
#include <Model.h>

const unsigned int width = 800;
const unsigned int height = 800;

int main()
{
	glfwInit();

	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile, so no deprecated functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Game Engine", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();
	// Tell opengl the size of the rendering window
	glViewport(0, 0, width, height);


	Engine engine(width, height, window);
	engine.StartInternal();

	while (!glfwWindowShouldClose(window))
	{
		engine.UpdateInternal();
	}

	engine.QuitInternal();

	return 0;
}