#include "MiniginPCH.h"
#include "VersusMode.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "TransformComponent.h"
#include "GameData.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerControllerComponent.h"
#include "ResourceManager.h"
#include "LevelLoader.h"

void VersusMode::Awake()
{
	GameObject* player{ ResourceManager::GetInstance().SpawnPrototype("Bub") };
	GameObject *enemy{ ResourceManager::GetInstance().SpawnPrototype("ControlledMaita") };

	player->GetComponent<TransformComponent>()->SetPosition(5, 3.8f, 0);
	player->GetComponent<TransformComponent>()->SetScale(float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()));

	enemy->GetComponent<TransformComponent>()->SetPosition(55, 3.8f, 0);
	enemy->GetComponent<TransformComponent>()->SetScale(float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()));

	enemy->GetComponent<PlayerControllerComponent>()->SetPlayerNumber(1);

	player->GetComponent<PlayerControllerComponent>()->AddObserver(this);
	enemy->GetComponent<PlayerControllerComponent>()->AddObserver(this);
	
	SceneManager::GetInstance().GetActiveScene()->Add(player);
	SceneManager::GetInstance().GetActiveScene()->Add(enemy);
}

void VersusMode::OnNotify( void *pData, int events )
{
	switch (events)
	{
	case BB_PLAYER_DIED:
	{
		PlayerControllerComponent* deadPlayer = reinterpret_cast<PlayerControllerComponent*>(pData);
		deadPlayer->RemoveObserver(this);
		ShowWinner(1 - deadPlayer->GetPlayerNumber());
		break;
	}
	default:;
	}
}

void VersusMode::StartNextLevel()
{
	LevelLoader::GetInstance().LoadLevel(++m_Level, BubbleBobbleGameMode::Versus);
}

void VersusMode::ShowWinner( int winner )
{
	if(winner == 0)
	{
		SceneManager::GetInstance().SetActiveScene("PlayerOneWinScene");
	}
	else
	{
		SceneManager::GetInstance().SetActiveScene("PlayerTwoWinScene");
	}
}
