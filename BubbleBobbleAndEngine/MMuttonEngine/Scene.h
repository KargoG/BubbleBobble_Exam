#pragma once
#include "SceneManager.h"
#include "BoxColliderComponent.h"

class GameObject;

class Scene final
{
	friend Scene* SceneManager::CreateScene(const std::string& name);
public:
	void Add(GameObject* object);

	void Start();
	void Update();
	void PhysicsUpdate();
	void Render() const;

	~Scene();
	
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;


	std::string GetName() { return m_Name; }
	void AddCollider(BoxColliderComponent* collider) { m_Collider.push_back(collider); }
	void RemoveCollider(BoxColliderComponent* colliderToRemove);

	const std::vector <BoxColliderComponent*>& GetCollider() const { return m_Collider; };

private: 
	explicit Scene(const std::string& name);

	std::string m_Name;
	std::vector <GameObject*> m_Objects{};
	std::vector <BoxColliderComponent*> m_Collider{};

	static unsigned int m_IdCounter; 
};
