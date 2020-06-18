#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameMode.h"
#include "Time.h"
#include <box2d/b2_contact.h>
#include <box2d/b2_draw.h>
#include "Renderer.h"
#include "ColliderComponent.h"

unsigned int Scene::m_IdCounter = 0;

void Scene::SetGameMode( GameMode *gameMode )
{
	if (!gameMode)
		throw std::exception("GameMode can not be null!\n");

	if (m_pGameMode)
		delete m_pGameMode;
	
	m_pGameMode = gameMode;
	
	if (m_IsInitialized)
	{
		m_pGameMode->Awake();
		m_pGameMode->Start();
	}
}

Scene::Scene(const std::string& name) : m_Name(name), m_PhysicsWorld{ b2Vec2{0.0f, -9.81f} } {}


void Scene::BeginContact( b2Contact *contact )
{
	bool aIsTrigger{ contact->GetFixtureA()->IsSensor() };
	bool bIsTrigger{ contact->GetFixtureB()->IsSensor() };

	Collision collsionA{ reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData()), reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData()) };
	Collision collsionB{ reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData()), reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData()) };
	
	if(collsionA.ownCollider && aIsTrigger)
	{
		reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData())->GetGameObject()->OnTriggerEnter(&collsionA);
		if(collsionB.ownCollider && bIsTrigger)
		{
			reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData())->GetGameObject()->OnTriggerEnter(&collsionB);
		}
		return;
	}

	if (collsionB.ownCollider && bIsTrigger)
	{
		reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData())->GetGameObject()->OnTriggerEnter(&collsionB);
		return;
	}

	if (collsionA.ownCollider)
		reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData())->GetGameObject()->OnCollisionEnter(&collsionA);
	if (collsionB.ownCollider)
		reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData())->GetGameObject()->OnCollisionEnter(&collsionB);
}

void Scene::EndContact( b2Contact *contact )
{
	bool aIsTrigger{ contact->GetFixtureA()->IsSensor() };
	bool bIsTrigger{ contact->GetFixtureB()->IsSensor() };
	
	Collision collsionA{ reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData()), reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData()) };
	Collision collsionB{ reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData()), reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData()) };
	
	if (collsionA.ownCollider && aIsTrigger)
	{
		reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData())->GetGameObject()->OnTriggerExit(&collsionA);
		if (collsionB.ownCollider && bIsTrigger)
		{
			reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData())->GetGameObject()->OnTriggerExit(&collsionB);
		}
		return;
	}

	if (collsionB.ownCollider && bIsTrigger)
	{
		reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData())->GetGameObject()->OnTriggerExit(&collsionB);

		return;
	}

	if(collsionA.ownCollider)
		reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData())->GetGameObject()->OnCollisionExit(&collsionA);
	if (collsionB.ownCollider)
		reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData())->GetGameObject()->OnCollisionExit(&collsionB);
}

void Scene::PreSolve( b2Contact *contact, const b2Manifold *oldManifold )
{
	ColliderComponent* coll1{ reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData()) };
	ColliderComponent* coll2{ reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData()) };
	if(coll1)
		coll1->PreSolve(contact, oldManifold);

	if(coll2)
		coll2->PreSolve(contact, oldManifold);
}

void Scene::PostSolve( b2Contact *contact, const b2ContactImpulse *impulse )
{
	ColliderComponent* coll1{ reinterpret_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData()) };
	ColliderComponent* coll2{ reinterpret_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData()) };

	if (coll1)
		coll1->GetGameObject()->PostSolve(contact, impulse);
	if (coll2)
		coll2->GetGameObject()->PostSolve(contact, impulse);
}

void Scene::RayCast(b2RayCastCallback* callback, glm::vec2 startpoint, glm::vec2 endpoint)
{
	m_PhysicsWorld.RayCast(callback, b2Vec2{ startpoint.x, startpoint.y }, b2Vec2{ endpoint.x, endpoint.y });
}

