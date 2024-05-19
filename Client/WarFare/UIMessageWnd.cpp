// UIMessageWnd.cpp: implementation of the CUIMessageWnd class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "GameProcMain.h"
#include "PacketDef.h"
#include "N3UIString.h"
#include "N3UIScrollBar.h"
#include "UIMessageWnd.h"
#include "UIManager.h"
#include "N3UIArea.h"
#include "N3UIImage.h"
#include "N3UIButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIMessageWnd::CUIMessageWnd()
{
	m_pChatOut = NULL;
	m_pScrollbar = NULL;
	m_pBtn_Fold = NULL;
	m_pArea_Resize = NULL;
	m_pImageBcgrnd = NULL;
	m_pBtnScrollBarLeftUp = NULL;
	m_pTrackBarScrollBarRight = NULL;
	m_iChatLineCount = 0;
	m_ppUILines = NULL;
	minHeight = 0;
	minWidth = 0;
	maxHeight = 0;
	maxWidth = 0;
	ZeroMemory(&m_rcChatOutRegion, sizeof(m_rcChatOutRegion));
}

CUIMessageWnd::~CUIMessageWnd()
{
	if (m_ppUILines) { delete[] m_ppUILines; m_ppUILines = NULL; }	// m_ppUILines[n]�� �����ʹ� �޸� �Ҵ�Ǿ� �־ �θ� �����ɶ� �ڵ����� �����ϹǷ� �������� �Ѵ�.

	ChatListItor itor;
	for (itor = m_ChatBuffer.begin(); m_ChatBuffer.end() != itor; ++itor)
	{
		__ChatInfo* pChatInfo = (*itor);
		if (pChatInfo) delete pChatInfo;
	}
	m_ChatBuffer.clear();

	for (itor = m_LineBuffer.begin(); m_LineBuffer.end() != itor; ++itor)
	{
		__ChatInfo* pChatInfo = (*itor);
		if (pChatInfo) delete pChatInfo;
	}
	m_LineBuffer.clear();
}

void CUIMessageWnd::Release()
{
	CN3UIBase::Release();

	m_pChatOut = NULL;
	m_pScrollbar = NULL;
	m_pBtn_Fold = NULL;
	m_pArea_Resize = NULL;
	m_pImageBcgrnd = NULL;
	m_pBtnScrollBarLeftUp = NULL;
	m_pTrackBarScrollBarRight = NULL;
	m_iChatLineCount = 0;
	if (m_ppUILines) { delete[] m_ppUILines; m_ppUILines = NULL; }	// m_ppUILines[n]�� �����ʹ� �޸� �Ҵ�Ǿ� �־ �θ� �����ɶ� �ڵ����� �����ϹǷ� �������� �Ѵ�.
	ZeroMemory(&m_rcChatOutRegion, sizeof(m_rcChatOutRegion));

	ChatListItor itor;
	for (itor = m_ChatBuffer.begin(); m_ChatBuffer.end() != itor; ++itor)
	{
		__ChatInfo* pChatInfo = (*itor);
		if (pChatInfo) delete pChatInfo;
	}
	m_ChatBuffer.clear();

	for (itor = m_LineBuffer.begin(); m_LineBuffer.end() != itor; ++itor)
	{
		__ChatInfo* pChatInfo = (*itor);
		if (pChatInfo) delete pChatInfo;
	}
	m_LineBuffer.clear();
}

BOOL CUIMessageWnd::MoveOffset(int iOffsetX, int iOffsetY)
{
	CN3UIBase::MoveOffset(iOffsetX, iOffsetY);

	auto dlg2 = CGameProcedure::s_pProcMain->m_pUIMsgDlg2;
	auto rc = GetRegion();
	auto rc2 = dlg2->GetRegion();
	CGameProcedure::s_pProcMain->m_pUIMsgDlg2->SetPos(rc.right - dlg2->GetWidth(), rc.bottom - dlg2->GetHeight());

	return TRUE;
}

