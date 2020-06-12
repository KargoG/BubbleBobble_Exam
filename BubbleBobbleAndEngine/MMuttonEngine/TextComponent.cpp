#include "pch.h"
#include "TextComponent.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "GameObject.h"

TextComponent::TextComponent(Font* font, const std::string& text)
		: m_NeedsUpdate(true), m_Text(text), m_pFont(font), m_pTexture(nullptr)
{
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
	if (m_NeedsUpdate) // TODO move to set Text?
	{
		delete m_pTexture;
		const SDL_Color color = { 255,255,255 }; // TODO only white text is supported now change dis
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

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}