#include "pch.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "GameObject.h"
#include <json.hpp>
#include <fstream>

void ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

Texture2D* ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_DataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	return new Texture2D(texture);
}

Font* ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return new Font(m_DataPath + file, size);
}

GameObject* ResourceManager::SpawnPrototype(const std::string& prototypeName)
{
	auto it{ m_Prototypes.find(prototypeName) };
	if (it == m_Prototypes.end())
	{
		std::string filepath{ m_DataPath + "Prototypes/" + prototypeName + ".json" };
		std::ifstream jsonInputStream{ filepath.c_str() };

		if (!jsonInputStream)
			throw std::exception(std::string{ "Couldn't open " + filepath + "!\n" }.c_str());
		
		nlohmann::json j{};
		jsonInputStream >> j;
		
		GameObject *newPrototype{ GameObject::CreateFromJson(j) };

		std::pair<std::string, GameObject*> newPair{ prototypeName, newPrototype };

		
		m_Prototypes.insert(newPair);

		return newPrototype->Clone();
	}

	return it->second->Clone();
}

void ResourceManager::CleanUp()
{
	for( std::pair<const std::string, GameObject *> &prototype : m_Prototypes )
	{
		delete prototype.second;
	}
	m_Prototypes.clear();
}
