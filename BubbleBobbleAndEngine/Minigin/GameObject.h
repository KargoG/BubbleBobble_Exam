#pragma once

class BaseComponent;
class Texture2D;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	virtual void Start();
	virtual void Update();
	virtual void PhysicsUpdate();
	virtual void Render() const;

	GameObject() = default;
	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	template<typename T>
	std::shared_ptr<T> GetComponent();
	
	void AddComponent(const std::shared_ptr<BaseComponent> object);
	
private:
	std::vector<std::shared_ptr<BaseComponent>> m_pComponents{};
};

template< typename T >
std::shared_ptr<T> GameObject::GetComponent()
{
	for(std::shared_ptr<BaseComponent>& pComponent : m_pComponents )
	{
		if(typeid(T) == typeid(*pComponent))
		{
			return std::reinterpret_pointer_cast<T>(pComponent);
		}
	}

	return nullptr;
}
