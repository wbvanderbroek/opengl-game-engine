#include <iostream>

#include <stb/stb_image.h>

#include <Engine.h>
#include <Model.h>

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_modelPath = path;
	m_directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = mesh->mNormals ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
		vertex.texUV = glm::vec2(mesh->mTextureCoords[0][i].y, mesh->mTextureCoords[0][i].x);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

void Model::Draw(Shader& shader, Camera& camera)
{
	glm::mat4 modelMatrix = m_gameObject->GetGlobalMatrix();

	for (auto& mesh : meshes)
	{
		mesh.Draw(shader, &camera, modelMatrix);
	}
}

void Model::LateUpdate(float deltaTime)
{
	Draw(m_gameObject->m_storage->m_engine->m_shaderProgram, *m_gameObject->m_storage->m_engine->m_camera);
}

void Model::Reload()
{
	meshes.clear();
	if (m_modelPath.empty()) return;
	LoadModel(m_modelPath);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string texturePath = std::string(str.C_Str());

		if (texturePath.find(":") != std::string::npos || texturePath.find("/") == 0)
		{
			size_t lastSlash = texturePath.find_last_of("/\\");
			std::string filenameOnly = (lastSlash != std::string::npos) ? texturePath.substr(lastSlash + 1) : texturePath;

			texturePath = m_directory + "/" + filenameOnly;
		}
		else
		{
			texturePath = m_directory + "/" + texturePath;
		}

		textures.push_back(Texture(texturePath.c_str(), typeName.c_str(), 0));
	}

	return textures;
}
