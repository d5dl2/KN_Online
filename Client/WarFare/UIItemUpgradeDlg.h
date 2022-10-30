#pragma once

#include "UIBaseInventory.h"

class CUIItemUpgradeDlg : public UIBaseInventory
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
	void				InitIconUpdate();
	virtual uint32_t	MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	__IconItemSkill*	GetHighlightIconItem(CN3UIIcon* pUIIcon);
	void				IconRestore();
	bool				ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur);
	void				SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
	void				LeaveAnvilState();
	bool				ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	void				Render();
	bool				Load(HANDLE hFile);
	e_UIWND_DISTRICT	GetWndDistrict(__IconItemSkill* spItem);
	bool				OnKeyPress(int iKey);
	int					GetItemiOrder(__IconItemSkill* spItem);
	__IconItemSkill*	m_pMyItemUpgradeInv[MAX_ITEM_INVENTORY];
	RECT				GetFirstEmptyUpgradeSlot();
	void				CancelUpgradeState();
	void				SendToServerFromItemUpgradeMsg();
	void				ReceiveResultItemUpgradeMsg(Packet& pkt);
	void				Tick();
	void				SetNpcId(uint16_t npcId);

	uint16_t npcId;
};

