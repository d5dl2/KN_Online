/*
*/
#define DEBUG_STATISTICS

//#include "StdAfx.h"
#include "UIChat.h"
#include "GameEng.h"
#include "resource.h"
#include "N3SndMgr.h"
#include "N3UIEdit.h"
#include "PacketDef.h"
#include "shared/APISocket.h"
#include "PlayerMySelf.h"
#include "GameProcMain.h"
#include "N3WorldManager.h"
#include "../Server/shared/Ini.h"
#include "UIManager.h"
#include "IMouseWheelInputDlg.h"
#include "time.h"
#include "LauncherReplacer.h"
#include "DFont.h"
#include <WinSock2.h>
//-----------------------------------------------------------------------------
BOOL g_bActive = true;

//-----------------------------------------------------------------------------
/*
- NOTE: WndProcMain processes the messages for the main window
*/
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETCURSOR:
	{
		if (GetDlgItem(hWnd, 4000) == nullptr)
			return true;
	}
	case WM_COMMAND: {
		uint16_t wNotifyCode = HIWORD(wParam); // notification code
		CN3UIEdit* pEdit = CN3UIEdit::GetFocusedEdit();

		if (wNotifyCode == EN_CHANGE && pEdit) {
			uint16_t wID = LOWORD(wParam); // item, control, or accelerator identifier
			HWND hwndCtl = (HWND)lParam;

			if (CN3UIEdit::s_hWndEdit == hwndCtl) {
				pEdit->UpdateTextFromEditCtrl();
				pEdit->UpdateCaretPosFromEditCtrl();
				CGameProcedure::SetGameCursor(CGameProcedure::s_hCursorNormal);
			}
		}
	} break;

	case WM_NOTIFY: {
		int idCtrl = (int)wParam;
		NMHDR* pnmh = (NMHDR*)lParam;
	} break;

	// Taiwan dili için input girişi
	//case WM_KEYDOWN: {
	//	int iLangID = ::GetUserDefaultLangID();
	//	if (iLangID == 0x0404) { // Taiwan Language
	//		CUIChat* pUIChat = CGameProcedure::s_pProcMain->m_pUIChatDlg;
	//		int iVK = (int)wParam;

	//		if (
	//			pUIChat && iVK != VK_ESCAPE && iVK != VK_RETURN &&
	//			CGameProcedure::s_pProcMain &&
	//			CGameProcedure::s_pProcActive == CGameProcedure::s_pProcMain &&
	//			!pUIChat->IsChatMode()
	//			) {
	//			if (!(GetKeyState(VK_CONTROL) & 0x8000)) {
	//				pUIChat->SetFocus();
	//				PostMessage(CN3UIEdit::s_hWndEdit, WM_KEYDOWN, wParam, lParam);
	//				return 0;
	//			}
	//		}
	//	}
	//} break;

	case WM_SOCKETMSG: {
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT: {
			//TRACE("Socket connected..\n");
		} break;
		case FD_CLOSE: {
			if (CGameProcedure::s_bNeedReportConnectionClosed)
				CGameProcedure::ReportServerConnectionClosed(true);
			//TRACE("Socket closed..\n");
		}  break;
		case FD_READ: {
			CGameProcedure::s_pSocket->Receive();
		} break;
		default: {
			__ASSERT(0, "WM_SOCKETMSG: unknown socket flag.");
		} break;
		}
	} break;


	case WM_ACTIVATE: {
		int iActive = LOWORD(wParam);           // activation flag
		int iMinimized = (BOOL)HIWORD(wParam); // minimized flag
		HWND hwndPrevious = (HWND)lParam;      // window handle

		switch (iActive)
		{
		case WA_CLICKACTIVE:
		case WA_ACTIVE: {
#ifdef _DEBUG
			g_bActive = TRUE;
#endif
		} break;
		case WA_INACTIVE: {
#ifdef _DEBUG
			g_bActive = FALSE;
#endif

			if (CGameProcedure::s_bWindowed == false) {
				CLogWriter::Write("WA_INACTIVE.");
				PostQuitMessage(0);
			}
		} break;
		}
	} break;



	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT: {
		CGameProcedure::s_pSocket->Disconnect();
		CGameProcedure::s_pSocketSub->Disconnect();

		PostQuitMessage(0);
	} break;


	case WM_MOUSEWHEEL: {
		if (CGameProcedure::s_pProcActive == CGameProcedure::s_pProcMain) {
			float fDelta = ((int16_t)HIWORD(wParam)) * 0.05f;

			CN3UIBase* focused = CGameProcedure::s_pUIMgr->GetFocusedUI();

			if (focused)
			{
				int key = fDelta > 0 ? DIK_PRIOR : DIK_NEXT;
				if (IMouseWheelInputDlg* t = dynamic_cast<IMouseWheelInputDlg*>(focused))
					t->OnKeyPress(key);
				else
					CGameProcedure::s_pEng->CameraZoom(fDelta);
			}
			else
				CGameProcedure::s_pEng->CameraZoom(fDelta);
		}
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProcSub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_SOCKETMSG) {
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT: {
		} break;
		case FD_ACCEPT: {

		} break;
		case FD_CLOSE: {

		} break;
		case FD_READ: {
			CGameProcedure::s_pSocketSub->Receive();
		} break;
		default: {
			__ASSERT(0, "WM_SOCKETMSG: unknown socket flag.");
		} break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


//-----------------------------------------------------------------------------
HWND CreateMainWindow(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProcMain;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Knight OnLine Client";

	if (::RegisterClass(&wc) == 0) {
		CLogWriter::Write("Cannot register window class.");
		exit(-1);
	}

	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);
	rect.left = (rect.right / 2) - (CN3Base::s_Options.iViewWidth / 2);
	rect.top = (rect.bottom / 2) - (CN3Base::s_Options.iViewHeight / 2);

	DWORD style = WS_POPUP | WS_CLIPCHILDREN; //WS_OVERLAPPEDWINDOW;
	return ::CreateWindow(
		"Knight OnLine Client",
		"Knight OnLine Client",
		style,
		rect.left,
		rect.top, //0, 0,
		CN3Base::s_Options.iViewWidth,
		CN3Base::s_Options.iViewHeight,
		NULL, NULL, hInstance, NULL
	);
}

