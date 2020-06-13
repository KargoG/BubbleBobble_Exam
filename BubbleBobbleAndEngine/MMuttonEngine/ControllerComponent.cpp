#include "pch.h"
#include "ControllerComponent.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"

void ControllerComponent::Start()
{
	m_pRB = m_pGameObject->GetComponent<RigidbodyComponent>();

	if (!m_pRB)
		throw std::exception("Controller needs a RigidbodyComponent to work!\n");
}
