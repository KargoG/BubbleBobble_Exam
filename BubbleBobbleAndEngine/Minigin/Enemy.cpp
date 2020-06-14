#include "MiniginPCH.h"
#include "Enemy.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "PlayerControllerComponent.h"
#include "TextureRendererComponent.h"
#include "BoxColliderComponent.h"
#include "GameData.h"
#include "AnimationStates.h"
#include "AIControllerComponent.h"
#include "BubbleBobbleAnimationComponent.h"

Enemy::Enemy()
{
	AddComponent(new BoxColliderComponent(float(GameData::GetInstance().GetSpriteWidth()), float(GameData::GetInstance().GetSpriteHeight())));

	TextureRendererComponent* texComp{ new TextureRendererComponent() };
	texComp->SetTexture("Textures/Sprites0.png");
	texComp->SetRenderDimensions(0, 14.f * GameData::GetInstance().GetSpriteHeight(), float(GameData::GetInstance().GetSpriteWidth()), float(GameData::GetInstance().GetSpriteHeight()));

	AddComponent(texComp);

	AddComponent(new RigidbodyComponent{});

	GetComponent<TransformComponent>()->SetPosition(20, 20);

	AddComponent(new InputComponent{});

	//AIControllerComponent* controller = new AIControllerComponent{};
	PlayerControllerComponent* controller = new PlayerControllerComponent{};
	AddComponent(controller);

	AddComponent(new BubbleBobbleAnimationComponent{ new MaitaWalkingRight{} });
	
	SetPhysicsLayer(PhysicsLayer::Layer03);
	AddIgnoredPhysicsLayers(PhysicsLayer::Layer03);
	AddIgnoredPhysicsLayers(PhysicsLayer::Layer04);
}
