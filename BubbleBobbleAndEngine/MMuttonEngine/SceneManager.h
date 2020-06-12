#pragma once
#include "Singleton.h"

class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene* CreateScene(const std::string& name);
	Scene* GetScene(const std::string& name);

	void Start();
	void Update();
	void PhysicsUpdate();
	void Render();
private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	~SceneManager();
	std::vector<Scene*> m_Scenes;
};