Scene::~Scene()
{
	for (GameObject* pGameObject : m_Objects)
	{
		delete pGameObject;
	}
	m_Objects.clear();

	delete m_pGameMode;
}

//void Scene::RemoveCollider( BoxColliderComponent *colliderToRemove ) TODO delete???
//{
//	m_ColliderToRemove.push_back(colliderToRemove);
//}

void Scene::Add(GameObject* object)
{
	if (object->GetScene())
		throw std::exception{"A GameObject can only be part of 1 Scene!"};
	
	m_ObjectsToAdd.push_back(object);
}

void Scene::Remove( GameObject *toRemove )
{
	if (toRemove->GetScene() != this)
		throw std::exception{ "You cant remove an object from a Scene its not part of!" };

	m_ObjectsToRemove.push_back(toRemove);
}

void Scene::Awake()
{
	m_PhysicsWorld.SetContactListener(this);
	b2Draw* pRenderer{ &Renderer::GetInstance() };
	pRenderer->SetFlags(b2Draw::e_aabbBit | b2Draw::e_shapeBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
	m_PhysicsWorld.SetDebugDraw(pRenderer);

	if (!m_pGameMode)
		m_pGameMode = new GameMode{};

	m_pGameMode->Awake();

	for (GameObject* objectToAdd : m_ObjectsToAdd)
	{
		m_Objects.push_back(objectToAdd);
		objectToAdd->SetScene(this);
	}
	m_ObjectsToAdd.clear();

	for (GameObject* pGameObject : m_Objects)
	{
		pGameObject->Awake();
	}
}

void Scene::Start()
{
	m_pGameMode->Start();
	
	for (GameObject* objectToAdd : m_ObjectsToAdd)
	{
		m_Objects.push_back(objectToAdd);
		objectToAdd->SetScene(this);
	}
	m_ObjectsToAdd.clear();
	
	for(GameObject * pGameObject : m_Objects )
	{
		pGameObject->Start();
	}

	m_IsInitialized = true;
}

void Scene::Update()
{
	if (!m_IsInitialized)
	{
		Awake();
		Start();
	}

	m_pGameMode->Update();
	
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::PhysicsUpdate()
{
	m_PhysicsWorld.Step(Time::GetInstance().GetPhysicsDeltaTime(), 8, 3); // TODO turn iterations into changeable variables?
	
	m_pGameMode->PhysicsUpdate();
	
	for (auto& object : m_Objects)
	{
		object->PhysicsUpdate();
	}
}

void Scene::Render()
{
	m_pGameMode->Render();
	
	for (const auto& object : m_Objects)
	{
		object->Render();
	}

#if _DEBUG
	m_PhysicsWorld.DebugDraw();
#endif
}

void Scene::Swap()
{
	
	m_pGameMode->Swap();
	for (GameObject* objectToRemove : m_ObjectsToRemove)
	{
		auto it = std::find(m_Objects.cbegin(), m_Objects.cend(), objectToRemove);
		if (it != m_Objects.cend())
		{
			m_PhysicsWorld.DestroyBody((*it)->GetComponent<RigidbodyComponent>()->GetRB());
			m_Objects.erase(it);
			delete objectToRemove;
		}
	}
	m_ObjectsToRemove.clear();
	
	//for (ColliderComponent* colliderToRemove : m_ColliderToRemove)
	//{
	//	m_Collider.erase(std::find(m_Collider.cbegin(), m_Collider.cend(), colliderToRemove));
	//}
	//m_ColliderToRemove.clear();
	
	for (GameObject* objectToAdd : m_ObjectsToAdd)
	{
		m_Objects.push_back(objectToAdd);
		objectToAdd->SetScene(this);
		objectToAdd->Awake();
		objectToAdd->Start();
	}
	m_ObjectsToAdd.clear();
	
	for( GameObject * object : m_Objects )
	{
		object->Swap();
	}
}

