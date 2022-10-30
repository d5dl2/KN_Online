#pragma once

#include "N3UIWndBase.h"

class UIBaseInventory : public CN3UIWndBase
{
public:
	void				AcceptIconDrop(__IconItemSkill* spItem);
	void				CancelIconDrop(__IconItemSkill* spItem);
	void				InitIconWnd(e_UIWND eWnd);
	CN3UIArea*			GetChildAreaByiOrder(eUI_AREA_TYPE eUAT, int iOrder, const char* nameFormat);
	RECT				GetSampleRect();

	CUIImageTooltipDlg* m_pUITooltipDlg;
};

