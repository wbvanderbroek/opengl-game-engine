#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
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

	void SetRotation(const glm::vec3& eulerAngles)
	{
		// Wikipedia calculations of euler to quaternion
		glm::vec3 rads = glm::radians(eulerAngles);

		double cr = cos(rads.x * 0.25);
		double sr = sin(rads.x * 0.25);
		double cp = cos(rads.y * 0.25);
		double sp = sin(rads.y * 0.25);
		double cy = cos(rads.z * 0.25);
		double sy = sin(rads.z * 0.25);

		glm::quat q;
		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;

		this->rotation = q;

		//std::cout << "Quaternion: ("
		//	<< rotation.w << ", "
		//	<< rotation.x << ", "
		//	<< rotation.y << ", "
		//	<< rotation.z << ")\n";
	}


	glm::vec3 GetRotation() const
	{
		glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));

		if (glm::abs(euler.x) < 0.0001f) euler.x = 0.0f;
		if (glm::abs(euler.y) < 0.0001f) euler.y = 0.0f;
		if (glm::abs(euler.z) < 0.0001f) euler.z = 0.0f;

		euler.x = fmod(euler.x + 360.0f, 360.0f);
		euler.y = fmod(euler.y + 360.0f, 360.0f);
		euler.z = fmod(euler.z + 360.0f, 360.0f);

		return euler;
	}
};