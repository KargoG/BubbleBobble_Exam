#pragma once
#include <json.hpp>

class Scene;
class BaseComponent;

enum class PhysicsLayer
{
	Layer00 = 1 << 0,
	Layer01 = 1 << 1,
	Layer02 = 1 << 2,
	Layer03 = 1 << 3,
	Layer04 = 1 << 4,
	Layer05 = 1 << 5,
	Layer06 = 1 << 6,
	Layer07 = 1 << 7,
	Layer08 = 1 << 8,
	Layer09 = 1 << 9,
	Layer10 = 1 << 10,
	Layer11 = 1 << 11,
	Layer12 = 1 << 12,
	Layer13 = 1 << 13,
	Layer14 = 1 << 14,
	Layer15 = 1 << 15,
	Layer16 = 1 << 16,
	Layer17 = 1 << 17,
	Layer18 = 1 << 18,
	Layer19 = 1 << 19,
	Layer20 = 1 << 20,
	Layer21 = 1 << 21,
	Layer22 = 1 << 22,
	Layer23 = 1 << 23,
	Layer24 = 1 << 24,
	Layer25 = 1 << 25,
	Layer26 = 1 << 26,
	Layer27 = 1 << 27,
	Layer28 = 1 << 28,
	Layer29 = 1 << 29,
	Layer30 = 1 << 30,
	Layer31 = 1 << 31,
	None = 0
};

class GameObject
{
public:
	virtual void Start();
	virtual void Update();
	virtual void PhysicsUpdate();
	virtual void Render() const;
	virtual void Swap();
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

	void SetPhysicsLayer(PhysicsLayer newLayer) { m_Layer = newLayer; }
	PhysicsLayer GetPhysicsLayer() const { return m_Layer; }
	void AddIgnoredPhysicsLayers(PhysicsLayer newLayers) { m_IgnoredLayers = PhysicsLayer(int(m_IgnoredLayers) | int(newLayers)); }
	void SetIgnoredPhysicsLayers(PhysicsLayer layers) { m_IgnoredLayers = layers; }
	PhysicsLayer GetIgnoredPhysicsLayers() const { return m_IgnoredLayers; }
	
private:
	std::vector<BaseComponent*> m_pComponents{};
	Scene* m_pScene;
	PhysicsLayer m_Layer{ PhysicsLayer::Layer00 };
	PhysicsLayer m_IgnoredLayers{ PhysicsLayer::None };
	
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
