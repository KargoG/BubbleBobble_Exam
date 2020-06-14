#pragma once
#include <unordered_map>
#include <json.hpp>
#include "BaseComponent.h"
#include "BaseComponent.h"
#include "BaseComponent.h"
#include "BaseComponent.h"
#include "BaseComponent.h"

class GameObject;

class BaseComponent
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void PhysicsUpdate() = 0;
	virtual void Render() const = 0;
	virtual BaseComponent* Clone() const = 0;
	virtual void LoadFromJson(const nlohmann::json& json) = 0;
	virtual void Swap(){};

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

