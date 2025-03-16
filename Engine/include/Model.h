#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

class Model : public GameObject
{
public:
	Model(ObjectStorage* storage, const std::string& path);

	void Draw(Shader& shader, Camera& camera);
	void LateUpdate(float deltaTime) override;

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
