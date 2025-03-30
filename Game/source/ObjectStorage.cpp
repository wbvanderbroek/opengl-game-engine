#include <algorithm> 
#include <GameObject.h>
#include <Light.h>
#include <Model.h>
#include <ObjectStorage.h>

ObjectStorage::ObjectStorage(Engine* engine) : m_engine(engine)
{
	// Initialize with a default scene
	// This can be removed if you want to start with an empty scene
	CreateDefaultScene();
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

// Create a default scene with some objects
// This is optional and can be removed if you want to start with an empty scene
void ObjectStorage::CreateDefaultScene()
{
	// Add a camera (this is essential)
	auto camera = Instantiate(GameObject());
	camera->AddComponent(Camera());

	// Add a default light
	auto light = Instantiate(GameObject());
	light->AddComponent(Light());
	light->translation = glm::vec3(0, 10, 0);

	// Add a ground plane
	auto plane = Instantiate(GameObject());
	plane->AddComponent(Model("Assets/models/plane/plane.fbx"));
	plane->SetRotation(glm::vec3(270, 0, 0));
	plane->scale = glm::vec3(100, 100, 100);
	plane->translation = glm::vec3(0, -10, 0);
}