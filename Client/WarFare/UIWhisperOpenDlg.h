// CUIWhisperOpenDlg.h: interface for the CUIWhisperOpenDlg class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


#include "N3UIBase.h"

class CUIWhisperCloseDlg : public CN3UIBase
{
protected:
	CN3UIBase*		m_pBtnOpen;
	CN3UIBase*		m_pBtnBar;
	CN3UIString*	m_pExitIdString;
	CN3UIImage*		m_pImage;
private:
	CN3UIBase*		m_buddy;
public:
	virtual bool	ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	virtual void	Release();
	virtual bool	Load(HANDLE hFile);

	void			SetTargetId(std::string targetId);
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

	bool			m_bKillFocus;
	bool			m_bHasFocus;
	bool			m_bPositionChanged;
public:

	virtual bool		Load(HANDLE hFile);
	virtual uint32_t	MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	virtual void		Release();
	virtual bool		ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	virtual BOOL		MoveOffset(int iOffsetX, int iOffsetY);
	virtual void		Init(CN3UIBase* pParent);
	virtual void		SetPos(int x, int y);
	virtual bool		OnKeyPress(int iKey);

	void			SetEnableKillFocus(bool bKillFocus) { m_bKillFocus = bKillFocus; }
	bool			LoadCloseDlgFromFile(std::string FN);
	bool			DefaultPositionChanged() { return m_bPositionChanged; };
	void			SetString(const std::string& szChat);
	void			SetFocus();
	void			KillFocus();
	void			SetTargetId(std::string& targetId);
	void			RemoveCloseDlgFromUIMgr();
	void			ToggleOpenClose(bool opened);
	CUIWhisperOpenDlg();
	virtual ~CUIWhisperOpenDlg();
};



