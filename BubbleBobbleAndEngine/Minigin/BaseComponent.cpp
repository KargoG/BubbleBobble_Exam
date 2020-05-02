#include "MiniginPCH.h"
#include "BaseComponent.h"

void BaseComponent::SetGameObject( std::shared_ptr<GameObject> pGameObject )
{
	m_pGameObject = pGameObject;
}
