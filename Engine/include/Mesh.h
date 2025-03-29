#pragma once

#include <string>

#include <Camera.h>
#include <EBO.h>
#include <Texture.h>
#include <VAO.h>

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