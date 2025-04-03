#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <Engine/Mesh.h>
#include <Engine/Shader.h>

class Model : public Component
{
public:
	Model(const std::string& path) : m_modelPath(path)
	{
		m_runInEditor = true;
		LoadModel(path);
	}
	std::string m_modelPath;
	std::string m_directory;

	void Draw(Shader& shader, Camera& camera);
	void LateUpdate(float deltaTime) override;
	void Reload();

private:
	std::vector<Mesh> meshes;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};