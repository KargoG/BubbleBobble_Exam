#include "pch.h"
#include "CircleColliderComponent.h"
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>

CircleColliderComponent::CircleColliderComponent( float radius, bool isTrigger, glm::vec2 center,
	const PhysicsMat &pMat ) : ColliderComponent{ isTrigger, center, pMat }, m_Radius{ radius }
{
}

void CircleColliderComponent::Start()
{
	ColliderComponent::Start();

	b2FixtureDef def;

	b2CircleShape circleShape{};
	circleShape.m_radius = m_Radius;
	
	def.shape = &circleShape;

	def.friction = m_Mat.friction;
	def.restitution = m_Mat.restitution;
	def.density = m_Mat.density;

	def.isSensor = m_IsTrigger;

	def.filter = b2Filter{};// TODO

	def.userData = this;

	m_pShape = m_pRB->AddCollider(def);
}

BaseComponent * CircleColliderComponent::Clone() const
{
	CircleColliderComponent* ccc = new CircleColliderComponent{ m_Radius, m_IsTrigger, m_Center, m_Mat };
	return ccc;
}

void CircleColliderComponent::LoadFromJson( const nlohmann::json &json )
{
	ColliderComponent::LoadFromJson(json);
	m_Radius = json.at("Radius").get<float>();
}
