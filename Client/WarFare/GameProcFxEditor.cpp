// GameProcMain.cpp: implementation of the CGameProcFxEditor class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <io.h>

//#include "stdafx.h"
#include "resource.h"

#include "GameEng.h"
#include "GameProcFxEditor.h"
#include "LocalInput.h"
#include "GameProcMain.h"

#include "shared\APISocket.h"
#include "PacketDef.h"

#include "PlayerMySelf.h"
#include "PlayerOtherMgr.h"
#include "ServerMesh.h"
#include "N3FXMgr.h"

#include "UIManager.h"
#include "UILoading.h"
#include "UIChat.h"
#include "UIInventory.h"
#include "UICmd.h"
#include "UIVarious.h"
#include "UIStateBar.h"
#include "UITargetBar.h"
#include "UITransactionDlg.h"
#include "UIHelp.h"
#include "UIMessageWnd.h"
#include "UINotice.h"
#include "UIDroppedItemDlg.h"
#include "UIPerTradeDlg.h"
#include "UIPartyOrForce.h"
#include "UISkillTreeDlg.h"
#include "UICmdList.h"
#include "UICmdEdit.h"
#include "UIHotKeyDlg.h"
#include "UIClassChange.h"
#include "UINpcEvent.h"
#include "UIItemExchange.h"
#include "UIRepairTooltipDlg.h"
#include "UINpcTalk.h"
#include "UIKnightsOperation.h"
#include "UIPartyBBS.h"
#include "UIWareHouseDlg.h"
#include "UINPCChangeEvent.h"
#include "UIWarp.h"
#include "UIInn.h"
#include "UICreateClanName.h"
#include "UIPartyBBS.h"
#include "UITradeSellBBS.h"
#include "UITradeBBSSelector.h"
#include "UITradeBBSEditDlg.h"
#include "UIQuestMenu.h"
#include "UIQuestTalk.h"
#include "UIDead.h"
#include "WhisperManager.h"
#include "SubProcPerTrade.h"
#include "CountableItemEditDlg.h"
#include "MagicSkillMng.h"
#include "WarMessage.h"
#include "GameCursor.h"
#include "UIAnvil.h"
#include "UIItemUpgradeDlg.h"
#include "UIDisguiseRing.h"

#include "N3WorldManager.h"

#include "LightMgr.h"

#include "N3SkyMng.h"
#include "N3ShapeExtra.h"
#include "N3Camera.h"
#include "N3SndObj.h"
#include "N3SndObjStream.h"
#include "N3SndMgr.h"

enum e_ChatCmd
{
	CMD_WHISPER, CMD_TOWN, CMD_EXIT, CMD_GREETING, CMD_GREETING2, CMD_GREETING3,
	CMD_PROVOKE, CMD_PROVOKE2, CMD_PROVOKE3, CMD_GAME_SAVE, CMD_RECOMMEND, CMD_INDIVIDUAL_BATTLE,
	CMDSIT_STAND, CMD_WALK_RUN, CMD_LOCATION,

	CMD_TRADE, CMD_FORBIDTRADE, CMD_PERMITTRADE, CMD_MERCHANT,

	CMD_PARTY, CMD_LEAVEPARTY, CMD_RECRUITPARTY, CMD_FORBIDPARTY, CMD_PERMITPARTY,

	CMD_JOINCLAN, CMD_WITHDRAWCLAN, CMD_FIRECLAN, CMD_COMMAND, CMD_CLAN_WAR,
	CMD_SURRENDER, CMD_APPOINTVICECHIEF, CMD_CLAN_CHAT, CMD_CLAN_BATTLE,

	CMD_CONFEDERACY, CMD_BAN_KNIGHTS, CMD_QUIT_KNIGHTS, CMD_BASE, CMD_DECLARATION,

	CMD_VISIBLE, CMD_INVISIBLE, CMD_CLEAN, CMD_RAINING, CMD_SNOWING, CMD_TIME, CMD_CU_COUNT,
	CMD_NOTICE, CMD_ARREST, CMD_FORBIDCONNECT, CMD_FORBIDCHAT, CMD_PERMITCHAT, CMD_NOTICEALL,
	CMD_CUTOFF, CMD_VIEW, CMD_UNVIEW, CMD_FORBIDUSER, CMD_SUMMONUSER,
	CMD_ATTACKDISABLE, CMD_ATTACKENABLE, CMD_PLC,

	CMD_COUNT,
	CMD_UNKNOWN = 0xffffffff
};
static std::string s_szCmdMsg[CMD_COUNT]; // °ÔÀÓ»ó ¸í·É¾î



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcFxEditor::CGameProcFxEditor()				// r±âº» »ý¼ºÀÚ.. °¢ º¯¼öÀÇ ¿ªÈ°Àº Çì´õ ÂüÁ¶..
{
	m_fLBClickTime = 0.0f;
	m_bLoadComplete = FALSE;
	m_fRequestGameSave = 300.0f;

	//sound obj...
	m_pSnd_Town = NULL;
	m_pSnd_Battle = NULL;

	m_iJoinReqClan = 0;
	m_iJoinReqClanRequierID = 0;

	//UI
	m_pUIMsgDlg = new CUIMessageWnd();
	m_pUIMsgDlg2 = new CUIMessageWnd2();
	m_pUIChatDlg = new CUIChat();
	m_pUIChatDlg2 = new CUIChat2();
	m_pUIStateBarAndMiniMap = new CUIStateBar();
	m_pUIVar = new CUIVarious();
	m_pUICmd = new CUICmd();
	m_pUITargetBar = new CUITargetBar();
	m_pUIHelp = new CUIHelp();
	m_pUINotice = new CUINotice();
	m_pUIClassChange = new CUIClassChange();
	m_pUINpcEvent = new CUINPCEvent();
	m_pUIItemREDlg = new CUIItemExchange();
	m_pUIRepairTooltip = new CUIRepairTooltipDlg();
	m_pUIDroppedItemDlg = new CUIDroppedItemDlg();
	m_pUITransactionDlg = new CUITransactionDlg();
	m_pUIInventory = new CUIInventory();
	m_pUIPartyOrForce = new CUIPartyOrForce();
	m_pUISkillTreeDlg = new CUISkillTreeDlg();
	m_pUICmdListDlg = new CUICmdList();
	m_pUICmdEditDlg = new CUICmdEdit();
	m_pUIHotKeyDlg = new CUIHotKeyDlg();
	m_pUINpcTalk = new CUINpcTalk();
	m_pUIKnightsOp = new CUIKnightsOperation();			// ±â»ç´Ü ¸®½ºÆ® º¸±â, °¡ÀÔ, µî...
	m_pUIPartyBBS = new CUIPartyBBS(); // ÆÄÆ¼ Áö¿ø ½Ã½ºÅÛ °Ô½ÃÆÇ??..
	m_pUIWareHouseDlg = new CUIWareHouseDlg();
	m_pUINpcChange = new CUINPCChangeEvent();
	m_pUIWarp = new CUIWarp();
	m_pUIInn = new CUIInn();
	m_pUICreateClanName = new CUICreateClanName();
	m_pUITradeBBS = new CUITradeSellBBS();
	m_pUITradeBBSSelector = new CUITradeBBSSelector();
	m_pUITradeBBSEdit = new CUITradeBBSEditDlg();
	m_pUIQuestMenu = new CUIQuestMenu();
	m_pUIQuestTalk = new CUIQuestTalk();
	m_pUIDead = new CUIDead();
	m_pUIAnvil = new CUIAnvil();
	m_pUIItemUpgradeDlg = new CUIItemUpgradeDlg();
	m_pUIDisguiseRingDlg = new CUIDisguiseRing();

	m_pSubProcPerTrade = new CSubProcPerTrade();
	m_pMagicSkillMng = new CMagicSkillMng();
	m_pTargetSymbol = new CN3Shape(); // ÇÃ·¹ÀÌ¾î°¡ Å¸°ÙÀ¸·Î ÀâÀº Ä³¸¯ÅÍÀÇ À§Ä¡À§¿¡ ±×¸®¸é µÈ´Ù..
	m_pWarMessage = new CWarMessage;

	m_pLightMgr = new CLightMgr;
	m_pWhisperMgr = new CWhisperManager();
}

CGameProcFxEditor::~CGameProcFxEditor()
{
	this->Release();

	//UI
	delete m_pUIMsgDlg;
	delete m_pUIMsgDlg2;
	delete m_pUIChatDlg;
	delete m_pUIChatDlg2;
	delete m_pUIStateBarAndMiniMap;
	delete m_pUIVar;
	delete m_pUICmd;
	delete m_pUITargetBar;
	delete m_pUIHelp;
	delete m_pUINotice;
	delete m_pUIClassChange;
	delete m_pUINpcEvent;
	delete m_pUIItemREDlg;
	delete m_pUIRepairTooltip;
	delete m_pUIDroppedItemDlg;
	delete m_pUITransactionDlg;
	delete m_pUIInventory;
	delete m_pUIPartyOrForce;
	delete m_pUISkillTreeDlg;
	delete m_pUICmdListDlg;
	delete m_pUICmdEditDlg;
	delete m_pUIHotKeyDlg;
	delete m_pUINpcTalk;
	delete m_pUIKnightsOp;
	delete m_pUIPartyBBS;
	delete m_pUIWareHouseDlg;
	delete m_pUINpcChange;
	delete m_pUIWarp;
	delete m_pUIInn;
	delete m_pUICreateClanName;
	delete m_pUITradeBBS;
	delete m_pUITradeBBSSelector;
	delete m_pUITradeBBSEdit;
	delete m_pUIQuestMenu;
	delete m_pUIQuestTalk;
	delete m_pUIDead;
	delete m_pSubProcPerTrade;
	delete m_pMagicSkillMng;
	delete m_pWarMessage;
	delete m_pTargetSymbol; // ÇÃ·¹ÀÌ¾î°¡ Å¸°ÙÀ¸·Î ÀâÀº Ä³¸¯ÅÍÀÇ À§Ä¡À§¿¡ ±×¸®¸é µÈ´Ù..
	delete m_pUIAnvil;
	delete m_pUIItemUpgradeDlg;
	delete m_pUIDisguiseRingDlg;

	delete m_pLightMgr;

	delete m_pWhisperMgr;
}

void CGameProcFxEditor::Release()
{
	this->ReleaseSound();
	this->ReleaseUIs();

	m_pSubProcPerTrade->Release();
	m_pMagicSkillMng->Release();
	m_pWarMessage->Release();
	m_pTargetSymbol->Release();
	m_pLightMgr->Release();

	CGameProcedure::Release();
}

void CGameProcFxEditor::ReleaseUIs()
{
	m_pUIChatDlg->Release();
	m_pUIChatDlg2->Release();
	m_pUIMsgDlg->Release();
	m_pUIMsgDlg2->Release();
	m_pUICmd->Release();
	m_pUIVar->Release();
	m_pUIStateBarAndMiniMap->Release();
	m_pUITargetBar->Release();
	m_pUIHelp->Release();
	m_pUINotice->Release();
	m_pUIClassChange->Release();
	m_pUINpcEvent->Release();
	m_pUIItemREDlg->Release();
	m_pUIRepairTooltip->Release();
	m_pUIPartyOrForce->Release();
	m_pUISkillTreeDlg->Release();
	m_pUICmdListDlg->Release();
	m_pUICmdEditDlg->Release();
	m_pUIHotKeyDlg->Release();
	m_pUINpcTalk->Release();
	//	m_pUITradeList->Release();
	m_pUIKnightsOp->Release();			// ±â»ç´Ü ¸®½ºÆ® º¸±â, °¡ÀÔ, µî...
	m_pUIPartyBBS->Release();
	m_pUIWareHouseDlg->Release();
	m_pUINpcChange->Release();
	m_pUIWarp->Release();
	m_pUIInn->Release();
	m_pUICreateClanName->Release();
	m_pUIAnvil->Release();
	m_pUIItemUpgradeDlg->Release();
	m_pUIDisguiseRingDlg->Release();
	CN3UIBase::DestroyTooltip();
}

