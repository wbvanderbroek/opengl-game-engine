#pragma once
#include <Game/ObjectStorage.h>
#include <iostream>
#include <memory>

class ObjectStorage;
class GameObject
{
private:
	ObjectStorage* m_storage;
public:
	explicit GameObject(ObjectStorage* storage) : m_storage(storage) {}

	virtual ~GameObject() = default;
	virtual void OnCreate() {}
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void OnDestroy() {}
	virtual void Quit() {}

	void Destroy();
};