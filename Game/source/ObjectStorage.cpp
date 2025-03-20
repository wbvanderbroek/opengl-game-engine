#include <algorithm> 
#include <GameObject.h>
#include <Light.h>
#include <Model.h>
#include <ObjectStorage.h>

ObjectStorage::ObjectStorage(Engine* engine) : m_engine(engine)
{
	CreateAndAdd<Model>("Assets/models/building/MetalMineBuilding.fbx");
	CreateAndAdd<Model>("Assets/models/character/char.fbx");
	CreateAndAdd<Light>();
}

template<typename T, typename... Args>
std::shared_ptr<T> ObjectStorage::CreateAndAdd(Args&&... args)
{
	auto object = std::make_shared<T>(this, std::forward<Args>(args)...);
	AddGameObject(object);
	return object;
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
