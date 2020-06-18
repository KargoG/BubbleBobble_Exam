#include "pch.h"
#include "BoxColliderComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "RigidbodyComponent.h"
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>

BoxColliderComponent::BoxColliderComponent(float width, float height, bool isTrigger, glm::vec2 center, const PhysicsMat& pMat) : ColliderComponent{isTrigger, center, pMat}, m_Dimensions { width, height }
{
}

void BoxColliderComponent::Start()
{
	ColliderComponent::Start();
	
	b2FixtureDef def;

	b2PolygonShape boxShape{};
	boxShape.SetAsBox(m_Dimensions.x / 2, m_Dimensions.y / 2, { m_Center.x, m_Center.y }, 0);
	
	def.shape = &boxShape;

	def.friction = m_Mat.friction;
	def.restitution = m_Mat.restitution;
	def.density = m_Mat.density;
	
	def.isSensor = m_IsTrigger;

	def.filter = b2Filter{};// TODO
	
	def.userData = this;
	
	m_pShape = m_pRB->AddCollider(def);
}

void BoxColliderComponent::PreSolve( b2Contact *contact, const b2Manifold * )
{
	if (!contact->GetFixtureA()->GetUserData() || !contact->GetFixtureB()->GetUserData())
		return;
	if(m_UseOneWay)
	{
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);
		if (worldManifold.normal.y < -0.5f)
			contact->SetEnabled(false);
		else if(m_pGameObject->GetComponent<TransformComponent>()->GetPosition().y >= reinterpret_cast<BoxColliderComponent*>(contact->GetFixtureB()->GetUserData())->GetGameObject()->GetComponent<TransformComponent>()->GetPosition().y )
			contact->SetEnabled(false);
	}
}

BaseComponent * BoxColliderComponent::Clone() const
{
	BoxColliderComponent* bcc = new BoxColliderComponent{ m_Dimensions.x, m_Dimensions.y, m_IsTrigger, m_Center, m_Mat };
	bcc->m_UseOneWay = m_UseOneWay;
	return bcc;
}

void BoxColliderComponent::LoadFromJson( const nlohmann::json &json )
{
	ColliderComponent::LoadFromJson(json);
	m_Dimensions.x = json.at("Width").get<float>();
	m_Dimensions.y = json.at("Height").get<float>();
	m_UseOneWay = json.at("UseOneWay").get<bool>();
}
