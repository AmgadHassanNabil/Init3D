#include "Input.h"

Input* Input::instance = nullptr;

Input::Input()
{
}


Input::~Input()
{
}

Input * Input::getInstance()
{
	if (instance == nullptr)
		instance = new Input;
	return instance;
}

IDirectInputDevice8 * Input::getKeyboard()
{
	return this->DIKeyboard;
}

IDirectInputDevice8 * Input::getMouse()
{
	return this->DIMouse;
}

void Input::getLastMouseState(DIMOUSESTATE & mouseLastState)
{
	mouseLastState = this->mouseLastState;
}

void Input::getLastKeyboardState(BYTE lastKeyboardState[])
{
	lastKeyboardState = this->lastKeyboardState;
}

void Input::setLastStates(DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
	this->mouseLastState = mouseCurrState;
	this->lastKeyboardState = currKeyboardState;
}

bool Input::initDirectInput(HWND hwnd, HINSTANCE hInstance)
{
	HRESULT hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);

	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Direct Input 8 Creation - Failed", "Direct Input Error");

	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);
	
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Keyboard creation - Failed", "Direct Input Error");

	hr = DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Mouse creation - Failed", "Direct Input Error");

	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Keyboard Set Data - Failed", "Direct Input Error");
	hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Keyboard Set Cooperative Level - Failed", "Direct Input Error");

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Mouse Set Data - Failed", "Direct Input Error");
	hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Mouse Set Cooperative Level - Failed", "Direct Input Error");

	return true;
}

void Input::detectInput(DIMOUSESTATE& mouseCurrState, BYTE keyboardState[])
{
	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(BYTE) * 256, (LPVOID)keyboardState);
}

void Input::release()
{
	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	DirectInput->Release();
	delete this;
}