#include <Engine.h>
#include <GameObject.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ObjectStorage.h>

class Light : public GameObject
{
private:
	glm::vec4 m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // RGBA, leave the alpha as 1.0f

public:
	Light() {}

	void Update(float deltaTime) override;
};