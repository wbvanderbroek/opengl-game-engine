#pragma once

class GameObject;
class Component
{
public:
	std::shared_ptr<GameObject> m_gameObject;

	void SetGameObject(std::shared_ptr<GameObject> gameObject) { m_gameObject = gameObject; }

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void LateUpdate(float deltaTime) {}
	virtual void OnDestroy() {}
	virtual void OnQuit() {}
};
