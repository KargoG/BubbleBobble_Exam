#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
//#include <glm/vec2.hpp>
#include "../3rdParty/glm/glm/detail/type_vec2.hpp"
#pragma warning(pop)

struct SDL_Texture;
/**
 * Simple RAII wrapper for an SDL_Texture
 */
class Texture2D
{
public:
	SDL_Texture* GetSDLTexture() const;
	explicit Texture2D(SDL_Texture* texture);

	Texture2D(const Texture2D &) = delete;
	Texture2D(Texture2D &&) = delete;
	Texture2D & operator= (const Texture2D &) = delete;
	Texture2D & operator= (const Texture2D &&) = delete;

	void SetX(float x) { m_Pos.x = x; };
	void SetY(float y) { m_Pos.y = y; };
	void SetWidth(float width) { m_Dimensions.x = width; };
	void SetHeight(float height) { m_Dimensions.y = height; };

	glm::vec2 GetPosition() const { return m_Pos; };
	glm::vec2 GetDimension() const { return m_Dimensions; };
private:
	SDL_Texture* m_Texture;

	glm::vec2 m_Pos{-1.f, -1.f};
	glm::vec2 m_Dimensions{-1.f, -1.f};
};
