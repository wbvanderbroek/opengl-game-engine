#include "ObjectStorage.h"
#include "GameObject.h"
#include <iostream>
#include "Player.h"

ObjectStorage::ObjectStorage()
{
    std::shared_ptr<Player> player = std::make_shared<Player>(this);
    AddGameObject(player);
}

void ObjectStorage::AddGameObject(std::shared_ptr<GameObject> object)
{
	m_objects.push_back(object);
	object->OnCreate();
}

void ObjectStorage::RemoveGameObject(std::shared_ptr<GameObject> object)
{
    auto it = std::remove(m_objects.begin(), m_objects.end(), object);
    if (it != m_objects.end())
    {
        (*it)->OnDestroy();
        m_objects.erase(it, m_objects.end());
    }
}