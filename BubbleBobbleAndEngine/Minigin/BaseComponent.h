#pragma once

class GameObject;

class BaseComponent
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void PhysicsUpdate() = 0;
	virtual void Render() const = 0;

	BaseComponent() = default;
	virtual ~BaseComponent() = default;
	
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) = delete;
protected:
	std::weak_ptr<GameObject> m_pGameObject;

private:
	void SetGameObject(std::shared_ptr<GameObject> pGameObject);
	friend GameObject;
};

