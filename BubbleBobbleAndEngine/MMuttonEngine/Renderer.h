#pragma once
#include "Singleton.h"
#pragma warning(push)
#pragma warning (disable:4201)
//#include <glm/vec2.hpp>
#include "../3rdParty/glm/glm/detail/type_vec2.hpp"
#pragma warning(pop)
#include <SDL_hints.h>
#include <SDL.h>

struct SDL_Window;
struct SDL_Renderer;

class Texture2D;
/**
 * Simple RAII wrapper for the SDL renderer
 */
class Renderer final : public Singleton<Renderer>
{
public:
	void Init(SDL_Window* window);
	void Render() const;
	void Destroy();

	void RenderTexture(const Texture2D& texture, glm::vec2 dstPos, glm::vec2 scale) const;
	void RenderTexture(const Texture2D& texture, glm::vec2 dstPos, glm::vec2 srcPos, glm::vec2 srcDimensions, glm::vec2 scale) const;
	void RenderRect(glm::vec2 pos, glm::vec2 dimensions) const;
	void RenderLine(glm::vec2 startPos, glm::vec2 endPos) const;
	void SetRenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;

	static void MirrorRect(SDL_Rect &rect);
	static void MirrorPoint(float &yPos);
	
	SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
private:
	SDL_Renderer* m_Renderer{};
};

