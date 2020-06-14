#pragma once
#include "SceneManager.h"
#include "BoxColliderComponent.h"
#include "GameObject.h"

class GameMode;
class GameObject;

class Scene final
{
	friend Scene* SceneManager::CreateScene(const std::string& name);
public:
	void Add(GameObject* object);
	void Remove(GameObject* toRemove);

	void Start();
	void Update();
	void PhysicsUpdate();
	void Render() const;
	void Swap();

	~Scene();

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;


	std::string GetName() { return m_Name; }
	void AddCollider(BoxColliderComponent* collider) { m_Collider.push_back(collider); }
	void RemoveCollider(BoxColliderComponent* colliderToRemove);

	const std::vector <BoxColliderComponent*>& GetCollider() const { return m_Collider; };

	template<typename T>
	T* GetObjectWithComponent() const;

	void SetGameMode(GameMode* gameMode);
	
private: 
	explicit Scene(const std::string& name);

	std::string m_Name;
	std::vector <GameObject*> m_Objects{};
	std::vector <GameObject*> m_ObjectsToAdd{};
	std::vector <GameObject*> m_ObjectsToRemove{};
	std::vector <BoxColliderComponent*> m_Collider{};
	std::vector <BoxColliderComponent*> m_ColliderToRemove{};

	static unsigned int m_IdCounter;
	
	bool m_IsInitialized{ false };

	GameMode* m_pGameMode{ nullptr };
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
