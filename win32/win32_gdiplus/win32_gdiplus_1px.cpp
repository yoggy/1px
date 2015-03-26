#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <ole2.h>
#include <gdiplus.h>
#pragma	comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

#include "resource.h"

HWND hwnd = NULL;

void MyDraw(HDC hdc)
{
	Graphics g(hdc);
	Color c;

	// create brush
	int n = (GetTickCount() / 250) % 4;
	switch (n) {
	case 0:
		c = Color(255, 0, 0);
		break;
	case 1:
		c = Color(0, 255, 0);
		break;
	case 2:
		c = Color(0, 0, 255);
		break;
	default:
		c = Color(0, 0, 0);
		break;
	}

	Rect rect(100, 100, 1, 1);
	g.FillRectangle(&SolidBrush(c), rect);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MyDraw(hdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		hwnd = NULL;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE) {
			PostMessage(hwnd, WM_CLOSE, NULL, NULL);
		}
		break;
	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	// startup GDI+
	ULONG_PTR	gdiplus_token;
	GdiplusStartupInput gdiplus_startup_input;
	GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, NULL);

	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = "dummyclassname";
	RegisterClassEx(&wc);

	hwnd = CreateWindow(wc.lpszClassName, "win32_gdi_1px", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, NULL, NULL, NULL, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	SetTimer(hwnd, 0, 100, NULL);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// shutdown GDI+
	GdiplusShutdown(gdiplus_token);

	return (int)msg.wParam;
}