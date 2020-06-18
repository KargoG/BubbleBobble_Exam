#include "pch.h"
#include "ColliderComponent.h"
#include <box2d/b2_fixture.h>
#include "GameObject.h"

ColliderComponent::ColliderComponent( bool isTrigger, glm::vec2 center, const PhysicsMat &pMat ) : m_Center{ center }, m_IsTrigger{ isTrigger }, m_Mat{ pMat }
{
}

ColliderComponent::~ColliderComponent()
{
	if (m_pShape)
		m_pShape->SetUserData(nullptr);
}

void ColliderComponent::Start()
{
	m_pRB = m_pGameObject->GetComponent<RigidbodyComponent>();

	if (!m_pRB)
		throw std::exception("A collider needs a rigidbody to work!");
}

void ColliderComponent::LoadFromJson( const nlohmann::json &json )
{
	m_Center.x = json.at("X").get<float>();
	m_Center.y = json.at("Y").get<float>();
	m_IsTrigger = json.at("IsTrigger").get<bool>();
}
