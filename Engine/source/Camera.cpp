#include <Camera.h>
#include <Engine.h>

Camera::Camera()
{
	m_window = glfwGetCurrentContext();
	Camera::m_windowWidth = 800;
	Camera::m_windowHeight = 800;
}

void Camera::UpdateMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(translation, translation + m_orientation, m_upDirection);
	projection = glm::perspective(glm::radians(m_fieldOfView), static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), m_nearPlane, m_farPlane);

	m_cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.m_id, uniform), 1, GL_FALSE, glm::value_ptr(m_cameraMatrix));
}

void Camera::Inputs(float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		translation += deltaTime * m_movementSpeed * m_orientation;
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		translation += deltaTime * m_movementSpeed * -glm::normalize(glm::cross(m_orientation, m_upDirection));
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		translation += deltaTime * m_movementSpeed * -m_orientation;
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		translation += deltaTime * m_movementSpeed * glm::normalize(glm::cross(m_orientation, m_upDirection));;


	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		translation += deltaTime * m_movementSpeed * m_upDirection;
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		translation += deltaTime * m_movementSpeed * -m_upDirection;
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_movementSpeed = 30;
	else if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		m_movementSpeed = 15;

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (m_firstMouseClick)
		{
			glfwSetCursorPos(m_window, (m_windowWidth / 2), (m_windowHeight / 2));
			m_firstMouseClick = false;
		}

		double mouseX;
		double mouseY;

		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		float rotX = m_mouseSensitivity * (float)(mouseY - (m_windowHeight / 2)) / m_windowHeight;
		float rotY = m_mouseSensitivity * (float)(mouseX - (m_windowWidth / 2)) / m_windowWidth;

		glm::vec3 newOrientation = glm::rotate(m_orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_orientation, m_upDirection)));

		if (abs(glm::angle(newOrientation, m_upDirection) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			m_orientation = newOrientation;
		}

		m_orientation = glm::rotate(m_orientation, glm::radians(-rotY), m_upDirection);

		glfwSetCursorPos(m_window, (m_windowWidth / 2), (m_windowHeight / 2));
	}
	else if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_firstMouseClick = true;
	}
}

void Camera::OnCreate()
{
	m_storage->m_engine->m_camera = shared_from_this();
}

void Camera::Update(float deltaTime)
{
	Inputs(deltaTime);
	UpdateMatrix();
}

void Camera::SetDimensions(unsigned int newWidth, unsigned int newHeight)
{
	m_windowWidth = newWidth;
	m_windowHeight = newHeight;
	UpdateMatrix();
}