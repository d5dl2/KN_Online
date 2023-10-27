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
static std::string s_szCmdMsg[CMD_COUNT]; // 게임상 명령어



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcFxEditor::CGameProcFxEditor()				// r기본 생성자.. 각 변수의 역활은 헤더 참조..
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
	m_pUIKnightsOp = new CUIKnightsOperation();			// 기사단 리스트 보기, 가입, 등...
	m_pUIPartyBBS = new CUIPartyBBS(); // 파티 지원 시스템 게시판??..
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
	m_pTargetSymbol = new CN3Shape(); // 플레이어가 타겟으로 잡은 캐릭터의 위치위에 그리면 된다..
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
	delete m_pTargetSymbol; // 플레이어가 타겟으로 잡은 캐릭터의 위치위에 그리면 된다..
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
	m_pUIKnightsOp->Release();			// 기사단 리스트 보기, 가입, 등...
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
	s_pPlayer->Release(); // 일단 몽창 다 해제 하고....
	s_pPlayer->m_InfoExt.iZoneCur = iZone;

	s_pPlayer->IDSet(0, "d5dl2", D3DCOLOR_XRGB(100, 210, 255)); // 밝은 파란색과 하늘색 중간..

	s_pPlayer->m_InfoBase.eNation = NATION_ELMORAD;
	s_pPlayer->m_InfoBase.eRace = RACE_EL_WOMEN;
	s_pPlayer->m_InfoBase.eClass = CLASS_EL_WIZARD;
	s_pPlayer->m_InfoExt.iFace = 1; // 얼굴 모양..
	s_pPlayer->m_InfoExt.iHair = 1; // 머리카락

	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks.Find(s_pPlayer->m_InfoBase.eRace);	// User Player Character Skin 구조체 포인터..
	if (NULL == pLooks) CLogWriter::Write("CGameProcMain::MsgRecv_MyInfo_All : failed find character resource data (Race : %d)", s_pPlayer->m_InfoBase.eRace);
	__ASSERT(pLooks, "failed find character resource data");
	s_pPlayer->InitChr(pLooks); // 관절 세팅..

	s_pPlayer->m_InfoExt.iRank = 1;
	s_pPlayer->m_InfoExt.iTitle = 1;
	s_pPlayer->m_InfoBase.iLevel = 80;
	s_pPlayer->m_InfoExt.iLevelPrev = s_pPlayer->m_InfoBase.iLevel;
	s_pPlayer->m_InfoExt.iBonusPointRemain = 0; // 남은 보너스 포인트..

	s_pPlayer->m_InfoExt.iExpNext = 1;
	s_pPlayer->m_InfoExt.iExp = 1;
	s_pPlayer->m_InfoExt.iRealmPoint = 1;

	// @Demircivi, implemented monthly np system.
	s_pPlayer->m_InfoExt.iRealmPointMonthly = 1;

	s_pPlayer->m_InfoExt.iCity = 1;

	std::string szKnightsName = "DENEME";
	int iKnightsID = 1; // 소속 기사단 ID
	e_KnightsDuty eKnightsDuty = KNIGHTS_DUTY_CHIEF; // 기사단에서의 권한..

	// NOTE(srmeier): adding alliance ID and knight's byFlag
	int iAllianceID = 1;
	uint8_t byFlag = 1;

	int iKnightsGrade = 1; // 소속 기사단 등급
	int	iKnightsRank = 1; // 소속 기사단 순위

	// NOTE(srmeier): adding mark version and cape ID
	int16_t sMarkVersion = 1;
	int16_t sCapeID = 1;

	// 기사단 관련 세팅..
	s_pPlayer->m_InfoExt.eKnightsDuty = eKnightsDuty; // 기사단에서의 권한..
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
	s_pPlayer->m_InfoBase.iAuthority = AUTHORITY_MANAGER; //권한.. 

	// NOTE(srmeier): adding national rank and leader rank
	uint8_t bKnightsRank = 1;
	uint8_t bPersonalRank = 1;

	//// 장착하고 있는 거..
	//int iItemIDInSlots[ITEM_SLOT_COUNT]; memset(iItemIDInSlots, -1, sizeof(iItemIDInSlots));
	//int iItemDurabilityInSlots[ITEM_SLOT_COUNT]; memset(iItemDurabilityInSlots, -1, sizeof(iItemDurabilityInSlots));
	//int iItemCountInSlots[ITEM_SLOT_COUNT]; memset(iItemCountInSlots, -1, sizeof(iItemCountInSlots));

	//for (int i = 0; i < ITEM_SLOT_COUNT; i++)				// 슬롯 갯수마큼..
	//{
	//	iItemIDInSlots[i] = pkt.read<uint32_t>();
	//	iItemDurabilityInSlots[i] = pkt.read<int16_t>();
	//	iItemCountInSlots[i] = pkt.read<int16_t>();

	//	// NOTE(srmeier): adding rental flag and remaining time
	//	uint8_t bRentFlag = pkt.read<uint8_t>();
	//	int16_t sRemainingRentalTime = pkt.read<int16_t>();
	//}
	s_pPlayer->PositionSet(__Vector3(100, 1, 100), true);
	m_fMsgSendTimeMove = 0;						// Network ReQuest 타이머 초기화..
	m_fMsgSendTimeRot = 0;
	m_fPlayerYawSended = 0;						// 최근에 메시지를 보낸 시점의 플레이어 y 축 회전값.
	m_vPlayerPosSended = s_pPlayer->Position();	// 최근에 메시지를 보낸 시점의 플레이어 위치.

		
	if (s_pPlayer->Part(PART_POS_UPPER)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->PartSet(PART_POS_UPPER, pLooks->szPartFNs[PART_POS_UPPER], NULL, NULL);	// 상체..
	if (s_pPlayer->Part(PART_POS_LOWER)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER], NULL, NULL);	// 하체..
	if (s_pPlayer->Part(PART_POS_HANDS)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->PartSet(PART_POS_HANDS, pLooks->szPartFNs[PART_POS_HANDS], NULL, NULL);	// 팔..
	if (s_pPlayer->Part(PART_POS_FEET)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->PartSet(PART_POS_FEET, pLooks->szPartFNs[PART_POS_FEET], NULL, NULL);	// 다리..
	if (s_pPlayer->Part(PART_POS_FACE)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->InitFace();
	if (s_pPlayer->Part(PART_POS_HAIR_HELMET)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->InitHair();
	s_pPlayer->SetSoundAndInitFont();
	//	s_pPlayer->SettingCollisionCheckPoint();

		////////////////////////////////////////////////////////////
		// 기본값 읽기..
	int iRun = 1;
	//	if(false == CGameProcedure::RegGetSetting("UserRun", &iRun, 4)) iRun = 1; // 걷고 뛰는 상태를 레지스트리에서 읽고.. 기본값은 뛰는 상태이다..
	//	if(1 == iRun)
	this->CommandToggleWalkRun(); // 뛰게 만든다..

	e_ViewPoint eVP = VP_THIRD_PERSON;
	if (false == CGameProcedure::RegGetSetting("CameraMode", &eVP, 4)) eVP = VP_THIRD_PERSON; // 카메라 상태 기록
	s_pEng->ViewPointChange(eVP);
	// 기본값 읽기..
	////////////////////////////////////////////////////////////

	//this->InitPlayerPosition(__Vector3(10, 10, 10)); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.

	// berserk temp
	//s_pPlayer->PlugSet(PLUG_POS_BACK, "item/babacloak.n3cplug_cloak", NULL);	// 파트를 셋팅..
	// end berserk temp

	// berserk
	//s_pPlayer->AttachCloak();

	//..
	s_pOPMgr->Release();							// 다른 유저 관리 클래스 초기화..

	m_bLoadComplete = TRUE;						// 로딩 끝..

	return;
}

void CGameProcFxEditor::Init()
{
	CGameProcedure::Init();
	m_pLightMgr->Release();
	s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

	CreateDummyCharacter();

	this->InitZone(s_pPlayer->m_InfoExt.iZoneCur, s_pPlayer->Position()); // 존 로딩..

	if (s_pUILoading) s_pUILoading->Render("Loading Character Data...", 0);

	// 경로 기억..
	char szPathOld[_MAX_PATH], szPathFind[_MAX_PATH];
	::GetCurrentDirectory(_MAX_PATH, szPathOld);

	_finddata_t fi;
	uint64_t hFind = -1;

	// 리소스 다 읽기..
	// 에니메이션 다 읽기..
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

	// 리소스 다 읽기..
	// 텍스처 다 읽기..
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

	// 리소스 다 읽기..
	// 조인트 다 읽기..
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

	// 리소스 다 읽기..
	// 스킨 읽기..
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

	// 리소스 다 읽기..
	// PMesh 읽기..
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

	// 경로 돌리기..
	::SetCurrentDirectory(szPathOld);
}

void CGameProcFxEditor::InitPlayerPosition(const __Vector3& vPos) // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
{
	__Vector3 vPosFinal = vPos;
	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(vPos.x, vPos.z);	// 지형의 높이값 얻기..
	float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(vPos, 1.0f); // 오브젝트에서 가장 가까운 높이값 얻기..
	if (!s_pWorldMgr->IsIndoor())
	{
		if (T_Abs(vPos.y - fYObject) < T_Abs(vPos.y - fYTerrain)) vPosFinal.y = fYObject; // 좀더 가까운 곳에 놓는다..
		else vPosFinal.y = fYTerrain;
	}
	else
	{
		if (fYObject > fYTerrain)
			vPosFinal.y = fYObject;
		else
			vPosFinal.y = fYTerrain;
	}

	s_pPlayer->PositionSet(vPosFinal, true);	// 캐릭터 위치 셋팅..	
	s_pPlayer->m_vPosFromServer = vPos;
	m_vPlayerPosSended = vPos;					// 최근에 보낸 위치 세팅..
	m_fMsgSendTimeMove = 0;						// 시간을 기록한다..

	this->CommandSitDown(false, false, true); // 일으켜 세운다.. 앉아있는 상태에서 워프하면.. 버그가 있다..
	this->TargetSelect(-1, false); // 타겟 해제..
	this->UpdateCameraAndLight(); // 카메라와 라이트 다시 계산..

	s_pPlayer->Action(PSA_BASIC, true, NULL, true); // 강제로 기본 자세..
}

void CGameProcFxEditor::Tick()
{
	CGameProcedure::Tick();	// 키, 마우스 입력 등등..
	if (FALSE == m_bLoadComplete) return;				// 로딩이 안되었으면.. 돌아간다.

#ifdef _DEBUG
	if (s_pLocalInput->IsKeyDown(DIK_F11))
	{
		static int i = 1; //10029 SPrint // 602 720 hp
		CGameProcedure::s_pFX->TriggerBundle(0, 0, i++, 0, 15);

	}
#endif

	uint32_t dwMouseFlags = s_pLocalInput->MouseGetFlag();	// 마우스 버튼 플래그 - LocalInput.h 참조
	this->ProcessLocalInput(dwMouseFlags);					// 키보드나 마우스 입력은 UI 다음에 처리...

	s_pPlayer->Tick();									// 플레이어 틱(갱신)
	s_pWorldMgr->Tick();
	s_pOPMgr->Tick(s_pPlayer->Position());				// 다른 유저 관리자 틱(갱신)
	//	s_pFX->Tick(); //내부에서 카메라 값을 쓸 경우 위치가 오차가 생겨 Render()함수 안으로 옮김...

	__Vector3 ListenerPos = s_pPlayer->Position();
	__Vector3 ListenerDir = s_pPlayer->Direction();
	__Vector3 ListenerUp(0, 1, 0);

	// Sound Tick...
	CN3SndObj::SetListenerPos(&ListenerPos);
	CN3SndObj::SetListenerOrientation(&ListenerDir, &ListenerUp);

	this->UpdateUI_MiniMap(); // 미니맵 업데이트..
	this->UpdateUI_TargetBar(); // 타겟바 처리..
	this->UpdateBGM(); // 배경음악을 상황에 따라 처리..
	this->UpdateCameraAndLight(); // 카메라와 라이트 처리..

	//	ProcessPlayerInclination();							// 경사 처리..(가만히 있어도 경사가 급하면 미끄러짐..).
#ifdef _N3_64GRID_
	m_SMesh.Tick(s_pPlayer, &m_Terrain);				// 서버 메시 틱.(갱신)
#endif

	m_pUIStateBarAndMiniMap->UpdatePosition(s_pPlayer->Position(), s_pPlayer->Yaw()); // 위치 업데이트.

	if (m_pMagicSkillMng) m_pMagicSkillMng->Tick();
	if (m_pWarMessage) m_pWarMessage->Tick();
	if (m_pLightMgr) m_pLightMgr->Tick();

	////////////////////////////////////////////////////////////////////////////////////
	// 아무 패킷도 안보냈으면 2초에 한번 WIZ_TIME_NOTIFY 보낸다..
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
	// 아무 패킷도 안보냈으면 2초에 한번 WIZ_TIME_NOTIFY 보낸다..
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// 타이머 비슷한 루틴..
	static float fInterval2 = 0, fInterval3 = 0, fInterval4 = 0, fInterval5 = 0;
	fInterval2 += fTime - fTimePrev;
	fInterval3 += fTime - fTimePrev;
	fInterval4 += fTime - fTimePrev;
	fInterval5 += fTime - fTimePrev;
	m_fRequestGameSave += fTime - fTimePrev;
	if (fInterval2 > 1200.0f) // 저장 요청..
	{
		uint8_t byBuff[4];												// 버퍼.. 
		int iOffset = 0;												// 옵셋..
		s_pSocket->MP_AddByte(byBuff, iOffset, WIZ_DATASAVE);	// 저장 요청 커멘드..
		s_pSocket->Send(byBuff, iOffset);				// 보냄..

		fInterval2 = 0.0f;
	}
	if (fInterval3 > 10.0f) // 스피드핵 체크.. 
	{
		fInterval3 = 0.0f;
	}
	if (s_pPlayer->m_InfoBase.iLevel < 12 && fInterval4 > 20.0f) // 시간이 지나면 팁 하나씩 표시..
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_HELP_TIP_ALL, szMsg);
		this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00);

		::_LoadStringFromResource(IDS_HELP_TIP1 + (rand() % 33), szMsg);
		this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00); // 헬프 표시..
		fInterval4 = 0;
	}
	if (fInterval5 > 5.0f) // 시간이 지나면 팁 하나씩 표시..
	{
		//		m_pUIChatDlg->ChangeChattingMode(N3_CHAT_CONTINUE); // 채팅모드 강제로 바꾸기...
		m_pUIChatDlg->ShowContinueMsg();
		fInterval5 = 0;
	}
	fTimePrev = fTime;
	// 타이머 비슷한 루틴..
	////////////////////////////////////////////////////////////////////////////////////
}