void CGameProcFxEditor::CreateDummyCharacter()
{
	s_pPlayer->m_InfoExt.iZoneCur = 210;

	int iZone = s_pPlayer->m_InfoExt.iZoneCur;
	s_pPlayer->Release(); // ÀÏ´Ü ¸ùÃ¢ ´Ù ÇØÁ¦ ÇÏ°í....
	s_pPlayer->m_InfoExt.iZoneCur = iZone;

	s_pPlayer->IDSet(0, "d5dl2", D3DCOLOR_XRGB(100, 210, 255)); // ¹àÀº ÆÄ¶õ»ö°ú ÇÏ´Ã»ö Áß°£..

	s_pPlayer->m_InfoBase.eNation = NATION_ELMORAD;
	s_pPlayer->m_InfoBase.eRace = RACE_EL_WOMEN;
	s_pPlayer->m_InfoBase.eClass = CLASS_EL_WIZARD;
	s_pPlayer->m_InfoExt.iFace = 1; // ¾ó±¼ ¸ð¾ç..
	s_pPlayer->m_InfoExt.iHair = 1; // ¸Ó¸®Ä«¶ô

	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks.Find(s_pPlayer->m_InfoBase.eRace);	// User Player Character Skin ±¸Á¶Ã¼ Æ÷ÀÎÅÍ..
	if (NULL == pLooks) CLogWriter::Write("CGameProcMain::MsgRecv_MyInfo_All : failed find character resource data (Race : %d)", s_pPlayer->m_InfoBase.eRace);
	__ASSERT(pLooks, "failed find character resource data");
	s_pPlayer->InitChr(pLooks); // °üÀý ¼¼ÆÃ..

	s_pPlayer->m_InfoExt.iRank = 1;
	s_pPlayer->m_InfoExt.iTitle = 1;
	s_pPlayer->m_InfoBase.iLevel = 80;
	s_pPlayer->m_InfoExt.iLevelPrev = s_pPlayer->m_InfoBase.iLevel;
	s_pPlayer->m_InfoExt.iBonusPointRemain = 0; // ³²Àº º¸³Ê½º Æ÷ÀÎÆ®..

	s_pPlayer->m_InfoExt.iExpNext = 1;
	s_pPlayer->m_InfoExt.iExp = 1;
	s_pPlayer->m_InfoExt.iRealmPoint = 1;

	// @Demircivi, implemented monthly np system.
	s_pPlayer->m_InfoExt.iRealmPointMonthly = 1;

	s_pPlayer->m_InfoExt.iCity = 1;

	std::string szKnightsName = "DENEME";
	int iKnightsID = 1; // ¼Ò¼Ó ±â»ç´Ü ID
	e_KnightsDuty eKnightsDuty = KNIGHTS_DUTY_CHIEF; // ±â»ç´Ü¿¡¼­ÀÇ ±ÇÇÑ..

	// NOTE(srmeier): adding alliance ID and knight's byFlag
	int iAllianceID = 1;
	uint8_t byFlag = 1;

	int iKnightsGrade = 1; // ¼Ò¼Ó ±â»ç´Ü µî±Þ
	int	iKnightsRank = 1; // ¼Ò¼Ó ±â»ç´Ü ¼øÀ§

	// NOTE(srmeier): adding mark version and cape ID
	int16_t sMarkVersion = 1;
	int16_t sCapeID = 1;

	// ±â»ç´Ü °ü·Ã ¼¼ÆÃ..
	s_pPlayer->m_InfoExt.eKnightsDuty = eKnightsDuty; // ±â»ç´Ü¿¡¼­ÀÇ ±ÇÇÑ..
	s_pPlayer->KnightsInfoSet(iKnightsID, szKnightsName, iKnightsGrade, iKnightsRank);
	m_pUIVar->UpdateKnightsInfo();

	s_pPlayer->m_InfoBase.iHPMax = 2023;
	s_pPlayer->m_InfoBase.iHP = 2023;
	s_pPlayer->m_InfoExt.iMSPMax = 2023;
	s_pPlayer->m_InfoExt.iMSP = 2023;
	s_pPlayer->m_InfoExt.iWeightMax = 2023;
	s_pPlayer->m_InfoExt.iWeight = 2023;

	s_pPlayer->m_InfoExt.iStrength = 255;
	s_pPlayer->m_InfoExt.iStrength_Delta = 0;
	s_pPlayer->m_InfoExt.iStamina = 255;
	s_pPlayer->m_InfoExt.iStamina_Delta = 0;
	s_pPlayer->m_InfoExt.iDexterity = 255;
	s_pPlayer->m_InfoExt.iDexterity_Delta = 0;
	s_pPlayer->m_InfoExt.iIntelligence = 255;
	s_pPlayer->m_InfoExt.iIntelligence_Delta = 0;
	s_pPlayer->m_InfoExt.iMagicAttak = 255;
	s_pPlayer->m_InfoExt.iMagicAttak_Delta = 0;

	s_pPlayer->m_InfoExt.iAttack = 9999;
	s_pPlayer->m_InfoExt.iGuard = 0;
	s_pPlayer->m_InfoExt.iRegistFire = 255;
	s_pPlayer->m_InfoExt.iRegistCold = 255;
	s_pPlayer->m_InfoExt.iRegistLight = 255;
	s_pPlayer->m_InfoExt.iRegistMagic = 255;
	s_pPlayer->m_InfoExt.iRegistCurse = 255;
	s_pPlayer->m_InfoExt.iRegistPoison = 255;

	s_pPlayer->m_InfoExt.iGold = 2100000000;
	s_pPlayer->m_InfoBase.iAuthority = AUTHORITY_MANAGER; //±ÇÇÑ.. 

	// NOTE(srmeier): adding national rank and leader rank
	uint8_t bKnightsRank = 1;
	uint8_t bPersonalRank = 1;

	//// ÀåÂøÇÏ°í ÀÖ´Â °Å..
	//int iItemIDInSlots[ITEM_SLOT_COUNT]; memset(iItemIDInSlots, -1, sizeof(iItemIDInSlots));
	//int iItemDurabilityInSlots[ITEM_SLOT_COUNT]; memset(iItemDurabilityInSlots, -1, sizeof(iItemDurabilityInSlots));
	//int iItemCountInSlots[ITEM_SLOT_COUNT]; memset(iItemCountInSlots, -1, sizeof(iItemCountInSlots));

	//for (int i = 0; i < ITEM_SLOT_COUNT; i++)				// ½½·Ô °¹¼ö¸¶Å­..
	//{
	//	iItemIDInSlots[i] = pkt.read<uint32_t>();
	//	iItemDurabilityInSlots[i] = pkt.read<int16_t>();
	//	iItemCountInSlots[i] = pkt.read<int16_t>();

	//	// NOTE(srmeier): adding rental flag and remaining time
	//	uint8_t bRentFlag = pkt.read<uint8_t>();
	//	int16_t sRemainingRentalTime = pkt.read<int16_t>();
	//}
	s_pPlayer->PositionSet(__Vector3(100, 1, 100), true);
	m_fMsgSendTimeMove = 0;						// Network ReQuest Å¸ÀÌ¸Ó ÃÊ±âÈ­..
	m_fMsgSendTimeRot = 0;
	m_fPlayerYawSended = 0;						// ÃÖ±Ù¿¡ ¸Þ½ÃÁö¸¦ º¸³½ ½ÃÁ¡ÀÇ ÇÃ·¹ÀÌ¾î y Ãà È¸Àü°ª.
	m_vPlayerPosSended = s_pPlayer->Position();	// ÃÖ±Ù¿¡ ¸Þ½ÃÁö¸¦ º¸³½ ½ÃÁ¡ÀÇ ÇÃ·¹ÀÌ¾î À§Ä¡.

		
	if (s_pPlayer->Part(PART_POS_UPPER)->FileName().empty()) // ¾Æ¹«°Íµµ ¼¼ÆÃ¾ÈµÇ¾î ÀÖÀ¸¸é ÆÄÀÏ ÀÌ¸§ÀÌ ¾ø´Ù..
		s_pPlayer->PartSet(PART_POS_UPPER, pLooks->szPartFNs[PART_POS_UPPER], NULL, NULL);	// »óÃ¼..
	if (s_pPlayer->Part(PART_POS_LOWER)->FileName().empty()) // ¾Æ¹«°Íµµ ¼¼ÆÃ¾ÈµÇ¾î ÀÖÀ¸¸é ÆÄÀÏ ÀÌ¸§ÀÌ ¾ø´Ù..
		s_pPlayer->PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER], NULL, NULL);	// ÇÏÃ¼..
	if (s_pPlayer->Part(PART_POS_HANDS)->FileName().empty()) // ¾Æ¹«°Íµµ ¼¼ÆÃ¾ÈµÇ¾î ÀÖÀ¸¸é ÆÄÀÏ ÀÌ¸§ÀÌ ¾ø´Ù..
		s_pPlayer->PartSet(PART_POS_HANDS, pLooks->szPartFNs[PART_POS_HANDS], NULL, NULL);	// ÆÈ..
	if (s_pPlayer->Part(PART_POS_FEET)->FileName().empty()) // ¾Æ¹«°Íµµ ¼¼ÆÃ¾ÈµÇ¾î ÀÖÀ¸¸é ÆÄÀÏ ÀÌ¸§ÀÌ ¾ø´Ù..
		s_pPlayer->PartSet(PART_POS_FEET, pLooks->szPartFNs[PART_POS_FEET], NULL, NULL);	// ´Ù¸®..
	if (s_pPlayer->Part(PART_POS_FACE)->FileName().empty()) // ¾Æ¹«°Íµµ ¼¼ÆÃ¾ÈµÇ¾î ÀÖÀ¸¸é ÆÄÀÏ ÀÌ¸§ÀÌ ¾ø´Ù..
		s_pPlayer->InitFace();
	if (s_pPlayer->Part(PART_POS_HAIR_HELMET)->FileName().empty()) // ¾Æ¹«°Íµµ ¼¼ÆÃ¾ÈµÇ¾î ÀÖÀ¸¸é ÆÄÀÏ ÀÌ¸§ÀÌ ¾ø´Ù..
		s_pPlayer->InitHair();
	s_pPlayer->SetSoundAndInitFont();
	//	s_pPlayer->SettingCollisionCheckPoint();

		////////////////////////////////////////////////////////////
		// ±âº»°ª ÀÐ±â..
	int iRun = 1;
	//	if(false == CGameProcedure::RegGetSetting("UserRun", &iRun, 4)) iRun = 1; // °È°í ¶Ù´Â »óÅÂ¸¦ ·¹Áö½ºÆ®¸®¿¡¼­ ÀÐ°í.. ±âº»°ªÀº ¶Ù´Â »óÅÂÀÌ´Ù..
	//	if(1 == iRun)
	this->CommandToggleWalkRun(); // ¶Ù°Ô ¸¸µç´Ù..

	e_ViewPoint eVP = VP_THIRD_PERSON;
	if (false == CGameProcedure::RegGetSetting("CameraMode", &eVP, 4)) eVP = VP_THIRD_PERSON; // Ä«¸Þ¶ó »óÅÂ ±â·Ï
	s_pEng->ViewPointChange(eVP);
	// ±âº»°ª ÀÐ±â..
	////////////////////////////////////////////////////////////

	//this->InitPlayerPosition(__Vector3(10, 10, 10)); // ÇÃ·¹ÀÌ¾î À§Ä¡ ÃÊ±âÈ­.. ÀÏÀ¸ÄÑ ¼¼¿ì°í, ±âº»µ¿ÀÛÀ» ÃëÇÏ°Ô ÇÑ´Ù.

	// berserk temp
	//s_pPlayer->PlugSet(PLUG_POS_BACK, "item/babacloak.n3cplug_cloak", NULL);	// ÆÄÆ®¸¦ ¼ÂÆÃ..
	// end berserk temp

	// berserk
	//s_pPlayer->AttachCloak();

	//..
	s_pOPMgr->Release();							// ´Ù¸¥ À¯Àú °ü¸® Å¬·¡½º ÃÊ±âÈ­..

	m_bLoadComplete = TRUE;						// ·Îµù ³¡..

	return;
}

