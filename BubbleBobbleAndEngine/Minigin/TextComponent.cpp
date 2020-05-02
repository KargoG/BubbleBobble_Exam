#include "MiniginPCH.h"
#include "TextComponent.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Transform.h"
#include "GameObject.h"

TextComponent::TextComponent(const std::shared_ptr<Font>& font, const std::string& text)
		: m_NeedsUpdate(true), m_Text(text), m_Font(font), m_Texture(nullptr)
{
}

void TextComponent::Start()
{
}

void TextComponent::Update()
{
	if (m_NeedsUpdate) // TODO move to set Text?
	{
		const SDL_Color color = { 255,255,255 }; // TODO only white text is supported now change dis
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
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
		m_Texture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void TextComponent::PhysicsUpdate()
{
}

void TextComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		auto pos = glm::vec3{ 0, 0, 0 };
		if(!m_pGameObject.expired())
		{
			const std::shared_ptr<Transform> pTransform = m_pGameObject.lock()->GetComponent<Transform>();
			
			if (pTransform)
				pos = pTransform->GetPosition();
		}
		 
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}