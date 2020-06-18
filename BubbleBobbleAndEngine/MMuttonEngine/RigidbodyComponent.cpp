#include "pch.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "BoxColliderComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Time.h"
#include "GameData.h"
#if _DEBUG
#include "Renderer.h"
#endif

RigidbodyComponent::RigidbodyComponent(b2BodyType bodyType) : m_BodyType{ bodyType }
{
}

RigidbodyComponent::~RigidbodyComponent()
{
	//b2Body* toDelete{ m_pRigidBody };
	//m_pRigidBody = nullptr;
	//m_pGameObject->GetScene()->GetPhysicsWorld().DestroyBody(toDelete);
}

void RigidbodyComponent::Awake()
{
	m_pPlayerTransform = m_pGameObject->GetComponent<TransformComponent>();

	b2BodyDef rbDefinition;

	glm::vec2 pos{ m_pPlayerTransform->GetPosition() };
	rbDefinition.type = m_BodyType;
	rbDefinition.position = { pos.x, pos.y };
	rbDefinition.fixedRotation = true; // TODO make changeable
	rbDefinition.gravityScale = m_ApplyGravity ? 1.f : 0.f;
	rbDefinition.userData = this;
	
	m_pRigidBody = m_pGameObject->GetScene()->GetPhysicsWorld().CreateBody(&rbDefinition);
}

void RigidbodyComponent::Update()
{
}

void RigidbodyComponent::PhysicsUpdate()
{
	b2Vec2 pos{ m_pRigidBody->GetPosition() };
	m_pPlayerTransform->SetPosition(pos.x, pos.y, 0, false);
}

b2Fixture* RigidbodyComponent::AddCollider(const b2FixtureDef& def)
{
	return m_pRigidBody->CreateFixture(&def);
}

BaseComponent * RigidbodyComponent::Clone() const
{
	RigidbodyComponent* rb{ new RigidbodyComponent{} };
	rb->m_BodyType = m_BodyType;
	rb->m_ApplyGravity = m_ApplyGravity;
	return rb;
}

void RigidbodyComponent::LoadFromJson( const nlohmann::json &json )
{
	std::string bodyType = json.at("BodyType").get<std::string>();
	if (bodyType == "kinematic")
		m_BodyType = b2_kinematicBody;
	else if(bodyType == "dynamic")
		m_BodyType = b2_dynamicBody;
	else
		m_BodyType = b2_staticBody;
	
	m_ApplyGravity = json.at("ApplyGravity").get<bool>();
}

void RigidbodyComponent::AddVelocity( float x, float y ) const
{
	m_pRigidBody->SetLinearVelocity(m_pRigidBody->GetLinearVelocity() + b2Vec2{ x, y });
}

void RigidbodyComponent::SetVelocity( float x, float y ) const
{
	m_pRigidBody->SetLinearVelocity(b2Vec2{ x, y });
}

void RigidbodyComponent::Move( float x, float y )
{
	m_pRigidBody->SetTransform(m_pRigidBody->GetPosition() + b2Vec2{ x, y }, 0);
}

void RigidbodyComponent::SetPosition( float x, float y )
{
	m_pRigidBody->SetTransform({ x, y }, 0);
}

void RigidbodyComponent::AddForce( float x, float y )
{
	m_pRigidBody->ApplyForce(b2Vec2{ x, y }, b2Vec2{ m_pPlayerTransform->GetPosition().x, m_pPlayerTransform->GetPosition().y }, true);
}
