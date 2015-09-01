#include <windows.h>
#include <d3d11.h>
IDXGISwapChain* swapChain; //buffer to stop screen flickering (multilayered visuals)
ID3D11Device* d3d11Device; //sofrware controller to the graphics device
ID3D11DeviceContext* d3d11DevCon; //renders the screen in the background
ID3D11RenderTargetView* renderTargetView; //render target for back buffer
//temporary  /**/**/**/**/**/
/**/ float red = 0.0f;   /**/
/**/ float green = 0.0f; /**/
/**/ float blue = 0.0f;  /**/
/**/ int colormodr = 1;  /**/
/**/ int colormodg = 1;  /**/
/**/ int colormodb = 1;  /**/
//end temporary /**/**/**/**/
LPCTSTR wndClassName = "perception"; //class name
LPCSTR wndTitle = "Perception - 0.0.0.1"; //window title
bool windowed = true; //windowed
HWND hwnd = NULL; //window instance

const int width = 1024; //window width
const int height = 768; //window height

bool initializeWindow(HINSTANCE hInstance, int showWnd, int width, int height, bool windowed); //sets up the window
bool initializeDirect3d11App(HINSTANCE hInstance); //initialize DirectX
void releaseObjects(); //releases objects to stop memory leaking
bool initScene(); //initializes the scene
void updateScene(); //updates scene per frame
void drawScene(); //draws the updated frame
int messageLoop(); //keeps the program running and always doing something 
LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); //our windows processing function

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	//initializes the window, and if it fails, quits and sends a message.
	if (!initializeWindow(hInstance, nShowCmd, width, height, true))
	{
		MessageBox(0, "Window Initialization - Failed",
				   "Error", MB_OK);
		return 0;
	}

	//initializes DirectX
	if (!initializeDirect3d11App(hInstance))
	{
		MessageBox(0, "Direct3D Initialization - Failed",
				   "Error", MB_OK);
		return 0;
	}

	//If all else succeeds, Initializes the scene.
	if (!initScene())
	{
		MessageBox(0, "Scene Initialization - Failed",
				   "Error", MB_OK);
		return 0;
	}
	//puts the program into a loop
	messageLoop();
	//releases unused objects
	releaseObjects();

	return 0;
}

