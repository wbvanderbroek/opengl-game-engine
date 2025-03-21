#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include"Camera.h"
#include"EBO.h"
#include"Texture.h"
#include"VAO.h"
#include<string>

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	VAO Vao;

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	void Draw
	(
		Shader& shader,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f)
	);
};

#endif