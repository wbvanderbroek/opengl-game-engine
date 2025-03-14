#include "ObjectStorage.h"
#include "GameObject.h"
#include <iostream>
#include "Player.h"

ObjectStorage::ObjectStorage()
{
	Player player = Player();

	AddGameObject(std::make_shared<Player>(player));
}

void ObjectStorage::AddGameObject(std::shared_ptr<GameObject> object)
{
	m_objects.push_back(object);
	object->OnCreate();
}