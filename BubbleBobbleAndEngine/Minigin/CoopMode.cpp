#include "MiniginPCH.h"
#include "CoopMode.h"
#include "GameObject.h"
#include "LevelLoader.h"
#include "ResourceManager.h"
#include "GameData.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "AIControllerComponent.h"
#include "PlayerControllerComponent.h"

CoopMode::CoopMode(const std::vector<EnemyData>& enemies, int level) : m_Enemies{ enemies }, m_Level{level}
{
}

void CoopMode::Start()
{
	for (const EnemyData& enemy : m_Enemies)
	{
		GameObject* newEnemy{  };
		switch (enemy.EnemyType)
		{
		case EnemyType::ZenChan:
			newEnemy = ResourceManager::GetInstance().SpawnPrototype("ZenChan");
			break;
		case EnemyType::Maita:
			newEnemy = ResourceManager::GetInstance().SpawnPrototype("Maita");
			break;
		default: continue;
		}
		glm::vec3 spawnPos{ enemy.Column * GameData::GetInstance().GetSpriteWidth(), enemy.Row * GameData::GetInstance().GetSpriteHeight() , 0 };

		spawnPos *= float(GameData::GetInstance().GetSpriteScale());

		TransformComponent* transform = newEnemy->GetComponent<TransformComponent>();
		transform->SetPosition(spawnPos);
		transform->SetScale(float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()));

		newEnemy->GetComponent<AIControllerComponent>()->AddObserver(this);
		
		SceneManager::GetInstance().GetActiveScene()->Add(newEnemy);
	}
	m_EnemiesLeft = int(m_Enemies.size());


	GameObject* player{ ResourceManager::GetInstance().SpawnPrototype("Bub") };

	player->GetComponent<TransformComponent>()->SetPosition(40, 30, 0);
	player->GetComponent<TransformComponent>()->SetScale(float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()));

	player->GetComponent<PlayerControllerComponent>()->AddObserver(this);
	
	SceneManager::GetInstance().GetActiveScene()->Add(player);

	GameObject* player2{ ResourceManager::GetInstance().SpawnPrototype("Bob") };

	player2->GetComponent<TransformComponent>()->SetPosition(440, 30, 0);
	player2->GetComponent<TransformComponent>()->SetScale(float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()));

	player2->GetComponent<PlayerControllerComponent>()->AddObserver(this);
	
	SceneManager::GetInstance().GetActiveScene()->Add(player2);
}

void CoopMode::OnNotify( void *pData, int events )
{
	switch (events)
	{
	case BB_ENEMY_DIED:
		reinterpret_cast<AIControllerComponent*>(pData)->RemoveObserver(this);
		m_EnemiesLeft--;
		if (m_EnemiesLeft <= 0)
			StartNextLevel();
		break;
	case BB_PLAYER_DIED:
		reinterpret_cast<PlayerControllerComponent*>(pData)->RemoveObserver(this);
		m_PlayerLeft--;
		if (m_PlayerLeft <= 0)
			GameOver();
		break;
	default:;
	}
}

void CoopMode::StartNextLevel()
{
	LevelLoader::GetInstance().LoadLevel(++m_Level, BubbleBobbleGameMode::Coop);
}

void CoopMode::GameOver()
{
	SceneManager::GetInstance().SetActiveScene("GameOverScene");
}
