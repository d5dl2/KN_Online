// GameProcedure.cpp: implementation of the CGameProcedure class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "resource.h"

#include "GameDef.h"
#include "GameEng.h"
#include "packetdef.h"
#include "LocalInput.h"
#include "shared\APISocket.h"
#include "UIMessageBox.h"
#include "UIMessageBoxManager.h"
#include "UIManager.h"

#include "N3FXMgr.h"
#include "PlayerMyself.h"
#include "GameProcedure.h"
#include "GameProcLogIn.h"
//#include "GameProcStart.h"
#include "GameProcNationSelect.h"
#include "GameProcCharacterCreate.h"
#include "GameProcCharacterSelect.h"
#include "GameProcMain.h"
#include "GameProcOption.h"

#include "UILoading.h"
#include "UINotice.h"
#include "UIHelp.h"
#include "UIHotKeyDlg.h"
#include "UIChat.h"
#include "UIVarious.h"
#include "UIPartyOrForce.h"
#include "UIMessageWnd.h"
#include "UIEndingDisplay.h"

#include "N3UIEdit.h"
#include "N3SndObjStream.h"
#include "N3FXBundle.h"

#include "BitmapFile.h"
#include "Jpeg.h"
#include "JpegFile.h"

#include "MagicSkillMng.h"
#include "GameCursor.h"

#include "shared/Compression.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CN3SndObjStream*	CGameProcedure::s_pSnd_BGM = NULL;			// ¸ÞÀÎ ¹è°æÀ½¾Ç Æ÷ÀÎÅÍ..
CLocalInput*		CGameProcedure::s_pLocalInput = NULL;		// ¸¶¿ì½º¿Í Å°º¸µå ÀÔ·Â °´Ã¼ .. Direct Input À» ½è´Ù.
CAPISocket*			CGameProcedure::s_pSocket = NULL;			// ¸ÞÀÎ ¼ÒÄÏ °´Ã¼
CAPISocket*			CGameProcedure::s_pSocketSub = NULL;		// ¼­ºê ¼ÒÄÏ °´Ã¼
CGameEng*			CGameProcedure::s_pEng = NULL;				// 3D Wrapper Engine
CN3FXMgr*			CGameProcedure::s_pFX = NULL;

CUIManager*			CGameProcedure::s_pUIMgr = NULL;			// UI Manager
CUILoading*			CGameProcedure::s_pUILoading = NULL;		// ·Îµù¹Ù..
CUIMessageBoxManager*	CGameProcedure::s_pMsgBoxMgr = NULL;		// MessageBox Manager
//bool				CGameProcedure::s_bUseSpeedHack = false;

CGameProcedure*				CGameProcedure::s_pProcPrev = NULL;
CGameProcedure*				CGameProcedure::s_pProcActive = NULL;

CGameProcLogIn*				CGameProcedure::s_pProcLogIn = NULL;
CGameProcNationSelect*		CGameProcedure::s_pProcNationSelect = NULL;
CGameProcCharacterCreate*	CGameProcedure::s_pProcCharacterCreate = NULL;
CGameProcCharacterSelect*	CGameProcedure::s_pProcCharacterSelect = NULL;
CGameProcMain*				CGameProcedure::s_pProcMain = NULL;
CGameProcOption*			CGameProcedure::s_pProcOption = NULL;
CGameCursor*				CGameProcedure::s_pGameCursor = NULL;

HCURSOR	CGameProcedure::s_hCursorNormal = NULL;
HCURSOR	CGameProcedure::s_hCursorNormal1 = NULL;
HCURSOR	CGameProcedure::s_hCursorClick = NULL;
HCURSOR	CGameProcedure::s_hCursorClick1 = NULL;
HCURSOR	CGameProcedure::s_hCursorAttack = NULL;
HCURSOR	CGameProcedure::s_hCursorPreRepair = NULL;
HCURSOR	CGameProcedure::s_hCursorNowRepair = NULL;
HCURSOR	CGameProcedure::m_hPrevGameCursor = NULL;

e_LogInClassification CGameProcedure::s_eLogInClassification; // Á¢¼ÓÇÑ ¼­ºñ½º.. MGame, Daum, KnightOnLine ....
std::string	CGameProcedure::s_szAccount = ""; // °èÁ¤ ¹®ÀÚ¿­..
std::string	CGameProcedure::s_szPassWord = ""; // °èÁ¤ ºñ¹ø..
std::string	CGameProcedure::s_szServer = ""; // ¼­¹ö ¹®ÀÚ¿­..
bool CGameProcedure::m_bCursorLocked = false;
HWND CGameProcedure::s_hWndSubSocket = NULL; // ¼­ºê ¼ÒÄÏ¿ë À©µµ¿ì ÇÚµé..
int	CGameProcedure::s_iChrSelectIndex = 0;
bool CGameProcedure::s_bNeedReportConnectionClosed = false; // ¼­¹öÁ¢¼ÓÀÌ ²÷¾îÁø°É º¸°íÇØ¾ß ÇÏ´ÂÁö..
bool CGameProcedure::s_bWindowed = false; // Ã¢¸ðµå ½ÇÇà??
bool CGameProcedure::s_bKeyPress = false;	//Å°°¡ ´­·ÁÁ³À»¶§ ui¿¡¼­ ÇØ´çÇÏ´Â Á¶ÀÛµÈÀûÀÌ ÀÖ´Ù¸é
bool CGameProcedure::s_bKeyPressed = false;	//Å°°¡ ¿Ã¶ó°¬À»¶§ ui¿¡¼­ ÇØ´çÇÏ´Â Á¶ÀÛµÈÀûÀÌ ÀÖ´Ù¸é

// NOTE: adding boolean to check if window has focus or not
bool CGameProcedure::s_bIsWindowInFocus = true;

// NOTE: added a bool for whether window has mouse focus or not
bool CGameProcedure::s_bWindowHasMouseFocus = true;

CGameProcedure::CGameProcedure()
{
	m_bCursorLocked = false;
}

CGameProcedure::~CGameProcedure()
{
	m_bCursorLocked = false;
}

void CGameProcedure::Release()
{
	s_pUIMgr->SetFocusedUI(NULL);
}

void CGameProcedure::Init()
{
	s_pUIMgr->SetFocusedUI(NULL);
}

