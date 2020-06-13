#pragma once
#include "Singleton.h"
class GameData :
    public Singleton<GameData>
{
public:
	void Init(int width, int height){ m_Width = width; m_Height = height; }

	int GetWindowWidth() const { return m_Width; };
	int GetWindowHeight() const { return m_Height; };
	
private:
	int m_Width{}, m_Height{};
};

