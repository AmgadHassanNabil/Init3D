#include "WinMain.h"

LPCTSTR WndClassName = "firstwindow";
HWND hwnd = NULL;

const int Width = 800;
const int Height = 600;


double countsPerSecond = 0.0;
__int64 CounterStart = 0;

int frameCount = 0;
int fps = 0;

__int64 frameTimeOld = 0;
double frameTime;

void StartTimer();
double GetTime();
double GetFrameTime();

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0, "Window Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	if (!Direct3D::getInstance()->initialize(hwnd, hInstance, Width, Height))
	{
		MessageBox(0, "Direct3D Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	if (!Game::getInstance()->initialize(Width, Height))
	{
		MessageBox(0, "Game Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	if (!Input::getInstance()->initDirectInput(hwnd, hInstance))
	{
		MessageBox(0, "Direct Input Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	messageloop();

	Game::getInstance()->release();
	Direct3D::getInstance()->shutdown();
	Input::getInstance()->release();

	return 0;
}

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd,	int width, int height,	bool windowed)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;    
	wc.cbWndExtra = NULL;    
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WndClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	hwnd = CreateWindowEx(
		NULL,
		WndClassName,
		"Window Title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd)
	{
		MessageBox(NULL, "Error creating window", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);

	return true;
}

int messageloop() 
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	DIMOUSESTATE mouseCurrState;
	

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			BYTE currKeyboardState[256];
			ZeroMemory(currKeyboardState, sizeof(currKeyboardState));
			Input::getInstance()->detectInput(mouseCurrState, currKeyboardState);

			frameCount++;
			if (GetTime() > 1.0f)
			{
				fps = frameCount;
				frameCount = 0;
				StartTimer();
			}

			frameTime = GetFrameTime();

			Game::getInstance()->update(frameTime, mouseCurrState, currKeyboardState);
			Game::getInstance()->draw(fps);
			Input::getInstance()->setLastStates(mouseCurrState, currKeyboardState);
		}
	}

	return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) 
			{
				if (MessageBox(0, "Are you sure you want to exit?",	"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)

					DestroyWindow(hwnd);
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, msg,	wParam,	lParam);
}
inline void StartTimer()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	countsPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	CounterStart = frequencyCount.QuadPart;
}

inline double GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart - CounterStart) / countsPerSecond;
}

inline double GetFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - frameTimeOld;
	frameTimeOld = currentTime.QuadPart;

	if (tickCount < 0.0f)
		tickCount = 0.0f;

	return float(tickCount) / countsPerSecond;
}