void CGameProcedure::StaticMemberInit(HINSTANCE hInstance, HWND hWndMain, HWND hWndSub)
{
#if _DEBUG 
	s_bWindowed = true;
#endif 

	s_hWndBase = hWndMain;
	s_hWndSubSocket = hWndSub; // ¼­ºê ¼ÒÄÏ¿ë À©µµ¿ì ÇÚµé..

	s_pEng = new CGameEng();
	if(false == s_pEng->Init(s_bWindowed, s_hWndBase, CN3Base::s_Options.iViewWidth, CN3Base::s_Options.iViewHeight, CN3Base::s_Options.iViewColorDepth, TRUE)) exit(-1);
	// °ÔÀÓ ±âº» 3D ¿£Áø ¸¸µé±â..

	
	RECT rc;
	::GetClientRect(s_hWndBase, &rc);
	RECT rcTmp = rc; rcTmp.left = (rc.right - rc.left) / 2; rcTmp.bottom = rcTmp.top + 30;
	CN3UIEdit::CreateEditWindow(s_hWndBase, rcTmp);
	//////////////////////////////////////////////////////////////////////////////////////////


	CGameBase::StaticMemberInit(); // Table ¹× ÁöÇü, ¿ÀºêÁ§Æ®, Ä³¸¯ÅÍ ÃÊ±âÈ­...

	//////////////////////////////////////////////////////////////////////////////////////////
	// Game Procedure ¼ÒÄÏ°ú ·ÎÄÃ ÀÎÇ², 3D¿£Áø, Resource Table ·Îµù ¹× ÃÊ±âÈ­...
	s_pSocket = new CAPISocket();
	s_pSocketSub = new CAPISocket();


	s_hCursorNormal = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL));	
	s_hCursorNormal1 = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL1));
	s_hCursorClick = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_CLICK));
	s_hCursorClick1 = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_CLICK1));
	s_hCursorAttack = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_ATTACK));
	s_hCursorPreRepair = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_PRE_REPAIR));
	s_hCursorNowRepair = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_NOW_REPAIR));

	if (!CN3Base::s_Options.bWindowCursor)
	{
		s_pGameCursor = new CGameCursor();
		s_pGameCursor->LoadFromFile("ui\\cursor.uif");
	}
	SetGameCursor(s_hCursorNormal);


	s_pLocalInput = new CLocalInput();
	s_pLocalInput->Init(hInstance, s_hWndBase, FALSE);

	//////////////////////////////////////////////////////////////////////////////////////////
	// Sound ÃÊ±âÈ­..
	if(CN3Base::s_Options.bSndEnable)
	{
		CN3Base::s_SndMgr.Init(s_hWndBase);//pWindow);
		CN3Base::s_SndMgr.SetDuplicated(CN3Base::s_Options.bSndDuplicated);
	}
	CN3FXBundle::SetEffectSndDistance(float(CN3Base::s_Options.iEffectSndDist));

	s_pFX = new CN3FXMgr();

	__TABLE_UI_RESRC* pTblUI = s_pTbl_UI.Find(NATION_ELMORAD); // ±âº»Àº ¿¤¸ð¶óµå UI ·Î ÇÑ´Ù..
	if(pTblUI == NULL) {
		printf("ERROR: UI table is NULL.\n");
		system("pause");
		exit(-1);
	}

	s_pUIMgr = new CUIManager(); 
	s_pMsgBoxMgr = new CUIMessageBoxManager();

	CN3UIBase::EnableTooltip(pTblUI->szToolTip);

	//////////////////////////////////////////////////////////////////////////////////////////
	// °¢ ÇÁ·Î½ÃÀúµé »ý¼º
	s_pProcLogIn			= new CGameProcLogIn();				// ·Î±×ÀÎ ÇÁ·Î½ÃÁ®
	s_pProcNationSelect		= new CGameProcNationSelect();		// ³ª¶ó ¼±ÅÃ
	s_pProcCharacterSelect	= new CGameProcCharacterSelect();	// Ä³¸¯ÅÍ ¼±ÅÃ
	s_pProcCharacterCreate	= new CGameProcCharacterCreate();	// Ä³¸¯ÅÍ ¸¸µé±â
	s_pProcMain				= new CGameProcMain();				// ¸ÞÀÎ °ÔÀÓ ÇÁ·Î½ÃÁ®
	s_pProcOption			= new CGameProcOption();			// °ÔÀÓ ¿É¼Ç ÇÁ·Î½ÃÁ®
}

