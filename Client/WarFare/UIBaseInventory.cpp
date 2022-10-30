#include "UIBaseInventory.h"
#include "UIImageTooltipDlg.h"
#include "PlayerMySelf.h"
#include "LocalInput.h"

void UIBaseInventory::AcceptIconDrop(__IconItemSkill* spItem)
{
	AllHighLightIconFree();
	SetState(UI_STATE_COMMON_NONE);
}

void UIBaseInventory::CancelIconDrop(__IconItemSkill* spItem) 
{
	AllHighLightIconFree();
	SetState(UI_STATE_COMMON_NONE);
}

void UIBaseInventory::InitIconWnd(e_UIWND eWnd) 
{
	__TABLE_UI_RESRC* pTbl = CGameBase::s_pTbl_UI.Find(CGameBase::s_pPlayer->m_InfoBase.eNation);

	m_pUITooltipDlg = new CUIImageTooltipDlg();
	m_pUITooltipDlg->Init(this);
	m_pUITooltipDlg->LoadFromFile(pTbl->szItemInfo);
	m_pUITooltipDlg->InitPos();
	m_pUITooltipDlg->SetVisible(FALSE);

	CN3UIWndBase::InitIconWnd(eWnd);
}

CN3UIArea* UIBaseInventory::GetChildAreaByiOrder(eUI_AREA_TYPE eUAT, int iOrder, const char* nameFormat)
{
	char pszID[32];
	sprintf(pszID, nameFormat, iOrder);

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIArea* pChild = (CN3UIArea*)(*itor);
		if ((pChild->UIType() == UI_TYPE_AREA) && (pChild->m_eAreaType == eUAT))
		{
			if (pChild->m_szID == pszID) return pChild;
		}
	}

	return NULL;
}

RECT UIBaseInventory::GetSampleRect()
{
	RECT rect;
	CN3UIArea* pArea;
	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
	pArea = GetChildAreaByiOrder(UI_AREA_TYPE_INV, 0, "a_slot_%d");
	rect = pArea->GetRegion();
	float fWidth = (float)(rect.right - rect.left);
	float fHeight = (float)(rect.bottom - rect.top);
	fWidth *= 0.5f; fHeight *= 0.5f;
	rect.left = ptCur.x - (int)fWidth;  rect.right = ptCur.x + (int)fWidth;
	rect.top = ptCur.y - (int)fHeight; rect.bottom = ptCur.y + (int)fHeight;
	return rect;
}