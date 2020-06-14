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
#include "Player.h"
#include "ResourceManager.h"
#include <BinaryReader.h>
#include "GameData.h"

void LevelLoader::Init()
{
	std::string fileLocation{ "../Data/Level/SeperatedLevelData.dat" };
	
	BinaryReader reader{};
	reader.OpenReader(fileLocation); // TODO Load enemies
	
	int levelNumber{0};
	for( Scene *& level : m_Levels )
	{
		glm::vec3 pos{0, GameData::GetInstance().GetSpriteHeight() * (GameData::GetInstance().GetLevelHeight() - 1) * GameData::GetInstance().GetSpriteScale(), 0};
		
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
					pos.x += GameData::GetInstance().GetSpriteWidth() * GameData::GetInstance().GetSpriteScale();
				}
			}

			pos.y -= GameData::GetInstance().GetSpriteHeight() * GameData::GetInstance().GetSpriteScale();
		}

		GameObject* levelCollider{ new GameObject{} };
		levelCollider->AddComponent(new BoxColliderComponent{ float(GameData::GetInstance().GetWindowWidth()), 20 });
		
		levelCollider->GetComponent<TransformComponent>()->SetPosition(0, float(GameData::GetInstance().GetWindowHeight()), 0);
		level->Add(levelCollider);
		
		levelNumber++;
	}

	
	reader.CloseReader();
}

void LevelLoader::LoadLevel( int level, GameMode )
{
	SceneManager::GetInstance().SetActiveScene("Level" + std::to_string(level));

	// TODO Spawn enemies in Single and duo game mode
}
