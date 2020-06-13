#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <SDL.h>

class GameObject;
class Texture2D;
class Font;

class ResourceManager final : public Singleton<ResourceManager>
{
public:
	void Init(const std::string& data);
	Texture2D* LoadTexture(const std::string& file);
	Font* LoadFont(const std::string& file, unsigned int size);
	GameObject* SpawnPrototype(const std::string& prototypeName);

	void CleanUp();
private:
	friend class Singleton<ResourceManager>;
	ResourceManager() = default;
	std::string m_DataPath;
	std::unordered_map<std::string, SDL_Texture*> m_Textures{};
	std::unordered_map<std::string, Font*> m_Fonts{};
	std::unordered_map<std::string, GameObject*> m_Prototypes{};
};

