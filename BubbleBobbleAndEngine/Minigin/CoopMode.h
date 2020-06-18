#pragma once
#include <GameMode.h>
#include <Observer.h>
struct EnemyData;

class CoopMode :
    public GameMode, public Observer
{
public:
	CoopMode(const std::vector<EnemyData>& enemies, int level);
	void Awake() override;

	void OnNotify(void* pData, int events) override;
	
private:
	std::vector<EnemyData> m_Enemies;
	int m_EnemiesLeft{};
	int m_Level;
	int m_PlayerLeft{2};

	void StartNextLevel();
	void GameOver();
};

