#pragma once
#include "Singleton.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#include <SDL_hints.h>
#pragma warning(pop)

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

	void RenderTexture(const Texture2D& texture, glm::vec2 dstPos) const;
	void RenderTexture(const Texture2D& texture, glm::vec2 dstPos, glm::vec2 srcPos, glm::vec2 srcDimensions) const;
	void RenderRect(glm::vec2 pos, glm::vec2 dimensions) const;
	void RenderLine(glm::vec2 startPos, glm::vec2 endPos) const;
	void SetRenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;

	SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
private:
	SDL_Renderer* m_Renderer{};
};
