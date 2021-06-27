// CWhisperManager.h: interface for the CWhisperManager class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <map>
#include "UIWhisperOpenDlg.h"

class CWhisperManager
{
private:
	typedef std::vector<std::pair<std::string, CUIWhisperOpenDlg*>>	WhisperVec;
	POINT												GetPosForNewDlg();
	POINT												m_DefaultPos;
	WhisperVec::iterator FindDlg(std::string ssenderId);
protected:
	WhisperVec	m_Whispers;

	void CreateNewDlg(std::string targetId, std::string msg);
	
	std::string szOpenChat;
	std::string szCloseChat;
public:

	void DeleteDlg(std::string targetId);
	void IncomingMessage(std::string senderId, std::string msg);
	void NewConnection(std::string targetId);
	void SetFileNames(std::string openChat, std::string closeChat);
	void SetDefaultPos(int x, int y) { m_DefaultPos.x = x; m_DefaultPos.y = y; };
	void RePosAll();
	CWhisperManager();
	~CWhisperManager();
};

