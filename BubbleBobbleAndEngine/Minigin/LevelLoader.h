#pragma once
#include "Singleton.h"
#include <array>

class Scene;

enum class GameMode
{
	Single,
	Coop,
	Versus
};

class LevelLoader : public Singleton<LevelLoader>
{
public:

	void Init();
	void LoadLevel(int level, GameMode gameMode = GameMode::Single);

private:
	std::array<Scene*, 100> m_Levels{};
};

