#include "MiniginPCH.h"
#include "ControllerComponent.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "GameData.h"

void ControllerComponent::Start()
{
	m_pRB = m_pGameObject->GetComponent<RigidbodyComponent>();

	if (!m_pRB)
		throw std::exception("Controller needs a RigidbodyComponent to work!\n");
}

void ControllerComponent::Update()
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	glm::vec3 position{ transform->GetPosition() };
	if(position.y < 0)
	{
		RigidbodyComponent* rb = m_pGameObject->GetComponent<RigidbodyComponent>();
		rb->Move(0, float(GameData::GetInstance().GetLevelHeight()));
	}
}
