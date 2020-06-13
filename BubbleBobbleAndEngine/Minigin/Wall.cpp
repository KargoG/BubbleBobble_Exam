#include "MiniginPCH.h"
#include "Wall.h"
#include "BoxColliderComponent.h"
#include "TextureRendererComponent.h"

Wall::Wall(int textureNumber)
{
	UNREFERENCED_PARAMETER(textureNumber);
	AddComponent(new BoxColliderComponent(8.f, 8.f));

	//TextureRendererComponent *texComp{ new TextureRendererComponent() }; TODO uncomment when physics works
	//texComp->SetTexture("Textures/LevelBlocks.png");
	//texComp->SetRenderDimensions(0, 0, 8, 8); // TODO make this work based on texture number
	//
	//AddComponent(texComp);
}