void CGameProcFxEditor::Init()
{
	CGameProcedure::Init();
	m_pLightMgr->Release();
	s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

	CreateDummyCharacter();

	this->InitZone(s_pPlayer->m_InfoExt.iZoneCur, s_pPlayer->Position()); // Á¸ ·Îµù..

	if (s_pUILoading) s_pUILoading->Render("Loading Character Data...", 0);

	// °æ·Î ±â¾ï..
	char szPathOld[_MAX_PATH], szPathFind[_MAX_PATH];
	::GetCurrentDirectory(_MAX_PATH, szPathOld);

	_finddata_t fi;
	uint64_t hFind = -1;

	// ¸®¼Ò½º ´Ù ÀÐ±â..
	// ¿¡´Ï¸ÞÀÌ¼Ç ´Ù ÀÐ±â..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Chr");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3Anim", &fi);
	if (hFind)
	{
		std::string szFN = "Chr\\";
		szFN += fi.name;
		CN3AnimControl* pObjTmp = s_MngAniCtrl.Get(szFN.c_str());
		while (_findnext(hFind, &fi) != -1)
		{
			szFN = "Chr\\";
			szFN += fi.name;
			pObjTmp = s_MngAniCtrl.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if (s_pUILoading) s_pUILoading->Render("Loading Character Data... 10 %", 10);

	// ¸®¼Ò½º ´Ù ÀÐ±â..
	// ÅØ½ºÃ³ ´Ù ÀÐ±â..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.dxt", &fi);
	if (hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3Texture* pObjTmp = s_MngTex.Get(szFN.c_str());
		while (_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngTex.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if (s_pUILoading) s_pUILoading->Render("Loading Character Data... 25 %", 25);

	// ¸®¼Ò½º ´Ù ÀÐ±â..
	// Á¶ÀÎÆ® ´Ù ÀÐ±â..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Chr");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3Joint", &fi);
	if (hFind)
	{
		std::string szFN = "Chr\\";
		szFN += fi.name;
		CN3Joint* pObjTmp = s_MngJoint.Get(szFN.c_str());
		while (_findnext(hFind, &fi) != -1)
		{
			szFN = "Chr\\";
			szFN += fi.name;
			pObjTmp = s_MngJoint.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if (s_pUILoading) s_pUILoading->Render("Loading Character Data... 50 %", 50);

	// ¸®¼Ò½º ´Ù ÀÐ±â..
	// ½ºÅ² ÀÐ±â..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3CSkins", &fi);
	if (hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3CPartSkins* pObjTmp = s_MngSkins.Get(szFN.c_str());
		while (_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngSkins.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if (s_pUILoading) s_pUILoading->Render("Loading Character Data... 75 %", 75);

	// ¸®¼Ò½º ´Ù ÀÐ±â..
	// PMesh ÀÐ±â..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3PMesh", &fi);
	if (hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3PMesh* pObjTmp = s_MngPMesh.Get(szFN.c_str());
		while (_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngPMesh.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if (s_pUILoading) s_pUILoading->Render("Loading Character Data... 100 %", 100);

	// °æ·Î µ¹¸®±â..
	::SetCurrentDirectory(szPathOld);
}

void CGameProcFxEditor::InitPlayerPosition(const __Vector3& vPos) // ÇÃ·¹ÀÌ¾î À§Ä¡ ÃÊ±âÈ­.. ÀÏÀ¸ÄÑ ¼¼¿ì°í, ±âº»µ¿ÀÛÀ» ÃëÇÏ°Ô ÇÑ´Ù.
{
	__Vector3 vPosFinal = vPos;
	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(vPos.x, vPos.z);	// ÁöÇüÀÇ ³ôÀÌ°ª ¾ò±â..
	float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(vPos, 1.0f); // ¿ÀºêÁ§Æ®¿¡¼­ °¡Àå °¡±î¿î ³ôÀÌ°ª ¾ò±â..
	if (!s_pWorldMgr->IsIndoor())
	{
		if (T_Abs(vPos.y - fYObject) < T_Abs(vPos.y - fYTerrain)) vPosFinal.y = fYObject; // Á»´õ °¡±î¿î °÷¿¡ ³õ´Â´Ù..
		else vPosFinal.y = fYTerrain;
	}
	else
	{
		if (fYObject > fYTerrain)
			vPosFinal.y = fYObject;
		else
			vPosFinal.y = fYTerrain;
	}

	s_pPlayer->PositionSet(vPosFinal, true);	// Ä³¸¯ÅÍ À§Ä¡ ¼ÂÆÃ..	
	s_pPlayer->m_vPosFromServer = vPos;
	m_vPlayerPosSended = vPos;					// ÃÖ±Ù¿¡ º¸³½ À§Ä¡ ¼¼ÆÃ..
	m_fMsgSendTimeMove = 0;						// ½Ã°£À» ±â·ÏÇÑ´Ù..

	this->CommandSitDown(false, false, true); // ÀÏÀ¸ÄÑ ¼¼¿î´Ù.. ¾É¾ÆÀÖ´Â »óÅÂ¿¡¼­ ¿öÇÁÇÏ¸é.. ¹ö±×°¡ ÀÖ´Ù..
	this->TargetSelect(-1, false); // Å¸°Ù ÇØÁ¦..
	this->UpdateCameraAndLight(); // Ä«¸Þ¶ó¿Í ¶óÀÌÆ® ´Ù½Ã °è»ê..

	s_pPlayer->Action(PSA_BASIC, true, NULL, true); // °­Á¦·Î ±âº» ÀÚ¼¼..
}

void CGameProcFxEditor::Tick()
{
	CGameProcedure::Tick();	// Å°, ¸¶¿ì½º ÀÔ·Â µîµî..
	if (FALSE == m_bLoadComplete) return;				// ·ÎµùÀÌ ¾ÈµÇ¾úÀ¸¸é.. µ¹¾Æ°£´Ù.

#ifdef _DEBUG
	if (s_pLocalInput->IsKeyDown(DIK_F11))
	{
		static int i = 1; //10029 SPrint // 602 720 hp
		CGameProcedure::s_pFX->TriggerBundle(0, 0, i++, 0, 15);

	}
#endif

	uint32_t dwMouseFlags = s_pLocalInput->MouseGetFlag();	// ¸¶¿ì½º ¹öÆ° ÇÃ·¡±× - LocalInput.h ÂüÁ¶
	this->ProcessLocalInput(dwMouseFlags);					// Å°º¸µå³ª ¸¶¿ì½º ÀÔ·ÂÀº UI ´ÙÀ½¿¡ Ã³¸®...

	s_pPlayer->Tick();									// ÇÃ·¹ÀÌ¾î Æ½(°»½Å)
	s_pWorldMgr->Tick();
	s_pOPMgr->Tick(s_pPlayer->Position());				// ´Ù¸¥ À¯Àú °ü¸®ÀÚ Æ½(°»½Å)
	//	s_pFX->Tick(); //³»ºÎ¿¡¼­ Ä«¸Þ¶ó °ªÀ» ¾µ °æ¿ì À§Ä¡°¡ ¿ÀÂ÷°¡ »ý°Ü Render()ÇÔ¼ö ¾ÈÀ¸·Î ¿Å±è...

	__Vector3 ListenerPos = s_pPlayer->Position();
	__Vector3 ListenerDir = s_pPlayer->Direction();
	__Vector3 ListenerUp(0, 1, 0);

	// Sound Tick...
	CN3SndObj::SetListenerPos(&ListenerPos);
	CN3SndObj::SetListenerOrientation(&ListenerDir, &ListenerUp);

	this->UpdateUI_MiniMap(); // ¹Ì´Ï¸Ê ¾÷µ¥ÀÌÆ®..
	this->UpdateUI_TargetBar(); // Å¸°Ù¹Ù Ã³¸®..
	this->UpdateBGM(); // ¹è°æÀ½¾ÇÀ» »óÈ²¿¡ µû¶ó Ã³¸®..
	this->UpdateCameraAndLight(); // Ä«¸Þ¶ó¿Í ¶óÀÌÆ® Ã³¸®..

	//	ProcessPlayerInclination();							// °æ»ç Ã³¸®..(°¡¸¸È÷ ÀÖ¾îµµ °æ»ç°¡ ±ÞÇÏ¸é ¹Ì²ô·¯Áü..).
#ifdef _N3_64GRID_
	m_SMesh.Tick(s_pPlayer, &m_Terrain);				// ¼­¹ö ¸Þ½Ã Æ½.(°»½Å)
#endif

	m_pUIStateBarAndMiniMap->UpdatePosition(s_pPlayer->Position(), s_pPlayer->Yaw()); // À§Ä¡ ¾÷µ¥ÀÌÆ®.

	if (m_pMagicSkillMng) m_pMagicSkillMng->Tick();
	if (m_pWarMessage) m_pWarMessage->Tick();
	if (m_pLightMgr) m_pLightMgr->Tick();

	////////////////////////////////////////////////////////////////////////////////////
	// ¾Æ¹« ÆÐÅ¶µµ ¾Èº¸³ÂÀ¸¸é 2ÃÊ¿¡ ÇÑ¹ø WIZ_TIME_NOTIFY º¸³½´Ù..
	float fTime = CN3Base::TimeGet();
	static float fTimePrev = fTime;

	static float fTimeInterval1 = 0;
	if (0 == s_pSocket->m_iSendByteCount)
	{
		fTimeInterval1 += fTime - fTimePrev;
		if (fTimeInterval1 >= 2.0f)
		{
			uint8_t byCmd = WIZ_TIMENOTIFY;
			s_pSocket->Send(&byCmd, 1);
			s_pSocket->m_iSendByteCount = 0;
			fTimeInterval1 = 0;
		}
	}
	else
	{
		s_pSocket->m_iSendByteCount = 0;
		fTimeInterval1 = 0;
	}
	// ¾Æ¹« ÆÐÅ¶µµ ¾Èº¸³ÂÀ¸¸é 2ÃÊ¿¡ ÇÑ¹ø WIZ_TIME_NOTIFY º¸³½´Ù..
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Å¸ÀÌ¸Ó ºñ½ÁÇÑ ·çÆ¾..
	static float fInterval2 = 0, fInterval3 = 0, fInterval4 = 0, fInterval5 = 0;
	fInterval2 += fTime - fTimePrev;
	fInterval3 += fTime - fTimePrev;
	fInterval4 += fTime - fTimePrev;
	fInterval5 += fTime - fTimePrev;
	m_fRequestGameSave += fTime - fTimePrev;
	if (fInterval2 > 1200.0f) // ÀúÀå ¿äÃ»..
	{
		uint8_t byBuff[4];												// ¹öÆÛ.. 
		int iOffset = 0;												// ¿É¼Â..
		s_pSocket->MP_AddByte(byBuff, iOffset, WIZ_DATASAVE);	// ÀúÀå ¿äÃ» Ä¿¸àµå..
		s_pSocket->Send(byBuff, iOffset);				// º¸³¿..

		fInterval2 = 0.0f;
	}
	if (fInterval3 > 10.0f) // ½ºÇÇµåÇÙ Ã¼Å©.. 
	{
		fInterval3 = 0.0f;
	}
	if (s_pPlayer->m_InfoBase.iLevel < 12 && fInterval4 > 20.0f) // ½Ã°£ÀÌ Áö³ª¸é ÆÁ ÇÏ³ª¾¿ Ç¥½Ã..
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_HELP_TIP_ALL, szMsg);
		this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00);

		::_LoadStringFromResource(IDS_HELP_TIP1 + (rand() % 33), szMsg);
		this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00); // ÇïÇÁ Ç¥½Ã..
		fInterval4 = 0;
	}
	if (fInterval5 > 5.0f) // ½Ã°£ÀÌ Áö³ª¸é ÆÁ ÇÏ³ª¾¿ Ç¥½Ã..
	{
		//		m_pUIChatDlg->ChangeChattingMode(N3_CHAT_CONTINUE); // Ã¤ÆÃ¸ðµå °­Á¦·Î ¹Ù²Ù±â...
		m_pUIChatDlg->ShowContinueMsg();
		fInterval5 = 0;
	}
	fTimePrev = fTime;
	// Å¸ÀÌ¸Ó ºñ½ÁÇÑ ·çÆ¾..
	////////////////////////////////////////////////////////////////////////////////////
}

void CGameProcFxEditor::Render()
{
	if (FALSE == m_bLoadComplete)	return; 		// ·ÎµùÀÌ ³¡³µ³Ä??

	D3DCOLOR crSky = ACT_WORLD->GetSkyColorWithSky();
	s_pEng->Clear(crSky); // ¾È°³ »ö±òÀ» ³Ö¾î¼­ Å¬¸®¾î.. -> ÇÏ´Ã»ö±ò·Î Å¬¸®¾î ÇØ¾ß ÇÏ´ÃÀÌ Á¦´ë·Î ³ª¿Â´Ù..
	s_pEng->s_lpD3DDev->BeginScene();			// ¾À ·»´õ ¤µÀÛ...

	ACT_WORLD->RenderSky();								// ÇÏ´Ã ·»´õ¸µ..
	float fSunAngle = ACT_WORLD->GetSunAngleByRadinWithSky(); // ÇØÀÇ °¢µµ¸¦ °¡Á®¿À°í..

	uint32_t dwFilter = D3DTEXF_LINEAR;
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MINFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MAGFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MIPFILTER, dwFilter);

	ACT_WORLD->RenderTerrain();						// ÁöÇü ·»´õ..
	ACT_WORLD->RenderShape();						// ¹°Ã¼ ·»´õ..
	s_pOPMgr->Render(fSunAngle);				// ´Ù¸¥ ÇÃ·¹ÀÌ¾î ·»´õ..
	s_pPlayer->Render(fSunAngle);			// ÇÃ·¹ÀÌ¾î ·»´õ..

	// NOTE(srmeier): uncomment to render the collision meshes
#ifdef _DEBUG
	ACT_WORLD->RenderCollisionWithShape(s_pPlayer->Position());				// Ãæµ¹ ¸Þ½¬ ·»´õ..
#endif

#ifdef _N3_64GRID_
	m_SMesh.Render();							// ¼­¹ö ¸Þ½¬ ·»´õ..
#endif

	this->RenderTarget();						// Å¸°ÙÀ¸·Î ÀâÀº Ä³¸¯ÅÍ È¤Àº ¿ÀºêÁ§Æ® ·»´õ¸µ..

	ACT_WORLD->RenderGrass();						//	Ç® ·»´õ (asm)
	s_pFX->Tick();
	s_pFX->Render();
	ACT_WORLD->RenderBirdMgr();

	CN3Base::s_AlphaMgr.Render(); // ¾ËÆÄ Á¤·ÄµÈ Æú¸®°ïµé ·»´õ¸µ..

	ACT_WORLD->RenderSkyWeather();							// ÇÏ´Ã ·»´õ¸µ..

	CGameProcedure::Render(); // UI ³ª ±×¹ÛÀÇ ±âº»ÀûÀÎ °Íµé ·»´õ¸µ..
	if (m_pWarMessage) m_pWarMessage->RenderMessage();
	if (s_pGameCursor) s_pGameCursor->Render();

	s_pEng->s_lpD3DDev->EndScene();
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcFxEditor::RenderTarget()
{
	if (NULL == m_pTargetSymbol) return;

	// ÇÃ·¹ÀÌ¾î°¡ Å¸°ÙÀ¸·Î ÀâÀº Ä³¸¯ÅÍÀÇ À§Ä¡À§¿¡ ±×¸®¸é µÈ´Ù..
	CPlayerBase* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);//½ÃÃ¼·Î ÆÇÁ¤µÇ±â Àü±îÁöÀÇ Ä³¸¯Àº Æ÷Ä¿½º¸¦ ÁØ´Ù.
	if (NULL == pTarget && NULL == s_pPlayer->m_pObjectTarget) return;

	float fScale = 1;
	float fYScale = 1;
	__Vector3 vPos(0, 0, 0);

	if (pTarget)
	{
		fYScale = pTarget->Height() * 1.3f;
		fScale = pTarget->Radius() * 2.0f;
		vPos = pTarget->Position();
	}
	else
	{
		__Vector3 vTmp = s_pPlayer->m_pObjectTarget->Max() - s_pPlayer->m_pObjectTarget->Min();
		fYScale = vTmp.y;
		vTmp.y = 0;
		fScale = vTmp.Magnitude();
		vPos = s_pPlayer->m_pObjectTarget->Pos();
	}

	m_pTargetSymbol->ScaleSet(fScale, fYScale, fScale);
	m_pTargetSymbol->PosSet(vPos);
	m_pTargetSymbol->Tick();
	if (m_pTargetSymbol->Part(1)) // ¹Ù´ÚÀÇ ½É¹úÀ» ¶¥¹Ù´Ú À§·Î ¸ÂÃá´Ù..
	{
		CN3PMesh* pPMesh = m_pTargetSymbol->Part(1)->Mesh();
		if (pPMesh && pPMesh->GetMaxNumVertices() == 4)
		{
			__VertexT1* pVs = m_pTargetSymbol->Part(1)->Mesh()->GetVertices();
			for (int i = 0; i < 4; i++)
			{
				pVs[i].y = ACT_WORLD->GetHeightWithTerrain(vPos.x + (pVs[i].x * fYScale), vPos.z + (pVs[i].z * fYScale));
				pVs[i].y -= vPos.y;
				pVs[i].y /= fYScale;
				pVs[i].y += 0.1f / fYScale;
			}
		}
	}
	m_pTargetSymbol->Render();

	// NOTE(srmeier): uncomment to render the collision meshes
#ifdef _DEBUG
	if (pTarget) pTarget->RenderCollisionMesh();
	if (s_pPlayer->m_pObjectTarget) s_pPlayer->m_pObjectTarget->RenderCollisionMesh();
#endif
}

// Å°º¸µå¿Í ¸¶¿ì½º ´­¸°°ÍÀ» Ã³¸®ÇÑ´Ù..
void CGameProcFxEditor::ProcessLocalInput(uint32_t dwMouseFlags)
{
	// LoadingÀÌ µÈ ÈÄ..
	if (FALSE == m_bLoadComplete) return;

	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return;

	if (!s_bIsWindowInFocus)
		return;

	if (s_bWindowHasMouseFocus)
	{
		POINT ptPrev = s_pLocalInput->MouseGetPosOld();
		POINT ptCur = s_pLocalInput->MouseGetPos();

		OnMouseMove(ptCur, ptPrev);

		//static POINT ptPrev_RB ={};

		if (dwMouseFlags & MOUSE_RBCLICK)
		{
			// NOTE: right click on NPCs, interactable shapes, item boxes, etc.
			OnMouseRBtnPress(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_RBDOWN)
		{
			// NOTE: this is where the right click rotation and zoom out occur
			//if(!SDL_GetRelativeMouseMode()) ptPrev_RB = ptCur;
			//else {
			//	int x, y;
			//	SDL_GetWindowPosition(s_hWndBase, &x, &y);
			//	SetCursorPos(ptPrev_RB.x+x, ptPrev_RB.y+y);
			//}
			OnMouseRbtnDown(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_RBCLICK)
		{
			OnMouseRBtnPressd(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_RBDBLCLK)
		{
			OnMouseRDBtnPress(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_LBCLICK)
		{
			// NOTE: move on click
			OnMouseLBtnPress(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_LBDOWN)
		{
			// NOTE: move on held down click
			OnMouseLbtnDown(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_LBCLICKED)
		{
			OnMouseLBtnPressd(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_LBDBLCLK)
		{
			OnMouseLDBtnPress(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_MBCLICK)
		{
			OnMouseMBtnPress(ptCur, ptPrev);
		}

		// reset mouse visibility
		/*if (!(dwMouseFlags&MOUSE_RBDOWN) && SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE) {
			SDL_ShowCursor(SDL_ENABLE);
		}*/
		//if(!(dwMouseFlags&MOUSE_RBDOWN) && SDL_GetRelativeMouseMode()) {
		//	SDL_SetRelativeMouseMode(SDL_FALSE);
		//	int x, y;
		//	SDL_GetWindowPosition(s_hWndBase, &x, &y);
		//	SetCursorPos(ptPrev_RB.x+x, ptPrev_RB.y+y);
		//	s_pLocalInput->MouseSetPos(ptPrev_RB.x+x, ptPrev_RB.y+y);
		//}


		// Moves camera when mouse is on the borders of the screen. For both X & Y
		if (!(dwMouseFlags & MOUSE_RBDOWN))
		{
			float fRotY = 0, fRotX = 0;
			if (0 == ptCur.x) fRotY = -2.0f;
			else if ((CN3Base::s_CameraData.vp.Width - 1) == ptCur.x) fRotY = 2.0f;
			if (0 == ptCur.y) fRotX = -1.0f;
			else if ((CN3Base::s_CameraData.vp.Height - 1) == ptCur.y) fRotX = 1.0f;
			if (fRotY)
			{
				if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(fRotY);
				else s_pPlayer->RotAdd(fRotY);
			}
			if (fRotX && VP_THIRD_PERSON != s_pEng->ViewPoint()) s_pEng->CameraPitchAdd(fRotX);
		}
	}

	int iHotKey = -1;
	if (s_pLocalInput->IsKeyPress(KM_HOTKEY1)) iHotKey = 0;
	else if (s_pLocalInput->IsKeyPress(KM_HOTKEY2)) iHotKey = 1;
	else if (s_pLocalInput->IsKeyPress(KM_HOTKEY3)) iHotKey = 2;
	else if (s_pLocalInput->IsKeyPress(KM_HOTKEY4)) iHotKey = 3;
	else if (s_pLocalInput->IsKeyPress(KM_HOTKEY5)) iHotKey = 4;
	else if (s_pLocalInput->IsKeyPress(KM_HOTKEY6)) iHotKey = 5;
	else if (s_pLocalInput->IsKeyPress(KM_HOTKEY7)) iHotKey = 6;
	else if (s_pLocalInput->IsKeyPress(KM_HOTKEY8)) iHotKey = 7;
	else if (s_pLocalInput->IsKeyPress(KM_HOTKEY9)) iHotKey = 8;

	if ((iHotKey >= 0 && iHotKey < 9) &&
		CN3UIBase::GetFocusedEdit() == NULL &&
		m_pSubProcPerTrade->m_ePerTradeState == PER_TRADE_STATE_NONE)
	{
		m_pUIHotKeyDlg->EffectTriggerByHotKey(iHotKey);
	}
	// ÇÖÅ°
	//////////////////////////////////////////

	if (s_pLocalInput->IsKeyPress(KM_CAMERA_CHANGE))												// ½ÃÁ¡ º¯È¯..
	{
		this->CommandCameraChange(); // Ä«¸Þ¶ó ½ÃÁ¡ ¹Ù²Ù±â..
	}

	// »ïÀÎÄªÀÏ¶§ È¨, ¿£µåÅ°·Î Ä«¸Þ·Î ¿Ã¸®°í ³»¸®±â..
	if (s_pEng->ViewPoint() == VP_THIRD_PERSON)
	{
		float fPitch = 0;
		if (s_pLocalInput->IsKeyDown(DIK_HOME)) fPitch = D3DXToRadian(45.0f);		// home Å°°¡ ´­¸®¸é..
		else if (s_pLocalInput->IsKeyDown(DIK_END)) fPitch = D3DXToRadian(-45.0f);	// End Å°°¡ ´­¸®¸é..
		if (fPitch) s_pEng->CameraPitchAdd(fPitch);
	}

	if (!IsUIKeyOperated() && NULL == CN3UIBase::GetFocusedEdit())			// Ã¤ÆÃ¸ðµå°¡ ¾Æ´Ò¶§ 
	{
		if (s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER) //°ÔÀÓ ¿î¿µÀÚ´Â ÀÌ ±â´ÉÀ» »ç¿ëÇÒ¼ö ÀÖ´Ù.
		{
			if (s_pLocalInput->IsKeyDown(DIK_Q)) s_pPlayer->m_bTempMoveTurbo = true; // ¾öÃ» »¡¸® ¿òÁ÷ÀÌ°Ô ÇÑ´Ù..  // ÀÓ½Ã ÇÔ¼ö.. ³ªÁß¿¡ ¾ø¾ÖÀÚ..
			else s_pPlayer->m_bTempMoveTurbo = false; // ¾öÃ» »¡¸® ¿òÁ÷ÀÌ°Ô ÇÑ´Ù..  // ÀÓ½Ã ÇÔ¼ö.. ³ªÁß¿¡ ¾ø¾ÖÀÚ..
		}

		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_ATTACK))
			this->CommandToggleAttackContinous();		// ÀÚµ¿ °ø°Ý..}
		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_RUN))
			this->CommandToggleWalkRun();				// °È±â / ¶Ù±â Åä±Û	
		if (s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_ENEMY))
			this->CommandTargetSelect_NearstEnemy();	// °¡Àå °¡±î¿î Àû Å¸°Ù Àâ±â..
		if (s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_PARTY))
			this->CommandTargetSelect_NearstOurForce(); // °¡Àå °¡±î¿î ÆÄÆ¼ Å¸°ÙÀâ±â..

		float fRotKeyDelta = D3DXToRadian(60); // ÃÊ´ç 60 µµ µ¹±â..
		if (s_pLocalInput->IsKeyDown(KM_ROTATE_LEFT) || s_pLocalInput->IsKeyDown(DIK_LEFT))
		{
			if (s_pPlayer->IsAlive()) s_pPlayer->RotAdd(-fRotKeyDelta); // ÃÊ´ç 180 µµ ¿ÞÂÊÀ¸·Î µ¹±â.
			if (m_pUIDroppedItemDlg->IsVisible()) m_pUIDroppedItemDlg->LeaveDroppedState();
		}
		if (s_pLocalInput->IsKeyDown(KM_ROTATE_RIGHT) || s_pLocalInput->IsKeyDown(DIK_RIGHT))
		{
			if (s_pPlayer->IsAlive()) s_pPlayer->RotAdd(fRotKeyDelta); // ÃÊ´ç 180 µµ ¿À¸¥ÂÊÀ¸·Î µ¹±â.
			if (m_pUIDroppedItemDlg->IsVisible()) m_pUIDroppedItemDlg->LeaveDroppedState();
		}

		if (s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyDown(DIK_UP))
		{
			bool bStart = false;
			if (s_pLocalInput->IsKeyPress(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPress(DIK_UP))
			{
				if (VP_THIRD_PERSON == s_pEng->ViewPoint())
				{
					if (!s_pPlayer->m_bTargetOrPosMove) bStart = true;
					s_pPlayer->m_bTargetOrPosMove = false;
					if (s_pPlayer->m_bAttackContinous)
						CommandToggleAttackContinous();
				}
				else
					bStart = true;// ´©¸£´Â ¼ø°£ÀÌ¸é
			}
			this->CommandMove(MD_FOWARD, bStart); // ¾ÕÀ¸·Î ÀÌµ¿..
		}
		else if (s_pLocalInput->IsKeyDown(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyDown(DIK_DOWN))
		{
			bool bStart = false;
			if (s_pLocalInput->IsKeyPress(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyPress(DIK_DOWN))
			{
				if (VP_THIRD_PERSON == s_pEng->ViewPoint())
				{
					if (!s_pPlayer->m_bTargetOrPosMove) bStart = true;
					s_pPlayer->m_bTargetOrPosMove = false;
					if (s_pPlayer->m_bAttackContinous)
						CommandToggleAttackContinous();
				}
				else
					bStart = true;// ´©¸£´Â ¼ø°£ÀÌ¸é
			}
			this->CommandMove(MD_BACKWARD, bStart); // µÚ·Î ÀÌµ¿..
		}
		else if (s_pLocalInput->IsKeyPress(KM_TOGGLE_MOVE_CONTINOUS))
		{
			this->CommandToggleMoveContinous();
		}

		if (s_pLocalInput->IsKeyPressed(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPressed(DIK_UP) ||
			s_pLocalInput->IsKeyPressed(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyPressed(DIK_DOWN)) // ÀüÁø/ÈÄÁø Å°¸¦ ¶¼´Â ¼ø°£. 
		{
			this->CommandMove(MD_STOP, true);
		}

		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_INVENTORY)) this->CommandToggleUIInventory();
		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_STATE)) this->CommandToggleUIState();
		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_SKILL)) this->CommandToggleUISkillTree();
		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_CMDLIST)) this->CommandToggleCmdList();
		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_SITDOWN)) this->CommandSitDown(true, !s_pPlayer->m_bSitDown);

		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_HELP))
		{
			if (m_pUIHelp->IsVisible()) m_pUIHelp->SetVisible(false);
			else m_pUIHelp->SetVisible(true);
		}

		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_MINIMAP))
		{
			this->CommandToggleUIMiniMap();
		}

		if (s_pLocalInput->IsKeyPress(DIK_PRIOR))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->PageUp();

		if (s_pLocalInput->IsKeyPress(DIK_NEXT))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->PageDown();

		if (s_pLocalInput->IsKeyPress(KM_HOTKEY_PAGE1))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(0);
		if (s_pLocalInput->IsKeyPress(KM_HOTKEY_PAGE2))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(1);
		if (s_pLocalInput->IsKeyPress(KM_HOTKEY_PAGE3))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(2);
		if (s_pLocalInput->IsKeyPress(KM_HOTKEY_PAGE4))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(3);
		if (s_pLocalInput->IsKeyPress(KM_HOTKEY_PAGE5))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(4);
		if (s_pLocalInput->IsKeyPress(KM_HOTKEY_PAGE6))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(5);
		if (s_pLocalInput->IsKeyPress(KM_HOTKEY_PAGE7))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(6);
		if (s_pLocalInput->IsKeyPress(KM_HOTKEY_PAGE8))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(7);

	} // end of if ( !m_UIChatDlg.IsChatMode() )

	// ..... ³ª¸ÓÁö Å°º¸µå Ã³¸®..
