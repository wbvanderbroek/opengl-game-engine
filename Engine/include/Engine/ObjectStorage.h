#pragma once

#include <memory>
#include <vector>

#include <json/json.h>

#include <Engine/GameObject.h>

class ObjectStorage
{
private:
	nlohmann::json SerializeScene();
	nlohmann::json SerializeGameObject(std::shared_ptr<GameObject> gameObject);
	nlohmann::json SerializeComponent(std::shared_ptr<Component> component);

	void DeserializeScene(const nlohmann::json& data);
	std::shared_ptr<GameObject> DeserializeGameObject(const nlohmann::json& data);
	void DeserializeComponent(std::shared_ptr<GameObject> gameObject, const nlohmann::json& data);

	std::shared_ptr<Component> CreateComponentByType(const std::string& type);

public:
	ObjectStorage(Engine* engine) : m_engine(engine) {}

	std::string m_currentScenePath = "";
	std::vector<std::shared_ptr<GameObject>> m_objects;
	Engine* m_engine;

	template<typename T>
	std::shared_ptr<T> Instantiate(T&& obj)
	{
		auto object = std::make_shared<std::decay_t<T>>(std::forward<T>(obj));
		object->SetStorage(this);
		m_objects.push_back(object);
		object->OnCreate();
		return object;
	}

	void RemoveGameObject(std::shared_ptr<GameObject> object);
	void LoadFirstScene();
	void CreateDefaultScene();
	void SaveScene(const std::string& filename);
	void LoadScene(const std::string& filename);
	void ClearScene();
};