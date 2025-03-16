#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GameObject.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camera : public GameObject
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	GLFWwindow* m_window;

	bool firstClick = true;

	int width;
	int height;

	int speed = 150000;
	float sensitivity = 100.0f;

	Camera(ObjectStorage* storage, int width, int height, glm::vec3 position, GLFWwindow* window);

	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);
	void Inputs(float deltaTime);

	void Update(float deltaTime) override;

	void SetDimensions(unsigned int width, unsigned int height);
};

#endif