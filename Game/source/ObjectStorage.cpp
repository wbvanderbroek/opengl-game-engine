#include <algorithm> 
#include <GameObject.h>
#include <Light.h>
#include <Model.h>
#include <ObjectStorage.h>

ObjectStorage::ObjectStorage(Engine* engine) : m_engine(engine)
{
	std::shared_ptr<Model> model = std::make_shared<Model>(this, "Assets/models/building/MetalMineBuilding.fbx");
	AddGameObject(model);
	model->translation = glm::vec3(5.0f, 0.0f, -25.0f);

	//std::shared_ptr<Model> model2 = std::make_shared<Model>(this, "Assets/models/door2/door.fbx");
	//model2->translation = glm::vec3(0.0f, 0.0f, 2.0f);
	//AddGameObject(model2);


	std::shared_ptr<Light> light = std::make_shared<Light>(this);
	AddGameObject(light);
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
