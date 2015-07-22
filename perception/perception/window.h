#pragma once
#include <Windows.h>
class Window
{
public:
	bool InitializeWindow(HINSTANCE* hInstance, HWND* hwnd, LPCTSTR WndClassName, LPCSTR WndTitle, int showWnd, int* width, int* height, bool* windowed);
	int messageLoop();
};