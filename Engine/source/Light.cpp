#include <Light.h>

Light::Light()
{
	m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightPos = glm::vec3(0.5f, -1.5f, 0.5f);
	m_lightModel = glm::mat4(1.0f);
	m_lightModel = glm::translate(m_lightModel, m_lightPos);
}

void Light::OnCreate()
{
	m_storage->m_engine->m_shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(m_storage->m_engine->m_shaderProgram.ID, "lightColor"), m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
	glUniform3f(glGetUniformLocation(m_storage->m_engine->m_shaderProgram.ID, "lightPos"), m_lightPos.x, m_lightPos.y, m_lightPos.z);
}