#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include <box2d/b2_world.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include "../3rdParty/glm/glm/detail/type_vec2.hpp"
#pragma warning(pop)

class GameMode;
class GameObject;

class Scene final : public b2ContactListener
{
	friend Scene* SceneManager::CreateScene(const std::string& name);
public:
	void Add(GameObject* object);
	void Remove(GameObject* toRemove);

	void Awake();
	void Start();
	void Update();
	void PhysicsUpdate();
	void Render();
	void Swap();

	~Scene();
	b2World& GetPhysicsWorld() { return m_PhysicsWorld; };

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;


	std::string GetName() const { return m_Name; }

	template<typename T>
	T* GetObjectWithComponent() const;

	void SetGameMode(GameMode* gameMode);

	void RayCast(b2RayCastCallback *callback, glm::vec2 startpoint, glm::vec2 endpoint);
	
private: 
	explicit Scene(const std::string& name);

	void BeginContact( b2Contact *contact ) override;
	void EndContact( b2Contact *contact ) override;
	void PreSolve( b2Contact *contact, const b2Manifold *oldManifold ) override;
	void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;
	
	std::string m_Name;
	std::vector <GameObject*> m_Objects{};
	std::vector <GameObject*> m_ObjectsToAdd{};
	std::vector <GameObject*> m_ObjectsToRemove{};

	static unsigned int m_IdCounter;
	
	bool m_IsInitialized{ false };

	GameMode* m_pGameMode{ nullptr };


	// physics stuff
	b2World m_PhysicsWorld;
};

template< typename T >
T * Scene::GetObjectWithComponent() const
{
	for( GameObject * const gameObject : m_Objects )
	{
		T* component = gameObject->GetComponent<T>();
		if (component)
			return component;
	}

	return nullptr;
}
