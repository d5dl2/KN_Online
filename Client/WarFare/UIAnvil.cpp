// UINPCEvent.cpp: implementation of the UINPCEvent class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "resource.h"
#include "UIAnvil.h"

#include "GameProcMain.h"
#include "GameProcedure.h"
#include "PacketDef.h"
#include "shared\APISocket.h"
#include "PlayerMySelf.h"
#include "UITradeBBSSelector.h"
#include "UIManager.h"

CUIAnvil::CUIAnvil()
{
}

CUIAnvil::~CUIAnvil()
{
}



bool CUIAnvil::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender->m_szID == "upgrade_1")	//Ã¢°íÀÌ¿ë..
		{
			MsgSend_OpenItemUpgrade();
			SetVisible(false);
			return true;
		}

		if (pSender->m_szID == "upgrade_2")
		{
			// takı upgrade aç
		}

		if (pSender->m_szID == "btn_close")
		{
			SetVisible(false);
			return true;
		}
	}
	return true;
}


void CUIAnvil::MsgSend_OpenItemUpgrade()
{
	uint8_t byBuff[4];		// ÆÐÅ¶ ¹öÆÛ..
	int iOffset = 0;		// ÆÐÅ¶ ¿ÀÇÁ¼Â..

	CGameProcedure::s_pProcMain->ItemUpgradeOpen(this->npcId);	
}

void CUIAnvil::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if (bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}

bool CUIAnvil::OnKeyPress(int iChar)
{
	switch (iChar)
	{
	case DIK_ESCAPE:
		SetVisible(false);
		return true;
	}

	return CN3UIBase::OnKeyPress(iChar);
}

void CUIAnvil::SetNpcId(uint16_t npcId)
{
	this->npcId = npcId;
}