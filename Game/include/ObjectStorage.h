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
	void CreateDefaultScene();

	template<typename T>
	std::shared_ptr<T> Instantiate(T&& obj)
	{
		auto object = std::make_shared<std::decay_t<T>>(std::forward<T>(obj));
		object->SetStorage(this);
		m_objects.push_back(object);
		object->OnCreate();
		return object;
	}

	Engine* m_engine;
	std::vector<std::shared_ptr<GameObject>> m_objects;
};