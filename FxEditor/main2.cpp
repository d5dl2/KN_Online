//// include the basic windows header files and the Direct3D header file
//#include <windows.h>
//#include <windowsx.h>
//#include <d3d9.h>
//#include <d3dx9.h>
//#include "../Client/N3Base/N3Chr.h"
//#include "../Client/N3Base/N3Light.h"
//#include "../Client/N3Base/N3Camera.h"
//#include "../Client/WarFare/N3FXMgr.h"
//#include "../Client/WarFare/PlayerOtherMgr.h"
//#include "../Client/WarFare/PlayerMySelf.h"
//
//// define the screen resolution
//#define SCREEN_WIDTH 1024
//#define SCREEN_HEIGHT 768
//
//// include the Direct3D Library files
//#pragma comment (lib, "shared.lib")
//#pragma comment (lib, "wsock32.lib")
//#pragma comment (lib, "ws2_32.lib")
//#pragma comment (lib, "winmm.lib")
//#pragma comment (lib, "imm32.lib")
//#pragma comment (lib, "d3d9.lib")
//#pragma comment (lib, "d3dx9.lib")
//#pragma comment (lib, "dsound.lib")
//#pragma comment (lib, "N3Base.lib")
//#pragma comment (lib, "dxguid.lib")
//#pragma comment (lib, "DInput8.lib")
//#pragma comment (lib, "KnightOnLine.lib")
//
//// global declarations
//LPDIRECT3D9 d3d;
//LPDIRECT3DDEVICE9 d3ddev;
//typedef struct __D3DDEV_INFO
//{
//    char szDeviceName[128];
//    char szDeviceDesc[128];
//    int  nAdapter;
//    int  nDevice;
//
//    D3DDEVTYPE      DevType;
//    int             nModeCount;
//    D3DDISPLAYMODE* pModes;
//} __D3DDevInfo;
//__D3DDEV_INFO m_DeviceInfo;
//__VertexTransformed vMoon[4];
//// function prototypes
//void initD3D(HWND hWnd);
//void render_frame(void);
//void tick(void);
//void cleanD3D(void);
//void init_graphics(void);
//BOOL FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat);
//
//class CN3Chr* m_pChr;
//class CN3Texture* m_pTexBkg;
//class CN3Light* m_pLights[3];
//class CN3Camera* m_pCamera;
//
//float cameraX = -0.19f;
//float cameraY = 1.1f;
//
//__Matrix44 matWorld;
//
//struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };
//#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
//
//// the WindowProc function prototype
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//
//// the entry point for any Windows program
//int WINAPI WinMain(HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPSTR lpCmdLine,
//    int nCmdShow)
//{
//    HWND hWnd;
//    WNDCLASSEX wc;
//
//    ZeroMemory(&wc, sizeof(WNDCLASSEX));
//
//    wc.cbSize = sizeof(WNDCLASSEX);
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpfnWndProc = WindowProc;
//    wc.hInstance = hInstance;
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.lpszClassName = "WindowClass";
//
//    RegisterClassEx(&wc);
//    RECT rect;
//    GetClientRect(GetDesktopWindow(), &rect);
//    rect.left = (rect.right / 2) - (CN3Base::s_Options.iViewWidth / 2);
//    rect.top = (rect.bottom / 2) - (CN3Base::s_Options.iViewHeight / 2);
//    hWnd = CreateWindowEx(NULL, "WindowClass", "Our Direct3D Program",
//        WS_OVERLAPPEDWINDOW, rect.left, rect.top, SCREEN_WIDTH, SCREEN_HEIGHT,
//        NULL, NULL, hInstance, NULL);
//
//    ShowWindow(hWnd, nCmdShow);
//
//    initD3D(hWnd);
//
//    MSG msg;
//    bool quit = false;
//    while (TRUE)
//    {
//        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//
//            if (msg.message == WM_QUIT)
//                quit = true;
//        }
//
//        if (quit)
//            break;
//
//        tick();
//        render_frame();
//    }
//
//    cleanD3D();
//
//    return msg.wParam;
//}
//
//
//// this is the main message handler for the program
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_DESTROY:
//    {
//        PostQuitMessage(0);
//        return 0;
//    } break;
//    case WM_KEYDOWN:
//    {
//        if (wParam == VK_LEFT)
//        {
//            cameraX += 100;
//        }
//        else if (wParam == VK_RIGHT)
//        {
//            cameraX-= 100;
//        }
//    }
//    }
//
//    return DefWindowProc(hWnd, message, wParam, lParam);
//}
//
//
//// this function initializes and prepares Direct3D for use
//void initD3D(HWND hWnd)
//{
//    d3d = Direct3DCreate9(D3D_SDK_VERSION);
//
//    D3DPRESENT_PARAMETERS d3dpp;
//
//    ZeroMemory(&d3dpp, sizeof(d3dpp));
//    d3dpp.Windowed = TRUE;
//    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//    d3dpp.hDeviceWindow = hWnd;
//    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
//    d3dpp.BackBufferWidth = SCREEN_WIDTH;
//    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
//    d3dpp.EnableAutoDepthStencil = TRUE;
//    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
//
//
//    //HRESULT rval = d3d->CreateDevice(0, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
//
//    // FIX (srmeier): I really have no idea what the second arguement here should be
//    int nAMC = d3d->GetAdapterModeCount(0, D3DFMT_X8R8G8B8); // 디스플레이 모드 카운트
//    if (nAMC <= 0)
//    {
//        
//    }
//
//    m_DeviceInfo.nAdapter = 0;
//    m_DeviceInfo.DevType = D3DDEVTYPE_HAL;
//    m_DeviceInfo.nDevice = 0;
//    m_DeviceInfo.nModeCount = nAMC;
//    delete[] m_DeviceInfo.pModes;
//    m_DeviceInfo.pModes = new D3DDISPLAYMODE[nAMC];
//    for (int i = 0; i < nAMC; i++)
//    {
//        // FIX (srmeier): I really have no idea what the second arguement here should be
//        d3d->EnumAdapterModes(0, D3DFMT_X8R8G8B8, i, &m_DeviceInfo.pModes[i]); // 디스플레이 모드 가져오기..
//    }
//    bool bWindowed = true;
//    bool bUseHW = true;
//    DWORD dwWidth = SCREEN_WIDTH;
//    DWORD dwHeight = SCREEN_HEIGHT;
//    DWORD dwBPP = 16;
//    D3DDEVTYPE DevType = D3DDEVTYPE_REF;
//    if (TRUE == bUseHW) DevType = D3DDEVTYPE_HAL;
//
//    memset(&CN3Base::s_DevParam, 0, sizeof(CN3Base::s_DevParam));
//    CN3Base::s_DevParam.Windowed = bWindowed;
//    CN3Base::s_DevParam.EnableAutoDepthStencil = TRUE;
//    CN3Base::s_DevParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
//    CN3Base::s_DevParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
//    CN3Base::s_DevParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
//
//    D3DFORMAT BBFormat = D3DFMT_UNKNOWN;
//    if (TRUE == bWindowed) // 윈도우 모드일 경우
//    {
//        D3DDISPLAYMODE dm;
//        d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
//        CN3Base::s_DevParam.BackBufferCount = 1;
//        if (dwWidth <= 0) dwWidth = dm.Width;
//        if (dwHeight <= 0) dwHeight = dm.Height;
//        BBFormat = dm.Format;
//        CN3Base::s_DevParam.hDeviceWindow = hWnd;
//    }
//    else
//    {
//        CN3Base::s_DevParam.BackBufferCount = 1;
//        CN3Base::s_DevParam.AutoDepthStencilFormat = D3DFMT_D16; // 자동 생성이면 무시된다.
//        if (16 == dwBPP) BBFormat = D3DFMT_R5G6B5;
//        else if (24 == dwBPP) BBFormat = D3DFMT_R8G8B8;
//        else if (32 == dwBPP) BBFormat = D3DFMT_X8R8G8B8;
//        CN3Base::s_DevParam.hDeviceWindow = hWnd;
//    }
//
//    CN3Base::s_DevParam.BackBufferWidth = dwWidth;
//    CN3Base::s_DevParam.BackBufferHeight = dwHeight;
//    CN3Base::s_DevParam.BackBufferFormat = BBFormat;
//    CN3Base::s_DevParam.MultiSampleType = D3DMULTISAMPLE_NONE; // Swap Effect 가 Discard 형태가 아니면 반드시 이런 식이어야 한다.
//    CN3Base::s_DevParam.Flags = 0;
//    //#ifdef _N3TOOL
//    CN3Base::s_DevParam.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
//    //#endif // end of _N3TOOL
//
//    int nMC = m_DeviceInfo.nModeCount;
//    for (int i = 0; i < nMC; i++)
//    {
//        //		if(	m_DeviceInfo.pModes[i].Width == dwWidth && 
//        //			m_DeviceInfo.pModes[i].Height == dwHeight && 
//        if (m_DeviceInfo.pModes[i].Format == BBFormat) // 모드가 일치하면
//        {
//            FindDepthStencilFormat(0, m_DeviceInfo.DevType, m_DeviceInfo.pModes[i].Format, &CN3Base::s_DevParam.AutoDepthStencilFormat); // 깊이와 스텐실 버퍼를 찾는다.
//            //m_nModeActive = i;
//            break;
//        }
//    }
//
//    HRESULT rval = d3d->CreateDevice(0, DevType, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &CN3Base::s_DevParam, &d3ddev);
//    if (rval != D3D_OK)
//    {
//        rval = d3d->CreateDevice(0, DevType, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &CN3Base::s_DevParam, &d3ddev);
//        if (rval != D3D_OK)
//        {           
//        }
//    }
//
//
//    // Device 지원 항목은??
//    // DXT 지원 여부..
//    CN3Base::s_dwTextureCaps = 0;
//    CN3Base::s_DevCaps.DeviceType = DevType;
//
//    d3ddev->GetDeviceCaps(&CN3Base::s_DevCaps);
//    if (CN3Base::s_DevCaps.MaxTextureWidth < 256 || CN3Base::s_DevCaps.MaxTextureHeight < 256) // 텍스처 지원 크기가 256 이하면.. 아예 포기..
//    {
//    }
//
//    if (D3D_OK == d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT1)) CN3Base::s_dwTextureCaps |= TEX_CAPS_DXT1;
//    if (D3D_OK == d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT2)) CN3Base::s_dwTextureCaps |= TEX_CAPS_DXT2;
//    if (D3D_OK == d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT3)) CN3Base::s_dwTextureCaps |= TEX_CAPS_DXT3;
//    if (D3D_OK == d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT4)) CN3Base::s_dwTextureCaps |= TEX_CAPS_DXT4;
//    if (D3D_OK == d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT5)) CN3Base::s_dwTextureCaps |= TEX_CAPS_DXT5;
//    if (CN3Base::s_DevCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) CN3Base::s_dwTextureCaps |= TEX_CAPS_SQUAREONLY;
//    if (CN3Base::s_DevCaps.TextureCaps & D3DPTEXTURECAPS_MIPMAP) CN3Base::s_dwTextureCaps |= TEX_CAPS_MIPMAP;
//    if (CN3Base::s_DevCaps.TextureCaps & D3DPTEXTURECAPS_POW2) CN3Base::s_dwTextureCaps |= TEX_CAPS_POW2;
//
//
//
//
//    for (int i = 0; i < 8; i++)
//    {
//        CN3Light::__Light Lgt;
//        _D3DCOLORVALUE LgtColor = { 1.0f, 1.0f, 1.0f, 1.0f };
//        Lgt.InitPoint(i, __Vector3(0, 0, 0), LgtColor);
//        d3ddev->SetLight(i, &Lgt);
//    }
//
//    __Matrix44 matView;
//    D3DXMatrixLookAtLH(&matView, &__Vector3(105, 105, -100), &__Vector3(0, 0, 0), &__Vector3(0, 1, 0));
//    d3ddev->SetTransform(D3DTS_VIEW, &matView);
//
//
//    __Matrix44 matProjection;
//    D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(45.0f), (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH, 0.1f, 256.0f);
//    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);
//
//
//    RECT rcView = { 0, 0, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT };
//    D3DVIEWPORT9 vp;
//    vp.X = rcView.left;
//    vp.Y = rcView.top;
//    vp.Width = rcView.right - rcView.left;
//    vp.Height = rcView.bottom - rcView.top;
//    vp.MinZ = 0.0f;
//    vp.MaxZ = 1.0f;
//
//    d3ddev->SetViewport(&vp);
//    
//    matWorld.Identity();
//    d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
//    d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
//    d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
//    
//    d3ddev->SetRenderState(D3DRS_DITHERENABLE, TRUE);
//    d3ddev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
//    
//    d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//    d3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//    
//    d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//    d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//    float fMipMapLODBias = -1.0f;
//
//    for (int i = 0; i < 8; ++i) {
//        d3ddev->SetTexture(i, NULL);
//        d3ddev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//        d3ddev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//        d3ddev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//        d3ddev->SetSamplerState(i, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD)(&fMipMapLODBias)));
//    }
//
//    D3DCLIPSTATUS9 cs;
//    cs.ClipUnion = cs.ClipIntersection = D3DCS_ALL;
//
//    d3ddev->SetClipStatus(&cs);
//
//
//    init_graphics();
//
//    CGameBase::s_pTbl_FXSource.LoadFromFile("Data\\fx.tbl");
//
//  
//}
//
//BOOL FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat)
//{
//    int nDSC = 6;
//    D3DFORMAT DepthFmts[] = { D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D24X8, D3DFMT_D16, D3DFMT_D15S1 };
//
//    HRESULT rval = 0;
//    for (int i = 0; i < nDSC; i++)
//    {
//        rval = d3d->CheckDeviceFormat(iAdapter, DeviceType, TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, DepthFmts[i]);
//        if (D3D_OK == rval)
//        {
//            rval = d3d->CheckDepthStencilMatch(iAdapter, DeviceType, TargetFormat, TargetFormat, DepthFmts[i]);
//            if (D3D_OK == rval)
//            {
//                *pDepthStencilFormat = DepthFmts[i];
//                return TRUE;
//            }
//        }
//    }
//
//    return FALSE;
//}
//
//
//void tick(void)
//{
//    for (int i = 0; i < 3; i++) 	m_pLights[i]->Tick();
//    m_pChr->Tick();
//    CGameProcedure::s_pFX->TriggerBundle(m_pChr, 0, 2602, m_pChr->Pos(), 0, -1, -2);
//    CGameProcedure::s_pFX->Tick();
//}
//
//// this is the function used to render a single frame
//void render_frame(void)
//{
//    m_pCamera->AtPosSet(cameraX, cameraY, 0.09f);
//    CN3Base::s_lpD3DDev->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
//    CN3Base::s_lpD3DDev->BeginScene();
//         // 카메라 잡기..
//    //m_pCamera->LookAt(__Vector3(5, 5, -10), __Vector3(0, 0, 0), __Vector3(0, 1, 0));
//    m_pCamera->Tick();
//    m_pCamera->Apply();
//
//    for (int i = 0; i < 8; i++) 	CN3Base::s_lpD3DDev->LightEnable(i, FALSE);
//    for (int i = 0; i < 3; i++) 	m_pLights[i]->Apply();
//
//   
//    
//
//    DWORD dwZWrite;
//    CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ZWRITEENABLE, &dwZWrite);
//    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//    CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
//    CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
//    CN3Base::s_lpD3DDev->SetTexture(0, m_pTexBkg->Get());
//    CN3Base::s_lpD3DDev->SetFVF(FVF_TRANSFORMED);
//    CN3Base::s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vMoon, sizeof(__VertexTransformed));
//
//    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZWRITEENABLE, dwZWrite);
//    // 달그리기..
//    ////////////////////////////////////////////
//
//
//    m_pChr->Render(); 
//
//    CGameProcedure::s_pFX->Render();
//
//    CN3Base::s_lpD3DDev->EndScene();
//
//    d3ddev->Present(NULL, NULL, NULL, NULL);
//}
//
//
//// this is the function that cleans up Direct3D and COM
//void cleanD3D(void)
//{
//    d3ddev->Release();
//    d3d->Release();
//
//    if (m_pChr) delete m_pChr;
//    m_pChr = NULL;
//    if (m_pTexBkg) delete m_pTexBkg;
//    m_pTexBkg = NULL;
//
//    for (int i = 0; i < 3; i++) 
//        if (m_pLights[i]) 
//            delete m_pLights[i];
//
//    if (m_pCamera)
//        delete m_pCamera;
//
//    if (CGameProcedure::s_pFX)
//        delete CGameProcedure::s_pFX;
//}
//
//
//// this is the function that puts the 3D models into video RAM
//void init_graphics(void)
//{
//    char szPath[_MAX_PATH] = "";
//    GetCurrentDirectory(_MAX_PATH, szPath);
//    CN3Base::PathSet(szPath);
//    CN3Base::s_lpD3DDev = d3ddev;
//    
//    m_pChr = new CN3Chr();
//    m_pChr->LoadFromFile("Intro\\Intro.N3Chr");
//    m_pChr->AniCurSet(0); // 루핑 에니메이션..
//
//    m_pTexBkg = new CN3Texture();
//    m_pTexBkg->LoadFromFile("Intro\\Moon.dxt");
//
//    for (int i = 0; i < 3; i++) m_pLights[i] = new CN3Light();
//    m_pLights[0]->LoadFromFile("Intro\\0.N3Light");
//    m_pLights[1]->LoadFromFile("Intro\\1.N3Light");
//    m_pLights[2]->LoadFromFile("Intro\\2.N3Light");
//
//    m_pCamera = new CN3Camera();
//    m_pCamera->EyePosSet(0.22f, 0.91f, -6.f);
//    m_pCamera->AtPosSet(-0.19f, 1.1f, 0.09f);
//    m_pCamera->m_Data.fNP = 0.1f;
//    m_pCamera->m_Data.fFP = 32.f;
//    m_pCamera->m_bFogUse = false;
//
//    CGameProcedure::s_pFX = new CN3FXMgr();
//
//    CGameBase::s_pOPMgr = new CPlayerOtherMgr();
//    CGameBase::s_pPlayer = new CPlayerMySelf();
//    CGameBase::s_pPlayer->m_InfoBase.iID = 1;
//    D3DVIEWPORT9 vp;
//    CN3Base::s_lpD3DDev->GetViewport(&vp);
//
//    float fMW = (m_pTexBkg->Width() * vp.Width / 1024.0f) * 1.3f;
//    float fMH = (m_pTexBkg->Height() * vp.Height / 768.0f) * 1.3f;
//    float fX = 100.0f * vp.Width / 1024.0f;
//    float fY = 50.0f * vp.Height / 768.0f;
//
//    float fRHW = 1.0f;
//    
//    vMoon[0].Set(10, 5, 0, fRHW, 0xffffffff, 0.0f, 0.0f);
//    vMoon[1].Set(30, 5, 0, fRHW, 0xffffffff, 1.0f, 0.0f);
//    vMoon[2].Set(30, 25, 0, fRHW, 0xffffffff, 1.0f, 1.0f);
//    vMoon[3].Set(10, 25, 0, fRHW, 0xffffffff, 0.0f, 1.0f);
//}
