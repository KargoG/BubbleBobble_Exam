#pragma once
#include <unordered_map>
#include <json.hpp>
#include "BaseComponent.h"

struct Collision;
class GameObject;
struct b2ContactImpulse;
class b2Contact;

class BaseComponent abstract
{
public:
	virtual void Awake(){}
	virtual void Start(){};
	virtual void Update(){};
	virtual void PhysicsUpdate(){};
	virtual void Render() const{};
	virtual BaseComponent* Clone() const = 0;
	virtual void LoadFromJson(const nlohmann::json& json) = 0;
	virtual void OnCollisionEnter(const Collision*) {}
	virtual void OnCollisionExit(const Collision*) {}
	virtual void OnTriggerEnter(const Collision*) {}
	virtual void OnTriggerExit(const Collision*) {}
	virtual void PostSolve(b2Contact*, const b2ContactImpulse*){};
	virtual void Swap(){}

	
	BaseComponent();
	virtual ~BaseComponent() = default;
	
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) = delete;

	GameObject* GetGameObject() const { return m_pGameObject; }

	
	static BaseComponent* CreateFromJson(const std::string& componentName, const nlohmann::json& json);
	static void RegisterEngineComponents();
	static void RegisterComponent(const std::string& componentName, BaseComponent* prototype);
	static void CleanUp();

protected:
	GameObject *m_pGameObject{};

private:
	void SetGameObject(GameObject *pGameObject);
	friend GameObject;

	static std::unordered_map<std::string, BaseComponent*> m_ComponentPrototypes;
};

