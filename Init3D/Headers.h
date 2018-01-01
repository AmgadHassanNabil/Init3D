#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <D3Dcompiler.h>

#include "Direct3D.h"
#include "Game.h"
#include "Vertices.h"
#include "WICTextureLoader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

#define SHOW_AND_RETURN_ERROR_ON_FAIL(hr, errMsg, errTitle)		if (FAILED(hr))											\
																{														\
																	MessageBox(0, errMsg, errTitle, MB_OK);				\
																	return false;										\
																}

#define	D3DINST	Direct3D::getInstance()