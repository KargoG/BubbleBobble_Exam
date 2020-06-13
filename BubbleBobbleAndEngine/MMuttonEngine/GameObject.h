#pragma once
#include <json.hpp>

class Scene;
class BaseComponent;

class GameObject
{
public:
	virtual void Start();
	virtual void Update();
	virtual void PhysicsUpdate();
	virtual void Render() const;
	virtual GameObject* Clone() const;

	GameObject();
	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	template<typename T>
	T* GetComponent();
	
	void AddComponent(BaseComponent * pComponent);
	void SetScene(Scene* pScene) { m_pScene = pScene; };
	Scene* GetScene() const { return m_pScene; };

	static GameObject* CreateFromJson(const nlohmann::json& json);
private:
	std::vector<BaseComponent*> m_pComponents{};
	Scene* m_pScene;
};

template< typename T >
T* GameObject::GetComponent()
{
	for(BaseComponent* pComponent : m_pComponents )
	{
		if(typeid(T) == typeid(*pComponent))
		{
			return reinterpret_cast<T*>(pComponent);
		}
	}

	return nullptr;
}
