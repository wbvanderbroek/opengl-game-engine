#pragma once
#include <GameObject.h>
#include <memory>
#include <vector>

class GameObject;
class ObjectStorage
{
public:
	ObjectStorage();
	void AddGameObject(std::shared_ptr<GameObject> object);
	void RemoveGameObject(std::shared_ptr<GameObject> object);

	std::vector<std::shared_ptr<GameObject>> m_objects;
};