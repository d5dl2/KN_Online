#include "UIWhisperOpenDlg.h"

#include "N3UIEdit.h"
#include "N3UIString.h"
#include "GameProcedure.h"
#include "GameProcMain.h"
#include "WhisperManager.h"
#include "UIManager.h"
#include "PacketDef.h"
CUIWhisperOpenDlg::CUIWhisperOpenDlg()
{
	m_pBtnChat = NULL;
	m_pExitIdString = NULL;
	m_pExitChatString = NULL;
	m_pBtnBar = NULL;
	m_pBtnHide = NULL;
	m_pEditChat = NULL;
	m_pScrollBar = NULL;
	m_pBtnClose = NULL;
	m_pImage = NULL;
	m_ppUILines = NULL;

	ZeroMemory(&m_rcChatOutRegion, sizeof(m_rcChatOutRegion));
	m_pUIWhisperCloseDlg = new CUIWhisperCloseDlg(this);
	m_bHasFocus = false;
	m_bHasFocus = false;
}


CUIWhisperOpenDlg::~CUIWhisperOpenDlg()
{
	if (m_ppUILines) { delete[] m_ppUILines; m_ppUILines = NULL; }
	ChatListItor itor;
	for (itor = m_LineBuffer.begin(); m_LineBuffer.end() != itor; ++itor)
	{
		__ChatInfo* pChatInfo = (*itor);
		if (pChatInfo) delete pChatInfo;
	}
	m_LineBuffer.clear();
}

void CUIWhisperOpenDlg::Release()
{
	CN3UIBase::Release();
	
	ZeroMemory(&m_rcChatOutRegion, sizeof(m_rcChatOutRegion));
	m_pBtnChat = NULL;
	m_pExitIdString = NULL;
	m_pExitChatString = NULL;
	m_pBtnBar = NULL;
	m_pBtnHide = NULL;
	m_pEditChat = NULL;
	m_pScrollBar = NULL;
	m_pBtnClose = NULL;
	m_pImage = NULL;

	if (m_ppUILines) { delete[] m_ppUILines; m_ppUILines = NULL; }
	ChatListItor itor;
	for (itor = m_LineBuffer.begin(); m_LineBuffer.end() != itor; ++itor)
	{
		__ChatInfo* pChatInfo = (*itor);
		if (pChatInfo) delete pChatInfo;
	}
	m_LineBuffer.clear();

	m_pUIWhisperCloseDlg->Release();

}


bool CUIWhisperOpenDlg::Load(HANDLE hFile)
{
	if (false == CN3UIBase::Load(hFile)) return false;

	m_pBtnChat = GetChildByID("btn_chat");							__ASSERT(m_pBtnChat, "NULL UI Component!!");
	m_pExitIdString = (CN3UIString*)GetChildByID("exit_id");		__ASSERT(m_pExitIdString, "NULL UI Component!!");
	m_pExitChatString = (CN3UIString*)GetChildByID("exit_chat");	__ASSERT(m_pExitChatString, "NULL UI Component!!");
	m_pBtnBar = GetChildByID("btn_bar");							__ASSERT(m_pBtnBar, "NULL UI Component!!");
	m_pBtnHide = GetChildByID("btn_hide");							__ASSERT(m_pBtnHide, "NULL UI Component!!");
	m_pEditChat = (CN3UIEdit*)GetChildByID("edit_chat");			__ASSERT(m_pEditChat, "NULL UI Component!!");
	m_pScrollBar = (CN3UIScrollBar*)GetChildByID("scroll");			__ASSERT(m_pScrollBar, "NULL UI Component!!");
	m_pBtnClose = GetChildByID("btn_close");						__ASSERT(m_pBtnClose, "NULL UI Component!!");
	//m_pImage = 
	m_pEditChat->SetMaxString(32); // 채팅 문자열 길이 제한..

	m_rcChatOutRegion = m_pExitChatString->GetRegion();
	CreateLines();
	return true;
}
void CUIWhisperOpenDlg::Init(CN3UIBase* pParent)
{
	CN3UIBase::Init(pParent);
	m_pUIWhisperCloseDlg->SetParent(pParent);
}

void CUIWhisperOpenDlg::SetCloseDialogPos(int x, int y)
{
	m_pUIWhisperCloseDlg->SetPos(x, y);
}

POINT CUIWhisperOpenDlg::GetCloseDialogPos()
{
	return	m_pUIWhisperCloseDlg->GetPos();
}

