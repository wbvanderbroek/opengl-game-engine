#pragma once
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
};