#if _DEBUG
	if (s_pLocalInput->IsKeyPress(DIK_F12)) // µð¹ö±ë Å×½ºÆ®..
		s_pEng->Lightning(); // ¹ø°³ Ä¡±â..
#endif
}

void CGameProcFxEditor::ProcessPlayerInclination()											// °æ»ç¿¡ ¼­ ÀÖÀ»¶§..
{
	/*	float fFrm = GetIndepTimeFrame();

		__Vector3 vNorm, vNormXZ;
		s_pTerrain->GetNormal( s_pEng->m_matPlayer.Pos().x, s_pEng->m_matPlayer.Pos().z, vNorm );

		vNorm.Normalize();
		vNormXZ = vNorm;
		vNormXZ.y = 0.0f;

		if ( vNormXZ.Magnitude() > 0.8f )
		{
			vNormXZ.x *= 0.01f*fFrm;
			vNormXZ.z *= 0.01f*fFrm;
			s_pPlayer->m_vPlayerIncline = vNormXZ;
		}*/
}


void CGameProcFxEditor::DoCommercialTransaction(int iTradeID)
{
	if (m_pUITransactionDlg->IsVisible())
		return;

	m_pUITransactionDlg->m_iTradeID = iTradeID;

	if (!m_pUITransactionDlg->IsVisible())
		m_pUITransactionDlg->SetVisible(true);

	if (m_pUIInventory->IsVisible()) // ÀÎº¥Åä¸®°¡ ¾È¿­·Á ÀÖÀ¸¸é..
		this->CommandToggleUIInventory();

	if (m_pUISkillTreeDlg->IsVisible())
		m_pUISkillTreeDlg->Close();

	m_pUITransactionDlg->EnterTransactionState();
}

