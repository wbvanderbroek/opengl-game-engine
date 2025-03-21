#pragma once
#include <GameObject.h>
#include <memory>
#include <vector>

class Engine;
class GameObject;
class ObjectStorage
{
public:
	ObjectStorage(Engine* engine);
	void RemoveGameObject(std::shared_ptr<GameObject> object);

	template<typename T>
	std::shared_ptr<T> Instantiate(T&& obj);

	Engine* m_engine;
	std::vector<std::shared_ptr<GameObject>> m_objects;
};