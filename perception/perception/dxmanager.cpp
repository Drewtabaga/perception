#pragma once
#include "dxmanager.h"
#include "window.h"
#include <d3d10.h>
//temp
float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;
int colormodr = 1;
int colormodg = 1;
int colormodb = 1;
//end temp
//ALL Direct3D Initialization

bool DXManager::InitializeDirect3dApp(HINSTANCE* hInstance, HWND* hwnd, ID3D10Device* d3dDevice, IDXGISwapChain* swapChain, ID3D10RenderTargetView* renderTargetView, int* width, int* height, bool* windowed)
{
	//definition of swap chain
	/*typedef struct DXGI_SWAP_CHAIN_DESC
	{
		DXGI_MODE_DESC BufferDesc;
		DXGI_SAMPLE_DESC SampleDesc;
		DXGI_USAGE BufferUsage;
		UINT BufferCount;
		HWND OutputWindow;
		BOOL Windowed;
		DXGI_SWAP_EFFECT SwapEffect;
		UINT Flags;
	}DXGI_SWAP_CHAIN_DESK;*/
	DXGI_SWAP_CHAIN_DESC scd;
	//defines general properties of our back buffer
	/*typedef struct DXGI_MODE_DESC
	{
		UINT Width;
		UINT Height;
		DXGI_RATIONAL RefreshRate;
		DXGI_FORMAT Format;
		DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
		DXGI_MODE_SCALING Scaling;
	}DXGI_MODE_DESC, *LPDXGI_MODE_DESC;*/
	scd.BufferDesc.Width = *width; //width of window
	scd.BufferDesc.Height = *height; //height of window
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	//defines number of antialiasing samples and quality
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	//specifies that the back buffer will be the target to render to
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	
	//number of back buffers to use in the swap chain (1=double buffering, 2=triple buffering)
	scd.BufferCount = 1;

	//the handle to the window
	scd.OutputWindow = *hwnd;

	//windowed?
	scd.Windowed = *windowed;

	//how we want to swap our front and back buffers
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //DXGI_S_E_D will let the GPU Driver the most efficient presentation technique for the swap chain
	scd.Flags = 0; //extra flags (we have none)

	//creation of swap chain
	/*HRESULT D3D10CreateDeviceAndSwapChain(
		__in IDXGIAdapter *pAdapter, //specifies the display adapter (0 or MULL = primary display adapter)
		__in D3D10_DRIVER_TYPE DriverType, //D3D10_DRIVER_TYPE_HARDWARE (graphics card) or D3D10_DRIVER_TYPE_REFERENCE (software)(slow)
		__in HMODULE Software, //using software rasterizing? we don't
		__in UINT Flags, //optional device creation flags, usually 0 for release builds
		__in UINT SDKVersion, //always set to D3D10_SDK_VERSION
		__in DXGI_SWAP_CHAIN_DESC *pSwapChainDesc, //a pointer to DXGI_SWAP_CHAIN_DESC which describes the swap chain we want to make
		__out IDXGISwapChain **ppSwapChain, //returns the created swap chain
		__out ID3D10Device **ppDevice //returns the created device
	);*/
	D3D10CreateDeviceAndSwapChain(0, //*pAdapter
								D3D10_DRIVER_TYPE_HARDWARE, //DriverType
								0, //Software Rasterizing
								0, //Flags
								D3D10_SDK_VERSION, //SDKVersion
								&scd, //*pSwapChainDesc
								&swapChain, //**ppSwapChain
								&d3dDevice); //**ppDevice
	ID3D10Texture2D* backBuffer; 	//2dtexture for the back buffer to render onto (COM object)
	swapChain->GetBuffer(0, _uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer); //give backbuffer to the swap chain. 

	//create the render target view by calling CreateRenderTargetView.
	d3dDevice->CreateRenderTargetView(backBuffer, //render target
									0, //pointer to a D3D10_RENDER_TARGET_VIEW_DESC (if created with a tyed format, then it can be NULL.
									&renderTargetView); //returs a pointer to the created render target view object
	backBuffer->Release(); //backBuffer is a COM object, therefore we release it at the end of the code.
	//sends the device to the merger stage of the pipeline to make resources the render target and depth/stencil buffers.
	d3dDevice->OMSetRenderTargets(1, //number of render targets to send to the pipeline
								&renderTargetView, //pointer to the first element in an array of render target view pointers to bind to the pipeline
								NULL); //pointer to the depth/stencil view to bind to the pipeline (we dont have one)
	/*Viewport Creation. (Causing an error-please examine.)
	//typedef struct D3D10_VIEWPORT
	//{
	//	INT TopLeftX; //define how big and where our vbiewport is drawn onto the backbuffer.
	//	INT TopLeftY; //...
	//	UINT Width; //...
	//	UINT Height; //...
	//	FLOAT MinDepth; //minimum depth value in the buffer
	//	FLOAT MaxDepth; //maximum depth value in the buffer
	//}D3D10_VIEWPORT;*/
	D3D10_VIEWPORT vp;
	vp.TopLeftY = 0;
	vp.Width = *width;
	vp.Height = *height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	//setting the viewport with direct3d using the RSSetViewports (ID3D10Device)
	d3dDevice->RSSetViewports(1, //number of viewports
							&vp); //pointer to the viewport
	const FLOAT bgColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f }; //creates a black color to use with the back buffer
	d3dDevice -> ClearRenderTargetView(renderTargetView, bgColor); //clears the back buffer to the color

	swapChain->Present(0, 0); //presents the back buffer

	return true; //d3d10 has initialized
}

bool DXManager::InitScene()
{
	return true; //where we prepare the necessary things before we draw our scene
}
void DXManager::DrawScene(ID3D10Device* d3dDevice, IDXGISwapChain* swapChain, ID3D10RenderTargetView* renderTargetView)
{
}