void CGameProcedure::StaticMemberRelease()
{
#ifndef _DEBUG
	// ´ë¸¸ ¹öÀüÀÏ °æ¿ì IME ¹®Á¦·Î ÀÎÇØ °¡Â¥ Ç®¸ðµå¸¦ ¾´´Ù..
	int iLangID = ::GetUserDefaultLangID();
	if(0x0404 == iLangID) 
	{
		DEVMODE dm;
		::EnumDisplaySettings(NULL, ENUM_REGISTRY_SETTINGS, &dm);

		// ·¹Áö½ºÆ®¸®ÀÇ µð½ºÇÃ·¹ÀÌ ¸ðµå¿Í ÇöÀç °ÔÀÓÀÇ µð½ºÇÃ·¹ÀÌ ¸ðµå°¡ ´Ù¸£¸é º¹±¸ ½ÃÄÑÁØ´Ù.
		if(	dm.dmPelsWidth != CN3Base::s_Options.iViewWidth || 
			dm.dmPelsHeight != CN3Base::s_Options.iViewHeight || 
			dm.dmBitsPerPel != CN3Base::s_Options.iViewColorDepth)
		{
			dm.dmSize = sizeof(DEVMODE);
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			::ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
		}
	}
#endif // #ifndef _DEBUG


	delete s_pSocket; s_pSocket = NULL; // Åë½Å ²÷±â..
	delete s_pSocketSub; s_pSocketSub = NULL; // ¼­ºê ¼ÒÄÏ ¾ø¾Ö±â..
	delete s_pFX; s_pFX = NULL;

	////////////////////////////////////////////////////////////
	// ±âº»°ª ¾²±â..
	if(s_pPlayer)
	{
		int iRun = s_pPlayer->IsRunning(); // ÀÌµ¿ ¸ðµå°¡ ¶Ù´Â »óÅÂ¿´À¸¸é 
		CGameProcedure::RegPutSetting("UserRun", &iRun, 4); // °È±â, ¶Ù±â »óÅÂ ±â·Ï..
	}
	
	if(s_pEng)
	{
		e_ViewPoint eVP = s_pEng->ViewPoint();
		CGameProcedure::RegPutSetting("CameraMode", &eVP, 4); // Ä«¸Þ¶ó »óÅÂ ±â·Ï
	}
	// ±âº»°ª ¾²±â..
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////
	// ¿£µùÈ­¸é º¸ÀÌ±â..
	if(s_pPlayer)
	{
		e_Nation eNation = s_pPlayer->m_InfoBase.eNation;
		__TABLE_UI_RESRC* pTbl = s_pTbl_UI.Find(eNation);
		if(pTbl)
		{
			CUIEndingDisplay Credit; // ¿£µù Ç¥½ÃÇÏ±â..
			Credit.LoadFromFile(pTbl->szEndingDisplay);
			Credit.Render();
		}
	}
	// ¿£µùÈ­¸é º¸ÀÌ±â..
	////////////////////////////////////////////////////////////////////////

//	if ( (s_pProcMain) && (s_pProcMain->m_pUIHotKeyDlg) )
//			s_pProcMain->m_pUIHotKeyDlg->CloseIconRegistry();

	// UI À§Ä¡¹× º¸ÀÌ±â µîÀÇ Á¤º¸ ÀúÀå..
	if(s_pProcMain)
	{
		UIPostData_Write(UI_POST_WND_CHAT, s_pProcMain->m_pUIChatDlg);
		UIPostData_Write(UI_POST_WND_HOTKEY, s_pProcMain->m_pUIHotKeyDlg);
		UIPostData_Write(UI_POST_WND_HELP, s_pProcMain->m_pUIHelp);
		UIPostData_Write(UI_POST_WND_PARTY, s_pProcMain->m_pUIPartyOrForce);
		UIPostData_Write(UI_POST_WND_INFO, s_pProcMain->m_pUIMsgDlg);
	}

	// °¢ ÇÁ·Î½ÃÀúµé
	delete s_pProcLogIn; s_pProcLogIn = NULL; 						// ·Î±×ÀÎ ÇÁ·Î½ÃÁ®
	delete s_pProcNationSelect; s_pProcNationSelect = NULL; 		// ³ª¶ó ¼±ÅÃ
	delete s_pProcCharacterSelect; s_pProcCharacterSelect = NULL; 	// Ä³¸¯ÅÍ ¼±ÅÃ
	delete s_pProcCharacterCreate; s_pProcCharacterCreate = NULL; 	// Ä³¸¯ÅÍ ¸¸µé±â
	delete s_pProcMain; s_pProcMain = NULL; 						// ¸ÞÀÎ °ÔÀÓ ÇÁ·Î½ÃÁ®
	delete s_pProcOption; s_pProcOption = NULL; 					// °ÔÀÓ ¿É¼Ç ÇÁ·Î½ÃÁ®

	// UI µé ³¯¸®±â..
	if(s_pUILoading) delete s_pUILoading; s_pUILoading = NULL;		// Loading Bar
	delete s_pMsgBoxMgr;
	delete s_pUIMgr; s_pUIMgr = NULL;				// UI Manager

//	delete s_pIME; s_pIME = NULL;
	delete s_pLocalInput; s_pLocalInput = NULL;
	delete s_pEng; s_pEng = NULL; // Á© ¸¶Áö¸·¿¡ ¿£Áø ³¯¸®±â.!!!!!

	if(s_pGameCursor) delete s_pGameCursor; s_pGameCursor = NULL;

	CGameBase::StaticMemberRelease();
}

void CGameProcedure::Tick()
{
	s_pLocalInput->Tick(); // Å°º¸µå¿Í ¸¶¿ì½º·ÎºÎÅÍ ÀÔ·ÂÀ» ¹Þ´Â´Ù.
	if(s_pGameCursor) s_pGameCursor->Tick();

	ProcessUIKeyInput();

	uint32_t dwMouseFlags = s_pLocalInput->MouseGetFlag();
	POINT ptPrev = s_pLocalInput->MouseGetPosOld();
	POINT ptCur = s_pLocalInput->MouseGetPos();

	e_Nation eNation = s_pPlayer->m_InfoBase.eNation;
	if(dwMouseFlags & MOUSE_LBCLICK) SetGameCursor(((NATION_ELMORAD == eNation) ? s_hCursorClick1 : s_hCursorClick));
	else if(dwMouseFlags & MOUSE_LBCLICKED) SetGameCursor(((NATION_ELMORAD == eNation) ? s_hCursorNormal1 : s_hCursorNormal));
	if(dwMouseFlags & MOUSE_RBCLICKED)
	{
		if(s_pPlayer->m_bAttackContinous && s_pProcActive == s_pProcMain) // ¸ÞÀÎ ÇÁ·Î½ÃÁ® ÀÌ¸é..
			SetGameCursor(s_hCursorAttack);
		else
			SetGameCursor(((NATION_ELMORAD == eNation) ? s_hCursorNormal1 : s_hCursorNormal));
	}

	uint32_t dwRet = 0;
	dwRet = s_pMsgBoxMgr->MouseProcAndTick(dwMouseFlags, s_pLocalInput->MouseGetPos(), s_pLocalInput->MouseGetPosOld());

	if(0 == dwRet)
	{
		dwRet = s_pUIMgr->MouseProc(dwMouseFlags, ptCur, ptPrev);
		s_pUIMgr->Tick();
	}

	// ¸ó°¡ ÇÏ¸é... 
//	if((dwRet & UI_MOUSEPROC_CHILDDONESOMETHING) || (dwRet & UI_MOUSEPROC_DONESOMETHING))
//		s_pLocalInput->MouseRemoveFlag(0xffMOUSE_LBCLICK | MOUSE_LBCLICKED | MOUSE_LBDBLCLK);
	s_pUIMgr->m_bDoneSomething = false;		// UI ¿¡¼­ Á¶ÀÛÀ» Çß´Ù...
	if(dwRet != UI_MOUSEPROC_NONE)
		s_pUIMgr->m_bDoneSomething = true;		// UI ¿¡¼­ Á¶ÀÛÀ» Çß´Ù...

	CN3Base::s_SndMgr.Tick(); // Sound Engine...

	// ½ºÅ©¸° Ä¸ÃÄ Å°..
	if(s_pLocalInput->IsKeyPress(DIK_NUMPADMINUS)) // Å°ÆÐµåÀÇ ¸¶ÀÌ³Ê½º Å°¸¦ ´©¸£¸é..
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);
		char szFN[128] = "";
