#pragma once
#include <vector>
#include <memory>
#include <Engine/GameObject.h>

class GameObject;
class ObjectStorage
{
public:
	ObjectStorage();
	void AddGameObject(std::shared_ptr<GameObject> object);
	void RemoveGameObject(std::shared_ptr<GameObject> object);

	std::vector<std::shared_ptr<GameObject>> m_objects;
};