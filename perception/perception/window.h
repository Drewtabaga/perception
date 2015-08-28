#pragma once
#include "dxmanager.h"
#include <Windows.h>
#include <d3d10.h>
class Window
{
public:
	bool InitializeWindow(HINSTANCE* hInstance, HWND* hwnd, LPCTSTR WndClassName, LPCSTR WndTitle, int showWnd, int* width, int* height, bool* windowed);
	int messageLoop(DXManager* DXM,ID3D10Device* d3dDevice, IDXGISwapChain* swapChain, ID3D10RenderTargetView* renderTargetView);
};