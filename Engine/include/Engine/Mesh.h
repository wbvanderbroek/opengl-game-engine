#pragma once

#include <string>

#include <Engine/Components/Camera.h>
#include <Engine/Texture.h>
#include <Engine/VAO.h>

class Mesh
{
public:
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	VAO Vao;

	void Draw(Shader& shader, Camera* camera, glm::mat4 matrix = glm::mat4(1.0f));
};