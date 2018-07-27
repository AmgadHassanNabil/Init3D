#pragma once
#include <dinput.h>
#include<string.h>
#include "Defines.h"

#define AMINPUT			Input::getInstance()
#define INPUT_DOWN(a)	a & 0x80
#define INPUT_UP(a)		a == 0

class Input
{
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	DIMOUSESTATE mouseLastState;
	BYTE lastKeyboardState[256];
	LPDIRECTINPUT8 DirectInput;

	Input();
	~Input();

	static Input* instance;

public:

	bool initDirectInput(HWND, HINSTANCE);
	void detectInput(DIMOUSESTATE& mouseCurrState, BYTE currKeyboardState[]);
	inline void setLastStates(DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[]) { this->mouseLastState = mouseCurrState; memcpy(this->lastKeyboardState, currKeyboardState, sizeof(BYTE) * 256); }

	void release();


	inline static Input* getInstance()
	{
		if (instance == nullptr)
			instance = new Input;
		return instance;
	}
	IDirectInputDevice8* getKeyboard() const;
	IDirectInputDevice8* getMouse() const;
	inline void getLastMouseState(DIMOUSESTATE& mouseLastState) { mouseLastState = this->mouseLastState; }
	inline void getLastKeyboardState(BYTE **lastKeyboardState) { (*lastKeyboardState) = this->lastKeyboardState; }
};

