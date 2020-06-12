#include "pch.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

void Renderer::Init(SDL_Window * window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();
	
	SDL_RenderPresent(m_Renderer);
}

void Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void Renderer::RenderTexture(const Texture2D& texture, glm::vec2 dstPos) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(dstPos.x);
	dst.y = static_cast<int>(dstPos.y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

//void Renderer::RenderTexture(const Texture2D& texture, glm::vec2 dstPos, glm::vec2 dstDimensions) const TODO remove if not needed
//{
//	SDL_Rect dst;
//	dst.x = static_cast<int>(dstPos.x);
//	dst.y = static_cast<int>(dstPos.y);
//	dst.w = static_cast<int>(dstDimensions.x);
//	dst.h = static_cast<int>(dstDimensions.y);
//	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
//}

void Renderer::RenderTexture( const Texture2D &texture, glm::vec2 dstPos, glm::vec2 srcPos,
	glm::vec2 srcDimensions ) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(dstPos.x);
	dst.y = static_cast<int>(dstPos.y);
	dst.w = static_cast<int>(srcDimensions.x);
	dst.h = static_cast<int>(srcDimensions.y); // TODO make use of scale of object?

	SDL_Rect src;
	src.x = static_cast<int>(srcPos.x);
	src.y = static_cast<int>(srcPos.y);
	src.w = static_cast<int>(srcDimensions.x);
	src.h = static_cast<int>(srcDimensions.y);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}
