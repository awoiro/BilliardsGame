#include <stdio.h>
#include <windows.h>
#include "Application.h"
#include "Audio.h"

#pragma comment(lib,"winmm.lib")

// �x�����\��
#pragma warning(disable : 4305)
#pragma warning(disable : 4996)
#pragma warning(disable : 4018)
#pragma warning(disable : 4101)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

HWND g_hWnd;
Application* g_pApp;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL SetClientSize(HWND hWnd, int width, int height);

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	static LPCSTR appName = "AwoEngine_�r�����[�h";
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = appName;
	wndclass.hIconSm = LoadIcon(nullptr, IDI_ASTERISK);
	RegisterClassEx(&wndclass);

	g_hWnd = CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInst, nullptr);
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	SIZE windowSize;
	windowSize.cx = WINDOW_WIDTH;
	windowSize.cy = WINDOW_HEIGHT;

	// app init
	g_pApp = new Application;
	g_pApp->Init(g_hWnd, windowSize);
	
	//
	SetClientSize(g_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);

	// message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// update
			g_pApp->Update(g_hWnd, windowSize);
			g_pApp->RenderSetUp(g_hWnd, windowSize);
			g_pApp->InputKeyUpdate();
		}
	}
	// delete
	delete g_pApp;
	return (INT)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if (TwEventWin(hWnd, iMsg, wParam, lParam)) { return 0; }

	switch (iMsg)
	{
	case WM_SIZE:
		if (g_pApp)
		{
			g_pApp->ResizeWindow(lParam);
		}
		return 0;
	case WM_CHAR:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

BOOL SetClientSize(HWND hWnd, int width, int height)
{
	RECT rw, rc;
	::GetWindowRect(hWnd, &rw);
	::GetClientRect(hWnd, &rc);

	int new_width = (rw.right - rw.left) - (rc.right - rc.left) + width;
	int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

	return ::SetWindowPos(hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
}