//-----------------------------------------------------------------------------
HWND CreateSubWindow(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProcSub;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Knight OnLine Sub";

	if (::RegisterClass(&wc) == 0) {
		CLogWriter::Write("Cannot register window class.");
		exit(-1);
	}

	DWORD style = WS_POPUP;
	return ::CreateWindow(
		"Knight OnLine Sub",
		"Knight OnLine Sub",
		style, 0, 0, 0, 0,
		NULL, NULL, hInstance, NULL
	);
}

//-----------------------------------------------------------------------------
/*
- NOTE: this is the main intry point for the knight online program
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	ReplaceLauncherIfUpdated();
	// NOTE: get the current directory and make it known to CN3Base
	char szPath[_MAX_PATH] = "";
	GetCurrentDirectory(_MAX_PATH, szPath);
	CN3Base::PathSet(szPath);

	// NOTE: we are anticipating an Options file to exist within this directory
	char szIniPath[_MAX_PATH] = "";
	lstrcpy(szIniPath, CN3Base::PathGet().c_str());
	lstrcat(szIniPath, "Option.ini");

	CIni ini(szIniPath);

	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Chr = ini.GetInt("Texture", "LOD_Chr", 0);
	if (CN3Base::s_Options.iTexLOD_Chr < 0) CN3Base::s_Options.iTexLOD_Chr = 0;
	if (CN3Base::s_Options.iTexLOD_Chr >= 2) CN3Base::s_Options.iTexLOD_Chr = 1;

	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Shape = ini.GetInt("Texture", "LOD_Shape", 0);
	if (CN3Base::s_Options.iTexLOD_Shape < 0) CN3Base::s_Options.iTexLOD_Shape = 0;
	if (CN3Base::s_Options.iTexLOD_Shape >= 2) CN3Base::s_Options.iTexLOD_Shape = 1;

	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Terrain = ini.GetInt("Texture", "LOD_Terrain", 0);
	if (CN3Base::s_Options.iTexLOD_Terrain < 0) CN3Base::s_Options.iTexLOD_Terrain = 0;
	if (CN3Base::s_Options.iTexLOD_Terrain >= 2) CN3Base::s_Options.iTexLOD_Terrain = 1;

	// NOTE: should we use shadows?
	CN3Base::s_Options.iUseShadow = ini.GetInt("Shadow", "Use", 1);

	// NOTE: what is the screen resolution?
	CN3Base::s_Options.iViewWidth = ini.GetInt("ViewPort", "Width", 1024);
	CN3Base::s_Options.iViewHeight = ini.GetInt("ViewPort", "Height", 768);

	if (CN3Base::s_Options.iViewWidth == 1024) CN3Base::s_Options.iViewHeight = 768;
	else if (1280 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1024;
	else if (1600 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1200;
	else if (1366 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 768;
	else if (1920 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1080;
	/*
	else {
		CN3Base::s_Options.iViewWidth = 1024;
		CN3Base::s_Options.iViewHeight = 768;
	*/

	// NOTE: what is the viewport's color depth?
	CN3Base::s_Options.iViewColorDepth = ini.GetInt("ViewPort", "ColorDepth", 16);
	if (CN3Base::s_Options.iViewColorDepth != 16 && CN3Base::s_Options.iViewColorDepth != 32)
		CN3Base::s_Options.iViewColorDepth = 32;

	// NOTE: what is the viewport's draw distance?
	CN3Base::s_Options.iViewDist = ini.GetInt("ViewPort", "Distance", 512);
	if (CN3Base::s_Options.iViewDist < 256) CN3Base::s_Options.iViewDist = 256;
	if (CN3Base::s_Options.iViewDist > 512) CN3Base::s_Options.iViewDist = 512;

	// NOTE: what is the distance for sound events?
	CN3Base::s_Options.iEffectSndDist = ini.GetInt("Sound", "Distance", 48);
	if (CN3Base::s_Options.iEffectSndDist < 20) CN3Base::s_Options.iEffectSndDist = 20;
	if (CN3Base::s_Options.iEffectSndDist > 48) CN3Base::s_Options.iEffectSndDist = 48;

	// NOTE: is sound enabled?
	CN3Base::s_Options.bSndEnable = ini.GetBool("Sound", "Enable", true);

	// NOTE: is sound duplicated?
	CN3Base::s_Options.bSndDuplicated = ini.GetBool("Sound", "Duplicate", false);

	// NOTE: should we show the window cursor?
	CN3Base::s_Options.bWindowCursor = ini.GetBool("Cursor", "WindowCursor", true);

	// NOTE: should we show window full screen?
	CN3Base::s_Options.bWindowMode = ini.GetBool("Screen", "WindowMode", true);

	srand((uint32_t)time(NULL));

	// NOTE: create the sub window
	HWND hWndSub = CreateSubWindow(hInstance);

	// NOTE: create the main window
	HWND hWndMain = CreateMainWindow(hInstance);

	// NOTE: check for success
	if (hWndMain == NULL || hWndSub == NULL) {
		fprintf(stderr, "Cannot create window.");
		Sleep(1000 * 5);
		exit(-1);
	}

	::ShowWindow(hWndSub, SW_HIDE);
	::ShowWindow(hWndMain, nShowCmd);

	// NOTE: set the main window to active
	::SetActiveWindow(hWndMain);

	CGameProcedure::s_bWindowed = true;

	// NOTE: allocate the static members
	CGameProcedure::StaticMemberInit(hInstance, hWndMain, hWndSub);

	// NOTE: set the games current procedure to s_pProcLogIn
	CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcLogIn);

	/*
	// TESTING ----------
	CGameBase::s_pPlayer->m_InfoBase.eRace = RACE_KA_ARKTUAREK;
	CGameBase::s_pPlayer->m_InfoBase.eNation = NATION_KARUS;
	CGameBase::s_pPlayer->m_InfoBase.eClass = CLASS_KA_WARRIOR;
	CGameBase::s_pPlayer->m_InfoExt.iZoneInit = 0x01;
	CGameBase::s_pPlayer->m_InfoExt.iZoneCur = 1;
	CGameBase::s_pPlayer->IDSet(-1, "testing", 0xffffffff);
	CGameBase::s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER;

	__TABLE_PLAYER_LOOKS* pLooks = CGameBase::s_pTbl_UPC_Looks.Find(CGameBase::s_pPlayer->m_InfoBase.eRace);
	CGameBase::s_pPlayer->InitChr(pLooks);

	CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcMain);
	//CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcCharacterSelect);
	// TESTING -----------
	*/

