#include <Engine/Components/Camera.h>
#include <Engine/Engine.h>

Camera::Camera()
{
	m_runInEditor = true;
	m_window = glfwGetCurrentContext();

	// Set the aspect ratio to the current window's aspect ratio
	// (getWindowSize actually sets the variables you pass in)
	glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
}

void Camera::UpdateMatrix()
{
	if (!m_gameObject) return;

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	glm::vec3 forward = glm::rotateZ(glm::rotateY(glm::rotateX(glm::vec3(0.0f, 0.0f, -1.0f), m_gameObject->localRotation.x), m_gameObject->localRotation.y), m_gameObject->localRotation.z);

	glm::vec3 right = glm::normalize(glm::cross(forward, m_upDirection));

	view = glm::lookAt(m_gameObject->localPosition, m_gameObject->localPosition + forward, m_upDirection);
	projection = glm::perspective(glm::radians(m_fieldOfView), static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), m_nearPlane, m_farPlane);

	m_cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.m_id, uniform), 1, GL_FALSE, glm::value_ptr(m_cameraMatrix));
}

void Camera::Inputs(float deltaTime)
{
	glm::vec3 forward = glm::rotateZ(glm::rotateY(glm::rotateX(glm::vec3(0.0f, 0.0f, -1.0f), m_gameObject->localRotation.x), m_gameObject->localRotation.y), m_gameObject->localRotation.z);
	glm::vec3 right = glm::normalize(glm::cross(forward, m_upDirection));

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		m_gameObject->localPosition += deltaTime * m_movementSpeed * forward;
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		m_gameObject->localPosition -= deltaTime * m_movementSpeed * right;
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		m_gameObject->localPosition -= deltaTime * m_movementSpeed * forward;
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		m_gameObject->localPosition += deltaTime * m_movementSpeed * right;

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_gameObject->localPosition += deltaTime * m_movementSpeed * m_upDirection;
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		m_gameObject->localPosition -= deltaTime * m_movementSpeed * m_upDirection;

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_movementSpeed = 30;
	else
		m_movementSpeed = 15;

	static bool lastAltState = false;
	bool currentAltState = glfwGetKey(m_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;

	if (currentAltState && !lastAltState)
	{
		m_mouseLocked = !m_mouseLocked;
		if (m_mouseLocked)
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			m_firstMouseClick = true;
		}
		else
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	lastAltState = currentAltState;

	if (m_mouseLocked)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (m_firstMouseClick)
		{
			glfwSetCursorPos(m_window, (m_windowWidth / 2), (m_windowHeight / 2));
			m_firstMouseClick = false;
		}

		double mouseX, mouseY;
		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		float rotX = m_mouseSensitivity * static_cast<float>(mouseY - (m_windowHeight / 2)) / m_windowHeight;
		float rotY = m_mouseSensitivity * static_cast<float>(mouseX - (m_windowWidth / 2)) / m_windowWidth;

		m_gameObject->localRotation.x += glm::radians(-rotX);
		m_gameObject->localRotation.y += glm::radians(-rotY);


		m_gameObject->localRotation.x = glm::clamp(m_gameObject->localRotation.x, m_minPitch, m_maxPitch);

		glfwSetCursorPos(m_window, (m_windowWidth / 2), (m_windowHeight / 2));
	}
}

void Camera::Awake()
{
	m_gameObject->m_storage->m_engine->m_camera = shared_from_this();
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