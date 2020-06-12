#pragma once
#include "SceneManager.h"

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
	std::string GetName() { return m_Name; };
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private: 
	explicit Scene(const std::string& name);

	std::string m_Name;
	std::vector <GameObject*> m_Objects{};

	static unsigned int m_IdCounter; 
};