void CGameProcFxEditor::Render()
{
	if (FALSE == m_bLoadComplete)	return; 		// 로딩이 끝났냐??

	D3DCOLOR crSky = ACT_WORLD->GetSkyColorWithSky();
	s_pEng->Clear(crSky); // 안개 색깔을 넣어서 클리어.. -> 하늘색깔로 클리어 해야 하늘이 제대로 나온다..
	s_pEng->s_lpD3DDev->BeginScene();			// 씬 렌더 ㅅ작...

	ACT_WORLD->RenderSky();								// 하늘 렌더링..
	float fSunAngle = ACT_WORLD->GetSunAngleByRadinWithSky(); // 해의 각도를 가져오고..

	uint32_t dwFilter = D3DTEXF_LINEAR;
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MINFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MAGFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MIPFILTER, dwFilter);

	ACT_WORLD->RenderTerrain();						// 지형 렌더..
	ACT_WORLD->RenderShape();						// 물체 렌더..
	s_pOPMgr->Render(fSunAngle);				// 다른 플레이어 렌더..
	s_pPlayer->Render(fSunAngle);			// 플레이어 렌더..

	// NOTE(srmeier): uncomment to render the collision meshes
#ifdef _DEBUG
	ACT_WORLD->RenderCollisionWithShape(s_pPlayer->Position());				// 충돌 메쉬 렌더..