void CGameProcFxEditor::InitZone(int iZone, const __Vector3& vPosPlayer)
{
	static int iZonePrev = -1;
	if (iZonePrev != iZone) // ´Ù¸¥ Á¸ÀÏ °æ¿ì¿¡¸¸ ·Îµù..
	{
		m_pLightMgr->Release();
		s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

		s_pPlayer->m_bMoveContinous = true; 

		m_bLoadComplete = false; // ·Îµù ³¡³²..
	
		s_pFX->ClearAll();

		s_pPlayer->m_InfoExt.iZoneCur = iZone;
		iZonePrev = iZone; // ÃÖ±Ù¿¡ ÀÐÀº Á¸ ¹øÈ£¸¦ ±â¾ïÇØµÐ´Ù.

		__TABLE_ZONE* pZoneData = s_pTbl_Zones.Find(s_pPlayer->m_InfoExt.iZoneCur);
		if (NULL == pZoneData) {
			CLogWriter::Write("can't find zone data. (zone : %d)", s_pPlayer->m_InfoExt.iZoneCur);
			__ASSERT(0, "Zone Data Not Found!");
			system("pause");
			return;
		}

		CLogWriter::Write("%d->Release()", s_pOPMgr); // TmpLog1122

		s_pOPMgr->Release(); // ´Ù¸¥ ³Ñµé ´Ù ³¯¸°´Ù..
		CLogWriter::Write("%d->InitWorld()", s_pWorldMgr); // TmpLog1122
		s_pWorldMgr->InitWorld(iZone, vPosPlayer);

		CLogWriter::Write("%d->szTerrainFN.c_str()", pZoneData); // TmpLog1122
		//char szBuf[256];
		char szFName[_MAX_PATH];
		_splitpath(pZoneData->szTerrainFN.c_str(), NULL, NULL, szFName, NULL);
		char szFName2[_MAX_PATH];
		char szFullPathName[_MAX_PATH];
		sprintf(szFName2, "%s_Bird", szFName);
		_makepath(szFullPathName, NULL, "misc\\bird", szFName2, "lst");

		m_pLightMgr->LoadZoneLight(pZoneData->szLightObjFN.c_str());

		m_bLoadComplete = true; // ·Îµù ³¡³²..
	}

	// Ä«¸Þ¶ó ¼¼ÆÃ..
	CN3Camera* pCamera = s_pEng->CameraGetActive();		// È°¼ºÈ­µÈ Ä«¸Þ¶ó ¾ò±â..
	if (pCamera)
	{
		__Vector3 vPosPlayer = s_pPlayer->Position();
		pCamera->m_Data.fFOV = D3DXToRadian(70);				// Field of View ..
		pCamera->m_Data.fFP = 512.0f;						// Far Plane..
		pCamera->m_Data.fNP = 0.5f;							// Near Plane..
		CLogWriter::Write("pCamera->LookAt()"); // TmpLog1122
		pCamera->LookAt(vPosPlayer + __Vector3(0, 0, -1), vPosPlayer, __Vector3(0, 1, 0));
		CLogWriter::Write("pCamera->Tick()"); // TmpLog1122
		pCamera->Tick();
		CLogWriter::Write("pCamera->Apply()"); // TmpLog1122
		pCamera->Apply();
	}
	// ±âº»ÀûÀÎ Ä³¸¯ÅÍÀ§Ä¡¿Í Ä«¸Þ¶ó À§Ä¡ Àâ±â..
	////////////////////////////////////////////////////////////////////////////////

	CLogWriter::Write("InitPlayerPosition() Position(%.1f, %.1f, %.1f)", vPosPlayer.x, vPosPlayer.y, vPosPlayer.z); // TmpLog1122
	this->InitPlayerPosition(vPosPlayer); // ÇÃ·¹ÀÌ¾î À§Ä¡ ÃÊ±âÈ­.. ÀÏÀ¸ÄÑ ¼¼¿ì°í, ±âº»µ¿ÀÛÀ» ÃëÇÏ°Ô ÇÑ´Ù.
	CLogWriter::Write("%d->Release()", s_pOPMgr); // TmpLog1122
	s_pOPMgr->Release(); // ´Ù¸¥ ÇÃ·¹ÀÌ¾î »èÁ¦...
}


bool CGameProcFxEditor::CommandToggleWalkRun()
{
	bool bRun = s_pPlayer->ToggleRunMode();
	bool bMove = s_pPlayer->m_bMoveContinous;

	// Ä¿¸Çµå ¹öÆ° ¾÷µ¥ÀÌÆ®..
	if (m_pUICmd->m_pBtn_Act_Walk)
	{
		m_pUICmd->m_pBtn_Act_Walk->SetVisible(bRun);
		if (bMove) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
	}
	if (m_pUICmd->m_pBtn_Act_Run)
	{
		m_pUICmd->m_pBtn_Act_Run->SetVisible(!bRun);
		if (bMove) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
	}

	return bRun;
}

bool CGameProcFxEditor::CommandToggleAttackContinous()
{
	CPlayerBase* pTarget = s_pPlayer->TargetPointerCheck(false);
	this->CommandEnableAttackContinous(!s_pPlayer->m_bAttackContinous, pTarget);

	return s_pPlayer->m_bAttackContinous;
}

