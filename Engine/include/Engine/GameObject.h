#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <Engine/Components/Component.h>
#include <Engine/Config.h>
#include <Engine/ObjectStorage.h>

class ObjectStorage;

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	Config& m_config;

public:
	ObjectStorage* m_storage = nullptr;

	glm::vec3 localPosition = glm::vec3(0.0f);
	glm::vec3 localRotation = glm::vec3(0.0f); // In radians
	glm::vec3 localScale = glm::vec3(1.0f);

	std::vector<std::shared_ptr<GameObject>> m_children;
	std::weak_ptr<GameObject> m_parent;
	std::vector<std::shared_ptr<Component>> m_components;

	std::string m_name = "GameObject";

	GameObject() : m_config(Config::Instance()) {}

	virtual void OnCreate()
	{
		for (auto& child : m_children)
			child->OnCreate();
	}

	virtual void Start()
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor || m_config.m_mode == Mode::Game)
				component->Start();
		}
		for (auto& child : m_children)
			child->Start();
	}

	virtual void Update(float deltaTime)
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor || m_config.m_mode == Mode::Game)
				component->Update(deltaTime);
		}
		for (auto& child : m_children)
			child->Update(deltaTime);
	}

	virtual void LateUpdate(float deltaTime)
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor || m_config.m_mode == Mode::Game)
				component->LateUpdate(deltaTime);
		}
		for (auto& child : m_children)
			child->LateUpdate(deltaTime);
	}

	virtual void OnDestroy()
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor || m_config.m_mode == Mode::Game)
				component->OnDestroy();
		}
		for (auto& child : m_children)
			child->OnDestroy();
	}

	virtual void OnQuit()
	{
		for (auto& component : m_components)
		{
			if (component->m_runInEditor || m_config.m_mode == Mode::Game)
				component->OnQuit();
		}
		for (auto& child : m_children)
			child->OnQuit();
	}

	void SetStorage(ObjectStorage* storage) { m_storage = storage; }

	void Destroy()
	{
		for (auto& child : m_children)
			child->Destroy();

		if (m_storage)
			m_storage->RemoveGameObject(shared_from_this());


		if (m_config.m_mode == Mode::Editor)
			m_config.m_editorUI->m_selectedObject = nullptr;

	}

	void AddComponent(const std::shared_ptr<Component>& component)
	{
		component->SetGameObject(shared_from_this());
		m_components.push_back(component);
		if (component->m_runInEditor || m_config.m_mode == Mode::Game)
			component->Awake();
	}

	void RemoveComponent(const std::shared_ptr<Component>& componentToRemove)
	{
		auto it = std::find(m_components.begin(), m_components.end(), componentToRemove);
		if (it != m_components.end())
		{
			m_components.erase(it);
		}
	}

	void AddChild(std::shared_ptr<GameObject> child)
	{
		auto& roots = m_storage->m_objects;
		roots.erase(std::remove(roots.begin(), roots.end(), child), roots.end());

		child->m_parent = shared_from_this();
		child->m_storage = m_storage;
		m_children.push_back(child);
	}

	glm::mat4 GetLocalMatrix() const
	{
		glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), localPosition);
		glm::mat4 rotationMat = glm::yawPitchRoll(localRotation.y, localRotation.x, localRotation.z);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), localScale);
		return translationMat * rotationMat * scaleMat;
	}


	glm::mat4 GetGlobalMatrix() const
	{
		if (auto parent = m_parent.lock()) {
			return parent->GetGlobalMatrix() * GetLocalMatrix();
		}
		return GetLocalMatrix();
	}

	glm::vec3 GetGlobalPosition() const {
		glm::mat4 global = GetGlobalMatrix();
		return glm::vec3(global[3]);
	}

	void SetGlobalPosition(const glm::vec3& worldPos) {
		if (auto parent = m_parent.lock()) {
			glm::mat4 parentGlobal = parent->GetGlobalMatrix();
			glm::mat4 invParent = glm::inverse(parentGlobal);
			glm::vec4 local = invParent * glm::vec4(worldPos, 1.0f);
			localPosition = glm::vec3(local);
		}
		else {
			localPosition = worldPos;
		}
	}

	void SetLocalPosition(const glm::vec3& pos) { localPosition = pos; }
	void SetLocalRotation(const glm::vec3& deg) { localRotation = glm::radians(deg); }
	void SetLocalScale(const glm::vec3& scl) { localScale = scl; }

	glm::vec3 GetLocalPosition() const { return localPosition; }
	glm::vec3 GetLocalRotation() const { return glm::degrees(localRotation); }
	glm::vec3 GetLocalScale() const { return localScale; }

	glm::vec3 GetGlobalRotationDegrees() const {
		glm::mat4 global = GetGlobalMatrix();
		glm::vec3 localScale, localPosition, skew;
		glm::vec4 perspective;
		glm::quat localRotation;
		glm::decompose(global, localScale, localRotation, localPosition, skew, perspective);
		return glm::degrees(glm::eulerAngles(localRotation));
	}
};
