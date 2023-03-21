#include "UIDisguiseRing.h"
#include "LocalInput.h"
#include "GameBase.h"
#include "N3UIList.h"

std::string resource;

CUIDisguiseRing::CUIDisguiseRing()
{
	m_pList_Group = NULL;
	m_pList_Monster = NULL;
}

CUIDisguiseRing::~CUIDisguiseRing()
{

}

bool CUIDisguiseRing::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;

	m_pList_Group = (CN3UIList*)this->GetChildByID("list_group");		__ASSERT(m_pList_Group, "NULL UI Component!!!");
	m_pList_Monster = (CN3UIList*)this->GetChildByID("list_monster");  __ASSERT(m_pList_Monster, "NULL UI Component!!!");
	::_LoadStringFromResource(10350, resource);

	FillLists();
	return true;
}

void CUIDisguiseRing::FillLists()
{
	uint16_t current = 0;
	for (size_t i = 0; i < CGameBase::s_pTbl_Disguise_Ring.GetSize(); i++)
	{
		TABLE_DISGUISE_RING* current = CGameBase::s_pTbl_Disguise_Ring.GetIndexedData(i);
		if (current->byGroupId != 3)
			continue;

		char buff[50];	
		sprintf(buff, resource.c_str(), current->iLevelRequired);
		if (m_pList_Group->GetChildStrFromList(buff) != NULL)
			break;

		m_pList_Group->AddString(buff);
		//m_mapMonsters[current]
	}
}

bool CUIDisguiseRing::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	return CN3UIBase::ReceiveMessage(pSender, dwMsg);
}

bool CUIDisguiseRing::OnKeyPress(int iChar)
{
	switch (iChar)
	{
	case DIK_ESCAPE:
		SetVisible(false);
		return true;
	case DIK_RETURN:
	case DIK_NUMPADENTER:
		/*uint8_t iSel = m_pList_Cmds->GetCurSel();
		ExecuteCommand(iSel);*/
		return true;
	}

	return CN3UIBase::OnKeyPress(iChar);
}