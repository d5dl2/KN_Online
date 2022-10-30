#include "UIItemUpgradeDlg.h"
#include "GameProcMain.h"
#include "UIInventory.h"
#include "UIImageTooltipDlg.h"
#include "PlayerMySelf.h"
#include "UISkillTreeDlg.h"
#include "UIHotKeyDlg.h"
#include "LocalInput.h"
#include "UIManager.h"
#include "shared\APISocket.h"
#include "PacketDef.h"
#include "resource.h"


#define FAIL_CODE {		\
				SetState(UI_STATE_COMMON_NONE);	\
				return false;	\
			}

#define FAIL_RETURN {	\
		CN3UIWndBase::AllHighLightIconFree();	\
		SetState(UI_STATE_COMMON_NONE);	\
		return false;	\
	}

const int MAX_UPGRADE_SLOT = 9;
const int IMAGE_RESULT_COUNT = 19;
const float fUVAspect = (float)45.0f / (float)64.0f;
int8_t UpgradeState = -1;
CN3UIImage* failedImageList[IMAGE_RESULT_COUNT];
CN3UIImage* succeedImageList[IMAGE_RESULT_COUNT];
size_t currentImageIndex = 0;
CN3UIImage* imageCover1;
CN3UIImage* imageCover2;
CN3UIImage* imgResult;
CN3UIImage* imgUpgrade;
CN3UIButton* m_pBtnClose;
CN3UIButton* m_pBtnOk;
CN3UIButton* m_pBtnCancel;
CN3UIButton* m_pBtnConversation;
CN3UIArea* m_pAreaUpgrade;
CN3UIArea* m_pAreaResult;
CN3UIArea* m_pAreaSlot[MAX_UPGRADE_SLOT];

__InfoSelectedIcon upgradeItem;
__InfoSelectedIcon upgradeSlots[MAX_UPGRADE_SLOT];

TABLE_ITEM_BASIC* trinasPiece;

CUIItemUpgradeDlg::CUIItemUpgradeDlg()
{
	m_pUITooltipDlg = NULL;
	trinasPiece = NULL;
	npcId = -1;
	int i;
	for (i = 0; i < MAX_ITEM_INVENTORY; i++)	m_pMyItemUpgradeInv[i] = NULL;
	for (i = 0; i < MAX_UPGRADE_SLOT; i++) upgradeSlots[i].pItemSelect = NULL;
	this->SetVisible(false);
}

CUIItemUpgradeDlg::~CUIItemUpgradeDlg()
{
	Release();
}

void CUIItemUpgradeDlg::Release()
{
	CN3UIBase::Release();

	int i;
	for (i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		if (m_pMyItemUpgradeInv[i] != NULL)
		{
			delete m_pMyItemUpgradeInv[i];
			m_pMyItemUpgradeInv[i] = NULL;
		}
	}
}

bool CUIItemUpgradeDlg::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;
	char sz[32];

	for (size_t i = 0; i < IMAGE_RESULT_COUNT; i++)
	{
		sprintf(sz, "img_f_load_%d", i);
		failedImageList[i] = (CN3UIImage*)(this->GetChildByID(sz));			__ASSERT(failedImageList[i], "NULL UI Component!!");

		sprintf(sz, "img_s_load_%d", i);
		succeedImageList[i] = (CN3UIImage*)(this->GetChildByID(sz));			__ASSERT(succeedImageList[i], "NULL UI Component!!");

		failedImageList[i]->SetVisible(false);
		succeedImageList[i]->SetVisible(false);
	}

	for (size_t i = 0; i < 9; i++)
	{
		sprintf(sz, "a_m_%d", i);
		m_pAreaSlot[i] = (CN3UIArea*)(this->GetChildByID(sz));			__ASSERT(m_pAreaSlot[i], "NULL UI Component!!");
	}

	imageCover1 = (CN3UIImage*)(this->GetChildByID("img_cover_01"));			__ASSERT(imageCover1, "NULL UI Component!!");
	imageCover2 = (CN3UIImage*)(this->GetChildByID("img_cover_02"));			__ASSERT(imageCover2, "NULL UI Component!!");
	imgUpgrade = (CN3UIImage*)(this->GetChildByID("img_upgrade"));			__ASSERT(imgUpgrade, "NULL UI Component!!");
	imgResult = (CN3UIImage*)(this->GetChildByID("img_result"));			__ASSERT(imgResult, "NULL UI Component!!");
	imageCover1->SetVisible(false);
	imageCover2->SetVisible(false);

	m_pBtnClose = (CN3UIButton*)(this->GetChildByID("btn_close"));				__ASSERT(m_pBtnClose, "NULL UI Component!!");
	m_pBtnOk = (CN3UIButton*)(this->GetChildByID("btn_ok"));				__ASSERT(m_pBtnOk, "NULL UI Component!!");
	m_pBtnCancel = (CN3UIButton*)(this->GetChildByID("btn_cancel"));				__ASSERT(m_pBtnCancel, "NULL UI Component!!");
	m_pBtnConversation = (CN3UIButton*)(this->GetChildByID("btn_conversation"));				__ASSERT(m_pBtnConversation, "NULL UI Component!!");
	m_pAreaUpgrade = (CN3UIArea*)(this->GetChildByID("a_upgrade"));				__ASSERT(m_pAreaUpgrade, "NULL UI Component!!");
	m_pAreaResult = (CN3UIArea*)(this->GetChildByID("a_result"));				__ASSERT(m_pAreaResult, "NULL UI Component!!");

	trinasPiece = CGameBase::s_pTbl_Items_Basic.Find(Trina / 1000 * 1000);
	return true;
}

