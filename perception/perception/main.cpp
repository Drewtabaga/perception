#pragma once
#include "window.h"
#include "dxmanager.h"
#include <Windows.h>
#include <d3d10.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//creates a pointer for the window
	Window* window = new Window();
	//creates a pointer for DirectX
	DXManager* DXM = new DXManager();
	ID3D10Device* d3dDevice = NULL; //sofrware controller to the graphics device
	IDXGISwapChain* swapChain = NULL; //buffer to stop screen flickering (multilayered visuals)
	ID3D10RenderTargetView* renderTargetView = NULL; //render target for back buffer
	int width = 1024; //window width
	int height = 768; //window height
	bool windowed = true; //windowed (T/F)
	HWND hwnd = NULL; //window instance
	LPCTSTR wndClassName = "Perception"; //class name
	LPCSTR wndTitle = "Perception - Pre-game"; //window title



	//initializes the window, and if it fails, quits and sends a message.
	if (!window->InitializeWindow(&hInstance, &hwnd, wndClassName, wndTitle, nShowCmd, &width, &height, &windowed))
	{
		MessageBox(0, "Window Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	//Initializes Direct3D
	if (!DXM->InitializeDirect3dApp(&hInstance, &hwnd, d3dDevice, swapChain, renderTargetView, &width, &height, &windowed))
	{
		MessageBox(0, "Direct3D Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	//If all else succeeds, Initializes the scene.
	if (!DXM->InitScene())
	{
		MessageBox(0, "Scene Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	window->messageLoop(DXM,d3dDevice, swapChain, renderTargetView);
	return 0;
}