bool CGameProcFxEditor::CommandToggleMoveContinous()
{
	if (s_pPlayer->IsDead()) return false;

	s_pPlayer->ToggleMoveMode();				// ÀÚµ¿ ÀüÁø Åä±Û.. 
	if (s_pPlayer->m_bMoveContinous)
	{
		this->CommandMove(MD_FOWARD, true);
		if (m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
		if (m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
	}
	else
	{
		this->CommandMove(MD_STOP, true); // ¿òÁ÷ÀÓ ÆÐÅ¶ º¸³»±â..
		if (m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
		if (m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
	}

	return s_pPlayer->m_bMoveContinous;
}

void CGameProcFxEditor::CommandMove(e_MoveDirection eMD, bool bStartOrEnd)
{
	if (bStartOrEnd) this->CloseUIs(); // °¢Á¾ »ó°Å·¡, ¿öÇÁµîµî... UI ´Ý±â..

	if (s_pPlayer->IsDead()) return; // Á×Àº ³ÑÀÌ ¾îµô °¨È÷!!

	if (MD_FOWARD == eMD || MD_BACKWARD == eMD)
	{
		s_pUIMgr->UserMoveHideUIs();
		this->CommandSitDown(false, false, true); // ÀÏÀ¸ÄÑ ¼¼¿ì°í..
		if (s_pPlayer->m_bStun) return; // ±âÀýÇØ ÀÖÀ½ ¿òÁ÷ÀÌÁö ¸øÇÔ..
		if (MD_FOWARD == eMD)
		{
			if (s_pPlayer->IsRunning()) s_pPlayer->ActionMove(PSM_RUN); // ¶Ù¾î°¡±â..
			else s_pPlayer->ActionMove(PSM_WALK); // °É¾î°¡±â..
		}
		else
		{
			s_pPlayer->ActionMove(PSM_WALK_BACKWARD); // ÈÄÁø..
		}

		if (bStartOrEnd) // ¿òÁ÷ÀÌ±â ½ÃÀÛÇÒ¶§¿¡´Â Ãæµ¹Ã¼Å©¸¦ ¸ÕÀú ÇØº»´Ù..
		{
			float fSpeed = s_pPlayer->MoveSpeedCalculationAndCheckCollision(); // ¼Óµµ¸¦ ±¸ÇÏ°í ±× ¼Óµµ·Î Ãæµ¹ Ã¼Å©¸¦ ÇÑ´Ù. ¸®ÅÏ°ªÀÌ 0 ÀÌ¸é Ãæµ¹ÀÌ´Ù..
			if (0 == fSpeed) // ¸ø¿òÁ÷ÀÌ´Â »óÈ²ÀÌ¸é..
			{
				s_pPlayer->ActionMove(PSM_STOP); // ¸ØÃã..
			}
			else
			{
			}

			if (m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
			if (m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
		}
	}
	else if (MD_STOP == eMD)
	{
		s_pPlayer->m_bMoveContinous = false; // °è¼Ó °ÉÀ½ ¸ØÃã..
		s_pPlayer->ActionMove(PSM_STOP);

		if (m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
		if (m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);

	}

	if (s_pPlayer->m_bAttackContinous) // °ø°ÝÁßÀÌ¸é..
	{
		CPlayerBase* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
		if (s_pPlayer->IsAttackableTarget(pTarget)) // °ø°Ý °¡´ÉÇÏ¸é..
			s_pPlayer->Action(PSA_ATTACK, false, pTarget); // °ø°Ý
	}
}

void CGameProcFxEditor::CommandEnableAttackContinous(bool bEnable, CPlayerBase* pTarget)
{
	if (bEnable == s_pPlayer->m_bAttackContinous) return;
	if (bEnable)
	{
		this->CloseUIs(); // °¢Á¾ »ó°Å·¡, ¿öÇÁµîµî... UI ´Ý±â..
		s_pUIMgr->UserMoveHideUIs();

		if (s_pPlayer->m_bStun) return; // ±âÀýÇØ ÀÖÀ½ °ø°Ý ¸øÇÔ..
		if (NULL == pTarget) return;
		s_pPlayer->RotateTo(pTarget); // ¹æÇâÀ» µ¹¸°´Ù.
		if (pTarget->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation) return; // ±¹°¡°¡ °°À¸¸é ³Ñ¾î°£´Ù..

		//-------------------------------------------------------------------------
		/*
		// TODO(srmeier): need to use ZoneAbilityType here
		// NOTE(srmeier): using zoneability information to determine if target is attackable
		if (!ACT_WORLD->canAttackSameNation() && (pTarget->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation))
			return;
		if (!ACT_WORLD->canAttackOtherNation() && (s_pPlayer->m_InfoBase.eNation == NATION_ELMORAD && pTarget->m_InfoBase.eNation == NATION_KARUS))
			return;
		if (!ACT_WORLD->canAttackOtherNation() && (s_pPlayer->m_InfoBase.eNation == NATION_KARUS && pTarget->m_InfoBase.eNation == NATION_ELMORAD))
			return;
		*/
		//-------------------------------------------------------------------------
	}
	s_pPlayer->m_bAttackContinous = bEnable; // »óÅÂ¸¦ ±â·ÏÇÏ°í..

	if (bEnable)
		SetGameCursor(s_hCursorAttack);
	else
	{
		e_Nation eNation = s_pPlayer->m_InfoBase.eNation;
		SetGameCursor(((NATION_ELMORAD == eNation) ? s_hCursorNormal1 : s_hCursorNormal));
	}

	if (m_pUICmd->m_pBtn_Act_Attack)
	{
		if (bEnable) m_pUICmd->m_pBtn_Act_Attack->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Attack->SetState(UI_STATE_BUTTON_NORMAL);
	}

	if (bEnable) // ÀÚµ¿ °ø°Ý!
	{
		char szMsg[128] = { 0 };
		std::string szFmt;
		::_LoadStringFromResource(IDS_MSG_ATTACK_START, szFmt);
		sprintf(szMsg, szFmt.c_str(), pTarget->IDString().c_str());

		this->PlayBGM_Battle();

		if (s_pPlayer->IsAttackableTarget(pTarget))
			s_pPlayer->Action(PSA_BASIC, true, pTarget);

		this->MsgOutput(szMsg, 0xff00ffff);
	}
	else // ÀÚµ¿ °ø°Ý ¾Æ´Ô.
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_MSG_ATTACK_STOP, szMsg);
		s_pPlayer->Action(PSA_BASIC, true, pTarget);
		this->MsgOutput(szMsg, 0xff00ffff);
	}

	if (bEnable && false == s_pPlayer->IsAttackableTarget(pTarget)) // ±¹°¡, °Å¸® ¹× °¢µµ Ã¼Å©ÇØ¼­ °ø°Ý ºÒ°¡´ÉÇÏ¸é µ¹¾Æ°¡±â..
	{
		std::string szMsg; ::_LoadStringFromResource(IDS_MSG_ATTACK_DISABLE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
	}
}

void CGameProcFxEditor::CommandToggleUIChat()
{
	bool visible = m_pUIChatDlg->IsVisible();

	m_pUIChatDlg->SetVisibleWithNoSound(!visible);
	m_pUIChatDlg2->SetVisibleWithNoSound(visible);
}

void CGameProcFxEditor::CommandToggleUIMsgWnd()
{
	bool visible = m_pUIMsgDlg->IsVisible();

	m_pUIMsgDlg->SetVisibleWithNoSound(!visible);
	m_pUIMsgDlg2->SetVisibleWithNoSound(visible);
}

bool CGameProcFxEditor::CommandToggleUIState()
{
	bool bNeedOpen = !(m_pUIVar->IsVisible());
	if (bNeedOpen)
	{
		s_pUIMgr->SetFocusedUI(m_pUIVar);
		m_pUIVar->Open();
	}
	else m_pUIVar->Close();

	// Ä¿¸Çµå ¹öÆ° ¾÷µ¥ÀÌÆ®..
//	if(m_pUICmd->m_pBtn_Character)
//	{
//		if(bNeedOpen) m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_DOWN); // ¹öÆ° ´©¸§ ÇØÁ¦..
//		else m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_NORMAL); // ¹öÆ° ´©¸§
//	}

	return bNeedOpen;
}

bool CGameProcFxEditor::CommandToggleUIInventory()
{
	bool bNeedOpen = false;

	// °³ÀÎ °Å·¡ÁßÀÌ¸é..
	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return bNeedOpen;

	if (m_pUINpcEvent->IsVisible())
		return bNeedOpen;

	if (m_pUIInn->IsVisible()) return bNeedOpen;
	if (m_pUICreateClanName->IsVisible()) return bNeedOpen;
	if (m_pUIAnvil->IsVisible()) return bNeedOpen;

	if (m_pUIWareHouseDlg->IsVisible())
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close(true);
		return bNeedOpen;
	}

	if (m_pUIItemUpgradeDlg->IsVisible())
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close(true);
		return bNeedOpen;
	}

	if (m_pUITransactionDlg->IsVisible())
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close(true);
		return bNeedOpen;
	}

	if (m_pUIInventory->IsVisible())
	{
		m_pUIInventory->Close(true);
		return bNeedOpen;
	}
	else
	{
		bNeedOpen = true;
		if (m_pUISkillTreeDlg->IsVisible()) m_pUISkillTreeDlg->Close();

		s_pUIMgr->SetFocusedUI(m_pUIInventory);
		m_pUIInventory->Open();
	}

	return bNeedOpen;
}

bool CGameProcFxEditor::CommandToggleUISkillTree()
{
	bool bNeedOpen = !(m_pUISkillTreeDlg->IsVisible());

	// °³ÀÎ °Å·¡ÁßÀÌ¸é..
	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return bNeedOpen;

	if (bNeedOpen)
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close();
		if (m_pUITransactionDlg->IsVisible())
			m_pUITransactionDlg->LeaveTransactionState();
		if (m_pUIWareHouseDlg->IsVisible())
			m_pUIWareHouseDlg->LeaveWareHouseState();
		if (m_pUIItemUpgradeDlg->IsVisible())
			m_pUIItemUpgradeDlg->LeaveAnvilState();
		s_pUIMgr->SetFocusedUI(m_pUISkillTreeDlg);
		m_pUISkillTreeDlg->Open();
	}
	else
	{
		m_pUISkillTreeDlg->Close();
	}

	return bNeedOpen;
}

bool CGameProcFxEditor::CommandToggleUIMiniMap()
{
	return m_pUIStateBarAndMiniMap->ToggleMiniMap();
}

bool CGameProcFxEditor::CommandToggleCmdList()
{

	bool bNeedOpen = !(m_pUICmdListDlg->IsVisible());

	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return bNeedOpen;

	if (bNeedOpen)
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close();
		if (m_pUITransactionDlg->IsVisible())
			m_pUITransactionDlg->LeaveTransactionState();
		if (m_pUIWareHouseDlg->IsVisible())
			m_pUIWareHouseDlg->LeaveWareHouseState();
		if (m_pUIItemUpgradeDlg->IsVisible())
			m_pUIItemUpgradeDlg->LeaveAnvilState();

		s_pUIMgr->SetFocusedUI(m_pUICmdListDlg);
		m_pUICmdListDlg->Open();
	}
	else
	{
		m_pUICmdListDlg->Close();
	}

	return bNeedOpen;
}

bool CGameProcFxEditor::OpenCmdEdit(std::string msg)
{

	bool bNeedOpen = !(m_pUICmdEditDlg->IsVisible());

	if (bNeedOpen)
	{
		s_pUIMgr->SetFocusedUI(m_pUICmdEditDlg);
		m_pUICmdEditDlg->Open(msg);
	}

	return bNeedOpen;
}

void CGameProcFxEditor::CommandCameraChange() // Ä«¸Þ¶ó ½ÃÁ¡ ¹Ù²Ù±â..
{
	if (VP_THIRD_PERSON == s_pEng->ViewPoint() && s_pPlayer->m_bTargetOrPosMove)
	{
		s_pPlayer->m_bMoveContinous = false;
		s_pPlayer->m_bTargetOrPosMove = false;
		CommandToggleMoveContinous();
	}

	s_pEng->ViewPointChange(VP_UNKNOWN); // ¼ø¼­´ë·Î ½ÃÁ¡À» ¹Ù²Û´Ù..
}

void CGameProcFxEditor::MsgOutput(const std::string& szMsg, D3DCOLOR crMsg)
{
	m_pUIMsgDlg->AddMsg(szMsg, crMsg);
}

void CGameProcFxEditor::CommandSitDown(bool bLimitInterval, bool bSitDown, bool bImmediately)
{
	if (bSitDown == s_pPlayer->m_bSitDown) return; // »óÅÂ°¡ °°À¸¸é µ¹¾Æ°£´Ù..

	// ¾É±â ¼­±â Á¦ÇÑ..
	if (bLimitInterval)
	{
		float fTime = CN3Base::TimeGet();
		static float fTimePrev = fTime - 4.0f;
		if (fTime - fTimePrev < 3.0f) return;
		fTimePrev = fTime;
	}

	e_StateAction eSA = s_pPlayer->State();
	e_StateMove eSM = s_pPlayer->StateMove();

	if (bSitDown)
	{
		if (eSM != PSM_STOP) // ¶Ù´ø ³ðÀÌ¸é ¸ØÃá´Ù.
			return;

		if (this->m_pMagicSkillMng->IsCasting()) // ½ºÅ³ Ä³½ºÆÃ ÁßÀÌ¸é ¾ÉÁö ¸øÇÑ´Ù..
			return;

		if (s_pPlayer->m_bAttackContinous) // °è¼Ó °ø°ÝÇÏ´Â ÁßÀÌ¸é..
			this->CommandEnableAttackContinous(false, NULL); // °è¼Ó °ø°Ý Ãë¼Ò..
	}

	bool bVBs[2] = { false, false };
	int iState = 0;
	if (bSitDown)
	{
		eSA = PSA_SITDOWN;
		bVBs[0] = false; bVBs[1] = true;
		iState = 0x02;
	} // ¾É´Â °æ¿ì
	else
	{
		eSA = PSA_BASIC;
		bVBs[0] = true; bVBs[1] = false;
		iState = 0x01;
	} // ÀÏ¾î¼­´Â °æ¿ì

	s_pPlayer->m_bSitDown = bSitDown;
	s_pPlayer->Action(eSA, true, NULL, bImmediately);

	if (m_pUICmd->m_pBtn_Act_SitDown)
	{
		m_pUICmd->m_pBtn_Act_SitDown->SetVisible(bVBs[0]);
		m_pUICmd->m_pBtn_Act_SitDown->SetState(UI_STATE_BUTTON_DOWN); // ¹öÆ° ´©¸§
	}
	if (m_pUICmd->m_pBtn_Act_StandUp)
	{
		m_pUICmd->m_pBtn_Act_StandUp->SetVisible(bVBs[1]);
		m_pUICmd->m_pBtn_Act_StandUp->SetState(UI_STATE_BUTTON_DOWN); // ¹öÆ° ´©¸§
	}

}

void CGameProcFxEditor::CommandTargetSelect_NearstEnemy() // °¡Àå °¡±î¿î Àû Å¸°Ù Àâ±â..
{
	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByNearstEnemy(s_pPlayer->m_InfoBase.eNation, s_pPlayer->Position());
	this->TargetSelect(pTarget);
	s_pPlayer->RotateTo(pTarget);
}

void CGameProcFxEditor::CommandTargetSelect_NearstOurForce() // °¡Àå °¡±î¿î ÆÄÆ¼ Å¸°ÙÀâ±â..
{
	CPlayerOther* pTarget = m_pUIPartyOrForce->MemberGetByNearst(s_pPlayer->Position());
	this->TargetSelect(pTarget);
	s_pPlayer->RotateTo(pTarget);
}

void CGameProcFxEditor::CloseUIs()
{
}

void CGameProcFxEditor::TargetSelect(int iID, bool bMustAlive)
{
	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(iID, bMustAlive);
	this->TargetSelect(pTarget);
}

void CGameProcFxEditor::TargetSelect(CPlayerNPC* pTarget)
{
	if (PSA_SPELLMAGIC == s_pPlayer->State()) // ¸¶¹ý Ä³½ºÆÃÀ» ÇÏ´Â Áß°£¿¡´Â Å¸°ÙÀ» ¸ø ¹Ù²Û´Ù..
		return;

	int iIDTargetPrev = s_pPlayer->m_iIDTarget; // ÀüÀÇ Å¸°Ù ±â¾ï..
	s_pPlayer->m_iIDTarget = -1;
	s_pPlayer->m_pObjectTarget = NULL;

	if (pTarget && pTarget->m_InfoBase.iAuthority != AUTHORITY_MANAGER)
	{ //¿î¿µÀÚ´Â Æ÷Ä¿½º¸¦ ÁÖÁö ¾Ê°Ô ÇÏ±â À§ÇØ¼­...
		s_pPlayer->m_iIDTarget = pTarget->IDNumber();
		if (iIDTargetPrev != s_pPlayer->m_iIDTarget) // ÀüÀÇ Å¸°Ù°ú ´Ù¸£¸é
		{
			this->CommandEnableAttackContinous(false, NULL);	// ÀÚµ¿ °ø°Ý Ãë¼Ò..

			D3DCOLOR crID = 0xffffffff;
			if (pTarget->PlayerType() == PLAYER_OTHER) // User..
			{
				if (pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) crID = 0xffff4040; // ´Ù¸¥ ±¹°¡ÀÌ¸é
				else crID = 0xff6b9fff;
			}
			else // NPC
			{
				if (pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) crID = 0xffff6060; // ´Ù¸¥ ±¹°¡ÀÌ¸é
				else crID = 0xff1064ff;
			}

			if (pTarget->IsAlive())//»ì¾ÆÀÖ´Â »óÅÂ¸¸ target bar¸¦ º¸¿©ÁØ´Ù.
			{
				m_pUITargetBar->SetVisible(true);
				m_pUITargetBar->SetIDString(pTarget->IDString(), crID); // ÀÌ¸§À» ³Ö°í..
			}
			else
			{
				m_pUITargetBar->SetVisible(false); // Å¸°Ù ¹Ù ¾ø¾Ö±â..
			}
		}
	}
	else
	{
		s_pPlayer->m_iIDTarget = -1; // Å¸°Ù ¾ÆÀÌµð¸¦ Àâ°í..
		m_pUITargetBar->SetVisible(false); // Å¸°Ù ¹Ù ¾ø¾Ö±â..

		this->CommandEnableAttackContinous(false, NULL); // ÀÚµ¿ °ø°Ý Ãë¼Ò..
	}

	if (PSA_SITDOWN != s_pPlayer->State() &&
		PSM_STOP == s_pPlayer->StateMove() &&
		PSA_BASIC == s_pPlayer->State()) s_pPlayer->Action(PSA_BASIC, true); // ±âº» ÀÚ¼¼ ¹Ù²Ù±â..

	this->UpdateUI_PartyOrForceButtons(); // Ä¿¸Çµå ÁÙ¿¡ ÀÖ´Â ÆÄÆ¼ ¹öÆ°À» »óÈ²¿¡ µû¶ó ¾÷µ¥ÀÌÆ® ÇØÁØ´Ù.
}

void CGameProcFxEditor::UpdateUI_PartyOrForceButtons()
{
	// ÆÄÆ¼ ¹öÆ° »óÅÂ ¹Ù²Ù±â..
	CPlayerBase* pTarget = NULL;
	bool bIAmLeader = false, bIAmMemberOfParty = false;
	int iMemberIndex = -1;
	this->PartyOrForceConditionGet(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
	m_pUIPartyOrForce->MemberSelect(iMemberIndex);

	m_pUICmd->UpdatePartyButtons(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
}

const __InfoPartyOrForce* CGameProcFxEditor::PartyOrForceConditionGet(bool& bIAmLeader, bool& bIAmMember, int& iMemberIndex, class CPlayerBase*& pTarget)
{
	// ÆÄÆ¼ ¹öÆ° »óÅÂ ¹Ù²Ù±â..
	bIAmLeader = false;
	iMemberIndex = -1;
	bIAmMember = false;
	pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);

	if (m_pUIPartyOrForce->MemberCount() >= 2)
	{
		bIAmMember = true;
		if (m_pUIPartyOrForce->MemberInfoGetByIndex(0)->iID == s_pPlayer->IDNumber()) bIAmLeader = true;
		return m_pUIPartyOrForce->MemberInfoGetByID(s_pPlayer->m_iIDTarget, iMemberIndex);
	}

	return NULL;
}

void CGameProcFxEditor::UpdateUI_MiniMap()
{
	if (NULL == m_pUIStateBarAndMiniMap || !m_pUIStateBarAndMiniMap->IsVisible()) return;


	m_pUIStateBarAndMiniMap->PositionInfoClear();

	D3DCOLOR crType = 0xffffffff;
	e_Nation eNation = s_pPlayer->m_InfoBase.eNation;

	it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
	CPlayerNPC* pNPC = NULL;
	for (; it != itEnd; it++) // NPC
	{
		pNPC = it->second;

		if (eNation != pNPC->m_InfoBase.eNation) crType = 0xff800000; // ´Ù¸¥ ±¹°¡ NPC È¤Àº ¸ó½ºÅÍ ÁÖÈ²»ö
		else crType = 0xff00a0ff; // °°Àº ±¹°¡ NPC ÇÏ´Ã»ö

		m_pUIStateBarAndMiniMap->PositionInfoAdd(pNPC->IDNumber(), pNPC->Position(), crType, false);
	}

	it_UPC it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
	CPlayerOther* pUPC = NULL;
	__TABLE_ZONE* pZoneInfo = s_pTbl_Zones.Find(s_pPlayer->m_InfoExt.iZoneCur);
	for (; it2 != itEnd2; it2++) // User
	{
		pUPC = it2->second;

		bool bDrawTop = false;
		if (eNation != pUPC->m_InfoBase.eNation) // Àû±¹ÀÏ°æ¿ì
		{
			if (pUPC->State() == PSA_SITDOWN)
			{
				pUPC->m_InfoBase.bRenderID = false; // ¾ÆÀÌµð Ç¥½ÃÇÏÁö ¾ÊÀ½.
				continue; // ¾É¾ÆÀÖÀ¸¸é.. Áö³ª°£´Ù..
			}
			else
			{
				pUPC->m_InfoBase.bRenderID = true; // ¾ÆÀÌµð Ç¥½ÃÇÔ.
			}

			if (pZoneInfo && FALSE == pZoneInfo->bIndicateEnemyPlayer) continue; // Àû±¹ÀÌ Ç¥½Ã µÇÁö ¾Ê¾Æ¾ß ÇÑ´Ù¸é.. Áö³ª°£´Ù..

			crType = 0xffff4040; // ´Ù¸¥ ±¹°¡  - ¹àÀº »¡°£»ö
		}
		else // °°Àº ±¹°¡¸é..
		{
			int iMemberIndex = -1;
			const __InfoPartyOrForce* pPI = m_pUIPartyOrForce->MemberInfoGetByID(pUPC->IDNumber(), iMemberIndex);
			if (pPI)
			{
				bDrawTop = true; // ¸Ç À§¿¡ ±×¸°´Ù.
				crType = 0xffffff00; // ÆÄÆ¼ ¸â¹ö¸é // ³ë¶õ»ö
			}
			else
			{
				crType = 0xff0040ff; // ÆÄÆ¼ ¸â¹ö°¡ ¾Æ´Ï¸é ¹àÀº ÆÄ¶õ»ö
			}
		}

		if (pUPC && pUPC->m_InfoBase.iAuthority != AUTHORITY_MANAGER)	// ¿î¿µÀÚ°¡ ¾Æ´Ñ°æ¿ì¸¸ ¹Ì´Ï¸Ê¿¡ Æ÷ÀÎÆ®¸¦ Âï¾îÁØ´Ù.
			m_pUIStateBarAndMiniMap->PositionInfoAdd(pUPC->IDNumber(), pUPC->Position(), crType, bDrawTop);
	}
}

void CGameProcFxEditor::UpdateUI_TargetBar()
{
	if (NULL == m_pUITargetBar || !m_pUITargetBar->IsVisible()) return;

	//Á×Àº Ä³¸¯ÅÍ°¡ ¼±ÅÃµÇ¾úÀ»¶§´Â target bar¸¦ ±×·ÁÁÖÁö ¾Ê´Â´Ù.
	CPlayerNPC* t_pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
	if (t_pTarget && t_pTarget->State() == PSA_DEATH)
	{
		m_pUITargetBar->SetVisible(false);
		return;
	}

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);
	if (pTarget) // Å¸°ÙÀÌ À¯È¿ÇÏ¸é..
	{
		float fTimeTmp = CN3Base::TimeGet();
		if (fTimeTmp > m_pUITargetBar->m_fTimeSendPacketLast + PACKET_INTERVAL_REQUEST_TARGET_HP) // 1ÃÊ°¡ Áö³ª¸é Å¸°Ù Á¤º¸ ¿äÃ»
		{
			m_pUITargetBar->m_fTimeSendPacketLast = fTimeTmp;
		}
	}
	else // Å¸°ÙÀÌ ¾øÀ¸¸é..
	{
		s_pPlayer->m_iIDTarget = -1;
		m_pUITargetBar->SetVisible(false);
	}
}

void CGameProcFxEditor::UpdateBGM()
{
	if (NULL == m_pSnd_Battle || !m_pSnd_Battle->IsPlaying()) return;

	//	if(s_pPlayer->pTarget && s_pPlayer->pTarget->IsAlive()) 
	__Vector3 vPosPlayer = s_pPlayer->Position();
	e_Nation eNationPlayer = s_pPlayer->m_InfoBase.eNation;

	bool bStopBattleBgm = true;
	CPlayerBase* pBPC;
	it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
	for (; it != itEnd && bStopBattleBgm; it++)
	{
		pBPC = it->second;
		if (eNationPlayer == pBPC->m_InfoBase.eNation) continue;

		if ((vPosPlayer - pBPC->Position()).Magnitude() < 12.0f)
			bStopBattleBgm = false;
	}

	CPlayerOther* pUPC;
	it_UPC it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
	for (; it2 != itEnd2 && bStopBattleBgm; it2++)
	{
		pUPC = it2->second;
		if (eNationPlayer == pUPC->m_InfoBase.eNation) continue;

		if ((vPosPlayer - pUPC->Position()).Magnitude() < 12.0f)
			bStopBattleBgm = false;
	}

	if (bStopBattleBgm) this->PlayBGM_Town();
}

void CGameProcFxEditor::UpdateCameraAndLight()
{
	__Vector3 vPosPlayer = s_pPlayer->Position();
	if (s_pPlayer->State() == PSA_SITDOWN)
	{
		float fRootY = s_pPlayer->RootPosition().y;
		float fH = s_pPlayer->Height();
		vPosPlayer.y += fRootY - (fH / 2.0f);
	}

	D3DCOLOR crDiffuses[MAX_GAME_LIGHT];
	D3DCOLOR crAmbients[MAX_GAME_LIGHT];
	for (int i = 0; i < MAX_GAME_LIGHT; i++)
	{
		crDiffuses[i] = ACT_WORLD->GetLightDiffuseColorWithSky(i);
		crAmbients[i] = ACT_WORLD->GetLightAmbientColorWithSky(i);
	}

	s_pEng->Tick(crDiffuses, crAmbients, ACT_WORLD->GetFogColorWithSky(),
		vPosPlayer, s_pPlayer->Rotation(),
		s_pPlayer->Height(), ACT_WORLD->GetSunAngleByRadinWithSky());		// Ä³¸¯ÅÍ À§Ä¡¿Í ÇØÀÇ °¢µµ¸¦ ³Ö¾îÁØ´Ù..
	s_pEng->ApplyCameraAndLight();	// Ä«¸Þ¶ó¿Í ¶óÀÌÆ®¿¡ ¼¼ÆÃµÈ °ªÀ» D3D Device ¿¡ Àû¿ëÇÑ´Ù.
}

void CGameProcFxEditor::ItemUpgradeOpen(uint16_t npcId)
{
	m_pUIItemUpgradeDlg->SetNpcId(npcId);
	m_pUIItemUpgradeDlg->EnterItemUpgradeStateStart();

	if (!m_pUIItemUpgradeDlg->IsVisible())
		m_pUIItemUpgradeDlg->SetVisible(true);

	if (m_pUIInventory->IsVisible())		// ÀÎº¥Åä¸®°¡ ¾È¿­·Á ÀÖÀ¸¸é..
		this->CommandToggleUIInventory();

	if (m_pUISkillTreeDlg->IsVisible())
		m_pUISkillTreeDlg->Close();

	m_pUIItemUpgradeDlg->EnterItemUpgradeStateEnd();
}

void CGameProcFxEditor::PlayBGM_Town()
{
	if (m_pSnd_Battle) m_pSnd_Battle->Stop(3.0f);
	if (NULL == m_pSnd_Town || m_pSnd_Town->IsPlaying()) return;
	m_pSnd_Town->SetMaxVolume(60);
	m_pSnd_Town->Play(NULL, 3.0f); // ÀüÅõ À½¾Ç ¼³Á¤.. ÇØÁ¦´Â ÁÖÀ§¿¡ ¸ó½ºÅÍ°¡ ¾øÀ»¶§ ÇÑ´Ù..
}

void CGameProcFxEditor::PlayBGM_Battle()
{
	if (m_pSnd_Town) m_pSnd_Town->Stop(3.0f);
	if (NULL == m_pSnd_Battle || m_pSnd_Battle->IsPlaying()) return;
	m_pSnd_Battle->SetMaxVolume(80);
	m_pSnd_Battle->Play(NULL, 3.0f); // ÀüÅõ À½¾Ç ¼³Á¤.. ÇØÁ¦´Â ÁÖÀ§¿¡ ¸ó½ºÅÍ°¡ ¾øÀ»¶§ ÇÑ´Ù..
}

void CGameProcFxEditor::ReleaseSound()
{
	if (ACT_WORLD && ACT_WORLD->GetSkyRef()) ACT_WORLD->GetSkyRef()->ReleaseSound();
	CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSnd_Town);
	CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSnd_Battle);
}


