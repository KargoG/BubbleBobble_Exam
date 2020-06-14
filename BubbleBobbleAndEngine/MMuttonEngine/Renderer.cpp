#include "pch.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "GameData.h"

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
	SetRenderColor(0, 0, 0);
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

void Renderer::RenderTexture(const Texture2D& texture, glm::vec2 dstPos, glm::vec2 scale) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(dstPos.x);
	dst.y = static_cast<int>(dstPos.y);
	
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	dst.w = int(scale.x * dst.w);
	dst.h = int(scale.y * dst.h);
	
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
	glm::vec2 srcDimensions, glm::vec2 scale ) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(dstPos.x);
	dst.y = static_cast<int>(dstPos.y);
	dst.w = static_cast<int>(srcDimensions.x);
	dst.h = static_cast<int>(srcDimensions.y);

	SDL_Rect src;
	src.x = static_cast<int>(srcPos.x);
	src.y = static_cast<int>(srcPos.y);
	src.w = static_cast<int>(srcDimensions.x);
	src.h = static_cast<int>(srcDimensions.y);

	dst.w = int(scale.x * dst.w);
	dst.h = int(scale.y * dst.h);
	
	MirrorRect(dst);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void Renderer::RenderRect( glm::vec2 pos, glm::vec2 dimensions ) const
{
	SDL_Rect rect{};

	rect.x = static_cast<int>(pos.x);
	rect.y = static_cast<int>(pos.y);
	rect.w = static_cast<int>(dimensions.x);
	rect.h = static_cast<int>(dimensions.y);

	MirrorRect(rect);
	SDL_RenderDrawRect(m_Renderer, &rect);
}

void Renderer::RenderLine( glm::vec2 startPos, glm::vec2 endPos ) const
{
	MirrorPoint(startPos.y);
	MirrorPoint(endPos.y);
	SDL_RenderDrawLine(m_Renderer, static_cast<int>(startPos.x), static_cast<int>(startPos.y), static_cast<int>(endPos.x), static_cast<int>(endPos.y));
}

void Renderer::SetRenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
}

void Renderer::MirrorRect( SDL_Rect &rect )
{
	int height = GameData::GetInstance().GetWindowHeight();

	rect.y = height - rect.y - rect.h;
}

void Renderer::MirrorPoint( float &yPos )
{
	int height = GameData::GetInstance().GetWindowHeight();

	yPos = height - yPos;
}