bool initializeWindow(HINSTANCE hInstance, //application handle
					  int ShowWnd, //how the window should be dispayed
					  int width, //window width
					  int height, //window height
					  bool windowed) //windowed
{
	/*typedef struct _WNDCLASS
	{
	UINT cbSize;
	UINT style;
	WNDPROC lpfnWndProc;
	int cbClsExtra;
	int cbWndExtra;
	HANDLE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	LPCTSTR lpszMenuName;
	LPCTSTR lpszClassName;
	} WNDCLASS;*/

	WNDCLASSEX wc; //extended styles windows class

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = //the styles (flags) of the window
		CS_HREDRAW | //double mouse clicks, window is refreshed if the horizontal dimensions are changed or moved
		CS_VREDRAW;  //window is refreshed if the vertical dimensions are changed or moved
	wc.lpfnWndProc = wndProc; //pointer to the windows message process
	wc.cbClsExtra = NULL; //number of extra bytes allocated
	wc.cbWndExtra = NULL; //number of bytes allocated after the windows process
	wc.hInstance = hInstance; //handle to our windows instance
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //icon for the top left of the window
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //cursor for the inside of the window
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2); //background color
	wc.lpszMenuName = NULL; //name for the menu attached to our window (we dont have one)
	wc.lpszClassName = wndClassName; //naming of the class
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //icon in the taskbar

	if (!RegisterClassEx(&wc)) //registering of the class, if it fails:
	{
		MessageBox(NULL, "Error registering class", //we return a message
				   "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	hwnd = CreateWindowEx( //creating our window
						  NULL, //extended styles
						  wndClassName, //the class we registered earlier
						  wndTitle, //the window title
						  WS_OVERLAPPEDWINDOW, //CreteWindow functions styles, 
						  CW_USEDEFAULT, CW_USEDEFAULT, //starting positions of the window
						  width, height, //size of the window
						  NULL, //handle to the parent window
						  NULL, //handle to the menu attached to the window
						  hInstance, //intance of the program
						  NULL //used if the program was a MDI program
						  );

	if (!hwnd) //check to see if the window was created
	{
		MessageBox(NULL, "Error creating window",
				   "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, ShowWnd); //windows function, shows the window (window handler, how we show it);
	UpdateWindow(hwnd); //refreshes the window

	return true;
}

bool initializeDirect3d11App(HINSTANCE hInstance) //initializes directx
{
	DXGI_MODE_DESC bufferDesc; //defines general purpose of our back buffer
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	DXGI_SWAP_CHAIN_DESC swapChainDesc; //defines general purpose of our swapChain buffer
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	bufferDesc.Width = width; //window width
	bufferDesc.Height = height; //window height
	bufferDesc.RefreshRate.Numerator = 60; //refresh rate
	bufferDesc.RefreshRate.Denominator = 1; //lowest fps
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //???
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //???
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //???
	swapChainDesc.BufferDesc = bufferDesc; //sets the buffer's back buffer to bufferDesc
	swapChainDesc.SampleDesc.Count = 1; //number of samples, antialising (LATER!)
	swapChainDesc.SampleDesc.Quality = 0; //^^^
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //specifies the back buffer to be our target
	swapChainDesc.BufferCount = 1; //number of back buffers
	swapChainDesc.OutputWindow = hwnd; //window handler
	swapChainDesc.Windowed = windowed; //windowed
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //how to swap front and back buffers
	swapChainDesc.Flags = 0; //extra scd flags

	D3D11CreateDeviceAndSwapChain(NULL, //pAdapter: specifies display adapter, NULL= default 
								  D3D_DRIVER_TYPE_HARDWARE, //specifies to target the GPU
								  NULL, //software rasterizer
								  NULL, //flags for development, usually NULL for release builds
								  NULL, //pFeatureLevels
								  NULL, //UINT FeatureLevels
								  D3D11_SDK_VERSION, //version of DIRECTX
								  &swapChainDesc, //swapchaindesc
								  &swapChain, //swapchain
								  &d3d11Device, //d3d11device
								  NULL,//pFeatureLevels
								  &d3d11DevCon); //renders the screen in the background

	ID3D11Texture2D* BackBuffer; //2d texture for our back buffer to render to
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer); //give back buffer to swap chain

	d3d11Device->CreateRenderTargetView(BackBuffer, //creates the render target view, 1stparam: which resource will be used as a render target
										NULL, //pointer to a D3D10_RENDER_TARGET_VIEW_DESC, since it was created with a typed format, can be null
										&renderTargetView); //returns a pointer to the created render target view object
	BackBuffer->Release(); //releases the backbuffer texture

	d3d11DevCon->OMSetRenderTargets(//makes the resouces the buffers's targets.
									1, //number of render targets
									&renderTargetView, //pointer to the render target
									NULL); //pointer to depth/stencil view

	return true;
}

void releaseObjects() //releases all objects
{
	swapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
}

bool initScene() //insert pre-init scene code here
{

	return true;
}

void updateScene() //the update code 
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
}

void drawScene() //draw scene code 
{
	float bgColor[4] = { red, green, blue, 1.0f }; //RGB color set
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor); //clears the back buffer to the new color selected

	swapChain->Present(0, 0); //presents the back buffer
}

int messageLoop() //keeps the program going
{
	MSG msg; //creates a MSG instance
	ZeroMemory(&msg, sizeof(MSG)); //sets msg to null       ZeroMemory(pointer of, size of);
	while (true)
	{
		/*BOOL PeekMessageL(
		LPMSG lpMsg,
		HWND hWnd,
		UINT wMsgFilterMin,
		UINT wMsgFilterMax,
		UINT wRemoveMsg
		);*/

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //if the message was a windows message
		{
			if (msg.message == WM_QUIT)break; //quits if the it was a break message
			TranslateMessage(&msg); //translates the message if its not to quit
			DispatchMessage(&msg); //dispatches the message
		}
		else //if it was not a windows message, update and draw the scene, put game code here
		{
			updateScene();
			drawScene();

		}
	}
	return msg.wParam; //for detecting keboard imput
}

LRESULT CALLBACK wndProc(HWND hwnd, //window handle
						 UINT msg, //message content
						 WPARAM wParam, //keyboard input
						 LPARAM lParam) //extra info?
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) //if the button pressed was escape
		{
			DestroyWindow(hwnd); //close the window
		}
		return 0;

	case WM_DESTROY:  //if the window is closed
		PostQuitMessage(0); //no post quit message
		return 0; //end the program
	}
	return DefWindowProc(hwnd, msg, wParam, lParam); //takes care of the rest of the unhandled messages
}
