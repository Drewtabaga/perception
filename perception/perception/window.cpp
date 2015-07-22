#include "window.h"
#include "dxmanager.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//Window Initialization Function
bool Window::InitializeWindow(HINSTANCE* hInstance, HWND* hwnd, LPCTSTR WndClassName, LPCSTR WndTitle, int showWnd, int* width, int* height, bool* windowed)
{
	//extended styles of WNDCLASSESEX
	typedef struct _WNDCLASS
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
	} WNDCLASS;
	WNDCLASSEX wc;

	//initializes wc to be the same sizes as WNDCLASSEX
	wc.cbSize = sizeof(WNDCLASSEX);

	//enabled styles for our window
	wc.style = CS_HREDRAW | CS_VREDRAW;

	//pointer to windows messages function
	wc.lpfnWndProc = WndProc;

	//#extra bytes allocated after WNDCLASSEX
	wc.cbClsExtra = NULL;

	//#extra bytes allocated after the windows instance
	wc.cbWndExtra = NULL;

	//makes wc's instance handler the same as the one we are using
	wc.hInstance = *hInstance;

	//sets the icon at the top left corner of the window
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	//sets the cursor's icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	//sets background color (black)
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);

	//name to the menu attached to the window. (We don't have one)
	wc.lpszMenuName = NULL;

	//Name of Class
	wc.lpszClassName = WndClassName;

	//Taskbar Icon
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register's class, ends if failed (would not create window)
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//Creation of window
	*hwnd = CreateWindowEx(NULL, //WNDCLASSEX Styles to be applied to this specific window
		WndClassName, //name of registered class that we use
		WndTitle, //name of the window
		WS_OVERLAPPEDWINDOW, //window styles (seperate by '|')
		CW_USEDEFAULT, CW_USEDEFAULT, //Starting x&y positions
		*width, *height, //size of window
		NULL, //handle to parent window (we don't have a parent)
		NULL, //handle to menu attached to window (no menu)
		*hInstance, //instance of current program
		NULL
		); //if use of MDI client (we don't)
	//if the window fails to initialize
	if (!*hwnd)
	{
		MessageBox(NULL, "Error creating window", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}
	ShowWindow(*hwnd, showWnd);
	UpdateWindow(*hwnd);

	return true;
}

//Message Loop until program is closed
int Window::messageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&msg, //LPMSG lpMsg, pointer to message structure
			NULL, //HWND hWndParam, hande to the window that is sending the message (NULL=takes message from any window in the program
			0, //UINT wMsgFilterMin, max value of the message range to be examined (MIN & MAX = 0, all messages are examined)
			0, //wMsgFilterMax, max value of the message range to be examined (MIN & MAX = 0, all messages are examined)
			PM_REMOVE)) //UINT wRemoveMsg, how the message will be handled
		{
			//if the message was a quit message, close the program
			if (msg.message == WM_QUIT)
				break;
			//if the message was a windows message, translate and dispatch.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//if it was not a windows message, run the game
		else
		{
		}
	}

	return msg.wParam;
}

//message processing
LRESULT CALLBACK WndProc(HWND hwnd, //window handler
	UINT msg, //message handler
	WPARAM wParam, //extra message information(keyboard imput detection)
	LPARAM lParam) //extra message information(keyboard imput detection)
{
	switch (msg)
	{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				if (MessageBox(0, "Are you sure you want to quit?", "Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
					DestroyWindow(hwnd);
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}