void EndAnimation()
{
	UpgradeState = -1;
	currentImageIndex = 0;
	imageCover1->SetVisible(false);
	imageCover2->SetVisible(false);
	imgResult->SetVisible(true);
	imgUpgrade->SetVisible(true);
	if (upgradeItem.pItemSelect)
		upgradeItem.pItemSelect->pUIIcon->SetVisible(true);

	for (size_t i = 0; i < IMAGE_RESULT_COUNT; i++)
	{
		failedImageList[i]->SetVisible(false);
		succeedImageList[i]->SetVisible(false);
	}
}

void AnimateImageList()
{
	static float time = 0;
	time += CN3Base::s_fSecPerFrm;

	if (currentImageIndex >= IMAGE_RESULT_COUNT)
	{
		EndAnimation();
	}

	if (time > 0.08 && (UpgradeState == UpgradeFailed || UpgradeState == UpgradeSucceeded))
	{
		if (UpgradeState == UpgradeFailed)
			failedImageList[currentImageIndex]->SetVisible(true);
		else if (UpgradeState == UpgradeSucceeded)
			succeedImageList[currentImageIndex]->SetVisible(true);

		if (currentImageIndex > 1)
		{
			succeedImageList[currentImageIndex - 1]->SetVisible(false);
			failedImageList[currentImageIndex - 1]->SetVisible(false);
		}

		currentImageIndex++;
		time = 0;
	}
}

void CUIItemUpgradeDlg::Tick()
{
	AnimateImageList();

	CN3UIBase::Tick();
}

