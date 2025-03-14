#include "GameObject.h"
#include "ObjectStorage.h"

void GameObject::Destroy()
{
    if (m_storage)
    {
        OnDestroy();
        m_storage->RemoveGameObject(std::shared_ptr<GameObject>(this, [](GameObject*) {}));
    }
}