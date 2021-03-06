#pragma once
#include <Windows.h>
#include <XInput.h>
#include <memory>
#include "BaseComponent.h"
#pragma comment(lib, "XInput.lib")

class ControllerComponent;

enum class ControllerButton
{
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	DPadLeft,
	DPadRight,
	DPadUp,
	DPadDown,
	Start,
	Back,
	LeftThumbstick,
	RightThumbstick,
	LeftShoulder,
	RightShoulder
	//...
};

enum class ControllerAxis
{
	LeftThumbstickX,
	LeftThumbstickY,
	RightThumbstickX,
	RightThumbstickY,
	LeftTrigger,
	RightTrigger
	
};

class Command abstract
{
public:
	virtual ~Command() = default;
	virtual void Execute(ControllerComponent* controller) = 0;
};

class InputComponent : public BaseComponent
{
public:
	~InputComponent() override;
	
	BaseComponent* Clone() const override;
	void LoadFromJson(const nlohmann::json& json) override;
	
	bool IsPressed(ControllerButton button) const;
	float GetAxis(ControllerAxis axis) const;

	void RegisterCommand(ControllerButton button, Command *command);

	void SetControllerNumber(int controllerNumber) { m_Controller = controllerNumber; };
	int GetControllerNumber( ) const { return m_Controller; };

	Command* ProcessInput();
private:
	int m_Controller{ 0 };
	
	XINPUT_STATE m_State{};

	Command* m_pButtonA{ nullptr };
	Command* m_pButtonB{ nullptr };
	Command* m_pButtonX{ nullptr };
	Command* m_pButtonY{ nullptr };
	Command* m_pDPadLeft{ nullptr };
	Command* m_pDPadRight{ nullptr };
	Command* m_pDPadUp{ nullptr };
	Command* m_pDPadDown{ nullptr };
	Command* m_pStart{ nullptr };
	Command* m_pBack{ nullptr };
	Command* m_pLeftThumbstick{ nullptr };
	Command* m_pRightThumbstick{ nullptr };
	Command* m_pLeftShoulder{ nullptr };
	Command* m_pRightShoulder{ nullptr };

	float m_DeadZone{ 0.1f };
};
