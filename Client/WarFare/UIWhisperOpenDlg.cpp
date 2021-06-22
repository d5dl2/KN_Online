#include "UIWhisperOpenDlg.h"

#include "N3UIEdit.h"
#include "N3UIString.h"
#include "GameProcedure.h"
#include "GameProcMain.h"
#include "WhisperManager.h"
#include "UIManager.h"
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

	m_pUIWhisperCloseDlg = new CUIWhisperCloseDlg(this);
	m_bHasFocus = false;
	m_bHasFocus = false;
	m_bPositionChanged = false;
}


CUIWhisperOpenDlg::~CUIWhisperOpenDlg()
{
}

void CUIWhisperOpenDlg::Release()
{
	CN3UIBase::Release();

	m_pBtnChat = NULL;
	m_pExitIdString = NULL;
	m_pExitChatString = NULL;
	m_pBtnBar = NULL;
	m_pBtnHide = NULL;
	m_pEditChat = NULL;
	m_pScrollBar = NULL;
	m_pBtnClose = NULL;
	m_pImage = NULL;

	m_pUIWhisperCloseDlg->Release();

}


bool CUIWhisperOpenDlg::Load(HANDLE hFile)
{
	if (false == CN3UIBase::Load(hFile)) return false;
	
	m_pBtnChat = GetChildByID("btn_chat");								__ASSERT(m_pBtnChat, "NULL UI Component!!");
	m_pExitIdString = (CN3UIString*)GetChildByID("exit_id");							__ASSERT(m_pExitIdString, "NULL UI Component!!");
	m_pExitChatString = (CN3UIString*)GetChildByID("exit_chat");							__ASSERT(m_pExitChatString, "NULL UI Component!!");
	m_pBtnBar = GetChildByID("btn_bar");							__ASSERT(m_pBtnBar, "NULL UI Component!!");
	m_pBtnHide = GetChildByID("btn_hide");							__ASSERT(m_pBtnHide, "NULL UI Component!!");
	m_pEditChat = (CN3UIEdit*)GetChildByID("edit_chat");							__ASSERT(m_pEditChat, "NULL UI Component!!");
	m_pScrollBar = (CN3UIScrollBar*)GetChildByID("scroll");							__ASSERT(m_pScrollBar, "NULL UI Component!!");
	m_pBtnClose = GetChildByID("btn_close");							__ASSERT(m_pBtnClose, "NULL UI Component!!");
	//m_pImage = 
	m_pEditChat->SetMaxString(32); // 채팅 문자열 길이 제한..
	
	
	
	/*m_pExitIdString->SetString("qwe");
	m_pExitChatString->SetString("asdasdasdasd");*/

	
	return true;
}
void CUIWhisperOpenDlg::Init(CN3UIBase* pParent)
{
	CN3UIBase::Init(pParent);
	m_pUIWhisperCloseDlg->SetParent(pParent);
}

void CUIWhisperOpenDlg::SetPos(int x, int y)
{
	CN3UIBase::SetPos(x, y);

	m_pUIWhisperCloseDlg->SetPos(x, y);
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
		if (dwFlags&UI_MOUSE_LBCLICKED)
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
		PtInRect(&m_rcMovable, ptCur) && (dwFlags&UI_MOUSE_LBCLICK))
	{
		SetState(UI_STATE_COMMON_MOVE);
		m_bPositionChanged = true;
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

	m_pUIWhisperCloseDlg->MoveOffset(iOffsetX, iOffsetY);
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
			this->SetVisible(false);
			m_pUIWhisperCloseDlg->SetVisible(true);
		}		
		else if (pSender == m_pBtnClose)
		{
			CGameProcedure::s_pProcMain->m_pWhisperMgr->DeleteDlg(m_szTargetId);
		}
		else if (pSender == m_pBtnHide)
		{
			ToggleOpenClose(false);
		}
	}
	else if (dwMsg == UIMSG_SCROLLBAR_POS)
	{
		return false;
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
		{	
			if (m_pEditChat)
			{
				if (m_pEditChat->HaveFocus())
				{
					SetString(m_pEditChat->GetString());
				}
				else
				{
				}
			}
		}
		return true;
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
}

void CUIWhisperOpenDlg::SetFocus()
{
	if (!m_bHasFocus)
	{
		if (m_pBtnBar)
			m_pBtnBar->SetState(UI_STATE_BUTTON_ON);

		//SetEnableKillFocus(false);


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
	//m_szEditString = szChat;
	if (m_pExitChatString)
	{
		m_pExitChatString->SetString(m_szEditString);
		
	}
}

void CUIWhisperOpenDlg::ToggleOpenClose(bool opened)
{
	SetVisible(opened);
	m_pUIWhisperCloseDlg->SetVisible(!opened);

	if (opened)
		SetFocus();
}


// CUIWhisperCloseDlg (FOLDED)

CUIWhisperCloseDlg::CUIWhisperCloseDlg(CN3UIBase* buddy)
{
	m_pBtnOpen = NULL;
	m_pBtnBar = NULL;
	m_pExitIdString = NULL;
	m_pImage = NULL;
	m_buddy = buddy;
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
				CUIWhisperOpenDlg* opened = (CUIWhisperOpenDlg*)m_buddy;
				opened->ToggleOpenClose(true);
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


void CUIWhisperCloseDlg::Release()
{
	CN3UIBase::Release();
	m_pBtnOpen = NULL;
	m_pBtnBar = NULL;
	m_pExitIdString = NULL;
	m_pImage = NULL;
}