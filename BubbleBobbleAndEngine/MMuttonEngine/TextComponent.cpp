#include "pch.h"
#include "TextComponent.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"

TextComponent::TextComponent( const std::string &fontName, int fontSize, const std::string &text ) :
	m_NeedsUpdate( true ), m_Text( text ), m_pFont( nullptr ),
	m_pTexture( nullptr ), m_FontSize{ fontSize }, m_FontName{ fontName }
{
	if(!fontName.empty() && fontSize > 0)
		m_pFont = ResourceManager::GetInstance().LoadFont(fontName, fontSize);
}

TextComponent::~TextComponent()
{
	delete m_pTexture;
	delete m_pFont;
}

void TextComponent::Start()
{
}

void TextComponent::Update()
{
	if (m_NeedsUpdate) // TODO move to swap method? (double buffer)
	{
		delete m_pTexture;
		const SDL_Color color = { 255,255,255 }; // TODO only white text is supported now change dis
		if (!m_pFont)
			return;
		
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture = new Texture2D(texture);
		m_NeedsUpdate = false;
	}
}

void TextComponent::PhysicsUpdate()
{
}

void TextComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		auto pos = glm::vec3{ 0, 0, 0 };
		if(m_pGameObject)
		{
			const TransformComponent* pTransform = m_pGameObject->GetComponent<TransformComponent>();
			
			if (pTransform)
				pos = pTransform->GetPosition();
		}
		 
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos);
	}
}

void TextComponent::SetFont( const std::string& fontName )
{
	if(m_pFont)
		delete m_pFont;
	
	m_FontName = fontName;
	
	if (!fontName.empty() && m_FontSize > 0)
		m_pFont = ResourceManager::GetInstance().LoadFont(fontName, m_FontSize);
	m_NeedsUpdate = true;
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

BaseComponent * TextComponent::Clone() const
{
	return new TextComponent{m_FontName, m_FontSize, m_Text};
}

void TextComponent::LoadFromJson( const nlohmann::json &json )
{
	m_FontSize = json.at("Size").get<int>();
	SetText(json.at("Text").get<std::string>());
	SetFont(json.at("Font").get<std::string>());
}