bool CUIMessageWnd::Resize(int leftOffset, int topOffset)
{
	if (minWidth > m_rcRegion.right - (m_rcRegion.left + leftOffset) || 
		maxWidth < m_rcRegion.right - (m_rcRegion.left + leftOffset))
		leftOffset = 0;
	if (minHeight > m_rcRegion.bottom - (m_rcRegion.top + topOffset) ||
		maxHeight < m_rcRegion.bottom - (m_rcRegion.top + topOffset))
		topOffset = 0;

	if (0 == topOffset && 0 == leftOffset) return FALSE;

	m_rcRegion.left += leftOffset;
	m_rcRegion.top += topOffset;

	m_pArea_Resize->MoveOffset(leftOffset, topOffset);
	CustomResize(m_pImageBcgrnd, leftOffset, topOffset);
	CustomResize(m_pChatOut, leftOffset, topOffset);
	m_pBtnScrollBarLeftUp->MoveOffset(0, topOffset);
	CustomResize(m_pTrackBarScrollBarRight, 0, topOffset);


	m_rcChatOutRegion = m_pChatOut->GetRegion();
	m_rcChatOutRegion.left += m_pArea_Resize->GetWidth();

	CreateLines();	// ä�� ������ ���� ���� ����ϰ� �ٽ� �����.
	RecalcLineBuffer();	// ���� ���۸� �� ����� �ٽ� ������ְ� �۾��� ǥ���Ѵ�.

	return TRUE;
}

bool CUIMessageWnd::Load(HANDLE hFile)
{
	if (false == CN3UIBase::Load(hFile)) return false;
	m_pChatOut = (CN3UIString*)GetChildByID("text_message");								__ASSERT(m_pChatOut, "NULL UI Component!!");
	m_pScrollbar = (CN3UIScrollBar*)GetChildByID("scroll");									__ASSERT(m_pScrollbar, "NULL UI Component!!");
	m_pBtn_Fold = (CN3UIBase*)GetChildByID("btn_off");										__ASSERT(m_pBtn_Fold, "NULL UI Component!!");
	m_pArea_Resize = (CN3UIArea*)GetChildByID("area_resize");								__ASSERT(m_pArea_Resize, "NULL UI Component!!");
	m_pImageBcgrnd = (CN3UIImage*)GetChildByID("img_bckgrnd");								__ASSERT(m_pImageBcgrnd, "NULL UI Component!!");
	m_pBtnScrollBarLeftUp = (CN3UIButton*)m_pScrollbar->GetChildByID("btn_leftup");			__ASSERT(m_pBtnScrollBarLeftUp, "NULL UI Component!!");
	m_pTrackBarScrollBarRight = (CN3UITrackBar*)m_pScrollbar->GetChildByID("trckbr_right");	__ASSERT(m_pTrackBarScrollBarRight, "NULL UI Component!!");

	m_rcChatOutRegion = m_pChatOut->GetRegion();
	m_rcChatOutRegion.left += m_pArea_Resize->GetWidth();
	CreateLines();
	auto rect = GetRegion();
	minWidth = rect.right - rect.left;
	minHeight = rect.bottom - rect.top;
	maxWidth = 600;
	maxHeight = 800;
	__ASSERT(0 < m_iChatLineCount, "ä��â�� �ʹ� �۾ƿ�");

	return true;
}

bool CUIMessageWnd::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (NULL == pSender) return false;

	if (dwMsg == UIMSG_SCROLLBAR_POS)
	{
		// ��ũ�ѹٿ� �´� ä�� Line ����
		int iCurLinePos = m_pScrollbar->GetCurrentPos();
		SetTopLine(iCurLinePos);
		return true;
	}
	else if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtn_Fold)
		{
			CGameProcedure::s_pProcMain->CommandToggleUIMsgWnd();
			return true;
		}
	}

	return false;
}

void CUIMessageWnd::CreateLines()
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
	if (m_pChatOut && m_pChatOut->GetTextExtent("��", lstrlen("��"), &size) && size.cy > 0)
	{
		m_iChatLineCount = (m_rcChatOutRegion.bottom - m_rcChatOutRegion.top) / size.cy;
	}
	else return;
	const std::string& szFontName = m_pChatOut->GetFontName();
	uint32_t dwFontHeight = m_pChatOut->GetFontHeight();
	uint32_t dwFontFlag = m_pChatOut->GetFontFlags();

	if (m_iChatLineCount <= 0 || szFontName.size() <= 0) return;

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

