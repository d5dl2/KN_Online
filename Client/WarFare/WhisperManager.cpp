#include "WhisperManager.h"
#include "GameProcedure.h"
#include "UIManager.h"


CWhisperManager::CWhisperManager()
{
	m_Whispers = std::vector<std::pair<std::string, CUIWhisperOpenDlg*>>();
}


CWhisperManager::~CWhisperManager()
{
	WhisperVec::iterator itr = m_Whispers.begin();
	for (; itr != m_Whispers.end(); itr++)
	{
		if(itr->second)
			delete itr->second;		
	}
	m_Whispers.clear();
}

CWhisperManager::WhisperVec::iterator CWhisperManager::FindDlg(std::string senderId)
{
	return std::find_if(m_Whispers.begin(), m_Whispers.end(),
		[senderId](const std::pair<std::string, CUIWhisperOpenDlg*>& element) { return element.first == senderId; });
}

void CWhisperManager::IncomingMessage(std::string senderId, std::string msg)
{
	if (m_Whispers.size() != 0)
	{
		auto itr = FindDlg(senderId);

		if (itr != m_Whispers.end())
		{
			itr->second->IncomingWhisper(msg);
			return;
		}
	}

	CreateNewDlg(senderId, msg);

}

void CWhisperManager::SetFileNames(std::string openChat, std::string closeChat)
{
	szOpenChat = openChat;
	szCloseChat = closeChat;
}

void CWhisperManager::CreateNewDlg(std::string targetId, std::string msg)
{
	CUIWhisperOpenDlg* temp = new CUIWhisperOpenDlg();
	m_Whispers.push_back(std::make_pair(targetId, temp));
	temp->manager = this;
	temp->Init(CGameProcedure::s_pUIMgr);
	temp->LoadFromFile(szOpenChat);
	temp->LoadCloseDlgFromFile(szCloseChat);
	POINT pos = GetPosForNewDlg();
	temp->SetCloseDialogPos(pos.x, pos.y);
	temp->SetPos(m_DefaultPos.x, m_DefaultPos.y);
	temp->SetTargetId(targetId);
	temp->ToggleOpenClose(false);
	temp->IncomingWhisper(msg);

}

void CWhisperManager::DeleteDlg(std::string targetId)
{
	WhisperVec::iterator itr = FindDlg(targetId);
	if (itr != m_Whispers.end())
	{
		CUIWhisperOpenDlg* temp = itr->second;
		CGameProcedure::s_pUIMgr->RemoveChild(temp);
		temp->RemoveCloseDlgFromUIMgr();

		m_Whispers.erase(itr);
		delete temp;
		CGameProcedure::s_pUIMgr->ReFocusUI();
	}
}

POINT CWhisperManager::GetPosForNewDlg()
{
	POINT topMost = m_DefaultPos;
	WhisperVec::iterator itr = m_Whispers.begin();
	for (; itr != m_Whispers.end(); itr++)
	{
		CUIWhisperOpenDlg* temp = itr->second;
		if (temp && !temp->DefaultPositionChanged())
		{
			POINT tempPos = temp->GetCloseDialogPos();
			if (tempPos.y < topMost.y)
			{
				topMost = tempPos;
			}
		}
	}
	topMost.y -= 20;
	return topMost;
}

void CWhisperManager::RePosAll()
{
	POINT first{ 0 , 0 };
	WhisperVec::iterator itr = m_Whispers.begin();
	for (; itr != m_Whispers.end(); itr++)
	{
		CUIWhisperOpenDlg* temp = itr->second;
		if (temp && !temp->DefaultPositionChanged() && !temp->IsVisible())
		{
			POINT tempPos = temp->GetCloseDialogPos();

			if (first.y == 0)
			{
				temp->SetCloseDialogPos(m_DefaultPos.x, m_DefaultPos.y - 20);
				first = temp->GetCloseDialogPos();
			}
			else
			{
				POINT target = first;
				target.y -= 20;
				temp->SetCloseDialogPos(target.x, target.y);
				first = target;
			}
		}
	}
}