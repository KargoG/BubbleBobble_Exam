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

Enemy::Enemy()
{
	AddComponent(new BoxColliderComponent(float(GameData::GetInstance().GetSpriteWidth()), float(GameData::GetInstance().GetSpriteHeight())));

	TextureRendererComponent* texComp{ new TextureRendererComponent() };
	texComp->SetTexture("Textures/Sprites0.png");
	texComp->SetRenderDimensions(0, 14.f * GameData::GetInstance().GetSpriteHeight(), float(GameData::GetInstance().GetSpriteWidth()), float(GameData::GetInstance().GetSpriteHeight())); // TODO make this work based on texture number

	AddComponent(texComp);

	AddComponent(new RigidbodyComponent{});

	GetComponent<TransformComponent>()->SetPosition(20, 20);

	AddComponent(new InputComponent{});

	PlayerControllerComponent* controller = new PlayerControllerComponent{};
	AddComponent(controller);

	AddComponent(new AnimationComponent{ new MaitaWalkingRight{} });
	
	SetPhysicsLayer(PhysicsLayer::Layer03);
	AddIgnoredPhysicsLayers(PhysicsLayer::Layer03);
}
