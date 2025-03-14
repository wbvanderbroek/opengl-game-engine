#include <Engine/GameObject.h>
#include <Game/ObjectStorage.h>

void GameObject::Destroy()
{
	if (m_storage)
	{
		m_storage->RemoveGameObject(std::shared_ptr<GameObject>(this, [](GameObject*) {}));
	}
}