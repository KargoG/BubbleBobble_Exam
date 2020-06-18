#include "MiniginPCH.h"
#include "BulletBehaviourComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Time.h"
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
	if(!m_TouchedWall)
		m_pRB->SetVelocity((m_MovingRight ? m_SpeedHorizontal : -m_SpeedHorizontal), 0);
	else
		m_pRB->SetVelocity(0, m_SpeedVertical);
}

void BulletBehaviourComponent::OnTriggerEnter(const Collision* collision)
{
	if (GetGameObject()->GetTag() == "Bubble") // Bubble
	{
		if(collision->otherCollider && collision->otherCollider->GetGameObject()->GetTag() == "Enemy")
		{
			collision->otherCollider->GetGameObject()->GetComponent<ControllerComponent>()->TakeDamage();
			m_pGameObject->GetScene()->Remove(m_pGameObject);

			return;
		}
	}
	else // Bullet
	{
		if (collision->otherCollider && collision->otherCollider->GetGameObject()->GetTag() == "Player")
		{
			collision->otherCollider->GetGameObject()->GetComponent<ControllerComponent>()->TakeDamage();
			m_pGameObject->GetScene()->Remove(m_pGameObject);
			
			return;
		}
	}

	m_TouchedWall = true;
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
