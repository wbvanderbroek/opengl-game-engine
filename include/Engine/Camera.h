#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Engine/GameObject.h>
#include <Engine/Shader.h>

class Camera : public Component, public std::enable_shared_from_this<Camera>
{
private:
	bool m_mouseLocked = false;

public:
	Camera();

	glm::vec3 m_upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 m_cameraMatrix = glm::mat4(1.0f);

	float m_fieldOfView = 45.0f;
	float m_nearPlane = 0.1f;
	float m_farPlane = 500.0f;
	float m_mouseSensitivity = 100.0f;

	int m_windowWidth;
	int m_windowHeight;
	int m_movementSpeed = 15;

	bool m_firstMouseClick = true;

	GLFWwindow* m_window;

	void UpdateMatrix();
	void Matrix(Shader& shader, const char* uniform);
	void Inputs(float deltaTime);
	void SetDimensions(unsigned int width, unsigned int height);

	void Awake() override;
	void Update(float deltaTime) override;

	inline void SetFOV(float fov) { m_fieldOfView = fov; UpdateMatrix(); }
	inline void SetNearClip(float nearClip) { m_nearPlane = nearClip; UpdateMatrix(); }
	inline void SetFarClip(float farClip) { m_farPlane = farClip; UpdateMatrix(); }

	inline float GetFOV() const { return m_fieldOfView; }
	inline float GetNearClip() const { return m_nearPlane; }
	inline float GetFarClip() const { return m_farPlane; }
};