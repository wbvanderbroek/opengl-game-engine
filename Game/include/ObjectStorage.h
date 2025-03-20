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
	void AddGameObject(std::shared_ptr<GameObject> object);
	void RemoveGameObject(std::shared_ptr<GameObject> object);

	template<typename T, typename... Args>
	std::shared_ptr<T> CreateAndAdd(Args&&... args);

	Engine* m_engine;
	std::vector<std::shared_ptr<GameObject>> m_objects;
};