//		sprintf(szFN, "%d_%d_%d_%d.%d.%d.jpg", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		sprintf(szFN, "%d_%d_%d_%d.%d.%d.ksc", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		this->CaptureScreenAndSaveToFile(szFN);
	}

	//////////////////////////////////
	// Network Msg Ã³¸®ÇÏ±â
	while (!s_pSocket->m_qRecvPkt.empty())
	{
		auto pkt = s_pSocket->m_qRecvPkt.front();
		if (!ProcessPacket(*pkt))
			CLogWriter::Write("Invalid Packet... (%d)", pkt->GetOpcode());

		delete pkt;
		s_pSocket->m_qRecvPkt.pop();
	}

	while (!s_pSocketSub->m_qRecvPkt.empty())
	{
		auto pkt = s_pSocketSub->m_qRecvPkt.front();
		if (!ProcessPacket(*pkt))
			break;

		delete pkt;
		s_pSocketSub->m_qRecvPkt.pop();
	}
	// Network Msg Ã³¸®ÇÏ±â
	//////////////////////////////////
}

void CGameProcedure::Render()
{
	if(s_pUIMgr) s_pUIMgr->Render(); // UI µé ·»´õ¸µ..

	s_pMsgBoxMgr->Render();
	if(s_pGameCursor) s_pGameCursor->Render();
}

void CGameProcedure::TickActive()
{
	if(s_pProcActive != s_pProcPrev) // ÇÁ·Î½ÃÀú°¡ ¹Ù²î¸é..
	{
		if(s_pProcPrev) s_pProcPrev->Release();
		if(s_pProcActive) s_pProcActive->Init();

		s_pProcPrev = s_pProcActive;
	}

	if(s_pProcActive)	s_pProcActive->Tick();		// ÇöÀç ÇÁ·Î½ÃÀú Tick ................................
}

void CGameProcedure::RenderActive()
{
//	if(s_pProcActive != s_pProcPrev) // ÇÁ·Î½ÃÀú°¡ ¹Ù²î¸é..
//	{
//		if(s_pProcPrev) s_pProcPrev->Release();
//		if(s_pProcActive) s_pProcActive->Init();
//
//		s_pProcPrev = s_pProcActive;
//	}
//	else 
//	{
		if(s_pProcActive == s_pProcPrev) s_pProcActive->Render();
//	}
}

bool CGameProcedure::CaptureScreenAndSaveToFile(const std::string& szFN)
{
	/*
	if(szFN.empty()) return false;
	CJpegFile file;

	RECT wndRect;
	GetWindowRect(CN3Base::s_hWndBase, &wndRect);
	
	HANDLE hDIB = file.CopyScreenToDIB(&wndRect);
	if(hDIB)
	{
		int nQuality = 90;
		char szBuf[256] = "";

		//¿î¿µÀÚ´Â ¾çÁúÀÇ ½ºÅ©¸° Ä¸ÃÄ¸¦ ÇÒ¼ö ÀÖ°Ô...
		if(s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER)
			nQuality = 100;

		if(file.EncryptJPEG(hDIB, nQuality, szFN, szBuf) == TRUE)
		{
			TRACE("Screen Captue %s\n", szFN.c_str());
		}
		GlobalFree(hDIB);
	}
	*/
	return true;
/*
	int iW = CN3Base::s_CameraData.vp.Width;
	int iH = CN3Base::s_CameraData.vp.Height;

	bool bResult = false;
	LPDIRECT3DSURFACE8 lpDDSTmp = NULL;
	LPDIRECT3DSURFACE8 lpDDSBack = NULL;
	CN3Base::s_lpD3DDev->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &lpDDSBack);
	if(lpDDSBack)
	{
		CN3Base::s_lpD3DDev->CreateImageSurface(iW, iH, D3DFMT_X8R8G8B8, &lpDDSTmp);
		if(lpDDSTmp)
		{
			HRESULT rval = D3DXLoadSurfaceFromSurface(lpDDSTmp, NULL, NULL, lpDDSBack, NULL, NULL, D3DX_FILTER_NONE, 0);
//			HRESULT rval = s_lpD3DDev->CopyRects(lpDDSBack, NULL, 0, lpDDSTmp, NULL);
//			char szErr[256];
//			::D3DXGetErrorString(rval, szErr, 256);

			if(D3D_OK == rval)
			{
				D3DLOCKED_RECT LR;
				if(D3D_OK == lpDDSTmp->LockRect(&LR, NULL, 0))
				{
//					std::vector<uint8_t> buff(iW * iH * 3, 0);
					CBitMapFile bmf;
					bmf.Create(iW, iH);

					for(int y = 0; y < iH; y++)
					{
						uint8_t* pPS = ((uint8_t*)LR.pBits) + LR.Pitch * y;
//						uint8_t* pPD = (uint8_t*)(&(buff[y * (iW * 3)]));
						uint8_t* pPD = (uint8_t*)(bmf.Pixels(0, y));

						for(int x = 0; x < iW; x++, pPS += 4, pPD +=3 )
						{
							pPD[0] = pPS[0];
							pPD[1] = pPS[1];
							pPD[2] = pPS[2];
						}
					}
					lpDDSTmp->UnlockRect();

//					CJpeg jpg;
//					jpg.SaveJPG(szFN.c_str(), iW, iH, &(buff[0]));
					bmf.SaveToFile(szFN.c_str());
				}
			}
			
			lpDDSTmp->Release();
			lpDDSTmp = NULL;
		}


		lpDDSBack->Release();
		lpDDSBack = NULL;
	}

	return bResult;
*/
}

void CGameProcedure::ProcActiveSet(CGameProcedure *pProc)
{
	if(NULL == pProc || s_pProcActive == pProc) return;

	if(s_pUIMgr) s_pUIMgr->EnableOperationSet(true); // UI¸¦ Á¶ÀÛÇÒ¼ö ÀÖ°Ô ÇÑ´Ù..
	CGameProcedure::MessageBoxClose(-1); // MessageBox °¡ ¶° ÀÖÀ¸¸é °¨Ãá´Ù.

	s_pProcPrev = s_pProcActive; // ÀüÀÇ °Í Æ÷ÀÎÅÍ ±â¾ï..
	s_pProcActive = pProc;
}

