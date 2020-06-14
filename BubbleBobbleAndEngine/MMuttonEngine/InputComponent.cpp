#include "pch.h"
#include "InputComponent.h"
#include <iostream>
#include <SDL.h>

Command* InputComponent::ProcessInput()
{
	DWORD result = XInputGetState(m_Controller, &m_State);

	if (result == ERROR_DEVICE_NOT_CONNECTED)
	{
		std::cout << "Controller 1 not connected\n";
		
		return nullptr;
	}
	else
	{
		// Configurable
		if (IsPressed(ControllerButton::ButtonA))
			return m_pButtonA;
		else if (IsPressed(ControllerButton::ButtonB))
			return m_pButtonB;
		else if (IsPressed(ControllerButton::ButtonY))
			return m_pButtonY;
		else if (IsPressed(ControllerButton::ButtonX))
			return m_pButtonX;
		else if (IsPressed(ControllerButton::DPadLeft))
			return m_pDPadLeft;
		else if (IsPressed(ControllerButton::DPadRight))
			return m_pDPadRight;
		else if (IsPressed(ControllerButton::DPadUp))
			return m_pDPadUp;
		else if (IsPressed(ControllerButton::DPadDown))
			return m_pDPadDown;
		else if (IsPressed(ControllerButton::Start))
			return m_pStart;
		else if (IsPressed(ControllerButton::Back))
			return m_pBack;
		else if (IsPressed(ControllerButton::LeftThumbstick))
			return m_pLeftThumbstick;
		else if (IsPressed(ControllerButton::RightThumbstick))
			return m_pRightThumbstick;
		else if (IsPressed(ControllerButton::LeftShoulder))
			return m_pLeftShoulder;
		else if (IsPressed(ControllerButton::RightShoulder))
			return m_pRightShoulder;
	}

	return nullptr;
}

bool InputComponent::IsPressed(ControllerButton button) const
{
	switch(button)
	{
	case ControllerButton::ButtonA:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_A);
	case ControllerButton::ButtonB:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_B);
	case ControllerButton::ButtonX:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_X);
	case ControllerButton::ButtonY:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
	case ControllerButton::DPadLeft:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	case ControllerButton::DPadRight:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
	case ControllerButton::DPadUp:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	case ControllerButton::DPadDown:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	case ControllerButton::Start:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_START);
	case ControllerButton::Back:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
	case ControllerButton::LeftThumbstick:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
	case ControllerButton::RightThumbstick:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
	case ControllerButton::LeftShoulder:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
	case ControllerButton::RightShoulder:
		return (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
	default: ;
	}
	
	return false;
}

float InputComponent::GetAxis(ControllerAxis axis) const
{
	float value{ 0 };
	switch(axis)
	{
	case ControllerAxis::LeftThumbstickX:
		value = float(m_State.Gamepad.sThumbLX) / float(SHRT_MAX);
		break;
	case ControllerAxis::LeftThumbstickY:
		value = float(m_State.Gamepad.sThumbLY) / float(SHRT_MAX);
		break;
	case ControllerAxis::RightThumbstickX:
		value = float(m_State.Gamepad.sThumbRX) / float(SHRT_MAX);
		break;
	case ControllerAxis::RightThumbstickY:
		value = float(m_State.Gamepad.sThumbRY) / float(SHRT_MAX);
		break;
	case ControllerAxis::LeftTrigger:
		value = float(m_State.Gamepad.bLeftTrigger) / 255.f;
		break;
	case ControllerAxis::RightTrigger:
		value = float(m_State.Gamepad.bRightTrigger) / 255.f;
		break;
	default: ;
	}

	value = (abs(value) < m_DeadZone) ? 0 : value;
	
	return value;
}

void InputComponent::RegisterCommand( ControllerButton button, Command *command )
{
	switch(button)
	{
	case ControllerButton::ButtonA:
		if (m_pButtonA)
			delete m_pButtonA;
		m_pButtonA = command;
		break;
	case ControllerButton::ButtonB:
		if (m_pButtonB)
			delete m_pButtonB;
		m_pButtonB = command;
		break;
	case ControllerButton::ButtonX:
		if (m_pButtonY)
			delete m_pButtonY;
		m_pButtonY = command;
		break;
	case ControllerButton::ButtonY:
		if (m_pButtonX)
			delete m_pButtonX;
		m_pButtonX = command;
		break;
	case ControllerButton::DPadLeft:
		if (m_pDPadLeft)
			delete m_pDPadLeft;
		m_pDPadLeft = command;
		break;
	case ControllerButton::DPadRight:
		if (m_pDPadRight)
			delete m_pDPadRight;
		m_pDPadRight = command;
		break;
	case ControllerButton::DPadUp:
		if (m_pDPadUp)
			delete m_pDPadUp;
		m_pDPadUp = command;
		break;
	case ControllerButton::DPadDown:
		if (m_pDPadDown)
			delete m_pDPadDown;
		m_pDPadDown = command;
		break;
	case ControllerButton::Start:
		if (m_pStart)
			delete m_pStart;
		m_pStart = command;
		break;
	case ControllerButton::Back:
		if (m_pBack)
			delete m_pBack;
		m_pBack = command;
		break;
	case ControllerButton::LeftThumbstick:
		if (m_pLeftThumbstick)
			delete m_pLeftThumbstick;
		m_pLeftThumbstick = command;
		break;
	case ControllerButton::RightThumbstick:
		if (m_pRightThumbstick)
			delete m_pRightThumbstick;
		m_pRightThumbstick = command;
		break;
	case ControllerButton::LeftShoulder:
		if (m_pLeftShoulder)
			delete m_pLeftShoulder;
		m_pLeftShoulder = command;
		break;
	case ControllerButton::RightShoulder:
		if (m_pRightShoulder)
			delete m_pRightShoulder;
		m_pRightShoulder = command;
		break;
	default: ;
	}
}

InputComponent::~InputComponent()
{
	delete m_pButtonA;
	delete m_pButtonB;
	delete m_pButtonY;
	delete m_pButtonX;
	delete m_pDPadLeft;
	delete m_pDPadRight;
	delete m_pDPadUp;
	delete m_pDPadDown;
	delete m_pStart;
	delete m_pBack;
	delete m_pLeftThumbstick;
	delete m_pRightThumbstick;
	delete m_pLeftShoulder;
	delete m_pRightShoulder;
}

BaseComponent * InputComponent::Clone() const
{
	InputComponent* ic{ new InputComponent{} };

	ic->SetControllerNumber(m_Controller);
	ic->m_pButtonA = m_pButtonA;
	ic->m_pButtonB = m_pButtonB;
	ic->m_pButtonY = m_pButtonY;
	ic->m_pButtonX = m_pButtonX;
	ic->m_pDPadLeft = m_pDPadLeft;
	ic->m_pDPadRight = m_pDPadRight;
	ic->m_pDPadUp = m_pDPadUp;
	ic->m_pDPadDown = m_pDPadDown;
	ic->m_pStart = m_pStart;
	ic->m_pBack = m_pBack;
	ic->m_pLeftThumbstick = m_pLeftThumbstick;
	ic->m_pRightThumbstick = m_pRightThumbstick;
	ic->m_pLeftShoulder = m_pLeftShoulder;
	ic->m_pRightShoulder = m_pRightShoulder;

	return ic;
}

void InputComponent::LoadFromJson( const nlohmann::json &json )
{
	m_Controller = json.at("Controller").get<int>();
}
