#include "MiniginPCH.h"
#include "Player.h"
#include "BoxColliderComponent.h"
#include "TextureRendererComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "PlayerControllerComponent.h"
#include "AnimationComponent.h"
#include "GameData.h"
#include "AnimationStates.h"
#include "BubbleBobbleAnimationComponent.h"

Player::Player()
{
	AddComponent(new BoxColliderComponent(float(GameData::GetInstance().GetSpriteWidth() - 1), float(GameData::GetInstance().GetSpriteHeight() - 1)));

	TextureRendererComponent *texComp{ new TextureRendererComponent() }; 
	texComp->SetTexture("Textures/Sprites0.png");
	texComp->SetRenderDimensions(0, 0, 8, 8); // TODO make this work based on texture number
	
	AddComponent(texComp);

	AddComponent(new RigidbodyComponent{});
	
	GetComponent<TransformComponent>()->SetPosition(20, 20);

	AddComponent(new InputComponent{});
	
	PlayerControllerComponent* controller = new PlayerControllerComponent{};
	AddComponent(controller);

	AddComponent(new BubbleBobbleAnimationComponent{new PlayerWalkingRight{}});
	
	SetPhysicsLayer(PhysicsLayer::Layer01);
	AddIgnoredPhysicsLayers(PhysicsLayer::Layer01);
	AddIgnoredPhysicsLayers(PhysicsLayer::Layer02);
}
