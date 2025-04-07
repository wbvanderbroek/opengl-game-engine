#pragma once

#include <Engine/Components/ComponentRegistry.h>
#include <glm/glm.hpp>
#include <memory>

class GameObject;
class Component
{
public:
	std::shared_ptr<GameObject> m_gameObject;
	bool m_runInEditor = false;
	void SetGameObject(std::shared_ptr<GameObject> gameObject) { m_gameObject = gameObject; }

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void LateUpdate(float deltaTime) {}
	virtual void OnDestroy() {}
	virtual void OnQuit() {}
};