void CUIMessageWnd::AddMsg(const std::string& szString, D3DCOLOR color)
{
	const int iStrLen = szString.size();
	__ASSERT(m_pScrollbar, "");
	if (0 >= iStrLen) return;

	// ChatBuffer�� �ֱ�
	__ChatInfo* pChatInfo = new __ChatInfo;
	pChatInfo->szChat = szString;
	pChatInfo->color = color;
	m_ChatBuffer.push_back(pChatInfo);

	if (m_ChatBuffer.size() > 255)	// 255���� ������ �տ������� �����
	{
		__ChatInfo* pTemp = m_ChatBuffer.front();
		if (pTemp) delete pTemp;

		m_ChatBuffer.pop_front();
	}

	// line buffer �� �ֱ�
	AddLineBuffer(szString, color);

	// Line buffer ���� ����
	int iCurLinePos = m_pScrollbar->GetCurrentPos();	// ���� scroll bar�� ����Ű�� �ִ� line
	BOOL bAutoScroll = (m_pScrollbar->GetMaxPos() == iCurLinePos) ? TRUE : FALSE;

	while (m_LineBuffer.size() > MAX_CHAT_LINES && 0 < iCurLinePos)	// MAX_CHAT_LINES�� �ִ� line�� �� (�� ��ũ�ѹٰ� 0�� ���� ������ line�� ������ �����Ƿ� 500���� �ѱ� �� �ִ�)
	{
		// ���� �����
		__ChatInfo* pTemp = m_LineBuffer.front();
		if (pTemp) delete pTemp;
		m_LineBuffer.pop_front();
		--iCurLinePos;
	}

	int iLineBufferSize = m_LineBuffer.size();
	int iMaxScrollPos = iLineBufferSize - m_iChatLineCount;
	if (iMaxScrollPos < 0) iMaxScrollPos = 0;
	m_pScrollbar->SetRange(0, iMaxScrollPos);	// scroll bar range ����

	// �ڵ����� ��ũ���̸�
	if (bAutoScroll) iCurLinePos = iMaxScrollPos;
	if (iCurLinePos < 0) iCurLinePos = 0;

	// ��ũ�ѹ� ���� ��ġ �缳��
	m_pScrollbar->SetCurrentPos(iCurLinePos);

	// ��ũ�ѹٿ� �´� ä�� Line ����
	SetTopLine(iCurLinePos);
}

uint32_t CUIMessageWnd::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	if (m_pArea_Resize->IsIn(ptCur.x, ptCur.y) && dwFlags & UI_MOUSE_LBDOWN)
		resizing = true;
	if (dwFlags & UI_MOUSE_LBCLICKED)
		resizing = false;

	if (resizing)
	{
		auto diffX = ptCur.x - ptOld.x;
		auto diffY = ptCur.y - ptOld.y;
		if (Resize(diffX, diffY))
			return UI_MOUSEPROC_DONESOMETHING;
	}

	return CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
}


void CUIMessageWnd::AddLineBuffer(const std::string& szString, D3DCOLOR color)
{
	if (szString.empty()) return;

	__ASSERT(m_pChatOut, "");
	const int iStrLen = szString.size();

	// line buffer �ֱ�
	SIZE size;
	if (FALSE == m_pChatOut->GetTextExtent(szString, iStrLen, &size)) { __ASSERT(0, "no device context"); return; }

	const int iRegionWidth = m_rcChatOutRegion.right - m_rcChatOutRegion.left;

	// ���� �ڸ��� �ڵ�, ���� ������ ����� ���ڴ� �ڸ��� �ؿ� �ٿ�..
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
			}	// ���ӵ� \n�� ��� pszLine = NULL�� �� �� �ִ�.

			++iCount;
			iLineStart = iCount;
			iCX = 0;
		}
		else
		{
			int iCC = 0;
			if (0x80 & szString[iCount])	iCC = 2;	// 2BYTE ����
			else							iCC = 1;	// 1BYTE ����

			BOOL bFlag = m_pChatOut->GetTextExtent(&(szString[iCount]), iCC, &size);
			__ASSERT(bFlag, "cannot get size of dfont");
			if ((iCX + size.cx) > iRegionWidth)	// ���� ���̰� �Ѿ�����
			{
				// �� ���� �� �߰��ϱ�

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
					__ASSERT(iRegionWidth > 15, "�ʹ� ���Ƽ� �ѱ��ڵ� ���� ���� ����");
					break;
				}
				iLineStart = iCount;
				iCX = 0;
			}
			// ���� ���ϱ�
			iCount += iCC;
			iCX += size.cx;
		}
	}

	// �� ������ �� ó��
	int iLineLength = iStrLen - iLineStart;
	if (iLineLength > 0)
	{
		__ChatInfo* pLineInfo = new __ChatInfo;
		m_LineBuffer.push_back(pLineInfo);

		pLineInfo->color = color;
		pLineInfo->szChat = szString.substr(iLineStart, iLineLength);
	}
}

