#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "resource.h"

HWND hwnd = NULL;

void MyDraw(HDC hdc)
{
	HBRUSH brush, oldbrush;

	// create brush
	int n = (GetTickCount() / 250) % 4;
	switch (n) {
	case 0:
		brush = CreateSolidBrush(RGB(255, 0, 0));
		break;
	case 1:
		brush = CreateSolidBrush(RGB(0, 255, 0));
		break;
	case 2:
		brush = CreateSolidBrush(RGB(0, 0, 255));
		break;
	default:
		brush = CreateSolidBrush(RGB(0, 0, 0));
		break;
	}
	oldbrush = (HBRUSH)SelectObject(hdc, brush);

	// draw 1px
	RECT r;
	r.left = 100;
	r.right = 101;
	r.top = 100;
	r.bottom = 101;
	FillRect(hdc, &r, brush);
	
	// delete pen & brush
	SelectObject(hdc, oldbrush);
	DeleteObject(brush); 
		// CreateSolidBrush()でブラシを作成した場合はDeleteObject()が必要。
		// GetStockObject()を使用して既存のブラシを取得した場合は、削除しなくてよいので注意。
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

	return (int)msg.wParam;
}