void CUIItemUpgradeDlg::Render()
{
	if (!m_bVisible) return;	// º¸ÀÌÁö ¾ÊÀ¸¸é ÀÚ½ÄµéÀ» renderÇÏÁö ¾Ê´Â´Ù.

	int i;

	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
	m_pUITooltipDlg->DisplayTooltipsDisable();

	bool bTooltipRender = false;
	__IconItemSkill* spItem = NULL;

	for (UIListReverseItor itor = m_Children.rbegin(); m_Children.rend() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);

		if ((GetState() == UI_STATE_ICON_MOVING) && (pChild->UIType() == UI_TYPE_ICON) && (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) &&
			((CN3UIIcon*)pChild == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon))	continue;

		pChild->Render();
		if ((GetState() == UI_STATE_COMMON_NONE) &&
			(pChild->UIType() == UI_TYPE_ICON) && (pChild->GetStyle() & UISTYLE_ICON_HIGHLIGHT))
		{
			bTooltipRender = true;
			spItem = GetHighlightIconItem((CN3UIIcon*)pChild);
		}
	}

	// sayılabilir itemlerin sayılarının render edilmesi
	for (i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		if (m_pMyItemUpgradeInv[i] && ((m_pMyItemUpgradeInv[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE)
			|| (m_pMyItemUpgradeInv[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)))
		{
			// string ¾ò±â..
			CN3UIString* pStr = GetChildStringByiOrder(i, "s_count_%d");
			if (pStr)
			{
				if ((GetState() == UI_STATE_ICON_MOVING) && (m_pMyItemUpgradeInv[i] == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect))
				{
					pStr->SetVisible(false);
				}
				else
				{
					if (m_pMyItemUpgradeInv[i]->pUIIcon->IsVisible())
					{
						pStr->SetVisible(true);
						pStr->SetStringAsInt(m_pMyItemUpgradeInv[i]->iCount);
						pStr->Render();
					}
					else
					{
						pStr->SetVisible(false);
					}
				}
			}
		}
		else
		{
			// string ¾ò±â..
			CN3UIString* pStr = GetChildStringByiOrder(i, "s_count_%d");
			if (pStr)
				pStr->SetVisible(false);
		}
	}

	if ((GetState() == UI_STATE_ICON_MOVING) && (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect))
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->Render();

	if (bTooltipRender && spItem)
		m_pUITooltipDlg->DisplayTooltipsEnable(ptCur.x, ptCur.y, spItem);
}


void CUIItemUpgradeDlg::EnterItemUpgradeStateStart()
{
	int i;
	for (i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		if (m_pMyItemUpgradeInv[i] != NULL)
		{
			if (m_pMyItemUpgradeInv[i]->pUIIcon)
			{
				RemoveChild(m_pMyItemUpgradeInv[i]->pUIIcon);
				m_pMyItemUpgradeInv[i]->pUIIcon->Release();
				delete m_pMyItemUpgradeInv[i]->pUIIcon;
				m_pMyItemUpgradeInv[i]->pUIIcon = NULL;
			}
			delete m_pMyItemUpgradeInv[i];
			m_pMyItemUpgradeInv[i] = NULL;
		}
	}
	UpgradeState = -1;
}

void CUIItemUpgradeDlg::EnterItemUpgradeStateEnd()
{
	ItemMoveFromInvToThis();
}

void CUIItemUpgradeDlg::ItemMoveFromInvToThis()
{
	CUIInventory* pInven = CGameProcedure::s_pProcMain->m_pUIInventory;
	if (!pInven) return;

	int i;
	for (i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		m_pMyItemUpgradeInv[i] = NULL;
	}

	for (i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		if (pInven->m_pMyInvWnd[i])
		{
			__IconItemSkill* spItem = pInven->m_pMyInvWnd[i];
			spItem->pUIIcon->SetParent(this);

			pInven->m_pMyInvWnd[i] = NULL;
			CN3UIArea* pArea;

			pArea = GetChildAreaByiOrder(UI_AREA_TYPE_INV, i, "a_slot_%d");
			if (pArea)
			{
				spItem->pUIIcon->SetRegion(pArea->GetRegion());
				spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
			}

			m_pMyItemUpgradeInv[i] = spItem;
		}
	}
}

void CUIItemUpgradeDlg::ItemMoveFromThisToInv()
{
	CUIInventory* pInven = CGameProcedure::s_pProcMain->m_pUIInventory;
	if (!pInven) return;

	CancelUpgradeState();

	int i;
	for (i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		if (m_pMyItemUpgradeInv[i])
		{
			__IconItemSkill* spItem = m_pMyItemUpgradeInv[i];
			spItem->pUIIcon->SetParent(pInven);

			m_pMyItemUpgradeInv[i] = NULL;

			CN3UIArea* pArea;

			pArea = pInven->GetChildAreaByiOrder(UI_AREA_TYPE_INV, i);
			if (pArea)
			{
				spItem->pUIIcon->SetRegion(pArea->GetRegion());
				spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
			}

			pInven->m_pMyInvWnd[i] = spItem;
		}
	}
}


int CUIItemUpgradeDlg::GetItemiOrder(__IconItemSkill* spItem)
{
	int iReturn = -1;
	int i;

	for (i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		if ((m_pMyItemUpgradeInv[i] != NULL) && (m_pMyItemUpgradeInv[i] == spItem))
			return i;
	}
	return iReturn;
}

uint32_t		CUIItemUpgradeDlg::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld) {
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;
	if (CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer) { dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);  return dwRet; }

	// µå·¡±× µÇ´Â ¾ÆÀÌÄÜ °»½Å..
	if ((GetState() == UI_STATE_ICON_MOVING) &&
		(CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd == UIWND_ANVIL))
	{
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetRegion(GetSampleRect());
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetMoveRect(GetSampleRect());
	}

	return CN3UIWndBase::MouseProc(dwFlags, ptCur, ptOld);
}