#endif

#ifdef _N3_64GRID_
	m_SMesh.Render();							// 서버 메쉬 렌더..
#endif

	this->RenderTarget();						// 타겟으로 잡은 캐릭터 혹은 오브젝트 렌더링..

	ACT_WORLD->RenderGrass();						//	풀 렌더 (asm)
	s_pFX->Tick();
	s_pFX->Render();
	ACT_WORLD->RenderBirdMgr();

	CN3Base::s_AlphaMgr.Render(); // 알파 정렬된 폴리곤들 렌더링..

	ACT_WORLD->RenderSkyWeather();							// 하늘 렌더링..

	CGameProcedure::Render(); // UI 나 그밖의 기본적인 것들 렌더링..
	if (m_pWarMessage) m_pWarMessage->RenderMessage();
	if (s_pGameCursor) s_pGameCursor->Render();

	s_pEng->s_lpD3DDev->EndScene();
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcFxEditor::RenderTarget()
{
	if (NULL == m_pTargetSymbol) return;

	// 플레이어가 타겟으로 잡은 캐릭터의 위치위에 그리면 된다..
	CPlayerBase* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);//시체로 판정되기 전까지의 캐릭은 포커스를 준다.
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
	if (m_pTargetSymbol->Part(1)) // 바닥의 심벌을 땅바닥 위로 맞춘다..
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

