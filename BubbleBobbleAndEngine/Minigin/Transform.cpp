#include "MiniginPCH.h"
#include "Transform.h"

Transform::Transform( float x, float y, float z ) : m_Position{x, y, z}
{
}

Transform::Transform( const glm::vec3 &position ) : m_Position{position}
{
}

void Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