bool CUIWhisperOpenDlg::LoadCloseDlgFromFile(std::string FN)
{
	return m_pUIWhisperCloseDlg->LoadFromFile(FN);
}

void CUIWhisperOpenDlg::RemoveCloseDlgFromUIMgr()
{
	CGameProcedure::s_pUIMgr->RemoveChild(m_pUIWhisperCloseDlg);
}

uint32_t CUIWhisperOpenDlg::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;

	if (dwFlags & UI_MOUSE_LBCLICK)
	{
		SetFocus();
	}

	if (UI_STATE_COMMON_MOVE == m_eState)
	{
		if (dwFlags & UI_MOUSE_LBCLICKED)
		{
			SetState(UI_STATE_COMMON_NONE);
		}
		else
		{
			MoveOffset(ptCur.x - ptOld.x, ptCur.y - ptOld.y);
		}
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if (false == IsIn(ptCur.x, ptCur.y))
	{
		if (false == IsIn(ptOld.x, ptOld.y))
		{
			if (dwFlags & UI_MOUSE_LBCLICK)
				KillFocus();
			return dwRet;
		}
		dwRet |= UI_MOUSEPROC_PREVINREGION;
	}

	dwRet |= UI_MOUSEPROC_INREGION;

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		if (pChild == m_pBtnBar)
			continue;
		uint32_t dwChildRet = pChild->MouseProc(dwFlags, ptCur, ptOld);
		if (UI_MOUSEPROC_DONESOMETHING & dwChildRet)
		{	// 이경우에는 먼가 포커스를 받은 경우이다.
			// (아래 코드는 dialog를 관리하는 곳에서 해야 한다. 따라서 막아놓음)
			//			m_Children.erase(itor);			// 우선 리스트에서 지우고
			//			m_Children.push_front(pChild);	// 맨앞에 넣는다. 그리는 순서를 맨 나중에 그리도록 하려고

			dwRet |= (UI_MOUSEPROC_CHILDDONESOMETHING | UI_MOUSEPROC_DONESOMETHING);
			return dwRet;
		}
	}

	if (UI_STATE_COMMON_MOVE != m_eState &&
		PtInRect(&m_rcMovable, ptCur) && (dwFlags & UI_MOUSE_LBCLICK))
	{
		SetState(UI_STATE_COMMON_MOVE);
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	return dwRet;
}

BOOL CUIWhisperOpenDlg::MoveOffset(int iOffsetX, int iOffsetY)
{
	if (0 == iOffsetX && 0 == iOffsetY) return FALSE;

	// ui 영역
	m_rcRegion.left += iOffsetX;		m_rcRegion.top += iOffsetY;
	m_rcRegion.right += iOffsetX;		m_rcRegion.bottom += iOffsetY;

	// movable 영역
	m_rcMovable.left += iOffsetX;		m_rcMovable.top += iOffsetY;
	m_rcMovable.right += iOffsetX;		m_rcMovable.bottom += iOffsetY;

	// children 좌표 갱신
	CN3UIBase* pCUI = NULL; // Child UI...
	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		pCUI = (*itor);
		__ASSERT(pCUI, "child UI pointer is NULL!");
		pCUI->MoveOffset(iOffsetX, iOffsetY);
	}

	//m_pUIWhisperCloseDlg->MoveOffset(iOffsetX, iOffsetY);
	return true;
}

bool CUIWhisperOpenDlg::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (NULL == pSender) return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtnHide)
		{
			KillFocus();
			ToggleOpenClose(false);
		}
		else if (pSender == m_pBtnClose)
		{
			CGameProcedure::s_pProcMain->m_pWhisperMgr->DeleteDlg(m_szTargetId);
		}
	}
	else if (dwMsg == UIMSG_SCROLLBAR_POS)
	{
		int iCurLinePos = m_pScrollBar->GetCurrentPos();
		SetTopLine(iCurLinePos);
		return true;
	}
	else if (dwMsg == UIMSG_EDIT_RETURN)
	{
		return false;
	}

	return true;
}

