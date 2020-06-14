#pragma once
#include "Singleton.h"
#include <array>

class Scene;

enum class BubbleBobbleGameMode
{
	Single,
	Coop,
	Versus
};

enum class EnemyType
{
	ZenChan = 0b00000000,
	Maita = 0b00000110
};

struct EnemyData
{
	int Column, Row;
	EnemyType EnemyType;
};

class LevelLoader : public Singleton<LevelLoader>
{
public:

	void Init();
	void LoadLevel(int level, BubbleBobbleGameMode gameMode = BubbleBobbleGameMode::Single);

private:
	std::array<Scene*, 100> m_Levels{};
	std::array<std::vector<EnemyData>, 100> m_Enemies{};
};

