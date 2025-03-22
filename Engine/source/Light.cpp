#include <Light.h>

void Light::Update(float deltaTime)
{
	glUniform4f(glGetUniformLocation(m_storage->m_engine->m_shaderProgram.m_id, "lightColor"), m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
	glUniform3f(glGetUniformLocation(m_storage->m_engine->m_shaderProgram.m_id, "lightPos"), translation.x, translation.y, translation.z);
}