#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent( float x, float y, float z ) : m_Position{x, y, z}
{
}

TransformComponent::TransformComponent( const glm::vec3 &position ) : m_Position{position}
{
}

void TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

