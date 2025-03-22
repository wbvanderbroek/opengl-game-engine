#include <Light.h>

Light::Light()
{
	m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightModel = glm::mat4(1.0f);
	m_lightModel = glm::translate(m_lightModel, translation);
}

void Light::OnCreate()
{
	m_storage->m_engine->m_shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(m_storage->m_engine->m_shaderProgram.ID, "lightColor"), m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
	glUniform3f(glGetUniformLocation(m_storage->m_engine->m_shaderProgram.ID, "lightPos"), translation.x, translation.y, translation.z);
}