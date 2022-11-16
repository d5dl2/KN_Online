#pragma once

#include "N3UIBase.h"

class CUIDisguiseRing : public CN3UIBase
{
protected:
	CN3UIList* m_pList_Group;
	CN3UIList* m_pList_Monster;
	std::map<uint16_t, std::string> m_mapMonsters;
	void FillLists();

public:
	CUIDisguiseRing();
	virtual ~CUIDisguiseRing();

	virtual bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	virtual bool OnKeyPress(int iChar);
	virtual bool Load(HANDLE hFile);
};

