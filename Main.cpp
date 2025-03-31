#include "Engine.h"
#include <Model.h>

const unsigned int width = 800;
const unsigned int height = 800;

bool isFullscreen = false;
bool f11Pressed = false;
int windowedX, windowedY, windowedWidth, windowedHeight;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (width == 0 || height == 0)
		return;

	glViewport(0, 0, width, height);

	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (engine) {
		engine->UpdateCameraSize(width, height);
	}
}

int main(int argc, char* argv[])
{
	std::cout << "Number of arguments: " << argc << std::endl;
	for (int i = 0; i < argc; ++i)
	{
		std::cout << "Argument " << i << ": " << argv[i] << std::endl;
	}
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

	glfwSwapInterval(0); // 0 = Disable VSync, 1 = Enable VSync
	gladLoadGL();

	// Tell opengl the size of the rendering window
	glViewport(0, 0, width, height);


	Engine engine(window, argc, argv);

	glfwSetWindowUserPointer(window, &engine);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	engine.StartInternal();

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
		{
			if (!f11Pressed)
			{
				f11Pressed = true;
				if (!isFullscreen)
				{
					glfwGetWindowPos(window, &windowedX, &windowedY);
					glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

					GLFWmonitor* monitor = glfwGetPrimaryMonitor();
					const GLFWvidmode* mode = glfwGetVideoMode(monitor);

					glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
				}
				else
				{
					glfwSetWindowMonitor(window, nullptr, windowedX, windowedY, windowedWidth, windowedHeight, 0);
				}
				isFullscreen = !isFullscreen;
			}
		}
		else
		{
			f11Pressed = false;
		}

		engine.UpdateInternal();
	}

	engine.QuitInternal();

	return 0;
}