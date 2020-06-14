#include "pch.h"
#include "Texture2D.h"
#include <SDL.h>

SDL_Texture* Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}
