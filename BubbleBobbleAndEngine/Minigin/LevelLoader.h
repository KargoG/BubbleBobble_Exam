#pragma once
#include "Singleton.h"
#include <array>

class Scene;

class LevelLoader : public Singleton<LevelLoader>
{
public:

	void Init();
	void LoadLevel(int level);

private:
	std::array<Scene*, 100> m_Levels{};
};

