#include <Game/ObjectStorage.h>
#include <Engine/GameObject.h>
#include <algorithm> 
#include <Game/Player.h>

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
    auto it = std::find(m_objects.begin(), m_objects.end(), object);
    if (it != m_objects.end())
    {
        (*it)->OnDestroy();
        m_objects.erase(it);
    }
}
