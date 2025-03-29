#pragma once

#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <ObjectStorage.h>

class ObjectStorage;
class GameObject
{
private:
	glm::vec3 rotationInRads = glm::vec3(0.0f, 0.0f, 0.0f);

protected:
	ObjectStorage* m_storage = nullptr;

public:
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	virtual void OnCreate() {}
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void LateUpdate(float deltaTime) {}
	virtual void OnDestroy() {}
	virtual void OnQuit() {}

	void SetStorage(ObjectStorage* storage) { m_storage = storage; }
	void Destroy() { if (m_storage) m_storage->RemoveGameObject(std::shared_ptr<GameObject>(this, [](GameObject*) {})); }
	void SetRotation(const glm::vec3& degrees) { rotationInRads = glm::radians(degrees); }
	glm::vec3 GetRotation() const { return glm::degrees(rotationInRads); }

	glm::mat4 GetModelMatrix() {
		const float c3 = glm::cos(rotationInRads.z);
		const float s3 = glm::sin(rotationInRads.z);
		const float c2 = glm::cos(rotationInRads.x);
		const float s2 = glm::sin(rotationInRads.x);
		const float c1 = glm::cos(rotationInRads.y);
		const float s1 = glm::sin(rotationInRads.y);
		return glm::mat4{
			{
				scale.x * (c1 * c3 + s1 * s2 * s3),
				scale.x * (c2 * s3),
				scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				scale.y * (c3 * s1 * s2 - c1 * s3),
				scale.y * (c2 * c3),
				scale.y * (c1 * c3 * s2 + s1 * s3),
				0.0f,
			},
			{
				scale.z * (c2 * s1),
				scale.z * (-s2),
				scale.z * (c1 * c2),
				0.0f,
			},
			{translation.x, translation.y, translation.z, 1.0f} };
	}
};