#include <Camera.h>

Camera::Camera(int width, int height)
	: width(width), height(height)
{
	m_window = glfwGetCurrentContext();
	Camera::width = width;
	Camera::height = height;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(translation, translation + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), static_cast<float>(width) / static_cast<float>(height), nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		translation += deltaTime * speed * Orientation;
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		translation += deltaTime * speed * -glm::normalize(glm::cross(Orientation, Up));
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		translation += deltaTime * speed * -Orientation;
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		translation += deltaTime * speed * glm::normalize(glm::cross(Orientation, Up));;


	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		translation += deltaTime * speed * Up;
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		translation += deltaTime * speed * -Up;
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 30;
	else if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		speed = 15;

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(m_window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;

		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		glfwSetCursorPos(m_window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}

void Camera::Update(float deltaTime)
{
	Inputs(deltaTime);
	updateMatrix(45.0f, 0.1f, 300.0f);
}

void Camera::SetDimensions(unsigned int newWidth, unsigned int newHeight)
{
	width = newWidth;
	height = newHeight;
	updateMatrix(45.0f, 0.1f, 300.0f);
}