#include <Engine.h>
#include <GameObject.h>
#include <GameObject.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ObjectStorage.h>

class Light : public GameObject
{
private:
	glm::vec4 m_lightColor;
	glm::vec3 m_lightPos;
	glm::mat4 m_lightModel;

public:
	Light();

	void OnCreate() override;
};