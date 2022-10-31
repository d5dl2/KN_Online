#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"
#include "N3UIBase.h"

//////////////////////////////////////////////////////////////////////

class CUIAnvil : public CN3UIBase
{
public:
	CUIAnvil();
	virtual ~CUIAnvil();

	virtual bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	virtual void SetVisible(bool bVisible);
	virtual bool OnKeyPress(int iChar);
	void		SetNpcId(uint16_t npcId);

	void OpenItemUpgradeDlg();

	uint16_t npcId;
};