#if _DEBUG
	HACCEL hAccel = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_MAIN_ACCELATOR));
	HDC hDC = GetDC(hWndMain);
#endif


	BOOL bGotMsg = FALSE;
	MSG msg; memset(&msg, 0, sizeof(MSG));

	// NOTE: this is the main game loop	
	while (msg.message != WM_QUIT)
	{
		if (g_bActive)
			bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
		else
			bGotMsg = GetMessage(&msg, NULL, 0U, 0U);

		if (bGotMsg) {
#if _DEBUG
			if (TranslateAccelerator(hWndMain, hAccel, &msg) == 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		}
#else
			TranslateMessage(&msg);
			DispatchMessage(&msg);
#endif
	}
		else {
			// NOTE: render a frame during idle time (no messages are waiting)
			if (g_bActive) {
				// NOTE: update and render the game
				CGameProcedure::TickActive();
				CGameProcedure::RenderActive();

				// NOTE: if we aredebugging then also draw helpful information
#if _DEBUG
#ifdef DEBUG_STATISTICS
				static float fTimePrev = CN3Base::TimeGet();
				static char szDebugs[4][256] = { "", "", "", "" };
				float fTime = CN3Base::TimeGet();

				if (fTime > fTimePrev + 0.5f) {
					fTimePrev = fTime;

					sprintf(szDebugs[0], "nTerrain_Polygon(%d), nTerrain_Tile_Polygon(%d), nShape(%d), nShape_Part(%d), nShape_Polygon(%d)",
						CN3Base::s_RenderInfo.nTerrain_Polygon,
						CN3Base::s_RenderInfo.nTerrain_Tile_Polygon,
						CN3Base::s_RenderInfo.nShape,
						CN3Base::s_RenderInfo.nShape_Part,
						CN3Base::s_RenderInfo.nShape_Polygon);

					sprintf(szDebugs[1], "nChr(%d), nChr_Part(%d), nChr_Polygon(%d), nChr_Plug(%d), nChr_Plug_Polygon(%d)",
						CN3Base::s_RenderInfo.nChr,
						CN3Base::s_RenderInfo.nChr_Part,
						CN3Base::s_RenderInfo.nChr_Polygon,
						CN3Base::s_RenderInfo.nChr_Plug,
						CN3Base::s_RenderInfo.nChr_Plug_Polygon);

					sprintf(szDebugs[2], "Camera : FieldOfView(%.1f), NearPlane(%.1f) FarPlane(%.1f)",
						D3DXToDegree(CN3Base::s_CameraData.fFOV),
						CN3Base::s_CameraData.fNP,
						CN3Base::s_CameraData.fFP);

					if (CGameProcedure::s_pProcMain && CGameBase::ACT_WORLD && CGameBase::ACT_WORLD->GetSkyRef()) {
						int iYear = 0, iMonth = 0, iDay = 0, iH = 0, iM = 0;
						CGameBase::ACT_WORLD->GetSkyRef()->GetGameTime(&iYear, &iMonth, &iDay, &iH, &iM);
						sprintf(szDebugs[3], "%.2f:FPS, %d/%d/%d : %d:%d", CN3Base::s_fFrmPerSec, iYear, iMonth, iDay, iH, iM);
					}
					else szDebugs[3][0] = NULL;
				}

				for (int i = 0; i < 4; i++)
					if (szDebugs[i]) TextOut(hDC, 0, i * 18, szDebugs[i], lstrlen(szDebugs[i]));
#endif
#endif
			}
		}
}

#if _DEBUG
	ReleaseDC(hWndMain, hDC);
	DestroyAcceleratorTable(hAccel);
#endif

	CGameProcedure::StaticMemberRelease();

	return msg.wParam;
}