// 키보드와 마우스 눌린것을 처리한다..
void CGameProcFxEditor::ProcessLocalInput(uint32_t dwMouseFlags)
{
	// Loading이 된 후..
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
	// 핫키
	//////////////////////////////////////////

	if (s_pLocalInput->IsKeyPress(KM_CAMERA_CHANGE))												// 시점 변환..
	{
		this->CommandCameraChange(); // 카메라 시점 바꾸기..
	}

	// 삼인칭일때 홈, 엔드키로 카메로 올리고 내리기..
	if (s_pEng->ViewPoint() == VP_THIRD_PERSON)
	{
		float fPitch = 0;
		if (s_pLocalInput->IsKeyDown(DIK_HOME)) fPitch = D3DXToRadian(45.0f);		// home 키가 눌리면..
		else if (s_pLocalInput->IsKeyDown(DIK_END)) fPitch = D3DXToRadian(-45.0f);	// End 키가 눌리면..
		if (fPitch) s_pEng->CameraPitchAdd(fPitch);
	}

	if (!IsUIKeyOperated() && NULL == CN3UIBase::GetFocusedEdit())			// 채팅모드가 아닐때 
	{
		if (s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER) //게임 운영자는 이 기능을 사용할수 있다.
		{
			if (s_pLocalInput->IsKeyDown(DIK_Q)) s_pPlayer->m_bTempMoveTurbo = true; // 엄청 빨리 움직이게 한다..  // 임시 함수.. 나중에 없애자..
			else s_pPlayer->m_bTempMoveTurbo = false; // 엄청 빨리 움직이게 한다..  // 임시 함수.. 나중에 없애자..
		}

		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_ATTACK))
			this->CommandToggleAttackContinous();		// 자동 공격..}
		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_RUN))
			this->CommandToggleWalkRun();				// 걷기 / 뛰기 토글	
		if (s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_ENEMY))
			this->CommandTargetSelect_NearstEnemy();	// 가장 가까운 적 타겟 잡기..
		if (s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_PARTY))
			this->CommandTargetSelect_NearstOurForce(); // 가장 가까운 파티 타겟잡기..

		float fRotKeyDelta = D3DXToRadian(60); // 초당 60 도 돌기..
		if (s_pLocalInput->IsKeyDown(KM_ROTATE_LEFT) || s_pLocalInput->IsKeyDown(DIK_LEFT))
		{
			if (s_pPlayer->IsAlive()) s_pPlayer->RotAdd(-fRotKeyDelta); // 초당 180 도 왼쪽으로 돌기.
			if (m_pUIDroppedItemDlg->IsVisible()) m_pUIDroppedItemDlg->LeaveDroppedState();
		}
		if (s_pLocalInput->IsKeyDown(KM_ROTATE_RIGHT) || s_pLocalInput->IsKeyDown(DIK_RIGHT))
		{
			if (s_pPlayer->IsAlive()) s_pPlayer->RotAdd(fRotKeyDelta); // 초당 180 도 오른쪽으로 돌기.
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
					bStart = true;// 누르는 순간이면
			}
			this->CommandMove(MD_FOWARD, bStart); // 앞으로 이동..
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
					bStart = true;// 누르는 순간이면
			}
			this->CommandMove(MD_BACKWARD, bStart); // 뒤로 이동..
		}
		else if (s_pLocalInput->IsKeyPress(KM_TOGGLE_MOVE_CONTINOUS))
		{
			this->CommandToggleMoveContinous();
		}

		if (s_pLocalInput->IsKeyPressed(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPressed(DIK_UP) ||
			s_pLocalInput->IsKeyPressed(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyPressed(DIK_DOWN)) // 전진/후진 키를 떼는 순간. 
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

	// ..... 나머지 키보드 처리..
#if _DEBUG
	if (s_pLocalInput->IsKeyPress(DIK_F12)) // 디버깅 테스트..
		s_pEng->Lightning(); // 번개 치기..
#endif
}

void CGameProcFxEditor::ProcessPlayerInclination()											// 경사에 서 있을때..
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

	if (m_pUIInventory->IsVisible()) // 인벤토리가 안열려 있으면..
		this->CommandToggleUIInventory();

	if (m_pUISkillTreeDlg->IsVisible())
		m_pUISkillTreeDlg->Close();

	m_pUITransactionDlg->EnterTransactionState();
}

void CGameProcFxEditor::InitZone(int iZone, const __Vector3& vPosPlayer)
{
	static int iZonePrev = -1;
	if (iZonePrev != iZone) // 다른 존일 경우에만 로딩..
	{
		m_pLightMgr->Release();
		s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

		s_pPlayer->m_bMoveContinous = true; 

		m_bLoadComplete = false; // 로딩 끝남..
	
		s_pFX->ClearAll();

		s_pPlayer->m_InfoExt.iZoneCur = iZone;
		iZonePrev = iZone; // 최근에 읽은 존 번호를 기억해둔다.

		__TABLE_ZONE* pZoneData = s_pTbl_Zones.Find(s_pPlayer->m_InfoExt.iZoneCur);
		if (NULL == pZoneData) {
			CLogWriter::Write("can't find zone data. (zone : %d)", s_pPlayer->m_InfoExt.iZoneCur);
			__ASSERT(0, "Zone Data Not Found!");
			system("pause");
			return;
		}

		CLogWriter::Write("%d->Release()", s_pOPMgr); // TmpLog1122

		s_pOPMgr->Release(); // 다른 넘들 다 날린다..
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

		m_bLoadComplete = true; // 로딩 끝남..
	}

	// 카메라 세팅..
	CN3Camera* pCamera = s_pEng->CameraGetActive();		// 활성화된 카메라 얻기..
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
	// 기본적인 캐릭터위치와 카메라 위치 잡기..
	////////////////////////////////////////////////////////////////////////////////

	CLogWriter::Write("InitPlayerPosition() Position(%.1f, %.1f, %.1f)", vPosPlayer.x, vPosPlayer.y, vPosPlayer.z); // TmpLog1122
	this->InitPlayerPosition(vPosPlayer); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
	CLogWriter::Write("%d->Release()", s_pOPMgr); // TmpLog1122
	s_pOPMgr->Release(); // 다른 플레이어 삭제...
}