__IconItemSkill* CUIItemUpgradeDlg::GetHighlightIconItem(CN3UIIcon* pUIIcon) {
	int i;

	for (i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		if ((m_pMyItemUpgradeInv[i] != NULL) && (m_pMyItemUpgradeInv[i]->pUIIcon == pUIIcon))
			return m_pMyItemUpgradeInv[i];
	}

	if (upgradeItem.pItemSelect && upgradeItem.pItemSelect->pUIIcon == pUIIcon)
	{
		return upgradeItem.pItemSelect;
	}

	for (i = 0; i < MAX_UPGRADE_SLOT; i++)
	{
		if (upgradeSlots[i].pItemSelect != NULL && upgradeSlots[i].pItemSelect->pUIIcon == pUIIcon)
			return upgradeSlots[i].pItemSelect;
	}

	return NULL;
}

void				CUIItemUpgradeDlg::IconRestore() {

	if (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict == UIWND_DISTRICT_TRADE_MY)
	{
		if (m_pMyItemUpgradeInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder] != NULL)
		{
			CN3UIArea* pArea = GetChildAreaByiOrder(UI_AREA_TYPE_INV, CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder, "a_slot_%d");
			if (pArea)
			{
				m_pMyItemUpgradeInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetRegion(pArea->GetRegion());
				m_pMyItemUpgradeInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetMoveRect(pArea->GetRegion());
			}
		}
	}
}

