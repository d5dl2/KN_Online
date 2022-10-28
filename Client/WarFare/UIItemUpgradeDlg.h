#pragma once

#include "N3UIWndBase.h"

class CUIItemUpgradeDlg : public CN3UIWndBase
{
	friend class CUIInventory;
public:
	CUIItemUpgradeDlg();
	virtual				~CUIItemUpgradeDlg();
	void				Release();
	void				EnterItemUpgradeStateStart();
	void				EnterItemUpgradeStateEnd();
	void				ItemMoveFromInvToThis();
	void				ItemMoveFromThisToInv();
	CN3UIArea*			GetChildAreaByiOrder(eUI_AREA_TYPE eUAT, int iOrder, const char* nameFormat);
	void				InitIconWnd(e_UIWND eWnd);
	void				InitIconUpdate();
	virtual uint32_t	MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	__IconItemSkill*	GetHighlightIconItem(CN3UIIcon* pUIIcon);
	void				IconRestore();
	bool				ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur);
	void				CancelIconDrop(__IconItemSkill* spItem);
	void				AcceptIconDrop(__IconItemSkill* spItem);
	void				SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
	void				LeaveAnvilState();
	bool				ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	RECT				GetSampleRect();
	void				Render();
	CN3UIString*		GetChildStringByiOrder(int iOrder);
	bool				Load(HANDLE hFile);
	e_UIWND_DISTRICT	GetWndDistrict(__IconItemSkill* spItem);
	bool				OnKeyPress(int iKey);
	int					GetItemiOrder(__IconItemSkill* spItem);
	__IconItemSkill*	m_pMyItemUpgradeInv[MAX_ITEM_INVENTORY];
	CUIImageTooltipDlg* m_pUITooltipDlg;
	RECT				GetFirstEmptyUpgradeSlot();
	void				CancelUpgradeState();
	void				SendToServerFromItemUpgradeMsg();
	void				ReceiveResultItemUpgradeMsg(Packet& pkt);
	void				Tick();
	void				SetNpcId(uint16_t npcId);

	uint16_t npcId;
};