void CUIMessageWnd::SetTopLine(int iTopLine)
{
	if (m_iChatLineCount <= 0) return;

	const int iLineBufferSize = m_LineBuffer.size();
	if (iTopLine < 0) iTopLine = 0;
	else if (iTopLine > iLineBufferSize) iTopLine = iLineBufferSize;

	int i;
	// ���ټ����� ���ʷ� �ӽù��ۿ� �����ϰ� string ���� ����
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
	// �տ������� �°� ���ʷ� ���� ���ۿ� �ֱ�
	int iRealLine = i;	// ���� ��µǴ� �� ��
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
		m_ppUILines[i]->SetString("");	// �������� ��ĭ �����
	}
	delete[] ppLineInfos;
}

void CUIMessageWnd::RecalcLineBuffer()	// ä��â ����� �������� ȣ�����ָ� line buffer�� �ٽ� ����ؼ� �־��ش�.
{
	// line buffer �ʱ�ȭ�ϱ�
	ChatListItor itor;
	for (itor = m_LineBuffer.begin(); m_LineBuffer.end() != itor; ++itor)
	{
		__ChatInfo* pLineBuff = (*itor);
		if (pLineBuff) delete pLineBuff;
	}
	m_LineBuffer.clear();

	// Line buffer �ٽ� �ֱ�
	for (itor = m_ChatBuffer.begin(); m_ChatBuffer.end() != itor; ++itor)
	{
		__ChatInfo* pChatBuff = (*itor);
		if (pChatBuff) AddLineBuffer(pChatBuff->szChat, pChatBuff->color);
	}

	// Line buffer ���� ����
	while (m_LineBuffer.size() > MAX_CHAT_LINES)	// MAX_CHAT_LINES�� �ִ� line�� ��
	{
		// ���� �����
		__ChatInfo* pLineBuff = m_LineBuffer.front();
		if (pLineBuff) delete pLineBuff;
		m_LineBuffer.pop_front();
	}

	int iLineBufferSize = m_LineBuffer.size();
	int iMaxScrollPos = iLineBufferSize - m_iChatLineCount;
	if (iMaxScrollPos < 0) iMaxScrollPos = 0;
	m_pScrollbar->SetRange(0, iMaxScrollPos);	// scroll bar range ����

	// ��ũ�ѹ� ���� ��ġ �缳��
	m_pScrollbar->SetCurrentPos(iMaxScrollPos);

	// ��ũ�ѹٿ� �´� ä�� Line ����
	SetTopLine(iMaxScrollPos);
}

void CUIMessageWnd::SetRegion(const RECT& Rect)
{
	CN3UIBase::SetRegion(Rect);
	// �ڽĵ��� ������ ��ġ�Ѵ�.
	// m_rcChatOutRegion = ;	// ä�� ��� ������ �ٽ� �������ش�.
	//CreateLines();	// ä�� ������ ���� ���� ����ϰ� �ٽ� �����.
	//RecalcLineBuffer();	// ���� ���۸� �� ����� �ٽ� ������ְ� �۾��� ǥ���Ѵ�.
}


bool CUIMessageWnd::OnKeyPress(int iKey)
{
	switch (iKey)
	{
	case DIK_ESCAPE:
	{	//hotkey�� ��Ŀ�� ������������ �ٸ� ui�� ������ �����Ƿ� DIK_ESCAPE�� ������ ��Ŀ���� �ٽ����
		//�����ִ� �ٸ� �����̸� �ݾ��ش�.
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
		CN3UIBase* pFocus = CGameProcedure::s_pUIMgr->GetFocusedUI();
		if (pFocus && pFocus != this) pFocus->OnKeyPress(iKey);
	}
	return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

// UI MESSAGE WND 2 (FOLDED)

CUIMessageWnd2::CUIMessageWnd2()
{
	m_pBtn_Fold = NULL;
}

bool CUIMessageWnd2::Load(HANDLE hFile)
{
	if (false == CN3UIBase::Load(hFile)) return false;
	m_pBtn_Fold = GetChildByID("btn_on");			__ASSERT(m_pBtn_Fold, "NULL UI Component!!");
	return true;
}

bool CUIMessageWnd2::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (NULL == pSender) return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtn_Fold)
		{
			CGameProcedure::s_pProcMain->CommandToggleUIMsgWnd();
			return true;
		}
	}

	return false;
}

void CUIMessageWnd2::Release()
{
	CN3UIBase::Release();
	m_pBtn_Fold = NULL;
}