void CUIItemUpgradeDlg::CancelUpgradeState()
{
	if (upgradeItem.pItemSelect)
	{
		m_pMyItemUpgradeInv[upgradeItem.UIWndSelect.iOrder] = upgradeItem.pItemSelect;
		upgradeItem.pItemSelect->pUIIcon->SetVisible(true);

		CN3UIArea* pArea = GetChildAreaByiOrder(UI_AREA_TYPE_INV, upgradeItem.UIWndSelect.iOrder, "a_slot_%d");
		if (pArea)
		{
			upgradeItem.pItemSelect->pUIIcon->SetRegion(pArea->GetRegion());
			upgradeItem.pItemSelect->pUIIcon->SetMoveRect(pArea->GetRegion());
		}
		upgradeItem.pItemSelect = NULL;
	}

	for (size_t i = 0; i < MAX_UPGRADE_SLOT; i++)
	{
		bool bFound = false;
		if (upgradeSlots[i].pItemSelect)
		{
			for (size_t j = 0; j < MAX_ITEM_INVENTORY; j++)
			{
				if (m_pMyItemUpgradeInv[j])
				{
					if (m_pMyItemUpgradeInv[j]->pItemBasic == upgradeSlots[i].pItemSelect->pItemBasic)
					{
						bFound = true;
						break;
					}
				}
			}

			if (((upgradeSlots[i].pItemSelect->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
				(upgradeSlots[i].pItemSelect->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) &&
				bFound)
			{
				m_pMyItemUpgradeInv[upgradeSlots[i].UIWndSelect.iOrder]->iCount++;
				RemoveChild(upgradeSlots[i].pItemSelect->pUIIcon);
				delete upgradeSlots[i].pItemSelect;
				upgradeSlots[i].pItemSelect = NULL;
			}
			else
			{
				m_pMyItemUpgradeInv[upgradeSlots[i].UIWndSelect.iOrder] = upgradeSlots[i].pItemSelect;

				CN3UIArea* pArea = GetChildAreaByiOrder(UI_AREA_TYPE_INV, upgradeSlots[i].UIWndSelect.iOrder, "a_slot_%d");
				if (pArea)
				{
					upgradeSlots[i].pItemSelect->pUIIcon->SetRegion(pArea->GetRegion());
					upgradeSlots[i].pItemSelect->pUIIcon->SetMoveRect(pArea->GetRegion());
				}

				upgradeSlots[i].pItemSelect = NULL;
			}
		}
	}

	EndAnimation();
}

bool CUIItemUpgradeDlg::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (NULL == pSender || UpgradeState != -1) return false;
	int i, j;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtnClose)
			LeaveAnvilState();
		else if (pSender == m_pBtnCancel)
			CancelUpgradeState();
		else if (pSender == m_pBtnOk)
		{
			SendToServerFromItemUpgradeMsg();
		}
	}

	__IconItemSkill* spItem = NULL;
	e_UIWND_DISTRICT eUIWnd;
	int iOrder;

	uint32_t dwBitMask = 0x0f0f0000;

	switch (dwMsg & dwBitMask)
	{
	case UIMSG_ICON_DOWN_FIRST:
		CN3UIWndBase::AllHighLightIconFree();
		spItem = GetHighlightIconItem((CN3UIIcon*)pSender);

		if (spItem->pItemExt->byMagicOrRare != 4 && spItem->pItemExt->byMagicOrRare != 5 &&
			spItem->pItemBasic->dwEffectID2 != 255)
		{
			CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = NULL;
			CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd = UIWND_UNKNOWN;
			return false;
		}

		// Save Select Info..
		CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd = UIWND_ANVIL;
		eUIWnd = GetWndDistrict(spItem);
		if (eUIWnd == UIWND_DISTRICT_UNKNOWN) FAIL_CODE;

		CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict = eUIWnd;
		iOrder = GetItemiOrder(spItem);
		if (iOrder == -1)	FAIL_CODE;
		CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder = iOrder;
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = spItem;
		// Do Ops..
		((CN3UIIcon*)pSender)->SetRegion(GetSampleRect());
		((CN3UIIcon*)pSender)->SetMoveRect(GetSampleRect());
		// Sound..
		if (spItem) PlayItemSound(spItem->pItemBasic);
		break;

	case UIMSG_ICON_UP:
		// ¾ÆÀÌÄÜ ¸Å´ÏÀú À©µµ¿ìµéÀ» µ¹¾Æ ´Ù´Ï¸é¼­ °Ë»ç..
		if (!CGameProcedure::s_pUIMgr->BroadcastIconDropMsg(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect))
			// ¾ÆÀÌÄÜ À§Ä¡ ¿ø·¡´ë·Î..
			IconRestore();
		// Sound..
		if (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) PlayItemSound(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic);
		break;

	case UIMSG_ICON_DOWN:
		if (GetState() == UI_STATE_ICON_MOVING && CN3UIWndBase::m_sSelectedIconInfo.pItemSelect)
		{
			CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetRegion(GetSampleRect());
			CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetMoveRect(GetSampleRect());
		}
		break;

	case UIMSG_ICON_RDOWN_FIRST:
	{
		CN3UIWndBase::AllHighLightIconFree();

		spItem = GetHighlightIconItem((CN3UIIcon*)pSender);

		if (spItem->pItemExt->byMagicOrRare != 4 && spItem->pItemExt->byMagicOrRare != 5 &&
			spItem->pItemBasic->dwEffectID2 != 255)
		{
			CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = NULL;
			CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd = UIWND_UNKNOWN;
			return false;
		}

		CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd = UIWND_ANVIL;
		eUIWnd = GetWndDistrict(spItem);
		if (eUIWnd == UIWND_DISTRICT_UNKNOWN)	FAIL_CODE;

		CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict = eUIWnd;
		iOrder = GetItemiOrder(spItem);
		if (iOrder == -1)	FAIL_CODE;
		CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder = iOrder;
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = spItem;
		((CN3UIIcon*)pSender)->SetRegion(GetSampleRect());

		RECT destRect;
		if (spItem->pItemBasic->dwEffectID2 == 255)
			destRect = GetFirstEmptyUpgradeSlot();
		else
			destRect = m_pAreaUpgrade->GetRegion();

		((CN3UIIcon*)pSender)->SetRegion(GetSampleRect());
		((CN3UIIcon*)pSender)->SetMoveRect(destRect);
		long x = destRect.left + ((destRect.right - destRect.left) / 2);
		long y = destRect.top + ((destRect.bottom - destRect.top) / 2);

		if (!CGameProcedure::s_pUIMgr->BroadcastIconDropWithRBMsg(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect, x, y))
			// ¾ÆÀÌÄÜ À§Ä¡ ¿ø·¡´ë·Î..
			IconRestore();

		if (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) PlayItemSound(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic);
		break;
	}
	}

	return true;
}

