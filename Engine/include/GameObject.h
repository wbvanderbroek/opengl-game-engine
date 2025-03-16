#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <memory>
#include <ObjectStorage.h>

class ObjectStorage;
class GameObject
{
protected:
	ObjectStorage* m_storage;
public:
	explicit GameObject(ObjectStorage* storage) : m_storage(storage) {}

	virtual ~GameObject() = default;
	virtual void OnCreate() {}
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void LateUpdate(float deltaTime) {}
	virtual void OnDestroy() {}
	virtual void OnQuit() {}

	void Destroy();

	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	void SetRotation(glm::vec3 rotAngles)
	{
		glm::quat qx = glm::angleAxis(glm::radians(rotAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat qy = glm::angleAxis(glm::radians(rotAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat qz = glm::angleAxis(glm::radians(rotAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::quat q = qz * qy * qx;
		this->rotation = q * this->rotation;
		this->rotation = glm::normalize(this->rotation);

	}
	glm::vec3 GetRotation() const
	{
		glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
		return euler;
	}
};