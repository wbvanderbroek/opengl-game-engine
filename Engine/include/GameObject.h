#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Component.h>
#include <ObjectStorage.h>

class ObjectStorage;
class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	ObjectStorage* m_storage = nullptr;

	glm::vec3 rotationInRads = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	std::vector<std::shared_ptr<GameObject>> m_children;
	std::weak_ptr<GameObject> m_parent;
	std::vector<std::shared_ptr<Component>> m_components;

	virtual void OnCreate()
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor)
				component->Awake();
		}
	}

	virtual void Start()
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor)
				component->Start();
		}
	}

	virtual void Update(float deltaTime)
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor)
				component->Update(deltaTime);
		}
	}

	virtual void LateUpdate(float deltaTime)
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor)
				component->LateUpdate(deltaTime);
		}
	}

	virtual void OnDestroy()
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor)
				component->OnDestroy();
		}
	}

	virtual void OnQuit()
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor)
				component->OnQuit();
		}
	}

	void SetStorage(ObjectStorage* storage) { m_storage = storage; }

	void Destroy()
	{
		for (auto& child : m_children)
			child->Destroy();

		if (m_storage)
			m_storage->RemoveGameObject(shared_from_this());
	}

	void SetRotation(const glm::vec3& degrees) { rotationInRads = glm::radians(degrees); }

	glm::vec3 GetRotation() const { return glm::degrees(rotationInRads); }

	template<typename T>
	void AddComponent(T&& componentToAdd)
	{
		auto component = std::make_shared<std::decay_t<T>>(std::forward<T>(componentToAdd));
		component->SetGameObject(shared_from_this());
		m_components.push_back(component);
		component->Awake();
	}

	void AddChild(std::shared_ptr<GameObject> child)
	{
		child->m_parent = shared_from_this();
		child->m_storage = m_storage;
		m_children.push_back(child);
	}

	glm::mat4 GetObjectMatrix() {
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