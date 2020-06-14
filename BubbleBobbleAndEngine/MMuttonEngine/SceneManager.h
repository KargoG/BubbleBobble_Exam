#pragma once
#include "Singleton.h"

class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	void Start();
	void Update();
	void PhysicsUpdate();
	void Render() const;


	Scene* CreateScene(const std::string& name);
	Scene* GetScene(const std::string& name);
	void SetActiveScene(const std::string& name) { m_pActiveScene = GetScene(name); }
	void Swap();
	void SetActiveScene(Scene* scene) { m_pActiveScene = scene; };
	Scene* GetActiveScene() const { return m_pActiveScene; };
	
private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	~SceneManager();
	std::vector<Scene*> m_Scenes;
	Scene* m_pActiveScene{};

};
