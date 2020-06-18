#include "MiniginPCH.h"
#include "LevelLoader.h"
#include "SceneManager.h"
#include <fstream>
#include "Scene.h"
#include "GameObject.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "TransformComponent.h"
#include "ResourceManager.h"
#include <BinaryReader.h>
#include "GameData.h"
#include "VersusMode.h"
#include "SingleMode.h"
#include "CoopMode.h"
#include "BoxColliderComponent.h"

void LevelLoader::Init()
{
	std::string fileLocation{ "../Data/Level/SeperatedLevelData.dat" };
	
	BinaryReader reader{};
	reader.OpenReader(fileLocation);
	
	int levelNumber{0};
	for( Scene *& level : m_Levels )
	{
		glm::vec3 pos{0, (GameData::GetInstance().GetLevelHeight() - 1), 0};
		
		level = SceneManager::GetInstance().CreateScene("Level" + std::to_string(levelNumber));
		for (int levelLine = 0; levelLine < GameData::GetInstance().GetLevelHeight(); ++levelLine)
		{
			pos.x = 0; // reset X at the beginning of a line
			
			// Read line
			char line{};
			
			for( int byte = 0; byte < GameData::GetInstance().GetLevelWidth() / 8; ++byte)
			{
				reader.ReadVarFromFile(line); // every lines has 4 bytes for 32 bits (bitmask)
				int mask{ 0b10000000 }; // 0b1000 0000
				
				for( int i = 0; i < 8; ++i ) // 8 bit in a byte (nom)
				{
					if (line & mask)// if block wanted
					{
						GameObject* newBlock{ ResourceManager::GetInstance().SpawnPrototype("BaseWall") };// TODO change Wall type based on level

						TransformComponent* blockTransform{ newBlock->GetComponent<TransformComponent>() };
						blockTransform->SetPosition(pos);
						blockTransform->SetScale(float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()), float(GameData::GetInstance().GetSpriteScale()));
						level->Add(newBlock);
					}
					
					mask >>= 1; // bit shift bitmask
					pos.x += GameData::GetInstance().GetSpriteScale();
				}
			}

			pos.y -= GameData::GetInstance().GetSpriteScale();
		}

		GameObject* levelCollider{ new GameObject{} };
		levelCollider->AddComponent(new RigidbodyComponent{});
		levelCollider->AddComponent(new BoxColliderComponent{ float(GameData::GetInstance().GetLevelWidth()), 1 });
		
		levelCollider->GetComponent<TransformComponent>()->SetPosition(float(GameData::GetInstance().GetLevelWidth()) / 2, float(GameData::GetInstance().GetLevelHeight()) + 0.5f, 0);
		level->Add(levelCollider);
		
		levelNumber++;
	}

	
	reader.CloseReader();

	// read Enemies
	fileLocation = "../Data/Level/SeperatedEnemyData.dat";
	reader.OpenReader(fileLocation);

	for (std::vector<EnemyData>& enemies : m_Enemies)
	{
		while (true)
		{
			EnemyData newEnemy{};
			
			char byte[3]{};
			reader.ReadVarFromFile(byte[0]);

			if (byte[0] == 0b00000000)
				break;

			reader.ReadVarFromFile(byte[1]);
			reader.ReadVarFromFile(byte[2]);

			char enemyType{ byte[0] & 0b00000111 };
			
			switch(EnemyType(enemyType))
			{
			case EnemyType::ZenChan:
				newEnemy.EnemyType = EnemyType::ZenChan;
				break;
			case EnemyType::Maita:
				newEnemy.EnemyType = EnemyType::Maita;
				break;
			default:
				newEnemy.EnemyType = EnemyType::ZenChan;
			}

			newEnemy.Column = (byte[0] & 0b11111000) >> 3;
			newEnemy.Row = (byte[1] & 0b11111000) >> 3;
			newEnemy.Row = GameData::GetInstance().GetLevelHeight() - newEnemy.Row + 1;
			
			enemies.push_back(newEnemy);
		}
	}

	reader.CloseReader();
}

void LevelLoader::LoadLevel( int level, BubbleBobbleGameMode gameMode )
{
	SceneManager::GetInstance().SetActiveScene("Level" + std::to_string(level));

	switch(gameMode)
	{
	case BubbleBobbleGameMode::Single:
	{
		SingleMode* singleMode{ new SingleMode{ m_Enemies[level], level } };

		SceneManager::GetInstance().GetActiveScene()->SetGameMode(singleMode);
		break;
	}
	case BubbleBobbleGameMode::Coop:
	{
		CoopMode* coopMode{ new CoopMode{ m_Enemies[level], level } };

		SceneManager::GetInstance().GetActiveScene()->SetGameMode(coopMode);
		break;
	}
	case BubbleBobbleGameMode::Versus:
	{
		VersusMode* versusMode{ new VersusMode{level} };

		SceneManager::GetInstance().GetActiveScene()->SetGameMode(versusMode);
		break;
	}
	default: ;
	}
}
