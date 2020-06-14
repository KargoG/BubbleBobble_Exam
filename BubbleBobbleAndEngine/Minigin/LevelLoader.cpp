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
		glm::vec3 pos{0, 8 * 24, 0}; // TODO turn literals into variables u bloke
		
		level = SceneManager::GetInstance().CreateScene("Level" + std::to_string(levelNumber));
		for (int levelLine = 0; levelLine < 25; ++levelLine) // TODO turn 25 into literal (maybe in Game information???)
		{
			pos.x = 0; // reset X at the beginning of a line
			
			// Read line
			char line{};
			
			for( int byte = 0; byte < 4; ++byte)
			{
				reader.ReadVarFromFile(line); // every lines has 4 bytes for 32 bits (bitmask)
				int mask{ 0b10000000 }; // 0b1000 0000
				
				for( int i = 0; i < 8; ++i ) // 8 bit in a byte (nom)
				{
					if (line & mask)// if block wanted
					{
						GameObject* newBlock{ ResourceManager::GetInstance().SpawnPrototype("BaseWall") };// TODO change Wall type based on level

						newBlock->GetComponent<TransformComponent>()->SetPosition(pos);
						level->Add(newBlock);
					}
					
					mask >>= 1; // bit shift bitmask
					pos.x += 8; // TODO remove literal
				}
			}

			pos.y -= 8; // TODO remove literal
		}

		GameObject* levelCollider{ new GameObject{} };
		levelCollider->AddComponent(new BoxColliderComponent{ float(GameData::GetInstance().GetWindowWidth()), 20 });
		levelCollider->GetComponent<TransformComponent>()->SetPosition(0, float(GameData::GetInstance().GetWindowHeight()), 0);
		level->Add(levelCollider);
		
		levelNumber++;
	}

	
	reader.CloseReader();
}

void LevelLoader::LoadLevel( int level, GameMode gameMode )
{
	SceneManager::GetInstance().SetActiveScene("Level" + std::to_string(level));

	// TODO Spawn enemies in Single and duo game mode
}
