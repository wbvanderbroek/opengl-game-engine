#pragma once
#include "ObjectStorage.h"
#include <iostream>
#include <memory>

class ObjectStorage;
class GameObject
{
private:
	ObjectStorage* m_storage;
public:
	virtual ~GameObject() = default;
	virtual void OnCreate() {}
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void Quit() {}
};