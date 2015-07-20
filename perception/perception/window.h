#pragma once
#include <Windows.h>
class Window
{
public:
	Window();
	~Window();
	//LRESULT CALLBACK WndProc(HWND* hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool InitializeWindow(HINSTANCE* hInstance, HWND *hwnd, LPCTSTR *WndClassName, int showWnd, int width, int height, bool windowed);
	int messageLoop();
};