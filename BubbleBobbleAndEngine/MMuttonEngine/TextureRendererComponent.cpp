#include "pch.h"
#include "TextureRendererComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"

TextureRendererComponent::~TextureRendererComponent()
{
	delete m_pTexture;
}

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
	if(m_pGameObject)
	{
		const auto transform = m_pGameObject->GetComponent<TransformComponent>();
		if (transform)
		{
			pos = transform->GetPosition();
		}
	}

	if(m_pTexture->GetPosition().x < 0)
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos);
	else
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos, m_pTexture->GetPosition(), m_pTexture->GetDimension());
}

void TextureRendererComponent::SetTexture( const char * filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureRendererComponent::SetRenderDimensions( float xPos, float yPos, float width, float height ) const
{
	SetSrcX(xPos);
	SetSrcY(yPos);
	SetSrcWidth(width);
	SetSrcHeight(height);
}
