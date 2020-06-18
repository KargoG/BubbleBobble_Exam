#include "pch.h"
#include "TextureRendererComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"

TextureRendererComponent::~TextureRendererComponent()
{
	if(m_pTexture)
		delete m_pTexture;
}

void TextureRendererComponent::Render() const
{
	glm::vec3 pos{ 0, 0, 0 };
	glm::vec3 scale{ 1, 1, 1 };
	if(m_pGameObject)
	{
		const auto transform = m_pGameObject->GetComponent<TransformComponent>();
		if (transform)
		{
			pos = transform->GetPosition();
			scale = transform->GetScale();
		}
	}

	if(m_pTexture->GetPosition().x < 0)
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos, scale);
	else
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos, m_pTexture->GetPosition(), m_pTexture->GetDimension(), scale);
}

void TextureRendererComponent::SetTexture( const char * filename)
{
	m_TextureName = filename;
	if (m_pTexture)
		delete m_pTexture;
	m_pTexture = ResourceManager::GetInstance().LoadTexture(m_TextureName);
}

void TextureRendererComponent::SetRenderDimensions( float xPos, float yPos, float width, float height ) const
{
	SetSrcX(xPos);
	SetSrcY(yPos);
	SetSrcWidth(width);
	SetSrcHeight(height);
}

BaseComponent * TextureRendererComponent::Clone() const
{
	TextureRendererComponent* tc{ new TextureRendererComponent{} };
	if (m_TextureName.empty())
	{
		tc->m_pTexture = nullptr;
	}
	else
	{
		tc->m_pTexture = ResourceManager::GetInstance().LoadTexture(m_TextureName);
		tc->m_TextureName = m_TextureName;
		tc->SetRenderDimensions(m_pTexture->GetPosition().x, m_pTexture->GetPosition().y, m_pTexture->GetDimension().x, m_pTexture->GetDimension().y);
	}

	return tc;
}

void TextureRendererComponent::LoadFromJson( const nlohmann::json &json )
{
	SetTexture(json.at("Texture").get<std::string>().c_str());

	float x = json.at("X").get<float>();
	float y = json.at("Y").get<float>();
	float width = json.at("Width").get<float>();
	float height = json.at("Height").get<float>();
	
	SetRenderDimensions(x, y, width, height);
}
