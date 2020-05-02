#include "MiniginPCH.h"
#include "TextureRendererComponent.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "ResourceManager.h"


void TextureRendererComponent::Start()
{
}

void TextureRendererComponent::Update()
{
}

void TextureRendererComponent::PhysicsUpdate()
{
}

void TextureRendererComponent::Render() const
{
	glm::vec3 pos{ 0, 0, 0 };
	if(!m_pGameObject.expired())
	{
		const auto transform = m_pGameObject.lock()->GetComponent<Transform>();
		if(transform)
			pos = transform->GetPosition();
	}
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void TextureRendererComponent::SetTexture( const char * filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}