#pragma once

class BaseComponent;

class GameObject
{
public:
	virtual void Start();
	virtual void Update();
	virtual void PhysicsUpdate();
	virtual void Render() const;

	GameObject();
	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	template<typename T>
	T* GetComponent();
	
	void AddComponent(BaseComponent * pComponent);
	
private:
	std::vector<BaseComponent*> m_pComponents{};
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
