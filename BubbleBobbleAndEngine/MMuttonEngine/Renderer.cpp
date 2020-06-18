#include "pch.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "GameData.h"
#include <glm/gtc/constants.hpp>

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
	dst.w = int(scale.x * dst.w / texture.GetPixelPerUnit() * m_RenderScale);
	dst.h = int(scale.y * dst.h / texture.GetPixelPerUnit() * m_RenderScale);
	
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
	dst.x = static_cast<int>(dstPos.x * m_RenderScale);
	dst.y = static_cast<int>(dstPos.y * m_RenderScale);
	dst.w = static_cast<int>(srcDimensions.x * m_RenderScale);
	dst.h = static_cast<int>(srcDimensions.y * m_RenderScale);

	SDL_Rect src;
	src.x = static_cast<int>(srcPos.x);
	src.y = static_cast<int>(srcPos.y);
	src.w = static_cast<int>(srcDimensions.x);
	src.h = static_cast<int>(srcDimensions.y);

	dst.w = int(scale.x * dst.w / texture.GetPixelPerUnit());
	dst.h = int(scale.y * dst.h / texture.GetPixelPerUnit());
	
	MirrorRect(dst);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void Renderer::RenderRect( glm::vec2 pos, glm::vec2 dimensions ) const
{
	SDL_Rect rect{};

	rect.x = static_cast<int>(pos.x * m_RenderScale);
	rect.y = static_cast<int>(pos.y * m_RenderScale);
	rect.w = static_cast<int>(dimensions.x * m_RenderScale);
	rect.h = static_cast<int>(dimensions.y * m_RenderScale);

	MirrorRect(rect);
	SDL_RenderDrawRect(m_Renderer, &rect);
}

void Renderer::RenderLine( glm::vec2 startPos, glm::vec2 endPos ) const
{
	startPos.x *= m_RenderScale;
	startPos.y *= m_RenderScale;
	endPos.x *= m_RenderScale;
	endPos.y *= m_RenderScale;
	
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

void Renderer::MirrorPoint( int &yPos )
{
	int height = GameData::GetInstance().GetWindowHeight();

	yPos = height - yPos;
}

void Renderer::DrawPolygon( const b2Vec2 *vertices, int32 vertexCount, const b2Color &color )
{
	SDL_Point *points{ new SDL_Point[vertexCount+1]{} };

	for( int i = 0; i < vertexCount; ++i )
	{
		points[i].x = int(vertices[i].x * m_RenderScale);
		points[i].y = int(vertices[i].y * m_RenderScale);

		MirrorPoint(points[i].y);
	}

	points[vertexCount].x = points[0].x;
	points[vertexCount].y = points[0].y;

	SetRenderColor(Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255));
	SDL_RenderDrawLines(m_Renderer, points, vertexCount + 1);

	delete[] points;
}

void Renderer::DrawSolidPolygon( const b2Vec2 *vertices, int32 vertexCount, const b2Color &color )
{
	// TODO
	DrawPolygon(vertices, vertexCount, color);
}

void Renderer::DrawCircle( const b2Vec2 &center, float radius, const b2Color &color )
{
	SDL_Point* points{ new SDL_Point[21]{ {int(center.x), int(center.y)} } };
	
	for( int i = 0; i < 21; i++)
	{
		points[i].x = int(cos(glm::two_pi<float>() / 20 * i) * radius * m_RenderScale);
		points[i].y = int(sin(glm::two_pi<float>() / 20 * i) * radius * m_RenderScale);

		MirrorPoint(points[i].y);
	}

	SetRenderColor(Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255));
	SDL_RenderDrawLines(m_Renderer, points, 21);
	
	delete[] points;
}

void Renderer::DrawSolidCircle( const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color )
{
	SetRenderColor(Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255));
	DrawSegment(center, center + (radius * axis), color); // TODO mirror points
	DrawCircle(center, radius, color);
}

void Renderer::DrawSegment( const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color )
{
	SetRenderColor(Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255));
	float yPos1{ p1.y };
	float yPos2{ p2.y };
	MirrorPoint(yPos1);
	MirrorPoint(yPos2);
	SDL_RenderDrawLine(m_Renderer, int(p1.x * m_RenderScale), int(yPos1 * m_RenderScale), int(p2.x * m_RenderScale), int(yPos2 * m_RenderScale));
}

void Renderer::DrawTransform( const b2Transform &xf )
{
	float yPos{ xf.p.y };
	MirrorPoint(yPos);
	SDL_RenderDrawPoint(m_Renderer, int(xf.p.x * m_RenderScale), int(yPos * m_RenderScale));
}

void Renderer::DrawPoint( const b2Vec2 &p, float, const b2Color &color )
{
	float yPos{ p.y };
	MirrorPoint(yPos);
	// TODO implement size
	SetRenderColor(Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255));
	SDL_RenderDrawPoint(m_Renderer, int(p.x * m_RenderScale), int(yPos * m_RenderScale));
}