bool CUIWhisperOpenDlg::OnKeyPress(int iKey)
{
	switch (iKey)
	{
	case DIK_RETURN:
	case DIK_NUMPADENTER:
	{
		if (m_pEditChat)
		{
			if (m_pEditChat->HaveFocus())
			{
				//SetString(m_pEditChat->GetString());					
				//CGameProcedure::s_pProcMain->MsgSend_ChatSelectTarget(m_szTargetId);
				std::string msg = m_pEditChat->GetString();
				CGameProcedure::s_pProcMain->MsgSend_ChatSelectTarget(m_szTargetId);
				CGameProcedure::s_pProcMain->MsgSend_Chat(N3_CHAT_PRIVATE, msg);

				this->AddLineBuffer(msg, SELF_MSG_COLOR);
				this->AdjustScroll();

				m_pEditChat->SetString("");
				m_pEditChat->KillFocus();
			}
			else
			{
				m_pEditChat->SetFocus();
			}
		}
		return true;
	}
	case DIK_ESCAPE:
	{
		ToggleOpenClose(false);
		return true;
	}
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUIWhisperOpenDlg::SetTargetId(std::string& targetId)
{
	m_szTargetId = targetId;
	if (m_pExitIdString)
	{
		m_pExitIdString->SetString(targetId);
	}
	m_pUIWhisperCloseDlg->SetTargetId(targetId);
}

void CUIWhisperOpenDlg::SetFocus()
{
	if (!m_bHasFocus)
	{
		if (m_pBtnBar)
			m_pBtnBar->SetState(UI_STATE_BUTTON_ON);

		this->SetString("");
		if (m_pEditChat) m_pEditChat->SetFocus();
		m_bHasFocus = true;
	}
}

void CUIWhisperOpenDlg::KillFocus()
{
	if (m_bHasFocus)
	{
		if (m_pBtnBar)
			m_pBtnBar->SetState(UI_STATE_BUTTON_NORMAL);
		this->SetString("");
		if (m_pEditChat) m_pEditChat->KillFocus();

		m_bHasFocus = false;
	}
}

void CUIWhisperOpenDlg::SetString(const std::string& szChat)
{
	m_szEditString = szChat;
	if (m_pExitChatString)
	{
		m_pExitChatString->SetString(m_szEditString);

	}
}

void CUIWhisperOpenDlg::CreateLines()
{
	int i;
	if (m_ppUILines) {
		for (i = 0; i < m_iChatLineCount; ++i)
		{
			if (m_ppUILines[i]) { delete m_ppUILines[i]; m_ppUILines[i] = NULL; }
		}
		delete[] m_ppUILines; m_ppUILines = NULL;
	}
	SIZE size;
	if (m_pExitChatString && m_pExitChatString->GetTextExtent("가", lstrlen("가"), &size) && size.cy > 0)
	{
		m_iChatLineCount = (m_rcChatOutRegion.bottom - m_rcChatOutRegion.top) / size.cy;
	}
	else return;
	const std::string& szFontName = m_pExitChatString->GetFontName();
	uint32_t dwFontHeight = m_pExitChatString->GetFontHeight();
	uint32_t dwFontFlag = m_pExitChatString->GetFontFlags();

	if (m_iChatLineCount <= 0 || szFontName.empty()) return;

	m_ppUILines = new CN3UIString * [m_iChatLineCount];
	for (i = 0; i < m_iChatLineCount; ++i)
	{
		RECT rc;
		SetRect(&rc, m_rcChatOutRegion.left, m_rcChatOutRegion.top + (i * size.cy),
			m_rcChatOutRegion.right, m_rcChatOutRegion.top + ((i + 1) * size.cy));
		m_ppUILines[i] = new CN3UIString();
		m_ppUILines[i]->Init(this);
		m_ppUILines[i]->SetRegion(rc);
		m_ppUILines[i]->SetStyle(UISTYLE_STRING_SINGLELINE | UISTYLE_STRING_ALIGNLEFT | UISTYLE_STRING_ALIGNTOP);
		m_ppUILines[i]->SetFont(szFontName, dwFontHeight, dwFontFlag & D3DFONT_BOLD, dwFontFlag & D3DFONT_ITALIC);
	}
}
void CUIWhisperOpenDlg::SetTopLine(int iTopLine)
{
	if (m_iChatLineCount <= 0) return;

	const int iLineBufferSize = m_LineBuffer.size();
	if (iTopLine < 0) iTopLine = 0;
	else if (iTopLine > iLineBufferSize) iTopLine = iLineBufferSize;

	int i;
	__ChatInfo** ppLineInfos = new __ChatInfo * [m_iChatLineCount];
	ZeroMemory(ppLineInfos, sizeof(__ChatInfo*) * m_iChatLineCount);

	int iCurLine = 0;
	for (i = 0; i < m_iChatLineCount; ++i)
	{
		iCurLine = iTopLine + i;
		if (iLineBufferSize <= iCurLine) break;
		ppLineInfos[i] = m_LineBuffer[iCurLine];
	}

	__ASSERT(m_ppUILines, "null pointer");
	int iRealLine = i;
	int iRealLineCount = 0;
	for (i = 0; i < iRealLine; ++i)
	{
		++iRealLineCount;
		if (NULL == m_ppUILines[i]) continue;
		m_ppUILines[i]->SetColor(ppLineInfos[i]->color);
		m_ppUILines[i]->SetString(ppLineInfos[i]->szChat);
	}
	for (i = iRealLineCount; i < m_iChatLineCount; ++i)
	{
		if (NULL == m_ppUILines[i]) continue;
		m_ppUILines[i]->SetString("");
	}
	delete[] ppLineInfos;
}
void CUIWhisperOpenDlg::ToggleOpenClose(bool opened)
{
	SetVisible(opened);
	m_pUIWhisperCloseDlg->SetVisible(!opened);
	m_pUIWhisperCloseDlg->SetBarState(UI_STATE_BUTTON_NORMAL);
	manager->RePosAll();
	if (opened)
	{
		SetFocus();
	}
}
void CUIWhisperOpenDlg::IncomingWhisper(std::string& szString)
{
	this->AddLineBuffer(szString, INCOMING_MSG_COLOR);
	this->AdjustScroll();
	m_pUIWhisperCloseDlg->SetBarState(UI_STATE_BUTTON_DOWN_2CHECKDOWN);
}
void CUIWhisperOpenDlg::AddLineBuffer(const std::string& szString, D3DCOLOR color)
{
	if (szString.empty()) return;

	__ASSERT(m_pExitChatString, "NULL UI Component !");
	const int iStrLen = szString.size();

	SIZE size;
	if (FALSE == m_pExitChatString->GetTextExtent(szString, iStrLen, &size)) { __ASSERT(0, "no device context"); return; }

	const int iRegionWidth = m_rcChatOutRegion.right - m_rcChatOutRegion.left;

	int iCX = 0;
	int iCount = 0;
	int iLineStart = 0;

	while (iCount < iStrLen)
	{
		if ('\n' == szString[iCount])		// \n
		{
			__ChatInfo* pLineInfo = new __ChatInfo;
			m_LineBuffer.push_back(pLineInfo);

			pLineInfo->color = color;
			if ((iCount - iLineStart) > 0)
			{
				int iLineLength = iCount - iLineStart + 1;
				std::string szLine;
				pLineInfo->szChat = szString.substr(iLineStart, iLineLength);
			}

			++iCount;
			iLineStart = iCount;
			iCX = 0;
		}
		else
		{
			int iCC = 0;
			if (0x80 & szString[iCount])	iCC = 2;	// 2BYTE
			else							iCC = 1;	// 1BYTE

			BOOL bFlag = m_pExitChatString->GetTextExtent(&(szString[iCount]), iCC, &size);
			__ASSERT(bFlag, "cannot get size of dfont");
			if ((iCX + size.cx) > iRegionWidth)
			{
				int iLineLength = iCount - iLineStart;
				if (iLineLength > 0)
				{
					__ChatInfo* pLineInfo = new __ChatInfo;
					m_LineBuffer.push_back(pLineInfo);

					pLineInfo->color = color;
					pLineInfo->szChat = szString.substr(iLineStart, iLineLength);
				}
				else
				{
					__ASSERT(iRegionWidth > 15, "너무 좁아서 한글자도 찍을 수가 없다");
					break;
				}
				iLineStart = iCount;
				iCX = 0;
			}
			iCount += iCC;
			iCX += size.cx;
		}
	}

	int iLineLength = iStrLen - iLineStart;
	if (iLineLength > 0)
	{
		__ChatInfo* pLineInfo = new __ChatInfo;
		m_LineBuffer.push_back(pLineInfo);

		pLineInfo->color = color;
		pLineInfo->szChat = szString.substr(iLineStart, iLineLength);
	}
}
void CUIWhisperOpenDlg::AdjustScroll()
{
	int iCurLinePos = m_pScrollBar->GetCurrentPos();
	BOOL bAutoScroll = (m_pScrollBar->GetMaxPos() == iCurLinePos) ? TRUE : FALSE;

	while (m_LineBuffer.size() > MAX_CHAT_LINES && 0 < iCurLinePos)
	{
		__ChatInfo* pTemp = m_LineBuffer.front();
		if (pTemp) delete pTemp;
		m_LineBuffer.pop_front();
		--iCurLinePos;
	}

	int iLineBufferSize = m_LineBuffer.size();
	int iMaxScrollPos = iLineBufferSize - m_iChatLineCount;
	if (iMaxScrollPos < 0) iMaxScrollPos = 0;
	m_pScrollBar->SetRange(0, iMaxScrollPos);

	if (bAutoScroll) iCurLinePos = iMaxScrollPos;
	if (iCurLinePos < 0) iCurLinePos = 0;

	m_pScrollBar->SetCurrentPos(iCurLinePos);

	SetTopLine(iCurLinePos);
}






// CUIWhisperCloseDlg (FOLDED)

CUIWhisperCloseDlg::CUIWhisperCloseDlg(CN3UIBase* buddy)
{
	m_pBtnOpen = NULL;
	m_pBtnBar = NULL;
	m_pExitIdString = NULL;
	m_pImage = NULL;
	m_buddy = (CUIWhisperOpenDlg*)buddy;
	m_bPositionChanged = false;
}

bool CUIWhisperCloseDlg::Load(HANDLE hFile)
{

	if (false == CN3UIBase::Load(hFile)) return false;
	m_pBtnOpen = GetChildByID("btn_open");								__ASSERT(m_pBtnOpen, "NULL UI Component!!");
	m_pBtnBar = GetChildByID("btn_bar");								__ASSERT(m_pBtnBar, "NULL UI Component!!");
	m_pExitIdString = (CN3UIString*)GetChildByID("exit_id");			__ASSERT(m_pExitIdString, "NULL UI Component!!");

	return true;
}

bool CUIWhisperCloseDlg::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (NULL == pSender) return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtnOpen)
		{
			if (m_buddy)
			{
				m_buddy->ToggleOpenClose(true);
			}
			return true;
		}
	}

	return false;
}

