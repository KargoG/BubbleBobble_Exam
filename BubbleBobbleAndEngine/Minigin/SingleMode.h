#pragma once
#include <GameMode.h>
#include "Observer.h"

struct EnemyData;

class SingleMode :
    public GameMode, public Observer
{
public:
	SingleMode(const std::vector<EnemyData>& enemies, int level);
	void Awake() override;

	void OnNotify(void* pData, int events) override;
private:
	std::vector<EnemyData> m_Enemies;
	int m_EnemiesLeft{};
	int m_Level;
	
	void StartNextLevel();
	void GameOver();
};

