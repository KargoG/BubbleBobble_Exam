#include "MiniginPCH.h"
#include "LevelLoader.h"
#include "SceneManager.h"
#include <fstream>
#include "Scene.h"
#include "GameObject.h"
#include "Wall.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "TransformComponent.h"

void LevelLoader::LoadLevel( std::string levelName )
{
	std::string fileLocation{ "../Data/Level/" + levelName };
	Scene *level = SceneManager::GetInstance().CreateScene(levelName);

	std::ifstream levelStream(fileLocation.c_str());

	if (!levelStream)
	{
		std::cerr << "SVGParser::GetVerticesFromSvgFile(..), failed to load vertices from file " << fileLocation << std::endl;
		return;
	}

	glm::vec3 pos{};
	
	// Read the file
	std::string levelLine;
	while (!levelStream.eof())
	{
		std::getline(levelStream, levelLine);

		pos.x = 0;
		
		for( const char blockType : levelLine )
		{
			switch(blockType)
			{
			case '0':
				break;
			case '1':
			{
				GameObject* newBlock{ new Wall{1} };
				newBlock->GetComponent<TransformComponent>()->SetPosition(pos);
				level->Add(newBlock);
				break;
			}
			default: ;
			}


			pos.x += 8;
		}

		pos.y += 8;
		
	}

	// close the file
	levelStream.close();
}
