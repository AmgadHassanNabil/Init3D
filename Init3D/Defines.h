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

#define clamp(min, max, val) (val < min ? min : (val > max ? max : val))
#define wrap(min, max, val) (val < min ? max : (val > max ? min : val))

#define TRANSFORMATION_MATRIX(forward, up, right, translation)	XMMatrixSet(																		\
															XMVectorGetX(right),		XMVectorGetY(right),		XMVectorGetZ(right),		0,	\
															XMVectorGetX(up),			XMVectorGetY(up),			XMVectorGetZ(up),			0,	\
															XMVectorGetX(forward),		XMVectorGetY(forward),		XMVectorGetZ(forward),		0,	\
															XMVectorGetX(translation),	XMVectorGetY(translation),	XMVectorGetZ(translation),	1)
#define ABS(a)	(a < 0 ? -a : a)

#define VALUE_IF_IN_THREASHOLD(value, threshold, toBeValue)	(ABS(value) < threshold) ? toBeValue : value

