#include <algorithm> 
#include <GameObject.h>
#include <Light.h>
#include <Model.h>
#include <ObjectStorage.h>

ObjectStorage::ObjectStorage(Engine* engine) : m_engine(engine)
{
	auto building = Instantiate(Model("Assets/models/building/MetalMineBuilding.fbx"));
	building->SetRotation(glm::vec3(270, 0, 180));
	building->translation = glm::vec3(0, 0, -35);
	building->scale = glm::vec3(1.5, 1.5, 1.5);

	auto building2 = Instantiate(Model("Assets/models/building/MetalMineBuilding.fbx"));
	building2->SetRotation(glm::vec3(270, 0, 135));
	building2->translation = glm::vec3(35, 0, -20);

	auto building3 = Instantiate(Model("Assets/models/building/MetalMineBuilding.fbx"));
	building3->SetRotation(glm::vec3(270, 0, 225));
	building3->translation = glm::vec3(-35, 0, -20);

	auto character = Instantiate(Model("Assets/models/character/char.fbx"));
	character->SetRotation(glm::vec3(270, 0, 0));
	character->scale = glm::vec3(3, 3, 3);

	auto plane = Instantiate(Model("Assets/models/plane/plane.fbx"));
	plane->SetRotation(glm::vec3(270, 0, 0));
	plane->scale = glm::vec3(300, 300, 300);
	plane->translation = glm::vec3(0, -10, 0);


	auto light = Instantiate(Light());
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