void CGameProcFxEditor::ControlViewVDegree(int16_t sValue)
{
	if (s_pEng->ViewPoint() == VP_THIRD_PERSON)
	{
		float fPitch = 0;
		if (sValue > 0)	fPitch = D3DXToRadian(45.0f) * 3;
		else			fPitch = D3DXToRadian(-45.0f) * 3;
		if (fPitch) s_pEng->CameraPitchAdd(fPitch);
	}
}
/*
bool CGameProcFxEditor::OnMouseMove(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	if(ptCur.x != ptPrev.x || ptCur.y != ptPrev.y)
	{
		if(m_pMagicSkillMng->m_dwRegionMagicState==1)
		{
			__Vector3 vPick;
			CPlayerNPC*	 pTarget = NULL;

			int iID = -1;
			pTarget = s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &vPick); // »ç¹æ¿¡ ±ò¸°³Ñµé ÇÈÅ·..
			this->TargetSelect(iID, false); // Å¸°ÙÀ» Àâ´Â´Ù..
			if(NULL == pTarget) // Å¸°ÙÀÌ ¾øÀ¸¸é..
			{
				s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &vPick); // ÂïÈù À§Ä¡¸¦ ÀúÀåÇÑ´Ù..
				if(NULL == s_pPlayer->m_pObjectTarget) // Å¸°Ùµµ ¾øÀ¸¸é..
				{
					ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vPick); // ÁöÇüÀ» Âï¾îº»´Ù..
				}
			}
			s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, vPick);
		}
	}

	return true;
}
*/
bool CGameProcFxEditor::OnMouseMove(POINT ptCur, POINT ptPrev)
{
	if (s_pUIMgr->m_bDoneSomething) return false;

	// NOTE: check if cursor position has changed
	if (ptCur.x != ptPrev.x || ptCur.y != ptPrev.y)
	{
		// NOTE: check if something has been selected by the cursor
		if (m_pMagicSkillMng->m_dwRegionMagicState == 1)
		{
			__Vector3 vNormal, vMyPos, vGap, vDir;

			vMyPos = s_pPlayer->Position();
			vMyPos.y += s_pPlayer->Height() / 2;

			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, m_vMouseSkillPos); // ÁöÇüÀ» Âï¾îº»´Ù..

			vDir = m_vMouseSkillPos - vMyPos;
			vGap = vDir;
			vDir.Normalize();

			bool bColShape = ACT_WORLD->CheckCollisionWithShape(vMyPos, vDir, vGap.Magnitude(), &m_vMouseSkillPos, &vNormal);

			if (!bColShape) // Å¸°Ùµµ ¾øÀ¸¸é..
			{
				int iID = -1;
				s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &m_vMouseSkillPos); // »ç¹æ¿¡ ±ò¸°³Ñµé ÇÈÅ·..
				this->TargetSelect(iID, false); // Å¸°ÙÀ» Àâ´Â´Ù..
			}
			else
			{
				m_vMouseSkillPos.y = ACT_WORLD->GetHeightWithTerrain(m_vMouseSkillPos.x, m_vMouseSkillPos.z);
			}
			s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseSkillPos);
		}
	}

	return true;
}

// ¿ÞÂÊ ´õºí Å¬¸¯
bool CGameProcFxEditor::OnMouseLDBtnPress(POINT ptCur, POINT ptPrev)
{
	if (s_pUIMgr->m_bDoneSomething) return false;

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);

	if (pTarget && pTarget->m_InfoBase.iAuthority == AUTHORITY_MANAGER)
	{
		s_pPlayer->m_iIDTarget = -1;
		pTarget = NULL;
	}

	if (VP_THIRD_PERSON == s_pEng->ViewPoint())
	{
		if (s_pPlayer->IsAttackableTarget(pTarget, false))
		{
			this->CommandMove(MD_STOP, true);
			this->CommandEnableAttackContinous(true, pTarget); // ÀÚµ¿ °ø°Ý
		}
		else if (pTarget && VP_THIRD_PERSON == s_pEng->ViewPoint())
		{
			this->CommandMove(MD_FOWARD, true);
			s_pPlayer->SetMoveTargetID(s_pPlayer->m_iIDTarget);
		}
	}
	else
	{
		s_pPlayer->m_bAttackContinous = false;
		CommandToggleAttackContinous();
	}
	return true;
}

