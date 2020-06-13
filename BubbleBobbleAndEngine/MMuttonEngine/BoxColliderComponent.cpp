#include "pch.h"
#include "BoxColliderComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "RigidbodyComponent.h"

BoxColliderComponent::BoxColliderComponent( float width, float height ) : m_Dimensions{width, height}
{
}

void BoxColliderComponent::Start()
{
	RigidbodyComponent *rb = m_pGameObject->GetComponent<RigidbodyComponent>();
	
	if (rb)
		rb->AddCollider(this);
	else
		m_pGameObject->GetScene()->AddCollider(this);
}

void BoxColliderComponent::Render() const
{
#if _DEBUG
	glm::vec2 pos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	
	Renderer::GetInstance().SetRenderColor(0, 255, 0);
	Renderer::GetInstance().RenderRect(pos, m_Dimensions);
#endif
}
