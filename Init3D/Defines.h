#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#define SHOW_AND_RETURN_ERROR_ON_FAIL(hr, errMsg, errTitle)		if (FAILED(hr))											\
																{														\
																	MessageBox(0, errMsg, errTitle, MB_OK);				\
																	return false;										\
																}