void CUIWhisperCloseDlg::SetTargetId(std::string targetId)
{
	if (m_pExitIdString)
		m_pExitIdString->SetString(targetId);
}

uint32_t  CUIWhisperCloseDlg::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;

	if (UI_STATE_COMMON_MOVE == m_eState)
	{
		if (dwFlags & UI_MOUSE_LBCLICKED)
		{
			SetState(UI_STATE_COMMON_NONE);
		}
		else
		{
			MoveOffset(ptCur.x - ptOld.x, ptCur.y - ptOld.y);
		}
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if (false == IsIn(ptCur.x, ptCur.y))
	{
		if (false == IsIn(ptOld.x, ptOld.y))
		{
			return dwRet;
		}
		dwRet |= UI_MOUSEPROC_PREVINREGION;
	}

	dwRet |= UI_MOUSEPROC_INREGION;

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		if (pChild == m_pBtnBar)
			continue;
		uint32_t dwChildRet = pChild->MouseProc(dwFlags, ptCur, ptOld);
		if (UI_MOUSEPROC_DONESOMETHING & dwChildRet)
		{	// 이경우에는 먼가 포커스를 받은 경우이다.
			// (아래 코드는 dialog를 관리하는 곳에서 해야 한다. 따라서 막아놓음)
			//			m_Children.erase(itor);			// 우선 리스트에서 지우고
			//			m_Children.push_front(pChild);	// 맨앞에 넣는다. 그리는 순서를 맨 나중에 그리도록 하려고

			dwRet |= (UI_MOUSEPROC_CHILDDONESOMETHING | UI_MOUSEPROC_DONESOMETHING);
			return dwRet;
		}
	}
	if (PtInRect(&m_rcMovable, ptCur))
	{
		dwRet |= UI_MOUSEPROC_DIALOGFOCUS;
	}

	if (UI_STATE_COMMON_MOVE != m_eState &&
		PtInRect(&m_rcMovable, ptCur) && (dwFlags & UI_MOUSE_LBCLICK))
	{
		SetState(UI_STATE_COMMON_MOVE);
		m_bPositionChanged = true;
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	return dwRet;
}
void CUIWhisperCloseDlg::SetBarState(eUI_STATE state)
{
	m_pBtnBar->SetState(state);
}


void CUIWhisperCloseDlg::Release()
{
	CN3UIBase::Release();
	m_pBtnOpen = NULL;
	m_pBtnBar = NULL;
	m_pExitIdString = NULL;
	m_pImage = NULL;
}