bool CUIItemUpgradeDlg::ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur) {

	CN3UIArea* pArea;
	e_UIWND_DISTRICT eUIWnd = UIWND_DISTRICT_UNKNOWN;
	if (!m_bVisible) return false;

	if (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd != m_eUIWnd)
		FAIL_RETURN;
	if ((CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict != UIWND_DISTRICT_TRADE_MY))
		FAIL_RETURN;

	int i, iDestiOrder = -1; bool bFound = false, scroolAllreadyAdded = false;

	if (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic->dwEffectID2 == 255) // trina ve scroll grubu
	{
		for (i = 0; i < 9; i++)
		{
			if (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic == trinasPiece) // eklenmek istenen item trina ise
			{
				if (upgradeSlots[i].pItemSelect && upgradeSlots[i].pItemSelect->pItemBasic == trinasPiece) // slotlardan birinde zaten trina var ise
				{
					scroolAllreadyAdded = true;
					break;
				}
			}
			else // eklenmek istenen item scroll ise
			{
				if (upgradeSlots[i].pItemSelect && upgradeSlots[i].pItemSelect->pItemBasic != trinasPiece) // slotlardan birinde scroll var ise
				{
					scroolAllreadyAdded = true;
					break;
				}
			}
		}

		if (!scroolAllreadyAdded)
		{
			for (i = 0; i < 9; i++)
			{
				pArea = m_pAreaSlot[i];
				if ((pArea) && (pArea->IsIn(ptCur.x, ptCur.y)))
				{
					bFound = true;
					eUIWnd = UIWND_DISTRICT_UPGRADE_SLOT;
					iDestiOrder = i;
					break;
				}
			}
		}
	}
	else if (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemExt->byMagicOrRare == 4 ||
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemExt->byMagicOrRare == 5)
	{
		if (m_pAreaUpgrade->IsIn(ptCur.x, ptCur.y))
		{
			bFound = true;
			eUIWnd = UIWND_DISTRICT_UPGRADE_ITEM;
		}
	}

	if (!bFound)	FAIL_RETURN;

	if (spItem != CN3UIWndBase::m_sSelectedIconInfo.pItemSelect)
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = spItem;

	if (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict == UIWND_DISTRICT_TRADE_MY)
		if (eUIWnd == UIWND_DISTRICT_UPGRADE_SLOT)
		{
			if (((CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
				(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) &&
				CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->iCount > 1)
			{
				if (upgradeSlots[iDestiOrder].pItemSelect)
					FAIL_RETURN;
				__IconItemSkill* spItemNew;
				spItemNew = new __IconItemSkill;
				spItemNew->pItemBasic = spItem->pItemBasic;
				spItemNew->pItemExt = spItem->pItemExt;
				spItemNew->szIconFN = spItem->szIconFN;
				spItemNew->iCount = 1;
				spItemNew->iDurability = spItem->iDurability;
				spItemNew->pUIIcon = new CN3UIIcon;
				spItemNew->pUIIcon->Init(this);
				spItemNew->pUIIcon->SetTex(spItem->szIconFN);
				spItemNew->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
				spItemNew->pUIIcon->SetUIType(UI_TYPE_ICON);
				spItemNew->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
				spItemNew->pUIIcon->SetVisible(true);

				upgradeSlots[iDestiOrder] = CN3UIWndBase::m_sSelectedIconInfo;
				upgradeSlots[iDestiOrder].pItemSelect = spItemNew;

				if (m_pAreaSlot[iDestiOrder])
				{
					upgradeSlots[iDestiOrder].pItemSelect->pUIIcon->SetRegion(m_pAreaSlot[iDestiOrder]->GetRegion());
					upgradeSlots[iDestiOrder].pItemSelect->pUIIcon->SetMoveRect(m_pAreaSlot[iDestiOrder]->GetRegion());
				}

				m_pMyItemUpgradeInv[upgradeSlots[iDestiOrder].UIWndSelect.iOrder]->iCount--;
				FAIL_RETURN;
			}
			else
			{
				if (upgradeSlots[iDestiOrder].pItemSelect)
					FAIL_RETURN;

				upgradeSlots[iDestiOrder] = CN3UIWndBase::m_sSelectedIconInfo;

				if (m_pAreaSlot[iDestiOrder])
				{
					upgradeSlots[iDestiOrder].pItemSelect->pUIIcon->SetRegion(m_pAreaSlot[iDestiOrder]->GetRegion());
					upgradeSlots[iDestiOrder].pItemSelect->pUIIcon->SetMoveRect(m_pAreaSlot[iDestiOrder]->GetRegion());
				}

				m_pMyItemUpgradeInv[upgradeSlots[iDestiOrder].UIWndSelect.iOrder] = NULL;
				FAIL_RETURN;
			}
		}
		else
		{
			if (upgradeItem.pItemSelect)
				FAIL_RETURN;

			upgradeItem = CN3UIWndBase::m_sSelectedIconInfo;

			if (m_pAreaUpgrade)
			{
				upgradeItem.pItemSelect->pUIIcon->SetRegion(m_pAreaUpgrade->GetRegion());
				upgradeItem.pItemSelect->pUIIcon->SetMoveRect(m_pAreaUpgrade->GetRegion());
			}

			m_pMyItemUpgradeInv[upgradeItem.UIWndSelect.iOrder] = NULL;
			FAIL_RETURN;
		}
	FAIL_RETURN;
}

RECT CUIItemUpgradeDlg::GetFirstEmptyUpgradeSlot()
{
	int i = 0;
	for (; i < MAX_UPGRADE_SLOT; i++)
	{
		if (upgradeSlots[i].pItemSelect == NULL)
			break;
	}

	return GetChildAreaByiOrder(UI_AREA_TYPE_SLOT, i, "a_m_%d")->GetRegion();
}


void CUIItemUpgradeDlg::SetVisibleWithNoSound(bool bVisible, bool bWork, bool bReFocus)
{
	CN3UIBase::SetVisibleWithNoSound(bVisible, bWork, bReFocus);

	if (bWork)
	{
		LeaveAnvilState();
	}
}

void CUIItemUpgradeDlg::LeaveAnvilState()
{
	if (IsVisible())
		SetVisible(false);

	if (GetState() == UI_STATE_ICON_MOVING)
		IconRestore();
	SetState(UI_STATE_COMMON_NONE);
	CN3UIWndBase::AllHighLightIconFree();

	// ÀÌ À©µµ¿ìÀÇ inv ¿µ¿ªÀÇ ¾ÆÀÌÅÛÀ» ÀÌ ÀÎº¥Åä¸® À©µµ¿ìÀÇ inv¿µ¿ªÀ¸·Î ¿Å±ä´Ù..	
	ItemMoveFromThisToInv();

	if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
	if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
}



void				CUIItemUpgradeDlg::InitIconUpdate()
{

}

e_UIWND_DISTRICT CUIItemUpgradeDlg::GetWndDistrict(__IconItemSkill* spItem)
{
	for (int i = 0; i < MAX_ITEM_INVENTORY; i++)
	{
		if ((m_pMyItemUpgradeInv[i] != NULL) && (m_pMyItemUpgradeInv[i] == spItem))
			return UIWND_DISTRICT_TRADE_MY;
	}
	return UIWND_DISTRICT_UNKNOWN;
}

bool CUIItemUpgradeDlg::OnKeyPress(int iKey)
{
	switch (iKey)
	{
	case DIK_ESCAPE:
		ReceiveMessage(m_pBtnClose, UIMSG_BUTTON_CLICK);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUIItemUpgradeDlg::SendToServerFromItemUpgradeMsg()
{
	bool bFound = false;
	for (size_t i = 0; i < MAX_UPGRADE_SLOT; i++)
	{
		if (upgradeSlots[i].pItemSelect)
		{
			bFound = true;
			break;
		}
	}

	if (!(upgradeItem.pItemSelect && bFound))
		return;

	imgResult->SetVisible(false);
	imgUpgrade->SetVisible(false);
	imageCover1->SetVisible(true);
	imageCover2->SetVisible(true);

	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;
	uint8_t byBuff[512];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_ITEM_UPGRADE);
	CAPISocket::MP_AddByte(byBuff, iOffset, ITEM_UPGRADE);
	CAPISocket::MP_AddWord(byBuff, iOffset, npcId);
	CAPISocket::MP_AddDword(byBuff, iOffset, upgradeItem.pItemSelect->pItemBasic->dwID + upgradeItem.pItemSelect->pItemExt->dwID);
	CAPISocket::MP_AddByte(byBuff, iOffset, upgradeItem.UIWndSelect.iOrder);
	
	// scrollar nereye konursa konsun ilk iki slotta gönder
	size_t i, scrollCount = 0;
	for (i = 0; i < MAX_UPGRADE_SLOT; i++)
	{
		__InfoSelectedIcon current = upgradeSlots[i];

		if (current.pItemSelect)
		{
			CAPISocket::MP_AddDword(byBuff, iOffset, current.pItemSelect->pItemBasic->dwID + current.pItemSelect->pItemExt->dwID);
			CAPISocket::MP_AddByte(byBuff, iOffset, current.UIWndSelect.iOrder);
			scrollCount++;
		}
	}
	for (i = scrollCount; i < MAX_UPGRADE_SLOT; i++)
	{
		CAPISocket::MP_AddDword(byBuff, iOffset, -1);
		CAPISocket::MP_AddByte(byBuff, iOffset, -1);
	}

	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIItemUpgradeDlg::ReceiveResultItemUpgradeMsg(Packet& pkt)
{
	int8_t resultItemPos;
	int32_t resultItemId;
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
	pkt >> UpgradeState;
	pkt >> resultItemId >> resultItemPos;

	e_PartPosition ePart;
	e_PlugPosition ePlug;
	e_ItemType eType;
	std::string szIconFN;

	TABLE_ITEM_BASIC* itemBasic;
	TABLE_ITEM_EXT* itemExt;
	std::string szMsg;
	const unsigned int errColor = 0xff9900ff;
	const unsigned int okColor = 0xff6666ff;
	switch (UpgradeState)
	{
	case UpgradeFailed:
		RemoveChild(upgradeItem.pItemSelect->pUIIcon);
		delete upgradeItem.pItemSelect;
		upgradeItem.pItemSelect = NULL;

		for (size_t i = 0; i < MAX_UPGRADE_SLOT; i++)
		{
			if (upgradeSlots[i].pItemSelect)
			{
				RemoveChild(upgradeSlots[i].pItemSelect->pUIIcon);
				delete upgradeSlots[i].pItemSelect;
				upgradeSlots[i].pItemSelect = NULL;
			}
		}

		::_LoadStringFromResource(IDS_UPGRADE_FAILED, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, errColor);

		break;
	case UpgradeSucceeded:
		for (size_t i = 0; i < MAX_UPGRADE_SLOT; i++)
		{
			if (upgradeSlots[i].pItemSelect)
			{
				RemoveChild(upgradeSlots[i].pItemSelect->pUIIcon);
				delete upgradeSlots[i].pItemSelect;
				upgradeSlots[i].pItemSelect = NULL;
			}
		}


		itemBasic = CGameBase::s_pTbl_Items_Basic.Find(resultItemId / 1000 * 1000);

		if (itemBasic && itemBasic->byExtIndex >= 0 && itemBasic->byExtIndex < MAX_ITEM_EXTENSION)
			itemExt = CGameBase::s_pTbl_Items_Exts[itemBasic->byExtIndex].Find(resultItemId % 1000);	// ¿­ µ¥ÀÌÅÍ ¾ò±â..
		else
			itemExt = NULL;
		eType = CGameProcedure::MakeResrcFileNameForUPC(itemBasic, NULL, &szIconFN, ePart, ePlug, CGameBase::s_pPlayer->m_InfoBase.eRace); // ¾ÆÀÌÅÛ¿¡ µû¸¥ ÆÄÀÏ ÀÌ¸§À» ¸¸µé¾î¼­
		if (ITEM_TYPE_UNKNOWN == eType) return;

		__IconItemSkill* spItemNew;
		spItemNew = new __IconItemSkill;
		spItemNew->pItemBasic = itemBasic;
		spItemNew->pItemExt = itemExt;
		spItemNew->szIconFN = szIconFN;
		spItemNew->iCount = 1;
		spItemNew->iDurability = upgradeItem.pItemSelect->iDurability;
		spItemNew->pUIIcon = new CN3UIIcon;
		spItemNew->pUIIcon->Init(this);
		spItemNew->pUIIcon->SetTex(szIconFN);
		spItemNew->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
		spItemNew->pUIIcon->SetUIType(UI_TYPE_ICON);
		spItemNew->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
		spItemNew->pUIIcon->SetVisible(false);

		RemoveChild(upgradeItem.pItemSelect->pUIIcon);
		delete upgradeItem.pItemSelect;

		if (m_pAreaResult)
		{
			spItemNew->pUIIcon->SetRegion(m_pAreaResult->GetRegion());
			spItemNew->pUIIcon->SetMoveRect(m_pAreaResult->GetRegion());
		}

		upgradeItem.pItemSelect = spItemNew;

		::_LoadStringFromResource(IDS_UPGRADE_SUCCEEDED, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, okColor);
		break;
	case UpgradeTrading:
	case UpgradeRental:
		::_LoadStringFromResource(IDS_UPGRADE_CANNOT_PERFORM, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, errColor);
		EndAnimation();
		break;
	case UpgradeNeedCoins:
		::_LoadStringFromResource(IDS_UPGRADE_NOT_ENOUGH_COINS, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, errColor);
		EndAnimation();
		break;
	case UpgradeNoMatch:
		::_LoadStringFromResource(IDS_UPGRADE_NOT_MATCH, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, errColor);
		EndAnimation();
		break;
	default:
		CGameProcedure::s_pProcMain->MsgOutput("Upgrade Result Unknown: " + UpgradeState, 0xffffff00);
		EndAnimation();
		break;
	}
}

void CUIItemUpgradeDlg::SetNpcId(uint16_t npcId)
{
	this->npcId = npcId;
}