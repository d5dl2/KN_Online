#include "WhisperManager.h"
#include "GameProcedure.h"
#include "UIManager.h"


CWhisperManager::CWhisperManager()
{
	m_Whispers = std::map<std::string, CUIWhisperOpenDlg*>();
}


CWhisperManager::~CWhisperManager()
{
}

void CWhisperManager::IncomingMessage(std::string senderId, std::string msg)
{
	if (m_Whispers.size() != 0)
	{
		WhisperMap::iterator itr = m_Whispers.find(senderId);
		if (itr != m_Whispers.end())
		{
			return;
		}
	}

	CreateNewDlg(senderId);

}
void CWhisperManager::NewConnection(std::string targetId)
{

}

void CWhisperManager::SetFileNames(std::string openChat, std::string closeChat)
{
	szOpenChat = openChat;
	szCloseChat = closeChat;
}

void CWhisperManager::CreateNewDlg(std::string targetId)
{
	CUIWhisperOpenDlg* temp = new CUIWhisperOpenDlg();
	temp->Init(CGameProcedure::s_pUIMgr);
	temp->LoadFromFile(szOpenChat);
	temp->LoadCloseDlgFromFile(szCloseChat);
	POINT pos = GetPosForNewDlg();
	temp->SetPos(pos.x, pos.y);
	temp->SetTargetId(targetId);
	temp->ToggleOpenClose(true);
	m_Whispers.insert(std::make_pair(targetId, temp));
}

void CWhisperManager::DeleteDlg(std::string targetId)
{
	WhisperMap::iterator itr = m_Whispers.find(targetId);
	if (itr != m_Whispers.end())
	{
		CUIWhisperOpenDlg* temp = itr->second;		
		CGameProcedure::s_pUIMgr->RemoveChild(temp);	
		temp->RemoveCloseDlgFromUIMgr();
		CGameProcedure::s_pUIMgr->ReFocusUI();
		
		m_Whispers.erase(targetId);

		if(temp)
			delete temp;
	}
}

POINT CWhisperManager::GetPosForNewDlg()
{
	POINT topMost = m_DefaultPos;
	WhisperMap::iterator itr = m_Whispers.begin();
	for (; itr != m_Whispers.end(); itr++)
	{
		CUIWhisperOpenDlg* temp = itr->second;
		if (temp)
		{			
			if (!temp->DefaultPositionChanged())
			{
				POINT tempPos = temp->GetPos();
				if (tempPos.y < topMost.y)
				{
					topMost = tempPos;
				}
			}
		}
	}
	topMost.y -= 20;
	return topMost;
}