void CGameProcedure::ReConnect()
{
	s_bNeedReportConnectionClosed = false; // ¼­¹öÁ¢¼ÓÀÌ ²÷¾îÁø°É º¸°íÇØ¾ß ÇÏ´ÂÁö..
	CGameProcedure::s_pSocket->ReConnect();
	s_bNeedReportConnectionClosed = true; // ¼­¹öÁ¢¼ÓÀÌ ²÷¾îÁø°É º¸°íÇØ¾ß ÇÏ´ÂÁö..
}

std::string CGameProcedure::MessageBoxPost(const std::string& szMsg, const std::string& szTitle, int iStyle, e_Behavior eBehavior)
{
	return s_pMsgBoxMgr->MessageBoxPost(szMsg, szTitle, iStyle, eBehavior);
}

void CGameProcedure::MessageBoxClose(const std::string& szMsg)
{
	s_pMsgBoxMgr->MessageBoxClose(szMsg);
}

void CGameProcedure::MessageBoxClose(int iMsgBoxIndex)
{
	if( iMsgBoxIndex == -1 )
		s_pMsgBoxMgr->MessageBoxCloseAll();
}


bool CGameProcedure::RegPutSetting( const char *ValueName, void *pValueData, long length )
{
	HKEY	hKey;

	if ( RegOpenKey( HKEY_CURRENT_USER, GetStrRegKeySetting().c_str(), &hKey ) != ERROR_SUCCESS ) 
	{
		if ( RegCreateKey( HKEY_CURRENT_USER, GetStrRegKeySetting().c_str(), &hKey ) != ERROR_SUCCESS )
		{
			__ASSERT(0, "Registry Create Failed!!!");
			return false;
		}
		if(RegOpenKey( HKEY_CURRENT_USER, GetStrRegKeySetting().c_str(), &hKey ) != ERROR_SUCCESS ) 
		{
			__ASSERT(0, "Registry Open Failed!!!");
			return false;
		}
	}

	// set the value
	if ( RegSetValueEx( hKey, ValueName, 0, REG_BINARY, (const uint8_t *)pValueData, length ) != ERROR_SUCCESS )
	{
		__ASSERT(0, "Registry Write Failed!!!");
		RegCloseKey( hKey );
		return false;
	}

	if ( RegCloseKey( hKey ) != ERROR_SUCCESS ) 
	{
		__ASSERT(0, "Registry Close Failed!!!");
		return false;
	}

	return true;
}

bool CGameProcedure::RegGetSetting( const char *ValueName, void *pValueData, long length)
{
	HKEY	hKey;
	DWORD	Type;
	DWORD	len;

	len = length;

	if ( RegOpenKey( HKEY_CURRENT_USER, GetStrRegKeySetting().c_str(), &hKey ) != ERROR_SUCCESS ) 
	{
//		__ASSERT(0, "Registry Open Failed!!!");
		return false;
	}

	// get the value
	if ( RegQueryValueEx( hKey, ValueName, NULL, &Type, (uint8_t *)pValueData, &len ) != ERROR_SUCCESS )
	{
//		__ASSERT(0, "Registry Query Failed!!!");
		RegCloseKey( hKey );
		return false;
	}

	if ( RegCloseKey( hKey ) != ERROR_SUCCESS ) 
	{
//		__ASSERT(0, "Registry Close Failed!!!");
		return false;
	}

	return true;
}

void CGameProcedure::UIPostData_Write(const std::string& szKey, CN3UIBase* pUI)
{
	if(szKey.empty() || NULL == pUI) return;

	__WndInfo WI;
	lstrcpyn(WI.szName, szKey.c_str(), 16);
	WI.bVisible = pUI->IsVisible();
	WI.ptPosition = pUI->GetPos();

	RegPutSetting(WI.szName, &WI, sizeof(__WndInfo) );
}

void CGameProcedure::UIPostData_Read(const std::string& szKey, CN3UIBase* pUI, int iDefaultX, int	 iDefaultY)
{
	if(szKey.empty() || NULL == pUI) return;

	// 1. µðÆúÆ® µ¥ÀÌÅÍ¸¦ ¸¸µç´Ù..
	// 2. µ¥ÀÌÅÍ¸¦ ÀÐ¾î¿Â´Ù..
	// 3. ¿µ¿ªÀÌ À¯È¿ÇÑÁö¸¦ ÆÇ´ÜÇÑ´Ù..

	__WndInfo WI;
	WI.ptPosition.x = iDefaultX;
	WI.ptPosition.y = iDefaultY;
	if(false == RegGetSetting(szKey.c_str(), &WI, sizeof(__WndInfo))) WI.bVisible = true; // ±âº» µ¥ÀÌÅÍ°¡ ¾øÀ¸¸é ¹«Á¶°Ç º¸ÀÌ°Ô ÇÑ´Ù..

	RECT rc = pUI->GetRegion();

	if (WI.ptPosition.x < 0) WI.ptPosition.x = 0;
	if (WI.ptPosition.x + (rc.right - rc.left) > (int)s_CameraData.vp.Width)
		WI.ptPosition.x = s_CameraData.vp.Width - (rc.right - rc.left);
	if (WI.ptPosition.y < 0) WI.ptPosition.y = 0;
	if (WI.ptPosition.y + (rc.bottom - rc.top) >(int)s_CameraData.vp.Height)
		WI.ptPosition.y = s_CameraData.vp.Height - (rc.bottom - rc.top);

	pUI->SetVisible(WI.bVisible);
	if(0 == WI.ptPosition.x && 0 == WI.ptPosition.y)
		pUI->SetPos(iDefaultX, iDefaultY);
	else pUI->SetPos(WI.ptPosition.x, WI.ptPosition.y);
}

