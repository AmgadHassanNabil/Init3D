#pragma once
#include "Headers.h"



class Input
{
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	DIMOUSESTATE mouseLastState;
	BYTE* lastKeyboardState;
	LPDIRECTINPUT8 DirectInput;

	Input();
	~Input();

	static Input* instance;

public:

	bool initDirectInput(HWND, HINSTANCE);
	void detectInput(DIMOUSESTATE& mouseCurrState, BYTE currKeyboardState[]);
	void setLastStates(DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[]);
	void release();


	static Input* getInstance();
	IDirectInputDevice8* getKeyboard();
	IDirectInputDevice8* getMouse();
	void getLastMouseState(DIMOUSESTATE& mouseLastState);
	void getLastKeyboardState(BYTE lastKeyboardState[]);
};