bool CGameProcFxEditor::CommandToggleWalkRun()
{
	bool bRun = s_pPlayer->ToggleRunMode();
	bool bMove = s_pPlayer->m_bMoveContinous;

	// 커맨드 버튼 업데이트..
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

	s_pPlayer->ToggleMoveMode();				// 자동 전진 토글.. 
	if (s_pPlayer->m_bMoveContinous)
	{
		this->CommandMove(MD_FOWARD, true);
		if (m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
		if (m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
	}
	else
	{
		this->CommandMove(MD_STOP, true); // 움직임 패킷 보내기..
		if (m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
		if (m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
	}

	return s_pPlayer->m_bMoveContinous;
}

void CGameProcFxEditor::CommandMove(e_MoveDirection eMD, bool bStartOrEnd)
{
	if (bStartOrEnd) this->CloseUIs(); // 각종 상거래, 워프등등... UI 닫기..

	if (s_pPlayer->IsDead()) return; // 죽은 넘이 어딜 감히!!

	if (MD_FOWARD == eMD || MD_BACKWARD == eMD)
	{
		s_pUIMgr->UserMoveHideUIs();
		this->CommandSitDown(false, false, true); // 일으켜 세우고..
		if (s_pPlayer->m_bStun) return; // 기절해 있음 움직이지 못함..
		if (MD_FOWARD == eMD)
		{
			if (s_pPlayer->IsRunning()) s_pPlayer->ActionMove(PSM_RUN); // 뛰어가기..
			else s_pPlayer->ActionMove(PSM_WALK); // 걸어가기..
		}
		else
		{
			s_pPlayer->ActionMove(PSM_WALK_BACKWARD); // 후진..
		}

		if (bStartOrEnd) // 움직이기 시작할때에는 충돌체크를 먼저 해본다..
		{
			float fSpeed = s_pPlayer->MoveSpeedCalculationAndCheckCollision(); // 속도를 구하고 그 속도로 충돌 체크를 한다. 리턴값이 0 이면 충돌이다..
			if (0 == fSpeed) // 못움직이는 상황이면..
			{
				s_pPlayer->ActionMove(PSM_STOP); // 멈춤..
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
		s_pPlayer->m_bMoveContinous = false; // 계속 걸음 멈춤..
		s_pPlayer->ActionMove(PSM_STOP);

		if (m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
		if (m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);

	}

	if (s_pPlayer->m_bAttackContinous) // 공격중이면..
	{
		CPlayerBase* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
		if (s_pPlayer->IsAttackableTarget(pTarget)) // 공격 가능하면..
			s_pPlayer->Action(PSA_ATTACK, false, pTarget); // 공격
	}
}

void CGameProcFxEditor::CommandEnableAttackContinous(bool bEnable, CPlayerBase* pTarget)
{
	if (bEnable == s_pPlayer->m_bAttackContinous) return;
	if (bEnable)
	{
		this->CloseUIs(); // 각종 상거래, 워프등등... UI 닫기..
		s_pUIMgr->UserMoveHideUIs();

		if (s_pPlayer->m_bStun) return; // 기절해 있음 공격 못함..
		if (NULL == pTarget) return;
		s_pPlayer->RotateTo(pTarget); // 방향을 돌린다.
		if (pTarget->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation) return; // 국가가 같으면 넘어간다..

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
	s_pPlayer->m_bAttackContinous = bEnable; // 상태를 기록하고..

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

	if (bEnable) // 자동 공격!
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
	else // 자동 공격 아님.
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_MSG_ATTACK_STOP, szMsg);
		s_pPlayer->Action(PSA_BASIC, true, pTarget);
		this->MsgOutput(szMsg, 0xff00ffff);
	}

	if (bEnable && false == s_pPlayer->IsAttackableTarget(pTarget)) // 국가, 거리 및 각도 체크해서 공격 불가능하면 돌아가기..
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

	// 커맨드 버튼 업데이트..
//	if(m_pUICmd->m_pBtn_Character)
//	{
//		if(bNeedOpen) m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_DOWN); // 버튼 누름 해제..
//		else m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_NORMAL); // 버튼 누름
//	}

	return bNeedOpen;
}

bool CGameProcFxEditor::CommandToggleUIInventory()
{
	bool bNeedOpen = false;

	// 개인 거래중이면..
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

	// 개인 거래중이면..
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

void CGameProcFxEditor::CommandCameraChange() // 카메라 시점 바꾸기..
{
	if (VP_THIRD_PERSON == s_pEng->ViewPoint() && s_pPlayer->m_bTargetOrPosMove)
	{
		s_pPlayer->m_bMoveContinous = false;
		s_pPlayer->m_bTargetOrPosMove = false;
		CommandToggleMoveContinous();
	}

	s_pEng->ViewPointChange(VP_UNKNOWN); // 순서대로 시점을 바꾼다..
}

void CGameProcFxEditor::MsgOutput(const std::string& szMsg, D3DCOLOR crMsg)
{
	m_pUIMsgDlg->AddMsg(szMsg, crMsg);
}

void CGameProcFxEditor::CommandSitDown(bool bLimitInterval, bool bSitDown, bool bImmediately)
{
	if (bSitDown == s_pPlayer->m_bSitDown) return; // 상태가 같으면 돌아간다..

	// 앉기 서기 제한..
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
		if (eSM != PSM_STOP) // 뛰던 놈이면 멈춘다.
			return;

		if (this->m_pMagicSkillMng->IsCasting()) // 스킬 캐스팅 중이면 앉지 못한다..
			return;

		if (s_pPlayer->m_bAttackContinous) // 계속 공격하는 중이면..
			this->CommandEnableAttackContinous(false, NULL); // 계속 공격 취소..
	}

	bool bVBs[2] = { false, false };
	int iState = 0;
	if (bSitDown)
	{
		eSA = PSA_SITDOWN;
		bVBs[0] = false; bVBs[1] = true;
		iState = 0x02;
	} // 앉는 경우
	else
	{
		eSA = PSA_BASIC;
		bVBs[0] = true; bVBs[1] = false;
		iState = 0x01;
	} // 일어서는 경우

	s_pPlayer->m_bSitDown = bSitDown;
	s_pPlayer->Action(eSA, true, NULL, bImmediately);

	if (m_pUICmd->m_pBtn_Act_SitDown)
	{
		m_pUICmd->m_pBtn_Act_SitDown->SetVisible(bVBs[0]);
		m_pUICmd->m_pBtn_Act_SitDown->SetState(UI_STATE_BUTTON_DOWN); // 버튼 누름
	}
	if (m_pUICmd->m_pBtn_Act_StandUp)
	{
		m_pUICmd->m_pBtn_Act_StandUp->SetVisible(bVBs[1]);
		m_pUICmd->m_pBtn_Act_StandUp->SetState(UI_STATE_BUTTON_DOWN); // 버튼 누름
	}

}

void CGameProcFxEditor::CommandTargetSelect_NearstEnemy() // 가장 가까운 적 타겟 잡기..
{
	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByNearstEnemy(s_pPlayer->m_InfoBase.eNation, s_pPlayer->Position());
	this->TargetSelect(pTarget);
	s_pPlayer->RotateTo(pTarget);
}

void CGameProcFxEditor::CommandTargetSelect_NearstOurForce() // 가장 가까운 파티 타겟잡기..
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
	if (PSA_SPELLMAGIC == s_pPlayer->State()) // 마법 캐스팅을 하는 중간에는 타겟을 못 바꾼다..
		return;

	int iIDTargetPrev = s_pPlayer->m_iIDTarget; // 전의 타겟 기억..
	s_pPlayer->m_iIDTarget = -1;
	s_pPlayer->m_pObjectTarget = NULL;

	if (pTarget && pTarget->m_InfoBase.iAuthority != AUTHORITY_MANAGER)
	{ //운영자는 포커스를 주지 않게 하기 위해서...
		s_pPlayer->m_iIDTarget = pTarget->IDNumber();
		if (iIDTargetPrev != s_pPlayer->m_iIDTarget) // 전의 타겟과 다르면
		{
			this->CommandEnableAttackContinous(false, NULL);	// 자동 공격 취소..

			D3DCOLOR crID = 0xffffffff;
			if (pTarget->PlayerType() == PLAYER_OTHER) // User..
			{
				if (pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) crID = 0xffff4040; // 다른 국가이면
				else crID = 0xff6b9fff;
			}
			else // NPC
			{
				if (pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) crID = 0xffff6060; // 다른 국가이면
				else crID = 0xff1064ff;
			}

			if (pTarget->IsAlive())//살아있는 상태만 target bar를 보여준다.
			{
				m_pUITargetBar->SetVisible(true);
				m_pUITargetBar->SetIDString(pTarget->IDString(), crID); // 이름을 넣고..
			}
			else
			{
				m_pUITargetBar->SetVisible(false); // 타겟 바 없애기..
			}
		}
	}
	else
	{
		s_pPlayer->m_iIDTarget = -1; // 타겟 아이디를 잡고..
		m_pUITargetBar->SetVisible(false); // 타겟 바 없애기..

		this->CommandEnableAttackContinous(false, NULL); // 자동 공격 취소..
	}

	if (PSA_SITDOWN != s_pPlayer->State() &&
		PSM_STOP == s_pPlayer->StateMove() &&
		PSA_BASIC == s_pPlayer->State()) s_pPlayer->Action(PSA_BASIC, true); // 기본 자세 바꾸기..

	this->UpdateUI_PartyOrForceButtons(); // 커맨드 줄에 있는 파티 버튼을 상황에 따라 업데이트 해준다.
}

void CGameProcFxEditor::UpdateUI_PartyOrForceButtons()
{
	// 파티 버튼 상태 바꾸기..
	CPlayerBase* pTarget = NULL;
	bool bIAmLeader = false, bIAmMemberOfParty = false;
	int iMemberIndex = -1;
	this->PartyOrForceConditionGet(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
	m_pUIPartyOrForce->MemberSelect(iMemberIndex);

	m_pUICmd->UpdatePartyButtons(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
}

const __InfoPartyOrForce* CGameProcFxEditor::PartyOrForceConditionGet(bool& bIAmLeader, bool& bIAmMember, int& iMemberIndex, class CPlayerBase*& pTarget)
{
	// 파티 버튼 상태 바꾸기..
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

		if (eNation != pNPC->m_InfoBase.eNation) crType = 0xff800000; // 다른 국가 NPC 혹은 몬스터 주황색
		else crType = 0xff00a0ff; // 같은 국가 NPC 하늘색

		m_pUIStateBarAndMiniMap->PositionInfoAdd(pNPC->IDNumber(), pNPC->Position(), crType, false);
	}

	it_UPC it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
	CPlayerOther* pUPC = NULL;
	__TABLE_ZONE* pZoneInfo = s_pTbl_Zones.Find(s_pPlayer->m_InfoExt.iZoneCur);
	for (; it2 != itEnd2; it2++) // User
	{
		pUPC = it2->second;

		bool bDrawTop = false;
		if (eNation != pUPC->m_InfoBase.eNation) // 적국일경우
		{
			if (pUPC->State() == PSA_SITDOWN)
			{
				pUPC->m_InfoBase.bRenderID = false; // 아이디 표시하지 않음.
				continue; // 앉아있으면.. 지나간다..
			}
			else
			{
				pUPC->m_InfoBase.bRenderID = true; // 아이디 표시함.
			}

			if (pZoneInfo && FALSE == pZoneInfo->bIndicateEnemyPlayer) continue; // 적국이 표시 되지 않아야 한다면.. 지나간다..

			crType = 0xffff4040; // 다른 국가  - 밝은 빨간색
		}
		else // 같은 국가면..
		{
			int iMemberIndex = -1;
			const __InfoPartyOrForce* pPI = m_pUIPartyOrForce->MemberInfoGetByID(pUPC->IDNumber(), iMemberIndex);
			if (pPI)
			{
				bDrawTop = true; // 맨 위에 그린다.
				crType = 0xffffff00; // 파티 멤버면 // 노란색
			}
			else
			{
				crType = 0xff0040ff; // 파티 멤버가 아니면 밝은 파란색
			}
		}

		if (pUPC && pUPC->m_InfoBase.iAuthority != AUTHORITY_MANAGER)	// 운영자가 아닌경우만 미니맵에 포인트를 찍어준다.
			m_pUIStateBarAndMiniMap->PositionInfoAdd(pUPC->IDNumber(), pUPC->Position(), crType, bDrawTop);
	}
}

void CGameProcFxEditor::UpdateUI_TargetBar()
{
	if (NULL == m_pUITargetBar || !m_pUITargetBar->IsVisible()) return;

	//죽은 캐릭터가 선택되었을때는 target bar를 그려주지 않는다.
	CPlayerNPC* t_pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
	if (t_pTarget && t_pTarget->State() == PSA_DEATH)
	{
		m_pUITargetBar->SetVisible(false);
		return;
	}

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);
	if (pTarget) // 타겟이 유효하면..
	{
		float fTimeTmp = CN3Base::TimeGet();
		if (fTimeTmp > m_pUITargetBar->m_fTimeSendPacketLast + PACKET_INTERVAL_REQUEST_TARGET_HP) // 1초가 지나면 타겟 정보 요청
		{
			m_pUITargetBar->m_fTimeSendPacketLast = fTimeTmp;
		}
	}
	else // 타겟이 없으면..
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
		s_pPlayer->Height(), ACT_WORLD->GetSunAngleByRadinWithSky());		// 캐릭터 위치와 해의 각도를 넣어준다..
	s_pEng->ApplyCameraAndLight();	// 카메라와 라이트에 세팅된 값을 D3D Device 에 적용한다.
}

void CGameProcFxEditor::ItemUpgradeOpen(uint16_t npcId)
{
	m_pUIItemUpgradeDlg->SetNpcId(npcId);
	m_pUIItemUpgradeDlg->EnterItemUpgradeStateStart();

	if (!m_pUIItemUpgradeDlg->IsVisible())
		m_pUIItemUpgradeDlg->SetVisible(true);

	if (m_pUIInventory->IsVisible())		// 인벤토리가 안열려 있으면..
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
	m_pSnd_Town->Play(NULL, 3.0f); // 전투 음악 설정.. 해제는 주위에 몬스터가 없을때 한다..
}

void CGameProcFxEditor::PlayBGM_Battle()
{
	if (m_pSnd_Town) m_pSnd_Town->Stop(3.0f);
	if (NULL == m_pSnd_Battle || m_pSnd_Battle->IsPlaying()) return;
	m_pSnd_Battle->SetMaxVolume(80);
	m_pSnd_Battle->Play(NULL, 3.0f); // 전투 음악 설정.. 해제는 주위에 몬스터가 없을때 한다..
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
			pTarget = s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &vPick); // 사방에 깔린넘들 픽킹..
			this->TargetSelect(iID, false); // 타겟을 잡는다..
			if(NULL == pTarget) // 타겟이 없으면..
			{
				s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &vPick); // 찍힌 위치를 저장한다..
				if(NULL == s_pPlayer->m_pObjectTarget) // 타겟도 없으면..
				{
					ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vPick); // 지형을 찍어본다..
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

			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, m_vMouseSkillPos); // 지형을 찍어본다..

			vDir = m_vMouseSkillPos - vMyPos;
			vGap = vDir;
			vDir.Normalize();

			bool bColShape = ACT_WORLD->CheckCollisionWithShape(vMyPos, vDir, vGap.Magnitude(), &m_vMouseSkillPos, &vNormal);

			if (!bColShape) // 타겟도 없으면..
			{
				int iID = -1;
				s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &m_vMouseSkillPos); // 사방에 깔린넘들 픽킹..
				this->TargetSelect(iID, false); // 타겟을 잡는다..
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

// 왼쪽 더블 클릭
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
			this->CommandEnableAttackContinous(true, pTarget); // 자동 공격
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

// 왼쪽 클릭
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
	pTarget = s_pOPMgr->PickPrecisely(ptCur.x, ptCur.y, iID, &m_vMouseLBClickedPos); // 사방에 깔린넘들 픽킹..
	this->TargetSelect(iID, false); // 타겟을 잡는다..
	if (NULL == pTarget) // 타겟이 없으면..
	{
		if (s_pPlayer->m_bAttackContinous) // 계속 공격하는 중이면..
			this->CommandEnableAttackContinous(false, NULL); // 계속 공격 취소..

		s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &m_vMouseLBClickedPos); // 찍힌 위치를 저장한다..
		if (NULL == s_pPlayer->m_pObjectTarget) // 타겟도 없으면..
		{
			// 시체 뒤저서 아이템 상자 열기..
			CPlayerNPC* pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // 픽킹..
		
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
		this->CommandSitDown(false, false); // 일단 일으켜 세운다..

		if (pTarget)
		{
			if (s_pLocalInput->IsKeyDown(DIK_LCONTROL) || s_pLocalInput->IsKeyDown(DIK_RCONTROL))
			{
				if (s_pPlayer->IsAttackableTarget(pTarget, false))
				{
					if (s_pPlayer->m_bTargetOrPosMove)
						this->CommandMove(MD_STOP, true);

					this->CommandEnableAttackContinous(true, pTarget); // 자동 공격
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
			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // 지형을 찍어본다..

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

				this->CommandEnableAttackContinous(true, pTarget); // 자동 공격
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

// 왼쪽 클릭업
bool CGameProcFxEditor::OnMouseLBtnPressd(POINT ptCur, POINT ptPrev)
{
	if (!s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD))
	{
		//		if(!s_pPlayer->m_bTargetOrPosMove)
		//			this->CommandMove(MD_STOP, true);
	}
	return true;
}

// 왼쪽 눌리고 있을때
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

	if (!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint() && !s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD)) // 삼인칭 시점이면.. UI 를 건들지 않았으면..
	{
		__Vector3 vMovePoint;
		float fDist;
		float fNowTime = CN3Base::TimeGet();

		if (fNowTime - m_fLBClickTime > 0.1f && s_pPlayer->m_bTargetOrPosMove)
		{
			if (s_pPlayer->m_bAttackContinous)
				CommandToggleAttackContinous();


			this->CommandSitDown(false, false); // 일단 일으켜 세운다..
			s_pPlayer->RotateTo(fYaw, true);
			this->CommandMove(MD_FOWARD, false);

			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // 지형을 찍어본다..
			fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			s_pPlayer->SetMoveTargetPos(vMovePoint);
		}
		else if (fNowTime - m_fLBClickTime > 0.1f && !s_pPlayer->m_bTargetOrPosMove)
		{
			if (s_pPlayer->m_bAttackContinous)
				CommandToggleAttackContinous();

			s_pPlayer->RotateTo(fYaw, true);
			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // 지형을 찍어본다..

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

// 오른쪽 클릭
bool CGameProcFxEditor::OnMouseRBtnPress(POINT ptCur, POINT ptPrev)
{
	if (s_pUIMgr->m_bDoneSomething) return false;

	if (s_pLocalInput->IsKeyDown(DIK_LCONTROL) || s_pLocalInput->IsKeyDown(DIK_RCONTROL))
	{
		m_pUIHotKeyDlg->EffectTriggerByMouse();
		return true;
	}

	int iID = -1;
	CPlayerNPC* pNPC = s_pOPMgr->PickNPC(ptCur.x, ptCur.y, iID); // 픽킹..

	if (NULL == pNPC)
	{
		// NOTE: sending the packet to pick up items
		CPlayerNPC* pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // 픽킹..
		
	}
	else if (pNPC->IDNumber() == s_pPlayer->m_iIDTarget) // 타겟으로 찍은 캐릭터와 피킹한 캐릭터가 같고 ..) 
	{
		if (pNPC->m_pShapeExtraRef) // 오브젝트 형태의 NPC 이면.. 컨트롤 할 NPC의 ID 가 있으면..
		{
			//if (pNPC->m_pShapeExtraRef->m_iNPC_ID > 0) // anvil i�in iNPC_ID 0 geliyor acaba bu kontrol gerekli mi ?
			{
				float fD = (s_pPlayer->Position() - pNPC->m_pShapeExtraRef->Pos()).Magnitude();
				float fDLimit = (s_pPlayer->Radius() + pNPC->m_pShapeExtraRef->Radius()) * 2.0f;
				if (fD > fDLimit) // 거리가 멀면
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
		else // 보통 NPC 이면..
		{
			// NOTE: an NPC has been clicked on
			// TODO(srmeier): need to use ZoneAbilityType here
			// NOTE(srmeier): using the zone type to decide if you can talk with NPC
			if (ACT_WORLD->GetZoneType() == ZoneAbilityNeutral || (pNPC->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation)) // 같은 국가 일때만..
			{
				float fD = (s_pPlayer->Position() - pNPC->Position()).Magnitude();
				float fDLimit = (s_pPlayer->Radius() + pNPC->Radius()) * 3.0f;
				if (fD > fDLimit) // 거리가 멀면
				{
					std::string szMsg;
					::_LoadStringFromResource(IDS_ERR_REQUEST_NPC_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
					s_pPlayer->ActionMove(PSM_STOP); // 일단 멈추고..
					pNPC->RotateTo(s_pPlayer); // 방향을 플레이어 쪽으로 돌린다.
					if (m_pUITransactionDlg) m_pUITransactionDlg->m_iNpcID = pNPC->IDNumber();
				}
			}
		}
	}
	return true;
}

// 오른쪽 클릭업
bool CGameProcFxEditor::OnMouseRBtnPressd(POINT ptCur, POINT ptPrev)
{
	return true;
}

// 오른쪽 눌리고 있을때
bool CGameProcFxEditor::OnMouseRbtnDown(POINT ptCur, POINT ptPrev)
{
	float fMouseSensivity = 0.05f;

	float fRotY = D3DXToRadian(180.0f) * ((ptCur.x - ptPrev.x) * fMouseSensivity); // 회전할 양을 계산하고..
	float fRotX = D3DXToRadian(180.0f) * ((ptCur.y - ptPrev.y) * fMouseSensivity);
	if (fRotY && s_pPlayer->IsAlive())
	{
		if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(fRotY);
		else if (false == s_pPlayer->m_bStun) s_pPlayer->RotAdd(fRotY); // 기절해 있지 않을때만..
	}
	if (fRotX)
	{
		if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraZoom(-fRotX); // 카메라 확대
		else s_pEng->CameraPitchAdd(fRotX); // 카메라 각도
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

// 오른쪽 더블 클릭
bool CGameProcFxEditor::OnMouseRDBtnPress(POINT ptCur, POINT ptPrev)
{
	if (s_pUIMgr->m_bDoneSomething) return false;

	//스킬 매직이 사용되었다면....
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