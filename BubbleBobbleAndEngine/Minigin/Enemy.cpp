#include "MiniginPCH.h"
#include "Enemy.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "PlayerControllerComponent.h"
#include "TextureRendererComponent.h"
#include "BoxColliderComponent.h"

Enemy::Enemy()
{
	AddComponent(new BoxColliderComponent(8.f, 8.f));

	TextureRendererComponent* texComp{ new TextureRendererComponent() };
	texComp->SetTexture("Textures/Sprites0.png");
	texComp->SetRenderDimensions(0, 14 * 8, 8, 8); // TODO make this work based on texture number

	AddComponent(texComp);

	AddComponent(new RigidbodyComponent{});

	GetComponent<TransformComponent>()->SetPosition(20, 20);

	AddComponent(new InputComponent{});

	PlayerControllerComponent* controller = new PlayerControllerComponent{};
	AddComponent(controller);

	SetPhysicsLayer(PhysicsLayer::Layer03);
	AddIgnoredPhysicsLayers(PhysicsLayer::Layer03);
}