void CGameProcedure::SetGameCursor(HCURSOR hCursor, bool bLocked)
{
	if(s_pGameCursor)
	{
		e_Cursor eCursor = CURSOR_KA_NORMAL;

		if(hCursor == s_hCursorNormal)
			eCursor = CURSOR_KA_NORMAL;
		else if(hCursor == s_hCursorNormal1)
			eCursor = CURSOR_EL_NORMAL;
		else if(hCursor == s_hCursorClick)
			eCursor = CURSOR_KA_CLICK;
		else if(hCursor == s_hCursorClick1)
			eCursor = CURSOR_EL_CLICK;
		else if(hCursor == s_hCursorAttack)
			eCursor = CURSOR_ATTACK;
		else if(hCursor == s_hCursorPreRepair)
			eCursor = CURSOR_PRE_REPAIR;
		else if(hCursor == s_hCursorNowRepair)
			eCursor = CURSOR_NOW_REPAIR;
		else if(hCursor == NULL)
			eCursor = CURSOR_UNKNOWN;

		SetGameCursor(eCursor, bLocked);

		if ((!m_bCursorLocked) && bLocked)
		{
			m_bCursorLocked = true;
		}

	}
	else
	{
		if ((m_bCursorLocked) && (!bLocked) ) return;
		else if ( ((m_bCursorLocked) && bLocked) || ((!m_bCursorLocked) && !bLocked) )
		{
			::SetCursor(hCursor);
			return;
		}
		else if ((!m_bCursorLocked) && bLocked)
		{
			m_hPrevGameCursor = ::GetCursor();
			m_bCursorLocked = true;
			::SetCursor(hCursor);
		}
	}
}

void CGameProcedure::SetGameCursor(e_Cursor eCursor, bool bLocked)
{
	if(s_pGameCursor == NULL) return;
	s_pGameCursor->SetGameCursor(eCursor, bLocked);
}

void CGameProcedure::RestoreGameCursor()
{
	if(s_pGameCursor)
	{
		if (m_bCursorLocked) 
			m_bCursorLocked = false;

		if(s_pGameCursor) s_pGameCursor->RestoreGameCursor();
	}
	else
	{
		if (m_bCursorLocked) 
			m_bCursorLocked = false;

		::SetCursor(m_hPrevGameCursor);
	}
}

std::string CGameProcedure::GetStrRegKeySetting()
{
	char szBuff[256];
	sprintf(szBuff, "Software\\KnightOnline\\%s_%s_%d", s_szAccount.c_str(), s_szServer.c_str(), s_iChrSelectIndex);
	return szBuff;
}

bool CGameProcedure::ProcessPacket(Packet& pkt)
{
	int iCmd = pkt.read<uint8_t>();	// Ä¿¸àµå ÆÄ½Ì..
	switch ( iCmd )										// Ä¿¸àµå¿¡ ´Ù¶ó¼­ ºÐ±â..
	{
		case WIZ_COMPRESS_PACKET:
			this->MsgRecv_CompressedPacket(pkt);
			return true;

		case WIZ_VERSION_CHECK: // ¾ÏÈ£È­µµ °°ÀÌ ¹Þ´Â´Ù..
			this->MsgRecv_VersionCheck(pkt); // virtual
			return true;

		case WIZ_LOGIN:
			this->MsgRecv_GameServerLogIn(pkt);
			return true;

		case WIZ_SERVER_CHANGE:				// ¼­¹ö ¹Ù²Ù±â ¸Þ½ÃÁö..
		{
			// ´Ù¸¥ Á¸ ¼­¹ö·Î ´Ù½Ã Á¢¼ÓÇÑ´Ù.
			int iLen = 0;
			std::string szName, szIP;
//			iLen = pkt.read<int16_t>(); // ¼­¹ö ÀÌ¸§
//			pkt.readString(szName, iLen);
			iLen = pkt.read<int16_t>(); // ¼­¹ö IP
			pkt.readString(szIP, iLen);
			uint32_t dwPort = pkt.read<int16_t>();
			s_pPlayer->m_InfoExt.iZoneInit = pkt.read<uint8_t>();
			s_pPlayer->m_InfoExt.iZoneCur = pkt.read<uint8_t>();
			int iVictoryNation = pkt.read<uint8_t>();
			CGameProcedure::LoadingUIChange(iVictoryNation);

			s_bNeedReportConnectionClosed = false; // ¼­¹öÁ¢¼ÓÀÌ ²÷¾îÁø°É º¸°íÇØ¾ß ÇÏ´ÂÁö..
			s_pSocket->Disconnect(); // ²÷°í...
			Sleep(2000); // 2ÃÊ µô·¹ÀÌ.. ¼­¹ö°¡ Ã³¸®ÇÒ ½Ã°£À» ÁØ´Ù.
			int iErr = s_pSocket->Connect(s_hWndBase, szIP.c_str(), dwPort);
			s_bNeedReportConnectionClosed = true; // ¼­¹öÁ¢¼ÓÀÌ ²÷¾îÁø°É º¸°íÇØ¾ß ÇÏ´ÂÁö..

			if(iErr) this->ReportServerConnectionFailed("Current Zone", iErr, true); // ¼­¹ö Á¢¼Ó ¿À·ù.. Exit.
			else
			{
				// ¹öÀüÃ¼Å©¸¦ º¸³»¸é.. ÀÀ´äÀ¸·Î ¹öÀü°ú ¾ÏÈ£È­ Å°°¡ ¿Â´Ù.
				// ¸ÞÀÎ ÇÁ·Î½ÃÀúÀÇ °æ¿ì Character_Select ¸¦ º¸³»°í ·Î±×ÀÎÀÏ°æ¿ì GameServer_LogIn À» º¸³½´Ù.
				this->MsgSend_VersionCheck(); 
			}
		}
		return true;

		case WIZ_SEL_CHAR:
		{
			this->MsgRecv_CharacterSelect(pkt); // virtual
		}
		return true;
	}

	return false;
}

void CGameProcedure::ReportServerConnectionFailed(const std::string& szServerName, int iErrCode, bool bNeedQuitGame)
{
	char szErr[256];
	std::string szFmt;
	::_LoadStringFromResource(IDS_FMT_CONNECT_ERROR, szFmt);
	sprintf(szErr, szFmt.c_str(), szServerName.c_str(), iErrCode);
	
	e_Behavior eBehavior = ((bNeedQuitGame) ? BEHAVIOR_EXIT : BEHAVIOR_NOTHING);
	CGameProcedure::MessageBoxPost(szErr, "", MB_OK, eBehavior);
	return;
}

