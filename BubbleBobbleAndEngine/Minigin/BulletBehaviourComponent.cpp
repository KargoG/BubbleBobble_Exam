#include "MiniginPCH.h"
#include "BulletBehaviourComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Time.h"
#include "BoxColliderComponent.h"
#include "PlayerControllerComponent.h"
#include "Scene.h"

void BulletBehaviourComponent::Start()
{
	m_pRB = m_pGameObject->GetComponent<RigidbodyComponent>();

	if (!m_pRB)
		throw std::exception("Bullet needs a RigidbodyComponent to work!\n");
}

void BulletBehaviourComponent::PhysicsUpdate()
{
	if (int(m_pRB->GetTouchFlags()) & (int(TouchFlags::Right) | int(TouchFlags::Left)))
		m_TouchedWall = true;
	
	if(!m_TouchedWall)
		m_pRB->Move((m_MovingRight ? m_SpeedHorizontal : -m_SpeedHorizontal) * Time::GetInstance().GetPhysicsDeltaTime(), 0);
	else
		m_pRB->Move(0, m_SpeedVertical * Time::GetInstance().GetPhysicsDeltaTime());
}

void BulletBehaviourComponent::OnCollision(const BoxColliderComponent* other)
{
	if (GetGameObject()->GetPhysicsLayer() == PhysicsLayer::Layer02) // Bubble
	{
		if(other->GetGameObject()->GetPhysicsLayer() == PhysicsLayer::Layer03)
		{
			other->GetGameObject()->GetComponent<ControllerComponent>()->TakeDamage();
			m_pGameObject->GetScene()->Remove(m_pGameObject);
		}
	}
	else // Bullet
	{
		if (other->GetGameObject()->GetPhysicsLayer() == PhysicsLayer::Layer01)
		{
			other->GetGameObject()->GetComponent<ControllerComponent>()->TakeDamage();
			m_pGameObject->GetScene()->Remove(m_pGameObject);
		}
	}
}

BaseComponent * BulletBehaviourComponent::Clone() const
{
	BulletBehaviourComponent *bc{ new BulletBehaviourComponent{} };

	bc->m_SpeedHorizontal = m_SpeedHorizontal;
	bc->m_SpeedVertical = m_SpeedVertical;
	
	return bc;
}

void BulletBehaviourComponent::LoadFromJson( const nlohmann::json &json )
{
	m_SpeedVertical = json.at("SpeedVertical").get<float>();
	m_SpeedHorizontal = json.at("SpeedHorizontal").get<float>();
}
