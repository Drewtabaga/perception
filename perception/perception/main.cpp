#include <Windows.h>
#include "window.h"
#include <d3d10.h>
#include "dxmanager.h"
ID3D10Device* d3dDevice; //sofrware controller to the graphics device
IDXGISwapChain* swapChain; //buffer to stop screen flickering (multilayered visuals)
ID3D10RenderTargetView* renderTargetView; //render target for back buffer
HWND hwnd = NULL; //window instance
LPCTSTR wndClassName = "Perception"; //class name
LPCSTR wndTitle = "Perception - Pre-game"; //window title
int width = 1024; //window width
int height = 768; //window height
bool windowed = true; //windowed (T/F)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//creates a pointer for the window
	Window* window = new Window();
	//creates a pointer for DirectX
	DXManager* dxm = new DXManager();
	//initializes the window, and if it fails, quits and sends a message.
	if (!window->InitializeWindow(&hInstance, &hwnd, wndClassName, wndTitle, nShowCmd, &width, &height, &windowed))
	{
		MessageBox(0, "Window Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	//Initializes Direct3D
	if (!dxm->InitializeDirect3dApp(&hInstance, &hwnd, d3dDevice, swapChain, renderTargetView, &width, &height, &windowed))
	{
		MessageBox(0, "Direct3D Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	//If all else succeeds, Initializes the scene.
	if (!dxm->InitScene())
	{
		MessageBox(0, "Scene Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	window->messageLoop();
	
	return 0;
}
/*
bool InitializeDirect3dApp(HINSTANCE hInstance)
{
	typedef struct DXGI_SWAP_CHAIN_DESC
	{
		DXGI_MODE_DESC   BufferDesc;
		DXGI_SAMPLE_DESC SampleDesc;
		DXGI_USAGE       BufferUsage;
		UINT             BufferCount;
		HWND             OutputWindow;
		BOOL             Windowed;
		DXGI_SWAP_EFFECT SwapEffect;
		UINT             Flags;
	} DXGI_SWAP_CHAIN_DESC;
	DXGI_SWAP_CHAIN_DESC scd;
	typedef struct DXGI_MODE_DESC
	{
		UINT                     Width;
		UINT                     Height;
		DXGI_RATIONAL            RefreshRate;
		DXGI_FORMAT              Format;
		DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
		DXGI_MODE_SCALING        Scaling;
	} DXGI_MODE_DESC, *LPDXGI_MODE_DESC;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd; 
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;
	HRESULT  D3D10CreateDeviceAndSwapChain(
		__in   IDXGIAdapter *pAdapter,
		__in   D3D10_DRIVER_TYPE DriverType,
		__in   HMODULE Software,
		__in   UINT Flags,
		__in   UINT SDKVersion,
		__in   DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
		__out  IDXGISwapChain **ppSwapChain,
		__out  ID3D10Device **ppDevice
		); 
	D3D10CreateDeviceAndSwapChain(0,
		D3D10_DRIVER_TYPE_HARDWARE,
		0,
		0,
		D3D10_SDK_VERSION,
		&scd,
		&swapChain,
		&d3dDevice);
	ID3D10Texture2D* backBuffer;
	swapChain->GetBuffer(0, _uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer);
	d3dDevice->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	backBuffer->Release();
	d3dDevice->OMSetRenderTargets(1, &renderTargetView, NULL);
	D3D10_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	d3dDevice->RSSetViewports(1, &vp);
	const FLOAT bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	d3dDevice->ClearRenderTargetView(renderTargetView, bgColor);

	swapChain->Present(0, 0);

	return true;
}
bool InitScene()
{

	return true;
}
void DrawScene()
{
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;


	const FLOAT bgColor[4] = { red, green, blue, 1.0f };

	d3dDevice->ClearRenderTargetView(renderTargetView, bgColor);

	swapChain->Present(0, 0);
}
*/