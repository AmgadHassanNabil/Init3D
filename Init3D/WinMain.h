#pragma once

#include <windows.h>

#include "Direct3D.h"
#include "Game.h"
#include "Input.h"

bool InitializeWindow(HINSTANCE hInstance,	int ShowWnd, int width, int height,	bool windowed);

int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);

int messageloop();

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);