void CGameProcedure::ReportServerConnectionClosed(bool bNeedQuitGame)
{
	if(!s_bNeedReportConnectionClosed) return;

	std::string szMsg;
	::_LoadStringFromResource(IDS_CONNECTION_CLOSED, szMsg);
	e_Behavior eBehavior = ((bNeedQuitGame) ? BEHAVIOR_EXIT : BEHAVIOR_NOTHING);
	CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, eBehavior);

	if(s_pPlayer)
	{
		__Vector3 vPos = s_pPlayer->Position();
		CLogWriter::Write("Socket Closed... Zone(%d) Pos(%.1f, %.1f, %.1f) Exp(%d)",
			s_pPlayer->m_InfoExt.iZoneCur, vPos.x, vPos.y, vPos.z, s_pPlayer->m_InfoExt.iExp);
	}
	else
	{
		CLogWriter::Write("Socket Closed...");
	}

	if(s_pSocket) s_pSocket->Release();
}

void CGameProcedure::ReportDebugStringAndSendToServer(const std::string& szDebug)
{
	if(szDebug.empty()) return;

	CLogWriter::Write(szDebug.c_str());

	if(s_pSocket && s_pSocket->IsConnected())
	{
		int iLen = szDebug.size();
		std::vector<uint8_t> buffer;	// ¹öÆÛ.. 
		buffer.assign(iLen + 4, 0x00);
		int iOffset=0;												// ¿É¼Â..
		s_pSocket->MP_AddByte(&(buffer[0]), iOffset, WIZ_DEBUG_STRING_PACKET);
		s_pSocket->MP_AddShort(&(buffer[0]), iOffset, iLen);
		s_pSocket->MP_AddString(&(buffer[0]), iOffset, szDebug);
		s_pSocket->Send(&(buffer[0]), iOffset);				// º¸³¿..
	}
}

void CGameProcedure::MsgSend_GameServerLogIn()
{
	uint8_t byBuff[128];										// ÆÐÅ¶ ¹öÆÛ..
	int iOffset = 0;										// ¹öÆÛÀÇ ¿ÀÇÁ¼Â..

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_LOGIN);	// Ä¿¸àµå.
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)s_szAccount.size());	// ¾ÆÀÌµð ±æÀÌ..
	CAPISocket::MP_AddString(byBuff, iOffset, s_szAccount);			// ½ÇÁ¦ ¾ÆÀÌµð..
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)s_szPassWord.size());	// ÆÐ½º¿öµå ±æÀÌ
	CAPISocket::MP_AddString(byBuff, iOffset, s_szPassWord);		// ½ÇÁ¦ ÆÐ½º¿öµå
		
	s_pSocket->Send(byBuff, iOffset);								// º¸³½´Ù
}

void CGameProcedure::MsgSend_VersionCheck() // virtual
{
	// Version Check
	int iOffset = 0;
	uint8_t byBuffs[4];
	CAPISocket::MP_AddByte(byBuffs, iOffset, WIZ_VERSION_CHECK);				// Ä¿¸àµå.
	s_pSocket->Send(byBuffs, iOffset);	// º¸³½´Ù

#ifdef _CRYPTION
	s_pSocket->m_bEnableSend = FALSE; // º¸³»±â °¡´É..?
#endif // #ifdef _CRYPTION
}

void CGameProcedure::MsgSend_CharacterSelect() // virtual
{
	uint8_t byBuff[64];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_SEL_CHAR);				// Ä¿¸àµå.
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)s_szAccount.size());				// °èÁ¤ ±æÀÌ..
	CAPISocket::MP_AddString(byBuff, iOffset, s_szAccount);						// °èÁ¤ ¹®ÀÚ¿­..
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)s_pPlayer->IDString().size());		// Ä³¸¯ ¾ÆÀÌµð ±æÀÌ..
	CAPISocket::MP_AddString(byBuff, iOffset, s_pPlayer->IDString());			// Ä³¸¯ ¾ÆÀÌµð ¹®ÀÚ¿­..
	CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->m_InfoExt.iZoneInit);	// Ã³À½ Á¢¼ÓÀÎÁö ¾Æ´ÑÁö 0x01:Ã³À½ Á¢¼Ó
	CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->m_InfoExt.iZoneCur);		// Ä³¸¯ÅÍ ¼±ÅÃÃ¢¿¡¼­ÀÇ Ä³¸¯ÅÍ Á¸ ¹øÈ£
	s_pSocket->Send(byBuff, iOffset);	// º¸³½´Ù

	CLogWriter::Write("MsgSend_CharacterSelect - name(%s) zone(%d)",
		s_pPlayer->IDString().c_str(), s_pPlayer->m_InfoExt.iZoneCur); // µð¹ö±ë ·Î±×..
}

void CGameProcedure::MsgRecv_CompressedPacket(Packet& pkt) // ¾ÐÃàµÈ µ¥ÀÌÅÍ ÀÌ´Ù... ÇÑ¹ø ´õ ÆÄ½ÌÇØ¾ß ÇÑ´Ù!!!
{
	uint16_t compressedLength = pkt.read<uint16_t>();
	uint16_t originalLength = pkt.read<uint16_t>();
	uint32_t crc = pkt.read<uint32_t>();

	uint8_t * decompressedBuffer = Compression::DecompressWithCRC32(pkt.contents() + pkt.rpos(), compressedLength, originalLength, crc);
	if (decompressedBuffer == NULL)
		return;

	Packet decompressedPkt;
	decompressedPkt.append(decompressedBuffer, originalLength);
	delete[] decompressedBuffer;

	ProcessPacket(decompressedPkt);
}

int CGameProcedure::MsgRecv_VersionCheck(Packet& pkt) // virtual
{
	int iVersion = pkt.read<int16_t>();	// ¹öÀü
#ifdef _CRYPTION
	uint64_t iPublicKey = pkt.read<uint64_t>(); // ¾ÏÈ£È­ °ø°³Å°
	CAPISocket::InitCrypt(iPublicKey);
	s_pSocket->m_bEnableSend = TRUE; // º¸³»±â °¡´É..?
#endif // #ifdef _CRYPTION

	if(iVersion != CURRENT_VERSION)
	{
		char szErr[256];

		int iLangID = ::GetUserDefaultLangID();
		if(0x0404 == iLangID)// Taiwan Language
		{
			std::string szFmt;
			::_LoadStringFromResource(IDS_VERSION_CONFIRM_TW, szFmt);
			CGameProcedure::MessageBoxPost(szFmt, "", MB_OK, BEHAVIOR_EXIT);
		}
		else
		{
			std::string szFmt;
			::_LoadStringFromResource(IDS_VERSION_CONFIRM, szFmt);
			sprintf(szErr, szFmt.c_str(), CURRENT_VERSION / 1000.0f, iVersion / 1000.0f);
			CGameProcedure::MessageBoxPost(szErr, "", MB_OK, BEHAVIOR_EXIT);
		}

	}

	return iVersion;
}

