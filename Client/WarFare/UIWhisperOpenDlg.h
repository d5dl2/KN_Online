// CUIWhisperOpenDlg.h: interface for the CUIWhisperOpenDlg class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


#include "N3UIBase.h"
#include "UIChat.h"
#include "N3UIScrollBar.h"

#define INCOMING_MSG_COLOR D3DCOLOR_ARGB(255,192,192,0)
#define SELF_MSG_COLOR D3DCOLOR_ARGB(255, 128, 255, 255)

class CUIWhisperCloseDlg : public CN3UIBase
{
protected:
	CN3UIBase*		m_pBtnOpen;
	CN3UIBase*		m_pBtnBar;
	CN3UIString*	m_pExitIdString;
	CN3UIImage*		m_pImage;
	bool			m_bPositionChanged;
private:
	class CUIWhisperOpenDlg*		m_buddy;
public:
	virtual bool	ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	virtual void	Release();
	virtual bool	Load(HANDLE hFile);
	virtual uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	void			SetTargetId(std::string targetId);
	void			SetBarState(eUI_STATE state);
	void			SetButtonState(eUI_STATE state);
	bool			DefaultPositionChanged() { return m_bPositionChanged; };
	CUIWhisperCloseDlg(CN3UIBase* buddy);
};


class CUIWhisperOpenDlg : public CN3UIBase
{
protected:
	CN3UIBase*		m_pBtnChat;
	CN3UIString*	m_pExitIdString;
	std::string		m_szTargetId;
	CN3UIString*	m_pExitChatString;
	CN3UIBase*		m_pBtnBar;
	CN3UIBase*		m_pBtnHide;
	CN3UIEdit*		m_pEditChat;
	std::string		m_szEditString;

	CN3UIScrollBar* m_pScrollBar;
	CN3UIBase*		m_pBtnClose;
	CN3UIImage*		m_pImage;

	CUIWhisperCloseDlg* m_pUIWhisperCloseDlg;
	CN3UIString**	m_ppUILines;
	RECT			m_rcChatOutRegion;
	ChatList		m_LineBuffer;

	int				m_iChatLineCount;

	bool			m_bKillFocus;
	bool			m_bHasFocus;
public:
	class CWhisperManager* manager;

	virtual bool		Load(HANDLE hFile);
	virtual uint32_t	MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	virtual void		Release();
	virtual bool		ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	virtual BOOL		MoveOffset(int iOffsetX, int iOffsetY);
	virtual void		Init(CN3UIBase* pParent);
	virtual bool		OnKeyPress(int iKey);	

	void			SetEnableKillFocus(bool bKillFocus) { m_bKillFocus = bKillFocus; }
	bool			LoadCloseDlgFromFile(std::string FN);
	bool			DefaultPositionChanged() { return m_pUIWhisperCloseDlg->DefaultPositionChanged(); };
	void			SetString(const std::string& szChat);
	void			SetFocus();
	void			KillFocus();
	void			SetTargetId(std::string& targetId);
	void			RemoveCloseDlgFromUIMgr();
	void			ToggleOpenClose(bool opened);
	void			IncomingWhisper(std::string& msg);
	void			AddLineBuffer(const std::string& szString, D3DCOLOR color);
	void			CreateLines();
	void			SetTopLine(int iTopLine);
	void			AdjustScroll();
	void			SetCloseDialogPos(int x, int y);
	POINT			GetCloseDialogPos();
	CUIWhisperOpenDlg();
	virtual ~CUIWhisperOpenDlg();
};



