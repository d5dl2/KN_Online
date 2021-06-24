// Launcher.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Launcher.h"
#include <CommCtrl.h>
#include "LauncherCore.h"

#define MAX_LOADSTRING 100

#define WIDTH 300
#define HEIGHT 300
const int ScreenX = (GetSystemMetrics(SM_CXSCREEN) - WIDTH) / 2;
const int ScreenY = (GetSystemMetrics(SM_CYSCREEN) - HEIGHT) / 2;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL InitControls(HWND hwndParent);

HWND hWnd;
HWND hwndPROGRESSBAR;
HWND hwndCONNECTIONLABEL;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAUNCHER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	LauncherCore eng;
	eng.Init(hWnd);
	
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAUNCHER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAUNCHER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAUNCHER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
		ScreenX, ScreenY, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	InitControls(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

BOOL InitControls(HWND hwndParent)
{
	RECT rcClient;
	GetClientRect(hwndParent, &rcClient);

	hwndCONNECTIONLABEL = CreateWindowEx(0,
		WC_STATIC,
		(LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE,
		rcClient.left + 10,
		rcClient.top + 10,
		rcClient.right / 10 * 8,
		rcClient.bottom / 10 * 8,
		hwndParent,
		(HMENU)0,
		hInst,
		NULL);


	/*  SCROLL BAR

	////////////////////////////////////SCROLL BAR

	//int cyVScroll;  // Height of scroll bar arrow.
	//
	//cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

	//hwndPROGRESSBAR = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
	//    WS_CHILD | WS_VISIBLE, rcClient.left + 10,
	//    rcClient.bottom - cyVScroll - 10,
	//    rcClient.right / 10 * 8, cyVScroll,
	//    hwndParent, (HMENU)0, hInst, NULL);

	//// Set the range and increment of the progress bar.
	//SendMessage(hwndPROGRESSBAR, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	//SendMessage(hwndPROGRESSBAR, PBM_SETSTEP, (WPARAM)1, 0);
	//for(int i = 0; i < 20; i++)
	//    SendMessage(hwndPROGRESSBAR, PBM_STEPIT, 0, 0);

	*/

	return true;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			/*case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;*/
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)lParam == hwndCONNECTIONLABEL)
		{
			HDC hdcStatic = (HDC)wParam;
			SetBkColor(hdcStatic, OK);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}