#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <D3Dcompiler.h>
#include <dinput.h>

#include "Direct3D.h"
#include "Game.h"
#include "Vertices.h"
#include "WICTextureLoader.h"
#include "Input.h"
#include "FBXImporter.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment (lib, "dinput8.lib")

#define SHOW_AND_RETURN_ERROR_ON_FAIL(hr, errMsg, errTitle)		if (FAILED(hr))											\
																{														\
																	MessageBox(0, errMsg, errTitle, MB_OK);				\
																	return false;										\
																}

#define	AMD3D			Direct3D::getInstance()
#define AMINPUT			Input::getInstance()
#define INPUT_DOWN(a)	a & 0x80