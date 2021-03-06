#pragma once
#include "Singleton.h"
class GameData :
    public Singleton<GameData>
{
public:
	int GetWindowWidth() const { return m_Width; };
	int GetWindowHeight() const { return m_Height; };
	int GetSpriteWidth() const { return m_SpriteWidth; };
	int GetSpriteHeight() const { return m_SpriteHeight; };
	int GetSpriteScale() const { return m_SpriteScale; };
	int GetLevelWidth() const { return m_LevelWidth; };
	int GetLevelHeight() const { return m_LevelHeight; };
	
private:
	const int m_Width{ 512 }, m_Height{ 400 };
	const int m_SpriteWidth{ 16 }, m_SpriteHeight{ 16 };
	const int m_SpriteScale{ 1 };
	const int m_LevelWidth{ 32 }, m_LevelHeight{ 25 };
};

