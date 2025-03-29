#pragma once

class GameObject;
class Component
{
public:
	std::shared_ptr<GameObject> gameObject;

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void LateUpdate(float deltaTime) {}
	virtual void OnDestroy() {}
	virtual void OnQuit() {}
};
