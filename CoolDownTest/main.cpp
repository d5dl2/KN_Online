#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <ctime>
#include <cmath>
#include <tchar.h>
#include <vector>

#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

#define APP_CLASS_NAME _T("Direct3D Sample")
#define APP_WINDOW_NAME _T("Direct3D Sample 19: Cooldown Icon")

#define SAFE_RELEASE(p) \
	if( (p) != NULL ) \
	{ \
		(p)->Release(); \
		(p) = NULL; \
	}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	WNDCLASSEX wc = { sizeof( WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		hInst, NULL, NULL, NULL, NULL, APP_CLASS_NAME, NULL };
	RegisterClassEx( &wc );

	int nWidth = 800;
	int nHeight = 600;

	RECT rc = { 0, 0, nWidth, nHeight };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, false );

	HWND hWnd = CreateWindow( APP_CLASS_NAME, APP_WINDOW_NAME, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right-rc.left, rc.bottom-rc.top,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	LPDIRECT3D9 pd3d = NULL;
	pd3d = Direct3DCreate9( D3D_SDK_VERSION );

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	pd3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);

	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	LPDIRECT3DTEXTURE9 pTexIcon = NULL;
	D3DXCreateTextureFromFile( pd3dDevice, _T("Ability_Ambush.png"), &pTexIcon );

	LPDIRECT3DTEXTURE9 pTexCooldown = NULL;
	D3DXCreateTextureFromFile( pd3dDevice, _T("cooldown2.png"), &pTexCooldown );

	struct D3DVERTEX
	{
		float fX, fY, fZ;
		float fu, fv;
	};

	struct D3DVERTEX2
	{
		float fX, fY;
	};

	D3DVERTEX vertices[] = {
		{  0,  0, 0.0f, 0, 0 },
		{  1,  0, 0.0f, 1, 0 },
		{  0, -1, 0.0f, 0, 1 },
		{  1, -1, 0.0f, 1, 1 },
	};

	D3DXMATRIX matTrans, matScale, matOri;
	D3DXMATRIX matWorld;

	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vEyePt   ( 0.0f, 0.0f, -3.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec   ( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIX matProj;
	D3DXMatrixOrthoOffCenterLH( &matProj, 0, nWidth, -nHeight, 0, 1, 100 );
	pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	float fRateWidth = nHeight / (float)nWidth;


	D3DVERTEX2 v_coordinates[1000];

	clock_t t = clock();
	int nCount = 9;
	MSG msg;
	float cd = 6400;
	float cx, cy;


	D3DVERTEX2 d_index0 = { 0.5, -0.5 };
	v_coordinates[0] = d_index0;

	for (int i = 1; i < 1000; i++) {
		
		float percent = (float)i / 10;
	
		if (percent <= 87.5 && percent > 62.5) {
			cx = 1;
			cy = 0 - (float)((25 - (percent - 62.5)) / 25);
		}
		if (percent <= 62.5 && percent > 37.5) {
			cx = 1 - (float)((25 - (percent - 37.5)) / 25);
			cy = -1;
		}
		if (percent <= 37.5 && percent > 12.5) {
			cx = 0;
			cy = -1 + (float)((25 - (percent - 12.5)) / 25);
		}
		if (percent <= 12.5) {
			cx = 0 + (float)((12.5 - percent) / 25);
			cy = 0;
		}
		if (percent > 87.5) {
			cx = 0.5 + (float)((12.5 - (percent - 87.5)) / 25);
			cy = 0;
		}


		D3DVERTEX2 d_nextIndex2 = { cx, cy };
		v_coordinates[i] = d_nextIndex2;
	
	}

	D3DVERTEX2 d1[11] = {
	{  0.5,  -0.5},
	{  0.5,     0},
	{  0,     0},
	{  0,     -0.5},
	{    0,    -1},
	{    0.5,    -1},
	{    1,     -1},
	{  1,     -0.5},
	{  1,     0},
	{  0.5,     0},
	{  0.5,     0},
	};

	for (int i = 0; i < 1000; i++)
	{
		v_coordinates[i].fX *= 5;
		v_coordinates[i].fY *= 5;
	}


	for(;;)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
				break;

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(45,50,170), 1.0f, 0 );

		D3DXMatrixScaling( &matScale, 64, 64, 0 );
		D3DXMatrixTranslation( &matTrans, nWidth / 2 - 32, -(nHeight / 2 - 32), 0 );


		D3DXMatrixIdentity( &matWorld );

		D3DXMatrixTranslation( &matOri, -0.5f, 0.5f, 0 );
		D3DXMatrixMultiply( &matWorld, &matWorld, &matOri );
		D3DXMatrixTranslation( &matOri, 0.5f, -0.5f, 0 );
		D3DXMatrixMultiply( &matWorld, &matWorld, &matOri );

		D3DXMatrixMultiply( &matWorld, &matWorld, &matScale );
		D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

		if( SUCCEEDED( pd3dDevice->BeginScene() ) )
		{
			//pd3dDevice->SetTexture( 0, pTexIcon );
		   
			//pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DVERTEX) );
			
			if( clock() - t > cd )
			{
				t = clock();
			}


			float percent = ((float)(cd - (clock() - t)) / cd) * 100;

			int i_percent = percent * 10;
			if (i_percent > 998) i_percent = 998;			

			//i_percent /= 100;

			/*pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
			pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
			pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
			pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );*/
			
			//pd3dDevice->SetTexture( 0, pTexCooldown );
			pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
			pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, i_percent, v_coordinates, sizeof(D3DVERTEX2));
			//pd3dDevice->SetTexture(0, NULL);
			//pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, nCount, vCooldown, sizeof(D3DVERTEX2) );
		

			//pd3dDevice->SetTexture( 1, NULL );
		

			pd3dDevice->EndScene();
		}

		pd3dDevice->Present( NULL, NULL, NULL, NULL );
	}

	SAFE_RELEASE( pTexCooldown );
	SAFE_RELEASE( pTexIcon );

	SAFE_RELEASE( pd3dDevice );
	SAFE_RELEASE( pd3d );

	UnregisterClass( APP_CLASS_NAME, wc.hInstance );

	return S_OK;
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_KEYUP:
		if( wParam == VK_ESCAPE )
			PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}