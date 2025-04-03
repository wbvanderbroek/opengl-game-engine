#include <Engine/Mesh.h>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	Vao.Bind();

	VBO VBO(vertices);
	EBO EBO(indices);

	Vao.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	Vao.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	Vao.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	Vao.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	Vao.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera* camera, glm::mat4 matrix)
{
	if (camera == nullptr)
	{
		std::cerr << "Camera is null" << std::endl;
		return;
	}

	Vao.Bind();

	if (textures.size() == 0)
	{
		static GLuint fallbackTextureID = 0;
		if (fallbackTextureID == 0)
		{
			glGenTextures(1, &fallbackTextureID);
			glBindTexture(GL_TEXTURE_2D, fallbackTextureID);
			unsigned char pink[3] = { 255, 255, 255 };
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pink);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, fallbackTextureID);
		}
	}
	else
	{
		unsigned int numDiffuse = 0;
		unsigned int numSpecular = 0;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			std::string num;
			std::string type = textures[i].m_type;

			if (type == "diffuse")
				num = std::to_string(numDiffuse++);
			else if (type == "specular")
				num = std::to_string(numSpecular++);

			textures[i].TexUnit(shader, (type + num).c_str(), i);
			textures[i].Bind();
		}
	}


	glUniform3f(glGetUniformLocation(shader.m_id, "camPos"), camera->m_gameObject->localPosition.x, camera->m_gameObject->localPosition.y, camera->m_gameObject->localPosition.z);
	camera->Matrix(shader, "camMatrix");

	glUniformMatrix4fv(glGetUniformLocation(shader.m_id, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}