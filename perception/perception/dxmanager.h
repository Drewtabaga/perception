#pragma once
#include <d3d10.h>
class DXManager
{
public:
	bool InitializeDirect3dApp(HINSTANCE* hInstance, HWND* hwnd, ID3D10Device* d3dDevice, IDXGISwapChain* swapChain, ID3D10RenderTargetView* renderTargetView, int* width, int* height, bool* windowed);
	void DrawScene(ID3D10Device* d3dDevice, IDXGISwapChain* swapChain, ID3D10RenderTargetView* renderTargetView);
	bool InitScene();
};