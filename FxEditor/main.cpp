// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>
#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream
#include <format>

// define the screen resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer
LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;    // the pointer to the index buffer
D3DXVECTOR3 cameraPos;
D3DXVECTOR3 lookAtPos;
ID3DXFont* font;

float velocity = 0.05f;
long currentFPS = 0;
// function prototypes
void initD3D(HWND hWnd);
void render(void);
void tick(void);
void cleanD3D(void);
void init_graphics(void);

struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, "WindowClass", "Our Direct3D Program",
		WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	initD3D(hWnd);

	MSG msg;
	bool stop = false;
	DWORD dwStartTime = timeGetTime();
	int frameCount = 0;
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				stop = true;
		}

		if (stop)
			break;

		frameCount++;
		DWORD dwCurrentTime = timeGetTime();
		DWORD dwElapsedTime = dwCurrentTime - dwStartTime;
		if (dwElapsedTime >= 100) { // her saniyede bir hesapla
			currentFPS = frameCount * 10;
			frameCount = 0;
			dwStartTime = dwCurrentTime;
		}

		tick();
		render();
	}

	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	case WM_KEYDOWN: {
		if (wParam == VK_LEFT || wParam == 0x41)
		{
			cameraPos.x += velocity;
			lookAtPos.x += velocity;
		}
		if (wParam == VK_RIGHT || wParam == 0x44)
		{
			cameraPos.x -= velocity;
			lookAtPos.x -= velocity;
		}
		if (wParam == VK_UP || wParam == 0x57)
		{
			cameraPos.y += velocity;
			lookAtPos.y += velocity;
		}
		if (wParam == VK_DOWN || wParam == 0x53d)
		{
			cameraPos.y -= velocity;
			lookAtPos.y -= velocity;
		}
	} break;
	case WM_MOUSEWHEEL: {
		float fDelta = ((short)HIWORD(wParam)) * 0.05f;
		if (fDelta < 0)
		{
			cameraPos.x += velocity;
			cameraPos.y += velocity;
			cameraPos.z += velocity;
		}
		else
		{
			cameraPos.x -= velocity;
			cameraPos.y -= velocity;
			cameraPos.z -= velocity;
		}
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	init_graphics();

	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);    // turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // turn off culling
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
}

void tick(void)
{
	// set the world transform
	static float index = 0.0f; /*index += 0.003f*/; // an ever-increasing float value
	D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
	D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));    // set the world transform	
}


// this is the function used to render a single frame
void render(void)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();

	d3ddev->SetFVF(CUSTOMFVF);

	// set the view transform
	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&cameraPos,    // the camera position
		&lookAtPos,      // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

	// set the projection transform
	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(90),    // the horizontal field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		0.05f,   // the near view-plane
		100.0f);    // the far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); // set the projection

	// select the vertex buffer to display
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->SetIndices(i_buffer);

	// draw the X Line
	d3ddev->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 6, 0, 6);

	if (font)
	{
		std::ostringstream os;
		os << "CameraPos: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
		os << "LookAtPos: (" << lookAtPos.x << ", " << lookAtPos.y << ", " << lookAtPos.z << ")" << std::endl;
		os << "FPS: " << currentFPS;
		RECT rect;
		SetRect(&rect, 10, 10, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
		font->DrawTextA(NULL, os.str().c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(0, 0, 0));
	}
	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	if (font)
	{
		font->Release();
	}
	v_buffer->Release();
	i_buffer->Release();
	d3ddev->Release();
	d3d->Release();
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	struct CUSTOMVERTEX vertices[] =
	{
		{ 0.f, 0.f, 0.f, D3DCOLOR_XRGB(255, 0, 0) },
		{ 1.f, 0.f, 0.f, D3DCOLOR_XRGB(255, 0, 0) },
		{ 0.f, 0.f, 0.f, D3DCOLOR_XRGB(0, 255, 0) },
		{ 0.f, 1.f, 0.f, D3DCOLOR_XRGB(0, 255, 0) },
		{ 0.f, 0.f, 0.f, D3DCOLOR_XRGB(0, 0, 255) },
		{ 0.f, 0.f, 1.f, D3DCOLOR_XRGB(0, 0, 255) }
	};

	// create a vertex buffer interface called v_buffer
	d3ddev->CreateVertexBuffer(sizeof(vertices),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer

	// lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	// create the indices using an int array
	short indices[] =
	{
		0, 1,
		2, 3,
		4, 5
	};

	// create a index buffer interface called i_buffer
	d3ddev->CreateIndexBuffer(sizeof(indices),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer,
		NULL);

	// lock i_buffer and load the indices into it
	i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	i_buffer->Unlock();

	cameraPos = D3DXVECTOR3(1, 2, 3);
	lookAtPos = D3DXVECTOR3(0, 0, 0);

	font = NULL;
	HRESULT hr = D3DXCreateFont(d3ddev, 40, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Arial", &font);
}