int CGameProcedure::MsgRecv_GameServerLogIn(Packet& pkt) // virtual
{
	int iNation = pkt.read<uint8_t>(); // ±¹°¡ - 0 ¾øÀ½ 0xff - ½ÇÆÐ..
	return iNation;
}

bool CGameProcedure::MsgRecv_CharacterSelect(Packet& pkt) // virtual
{
	int iResult = pkt.read<uint8_t>(); // 0x00 ½ÇÆÐ
	if(1 == iResult) // ¼º°ø..
	{
		int iZoneCur = pkt.read<uint8_t>();
		float fX = (pkt.read<uint16_t>())/10.0f;
		float fZ = (pkt.read<uint16_t>())/10.0f;
		float fY = (pkt.read<int16_t>())/10.0f;

		int iVictoryNation = pkt.read<uint8_t>();
		CGameProcedure::LoadingUIChange(iVictoryNation);

		int iZonePrev;
		if(N3FORMAT_VER_DEFAULT & N3FORMAT_VER_1264) {
			iZonePrev = s_pPlayer->m_InfoExt.iZoneCur = 10*iZoneCur;
		} else {
			iZonePrev = s_pPlayer->m_InfoExt.iZoneCur = iZoneCur;
		}
		s_pPlayer->PositionSet(__Vector3(fX, fY, fZ), true);

		CLogWriter::Write("MsgRecv_CharacterSelect - name(%s) zone(%d -> %d)", s_pPlayer->m_InfoBase.szID.c_str(), iZonePrev, iZoneCur);
		return true;
	}
	else // ½ÇÆÐ
	{
		CLogWriter::Write("MsgRecv_CharacterSelect - failed(%d)", iResult);
		return false;
	}

	if(iResult) return true;
	else return false;
}

void CGameProcedure::ProcessUIKeyInput(bool bEnable)
{
	s_bKeyPressed	= false;	//Å°°¡ ¿Ã¶ó°¬À»¶§ ui¿¡¼­ ÇØ´çÇÏ´Â Á¶ÀÛµÈÀûÀÌ ÀÖ´Ù¸é

	if(!bEnable)
	{
		if(s_bKeyPress)
		{
			for(int i = 0; i < NUMDIKEYS; i++)
			{
				if(s_pLocalInput->IsKeyPressed(i))
				{
					if(!s_bKeyPressed) s_bKeyPress = false;
					break;
				}
			}
		}
		return;
	}


	CN3UIBase* pMsgBox = s_pMsgBoxMgr->GetFocusMsgBox();
	CN3UIBase* pUIFocus = s_pUIMgr->GetFocusedUI();

	if(pMsgBox && pMsgBox->IsVisible())//this_ui
	{
		for(int i = 0; i < NUMDIKEYS; i++)
		{
			if(s_pLocalInput->IsKeyPress(i))
				s_bKeyPress |= pMsgBox->OnKeyPress(i);
			if(s_pLocalInput->IsKeyPressed(i))
				s_bKeyPressed |= pMsgBox->OnKeyPressed(i);
		}
	}
	else if(pUIFocus && pUIFocus->IsVisible()) // Æ÷Ä¿½Ì µÈ UI °¡ ÀÖÀ¸¸é...
	{
		for(int i = 0; i < NUMDIKEYS; i++)
		{
			if(s_pLocalInput->IsKeyPress(i))
			{
				if(pUIFocus->m_pChildUI && pUIFocus->m_pChildUI->IsVisible())
					s_bKeyPress |= pUIFocus->m_pChildUI->OnKeyPress(i);
				else
					s_bKeyPress |= pUIFocus->OnKeyPress(i);
			}
			if(s_pLocalInput->IsKeyPressed(i))
			{
				if(pUIFocus->m_pChildUI && pUIFocus->m_pChildUI->IsVisible())
					s_bKeyPressed |= pUIFocus->m_pChildUI->OnKeyPressed(i);
				else
					s_bKeyPressed |= pUIFocus->OnKeyPressed(i);
			}
		}
	}

	if(s_bKeyPress)
	{
		for(int i = 0; i < NUMDIKEYS; i++)
		{
			if(s_pLocalInput->IsKeyPressed(i))
			{
				if(!s_bKeyPressed) s_bKeyPress = false;
				break;
			}
		}
	}
}

bool CGameProcedure::IsUIKeyOperated()
{
	if (!s_bKeyPress && !s_bKeyPressed)
		return false;

	return true;
}

void CGameProcedure::LoadingUIChange(int iVictoryNation)
{
	if(s_pPlayer->m_InfoExt.iVictoryNation == iVictoryNation)
		return;

	s_pPlayer->m_InfoExt.iVictoryNation = iVictoryNation;

	std::string szLoading;
	if(s_pUILoading) delete s_pUILoading; s_pUILoading = NULL;		// Loading Bar

	s_pUILoading = new CUILoading();
	__ASSERT(s_pUILoading, "·ÎµùÈ­¸é »ý¼º ½ÇÆÐ");
	if(s_pUILoading == NULL) return;

	__TABLE_UI_RESRC* pTblUI = s_pTbl_UI.Find(NATION_ELMORAD); // ±âº»Àº ¿¤¸ð¶óµå UI ·Î ÇÑ´Ù..
	__ASSERT(pTblUI, "±âº» UI °¡ ¾ø½À´Ï´Ù.");
	if(pTblUI == NULL) return;

	switch(iVictoryNation)
	{
	case VICTORY_ABSENCE:
		szLoading = pTblUI->szLoading;
		break;
	case VICTORY_ELMORAD:
		szLoading = pTblUI->szElLoading;
		break;
	case VICTORY_KARUS:
		szLoading = pTblUI->szKaLoading;
		break;
	default:
		szLoading = pTblUI->szLoading;
		break;
	}

	//TRACE("Loading UIF : %s\n", szLoading.c_str());
	s_pUILoading->LoadFromFile(szLoading); // ±âº»ÀûÀÎ ·Îµù ¹Ù ¸¸µé±â..
}
