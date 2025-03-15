#include <algorithm> 
#include <GameObject.h>
#include <Model.h>
#include <ObjectStorage.h>
#include <Player.h>

ObjectStorage::ObjectStorage(Engine* engine) : m_engine(engine)
{
	std::shared_ptr<Model> model = std::make_shared<Model>(this, "Assets/models/door/door.gltf");
	AddGameObject(model);

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