// ¿ÞÂÊ Å¬¸¯
bool CGameProcFxEditor::OnMouseLBtnPress(POINT ptCur, POINT ptPrev)
{
	if (s_pUIMgr->m_bDoneSomething) return false;

	POINT ptPlayer = ::_Convert3D_To_2DCoordinate(s_pPlayer->Position(),
		CN3Base::s_CameraData.mtxView,
		CN3Base::s_CameraData.mtxProjection,
		CN3Base::s_CameraData.vp.Width,
		CN3Base::s_CameraData.vp.Height);

	__Vector3 vDir((float)(ptCur.x - ptPlayer.x), 0, (float)(ptPlayer.y - ptCur.y));
	__Matrix44 mtxTmp; mtxTmp.RotationY(s_pEng->CameraYaw());
	vDir *= mtxTmp;
	vDir.Normalize();
	float fYaw = ::_Yaw2D(vDir.x, vDir.z);

	CPlayerNPC* pTarget = NULL;
	BOOL		 bFindCorpse = false;

	int iID = -1;
	pTarget = s_pOPMgr->PickPrecisely(ptCur.x, ptCur.y, iID, &m_vMouseLBClickedPos); // »ç¹æ¿¡ ±ò¸°³Ñµé ÇÈÅ·..
	this->TargetSelect(iID, false); // Å¸°ÙÀ» Àâ´Â´Ù..
	if (NULL == pTarget) // Å¸°ÙÀÌ ¾øÀ¸¸é..
	{
		if (s_pPlayer->m_bAttackContinous) // °è¼Ó °ø°ÝÇÏ´Â ÁßÀÌ¸é..
			this->CommandEnableAttackContinous(false, NULL); // °è¼Ó °ø°Ý Ãë¼Ò..

		s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &m_vMouseLBClickedPos); // ÂïÈù À§Ä¡¸¦ ÀúÀåÇÑ´Ù..
		if (NULL == s_pPlayer->m_pObjectTarget) // Å¸°Ùµµ ¾øÀ¸¸é..
		{
			// ½ÃÃ¼ µÚÀú¼­ ¾ÆÀÌÅÛ »óÀÚ ¿­±â..
			CPlayerNPC* pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // ÇÈÅ·..
		
		}
	}

	if (m_pMagicSkillMng->m_dwRegionMagicState == 1)
	{
		//		s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseLBClickedPos);
		s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseSkillPos);
		m_pMagicSkillMng->m_dwRegionMagicState = 2;
	}

	if (!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint())
	{
		m_fLBClickTime = CN3Base::TimeGet();
		this->CommandSitDown(false, false); // ÀÏ´Ü ÀÏÀ¸ÄÑ ¼¼¿î´Ù..

		if (pTarget)
		{
			if (s_pLocalInput->IsKeyDown(DIK_LCONTROL) || s_pLocalInput->IsKeyDown(DIK_RCONTROL))
			{
				if (s_pPlayer->IsAttackableTarget(pTarget, false))
				{
					if (s_pPlayer->m_bTargetOrPosMove)
						this->CommandMove(MD_STOP, true);

					this->CommandEnableAttackContinous(true, pTarget); // ÀÚµ¿ °ø°Ý
				}
				else
				{
					this->CommandMove(MD_FOWARD, true);
					s_pPlayer->SetMoveTargetID(s_pPlayer->m_iIDTarget);
				}
			}
			else
			{
				if (s_pPlayer->m_bTargetOrPosMove && s_pPlayer->m_iMoveTarget != pTarget->IDNumber())
					this->CommandMove(MD_STOP, true);
			}
		}
		else if (!bFindCorpse)
		{
			s_pPlayer->RotateTo(fYaw, true);
			__Vector3 vMovePoint;
			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ÁöÇüÀ» Âï¾îº»´Ù..

			float fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			if (!s_pPlayer->m_bTargetOrPosMove && fDist > 1.5f)
				this->CommandMove(MD_FOWARD, true);

			if (fDist > 1.5f)
			{
				s_pPlayer->SetMoveTargetPos(vMovePoint);
			}
			else if (s_pPlayer->m_bTargetOrPosMove)
			{
				this->CommandMove(MD_STOP, true);
			}
		}
	}
	else if (!s_pPlayer->IsDead())
	{
		if (pTarget && (s_pLocalInput->IsKeyDown(DIK_LCONTROL) || s_pLocalInput->IsKeyDown(DIK_RCONTROL)))
		{
			if (s_pPlayer->IsAttackableTarget(pTarget, false))
			{
				if (s_pPlayer->m_bTargetOrPosMove)
					this->CommandMove(MD_STOP, true);

				this->CommandEnableAttackContinous(true, pTarget); // ÀÚµ¿ °ø°Ý
			}
			else
			{
				s_pPlayer->m_bAttackContinous = false;
				CommandToggleAttackContinous();
			}
		}
	}

	return true;
}

// ¿ÞÂÊ Å¬¸¯¾÷
bool CGameProcFxEditor::OnMouseLBtnPressd(POINT ptCur, POINT ptPrev)
{
	if (!s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD))
	{
		//		if(!s_pPlayer->m_bTargetOrPosMove)
		//			this->CommandMove(MD_STOP, true);
	}
	return true;
}

// ¿ÞÂÊ ´­¸®°í ÀÖÀ»¶§
bool CGameProcFxEditor::OnMouseLbtnDown(POINT ptCur, POINT ptPrev)
{
	if (s_pUIMgr->m_bDoneSomething) return false;

	POINT ptPlayer = ::_Convert3D_To_2DCoordinate(s_pPlayer->Position(),
		CN3Base::s_CameraData.mtxView,
		CN3Base::s_CameraData.mtxProjection,
		CN3Base::s_CameraData.vp.Width,
		CN3Base::s_CameraData.vp.Height);

	__Vector3 vDir((float)(ptCur.x - ptPlayer.x), 0, (float)(ptPlayer.y - ptCur.y));
	__Matrix44 mtxTmp; mtxTmp.RotationY(s_pEng->CameraYaw());
	vDir *= mtxTmp;
	vDir.Normalize();
	float fYaw = ::_Yaw2D(vDir.x, vDir.z);

	if (!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint() && !s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD)) // »ïÀÎÄª ½ÃÁ¡ÀÌ¸é.. UI ¸¦ °ÇµéÁö ¾Ê¾ÒÀ¸¸é..
	{
		__Vector3 vMovePoint;
		float fDist;
		float fNowTime = CN3Base::TimeGet();

		if (fNowTime - m_fLBClickTime > 0.1f && s_pPlayer->m_bTargetOrPosMove)
		{
			if (s_pPlayer->m_bAttackContinous)
				CommandToggleAttackContinous();


			this->CommandSitDown(false, false); // ÀÏ´Ü ÀÏÀ¸ÄÑ ¼¼¿î´Ù..
			s_pPlayer->RotateTo(fYaw, true);
			this->CommandMove(MD_FOWARD, false);

			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ÁöÇüÀ» Âï¾îº»´Ù..
			fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			s_pPlayer->SetMoveTargetPos(vMovePoint);
		}
		else if (fNowTime - m_fLBClickTime > 0.1f && !s_pPlayer->m_bTargetOrPosMove)
		{
			if (s_pPlayer->m_bAttackContinous)
				CommandToggleAttackContinous();

			s_pPlayer->RotateTo(fYaw, true);
			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ÁöÇüÀ» Âï¾îº»´Ù..

			fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			if (fDist > 1.5f)
			{
				this->CommandMove(MD_FOWARD, true);
				s_pPlayer->SetMoveTargetPos(vMovePoint);
			}
		}
	}
	return true;
}

// ¿À¸¥ÂÊ Å¬¸¯
bool CGameProcFxEditor::OnMouseRBtnPress(POINT ptCur, POINT ptPrev)
{
	if (s_pUIMgr->m_bDoneSomething) return false;

	if (s_pLocalInput->IsKeyDown(DIK_LCONTROL) || s_pLocalInput->IsKeyDown(DIK_RCONTROL))
	{
		m_pUIHotKeyDlg->EffectTriggerByMouse();
		return true;
	}

	int iID = -1;
	CPlayerNPC* pNPC = s_pOPMgr->PickNPC(ptCur.x, ptCur.y, iID); // ÇÈÅ·..

	if (NULL == pNPC)
	{
		// NOTE: sending the packet to pick up items
		CPlayerNPC* pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // ÇÈÅ·..
		
	}
	else if (pNPC->IDNumber() == s_pPlayer->m_iIDTarget) // Å¸°ÙÀ¸·Î ÂïÀº Ä³¸¯ÅÍ¿Í ÇÇÅ·ÇÑ Ä³¸¯ÅÍ°¡ °°°í ..) 
	{
		if (pNPC->m_pShapeExtraRef) // ¿ÀºêÁ§Æ® ÇüÅÂÀÇ NPC ÀÌ¸é.. ÄÁÆ®·Ñ ÇÒ NPCÀÇ ID °¡ ÀÖÀ¸¸é..
		{
			//if (pNPC->m_pShapeExtraRef->m_iNPC_ID > 0) // anvil için iNPC_ID 0 geliyor acaba bu kontrol gerekli mi ?
			{
				float fD = (s_pPlayer->Position() - pNPC->m_pShapeExtraRef->Pos()).Magnitude();
				float fDLimit = (s_pPlayer->Radius() + pNPC->m_pShapeExtraRef->Radius()) * 2.0f;
				if (fD > fDLimit) // °Å¸®°¡ ¸Ö¸é
				{
					std::string szMsg;
					::_LoadStringFromResource(IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
				}
			}
		}
		else // º¸Åë NPC ÀÌ¸é..
		{
			// NOTE: an NPC has been clicked on
			// TODO(srmeier): need to use ZoneAbilityType here
			// NOTE(srmeier): using the zone type to decide if you can talk with NPC
			if (ACT_WORLD->GetZoneType() == ZoneAbilityNeutral || (pNPC->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation)) // °°Àº ±¹°¡ ÀÏ¶§¸¸..
			{
				float fD = (s_pPlayer->Position() - pNPC->Position()).Magnitude();
				float fDLimit = (s_pPlayer->Radius() + pNPC->Radius()) * 3.0f;
				if (fD > fDLimit) // °Å¸®°¡ ¸Ö¸é
				{
					std::string szMsg;
					::_LoadStringFromResource(IDS_ERR_REQUEST_NPC_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
					s_pPlayer->ActionMove(PSM_STOP); // ÀÏ´Ü ¸ØÃß°í..
					pNPC->RotateTo(s_pPlayer); // ¹æÇâÀ» ÇÃ·¹ÀÌ¾î ÂÊÀ¸·Î µ¹¸°´Ù.
					if (m_pUITransactionDlg) m_pUITransactionDlg->m_iNpcID = pNPC->IDNumber();
				}
			}
		}
	}
	return true;
}

// ¿À¸¥ÂÊ Å¬¸¯¾÷
bool CGameProcFxEditor::OnMouseRBtnPressd(POINT ptCur, POINT ptPrev)
{
	return true;
}

// ¿À¸¥ÂÊ ´­¸®°í ÀÖÀ»¶§
bool CGameProcFxEditor::OnMouseRbtnDown(POINT ptCur, POINT ptPrev)
{
	float fMouseSensivity = 0.05f;

	float fRotY = D3DXToRadian(180.0f) * ((ptCur.x - ptPrev.x) * fMouseSensivity); // È¸ÀüÇÒ ¾çÀ» °è»êÇÏ°í..
	float fRotX = D3DXToRadian(180.0f) * ((ptCur.y - ptPrev.y) * fMouseSensivity);
	if (fRotY && s_pPlayer->IsAlive())
	{
		if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(fRotY);
		else if (false == s_pPlayer->m_bStun) s_pPlayer->RotAdd(fRotY); // ±âÀýÇØ ÀÖÁö ¾ÊÀ»¶§¸¸..
	}
	if (fRotX)
	{
		if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraZoom(-fRotX); // Ä«¸Þ¶ó È®´ë
		else s_pEng->CameraPitchAdd(fRotX); // Ä«¸Þ¶ó °¢µµ
	}

	if (fRotY || fRotX)
	{
		SetGameCursor(NULL);
		s_pLocalInput->MouseSetPos(ptPrev.x, ptPrev.y);
		::ClientToScreen(CN3Base::s_hWndBase, &ptPrev);
		::SetCursorPos(ptPrev.x, ptPrev.y);
	}

	return true;
}

// ¿À¸¥ÂÊ ´õºí Å¬¸¯
bool CGameProcFxEditor::OnMouseRDBtnPress(POINT ptCur, POINT ptPrev)
{
	if (s_pUIMgr->m_bDoneSomething) return false;

	//½ºÅ³ ¸ÅÁ÷ÀÌ »ç¿ëµÇ¾ú´Ù¸é....
	m_pUIHotKeyDlg->EffectTriggerByMouse();

	return true;
}

// When Mouse Middle Button Pressed - This will need to be change to 180* Camera Turn
bool CGameProcFxEditor::OnMouseMBtnPress(POINT ptCur, POINT ptPrev)
{
	if (s_pPlayer->IsAlive())
	{
		if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->ActivateCameraTurnArround();
	}

	return true;
}

void CGameProcFxEditor::ProcessUIKeyInput(bool bEnable)
{
	if (m_pUIChatDlg && !m_pUIChatDlg->IsChatMode())
	{
		CGameProcedure::ProcessUIKeyInput();
		if (s_pLocalInput->IsKeyPress(DIK_RETURN) && !s_bKeyPress)
		{
			if (!CGameProcedure::s_pUIMgr->GetFocusedEdit())
				m_pUIChatDlg->SetFocus();
		}
	}
	else if (m_pUIChatDlg && m_pUIChatDlg->IsChatMode())
	{
		s_bKeyPress = false;
		if (m_pUIChatDlg->GetEnableKillFocus())
		{
			m_pUIChatDlg->SetEnableKillFocus(false);
			m_pUIChatDlg->KillFocus();
		}
	}
}


void CGameProcFxEditor::NoahTrade(uint8_t bType, uint32_t dwGoldOffset, uint32_t dwGold)
{
	char szBuf[256] = "";
	std::string szMsg;

	switch (bType)
	{
	case N3_SP_NOAH_GET:
		::_LoadStringFromResource(IDS_TRADE_COIN_RECV, szMsg);
		sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
		MsgOutput(szBuf, 0xff6565ff);
		break;

	case N3_SP_NOAH_LOST:
		::_LoadStringFromResource(IDS_TRADE_COIN_PAID, szMsg);
		sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
		MsgOutput(szBuf, 0xffff3b3b);
		break;
	}

	//s_pPlayer->m_InfoExt.iGold = dwGold;
	if (m_pUIInventory->IsVisible())
		m_pUIInventory->GoldUpdate();
	if (m_pUITransactionDlg->IsVisible())
		m_pUITransactionDlg->GoldUpdate();
	if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
		m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();
}