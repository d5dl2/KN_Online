// GameProcMain.h: interface for the CGameProcMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCMAIN_H__E1C4F2CC_5AF3_4417_8917_A52CD5523DB3__INCLUDED_)
#define AFX_GAMEPROCMAIN_H__E1C4F2CC_5AF3_4417_8917_A52CD5523DB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameProcedure.h"
#include <set>

typedef std::set<int>::iterator it_ID;
typedef std::pair<it_ID, bool> pair_ID;

class CGameProcMain : public CGameProcedure
{
	friend class CGameProcedure;
protected:
	std::set<int> m_SetNPCID;
	std::set<int> m_SetUPCID;

public:
#ifdef _N3_64GRID_
	CServerMesh* m_pSMesh;									// ¼­¹ö¿¡°Ô ÇÊ¿äÇÑ ¸Þ½¬ Å¬·¡½º..
#endif
	class CUIInventory*			m_pUIInventory;			// ÀÎº¥Åä¸®
	class CUIVarious* m_pUIVar;				// Ä³¸¯ÅÍ »óÅÂÃ¢, ±â»ç´Ü °ü¸® µîÀÌ ÆäÀÌÁö·Î µé¾î°£ ´Ù¿ëµµ UI
	class CUIChat* m_pUIChatDlg;			// Ã¤ÆÃ ÀÔÃâ·Â ´ëÈ­»óÀÚ..
	class CUIMessageWnd* m_pUIMsgDlg;			// °ÔÀÓ ¸Þ½ÃÁö Ãâ·Â »óÀÚ.

	// Folded UI
	class CUIChat2* m_pUIChatDlg2;
	class CUIMessageWnd2* m_pUIMsgDlg2;

	class CUIStateBar* m_pUIStateBarAndMiniMap;	// mp,hp,exp, minimap....
	class CUICmd* m_pUICmd;				// ¿ÞÂÊ ÇÏ´ÜÀÇ ¸í·É¹öÆ° Ã¢..
	class CUITargetBar* m_pUITargetBar;			// Å¸°Ù »óÅÂÃ¢..
	class CUICmdList* m_pUICmdListDlg;
	class CUICmdEdit* m_pUICmdEditDlg;
	class CUITransactionDlg* m_pUITransactionDlg;
	class CUIDroppedItemDlg* m_pUIDroppedItemDlg;
	class CSubProcPerTrade* m_pSubProcPerTrade;
	class CUIHelp* m_pUIHelp;
	class CUIPartyOrForce* m_pUIPartyOrForce;
	class CUISkillTreeDlg* m_pUISkillTreeDlg;
	class CUIHotKeyDlg* m_pUIHotKeyDlg;
	class CUINotice* m_pUINotice;
	class CUIClassChange* m_pUIClassChange;
	class CUINPCEvent* m_pUINpcEvent;
	class CUIItemExchange* m_pUIItemREDlg;
	class CUIRepairTooltipDlg* m_pUIRepairTooltip;
	class CUIWareHouseDlg* m_pUIWareHouseDlg;
	class CUIInn* m_pUIInn;
	class CUICreateClanName* m_pUICreateClanName;
	class CUIAnvil* m_pUIAnvil;
	class CUIItemUpgradeDlg* m_pUIItemUpgradeDlg;


	class CUIKnightsOperation* m_pUIKnightsOp;					// ±â»ç´Ü ¸®½ºÆ® º¸±â, °¡ÀÔ, µî...
	class CUIPartyBBS* m_pUIPartyBBS;					// ÆÄÆ¼ Áö¿ø °Ô½ÃÆÇ.
	class CUITradeSellBBS* m_pUITradeBBS;					// »ó°Å·¡ °Ô½ÃÆÇ
	class CUIQuestMenu* m_pUIQuestMenu;					// Quest Menu
	class CUIQuestTalk* m_pUIQuestTalk;					// Quest Talk
	class CUIDead* m_pUIDead;						// Dead UI
	class CUITradeBBSSelector* m_pUITradeBBSSelector;			// »ó°Å·¡ °Ô½ÃÆÇ Á¾·ù ¼±ÅÃ
	class CUITradeBBSEditDlg* m_pUITradeBBSEdit;				// »ó°Å·¡ °Ô½Ã¹° ¼³¸í

	class CN3Shape* m_pTargetSymbol;				// ÇÃ·¹ÀÌ¾î°¡ Å¸°ÙÀ¸·Î ÀâÀº Ä³¸¯ÅÍÀÇ À§Ä¡À§¿¡ ±×¸®¸é µÈ´Ù..

	class CN3SndObjStream* m_pSnd_Town, * m_pSnd_Battle;	//¸¶À»À½¾Ç, ÀüÅõÀ½¾Ç Æ÷ÀÎÅÍ..
	class CMagicSkillMng* m_pMagicSkillMng;

	class CUINpcTalk* m_pUINpcTalk;
	class CUINPCChangeEvent* m_pUINpcChange;
	class CUIWarp* m_pUIWarp;
	class CWarMessage* m_pWarMessage;					// ÀüÀï°ü·Ã ¸Þ½ÃÁö
	class CLightMgr* m_pLightMgr;

	class CWhisperManager* m_pWhisperMgr;

	//..
	BOOL		m_bLoadComplete;							// ·ÎµùÀÌ ¿Ï·áµÇ¾ú³ª??

	float		m_fMsgSendTimeMove;							// ÃÖ±Ù ÀÌµ¿ ¸Þ½ÃÁö º¸³½ ½Ã°£..
	float		m_fMsgSendTimeRot;							// ÃÖ±Ù È¸Àü ¸Þ½ÃÁö º¸³½ ½Ã°£..
	float		m_fPlayerYawSended; // ÃÖ±Ù¿¡ ¸Þ½ÃÁö¸¦ º¸³½ ½ÃÁ¡ÀÇ ÇÃ·¹ÀÌ¾î y Ãà È¸Àü°ª.
	float		m_fRequestGameSave;	// ÃÖ±Ù¿¡ °ÔÀÓ µ¥ÀÌÅÍ ÀúÀåÀ» ¿äÃ»ÇÑ ½Ã°£
	__Vector3	m_vPlayerPosSended; // ÃÖ±Ù¿¡ ¸Þ½ÃÁö¸¦ º¸³½ ½ÃÁ¡ÀÇ ÇÃ·¹ÀÌ¾î À§Ä¡.

	__Vector3	m_vMouseLBClickedPos;
	__Vector3	m_vMouseSkillPos;

	float		m_fLBClickTime;

	int			m_iJoinReqClan;
	int			m_iJoinReqClanRequierID;

	int			KM_COUNT;

protected:
	virtual bool ProcessPacket(Packet& pkt);

	bool	MsgRecv_CharacterSelect(Packet& pkt); // virtual
	int		MsgRecv_VersionCheck(Packet& pkt); // virtual

	bool	MsgRecv_MyInfo_All(Packet& pkt);
	void	MsgRecv_MyInfo_HP(Packet& pkt);
	void	MsgRecv_MyInfo_MSP(Packet& pkt);
	void	MsgRecv_MyInfo_EXP(Packet& pkt);
	bool	MsgRecv_MyInfo_LevelChange(Packet& pkt);
	void	MsgRecv_MyInfo_RealmPoint(Packet& pkt);
	void	MsgRecv_MyInfo_PointChange(Packet& pkt);

	bool	MsgRecv_Chat(Packet& pkt);
	bool	MsgRecv_UserMove(Packet& pkt);
	bool	MsgRecv_Rotation(Packet& pkt);
	//	bool	MsgRecv_Dead(Packet& pkt);
	bool	MsgRecv_Regen(Packet& pkt);
	void	MsgRecv_Corpse(Packet& pkt);
	bool	MsgRecv_Time(Packet& pkt);
	bool	MsgRecv_Weather(Packet& pkt);

	bool	MsgRecv_UserInAndRequest(Packet& pkt);		// ÁÖÀ§ ¿µ¿ªÀÇ ¸ðµç User ¾ÆÀÌµð¸¦ Ä«¿îÆ®¸¸Å­ ¹Þ´Â´Ù... ±Û±¸.. ¾÷µ¥ÀÌÆ®°¡ ÇÊ¿äÇÑ °Í¸¸ ¼­¹ö¿¡°Ô ¿äÃ»..
	bool	MsgRecv_UserInRequested(Packet& pkt);					// ¼­¹ö¿¡°Ô ¿äÃ»ÇÑ À¯ÀúÀÇ ¾ÆÀÌµðµéÀ» ¹Þ¾Æ¼­ User Ã³¸®..
	bool	MsgRecv_UserInOut(Packet& pkt);
	bool	MsgRecv_UserIn(Packet& pkt, bool bWithFX = false);
	bool	MsgRecv_UserOut(Packet& pkt);
	void	MsgRecv_UserState(Packet& pkt);

	bool	MsgRecv_NPCInAndRequest(Packet& pkt);		// ÁÖÀ§ ¿µ¿ªÀÇ ¸ðµç NPC ¾ÆÀÌµð¸¦ Ä«¿îÆ®¸¸Å­ ¹Þ´Â´Ù... ±Û±¸.. ¾÷µ¥ÀÌÆ®°¡ ÇÊ¿äÇÑ °Í¸¸ ¼­¹ö¿¡°Ô ¿äÃ»..
	bool	MsgRecv_NPCInRequested(Packet& pkt);				// ¼­¹ö¿¡°Ô ¿äÃ»ÇÑ NPC ¾ÆÀÌµðµéÀ» ¹Þ¾Æ¼­ User Ã³¸®..
	bool	MsgRecv_NPCInOut(Packet& pkt);
	bool	MsgRecv_NPCIn(Packet& pkt);
	bool	MsgRecv_NPCOut(Packet& pkt);
	bool	MsgRecv_NPCMove(Packet& pkt);

	bool	MsgRecv_Attack(Packet& pkt);
	bool	MsgRecv_Dead(Packet& pkt);

	bool	MsgRecv_ItemMove(Packet& pkt);					// Item Move¿¡ ´ëÇÑ ÀÀ´ä..
	bool	MsgRecv_ItemBundleDrop(Packet& pkt);			// Item ÀÌ ÇÊµå¿¡ ³ªÅ¸³ª´Âµ¥¿¡ ´ëÇÑ ÀÀ´ä
	bool	MsgRecv_ItemBundleOpen(Packet& pkt);			// ¾ÆÀÌÅÛ »óÀÚ¸¦ ¿­°Å³ª ½ÃÃ¼¸¦ µÚÁø´Ù..
	bool	MsgRecv_ItemTradeStart(Packet& pkt);			// ¾ÆÀÌÅÛ »ó°Å·¡..
	bool	MsgRecv_ItemTradeResult(Packet& pkt);			// ¾ÆÀÌÅÛ »ó°Å·¡ °á°ú..
	bool	MsgRecv_ItemDroppedGetResult(Packet& pkt);	// ¶¥¿¡ ¶³¾îÁø ¾ÆÀÌÅÛ ¸Ô±â °á°ú..
	bool	MsgRecv_ItemWeightChange(Packet& pkt);		// ¾ÆÀÌÅÛ ¹«°Ô º¯È­..

	bool	MsgRecv_UserLookChange(Packet& pkt);			// °Ñ¸ð½ÀÀÌ ¹Ù²ï´Ù.. ¾ÆÀÌÅÛ Âø¿ëµî

	void	MsgRecv_TargetHP(Packet& pkt);

	void	MsgRecv_ZoneChange(Packet& pkt);			// °Á ´Ü¼øÇÑ Á¸ Ã¼ÀÎÁö.

	void	MsgRecv_Notice(Packet& pkt);
	void	MsgRecv_PartyOrForce(Packet& pkt);
	void	MsgRecv_PerTrade(Packet& pkt);

	void	MsgRecv_SkillChange(Packet& pkt);			// ½ºÅ³ º¯È­..
	void	MsgRecv_MagicProcess(Packet& pkt);			// ½ºÅ³ º¯È­..
	void	MsgRecv_ClassChange(Packet& pkt);			// Á÷¾÷ º¯È­..
	void	MsgRecv_ClassPromotion(Packet& pkt);

	void	MsgRecv_ObjectEvent(Packet& pkt);			// ¿ÀºêÁ§Æ® ÀÌº¥Æ® ½ÅÃ»¿¡ ´ëÇÑ ÀÀ´ä

	void	MsgRecv_DurabilityChange(Packet& pkt);		// ³»±¸·Â º¯°æ..
	void	MsgRecv_NpcEvent(Packet& pkt);				// Npc Event(Exchange, Repair both).. 

	void	MsgRecv_Knights(Packet& pkt);				// ±â»ç´Ü Á¶ÀÛ °ü·Ã ÆÐÅ¶..
	void	MsgRecv_KnightsListBasic(Packet& pkt);		// ±â»ç´Ü ±âº» Á¤º¸µé..

	void	MsgRecv_CompressedPacket(Packet& pkt);		// ¾ÐÃàµÈ ÆÐÅ¶ÀÌ´Ù... ¾ÐÃà Ç®°í ·çÇÁ¸¦ µ¹¸é¼­ ÇÑ¹ø´õ ÆÄ½ÌÇØ¾ß ÇÑ´Ù!!!
	void	MsgRecv_ContinousPacket(Packet& pkt);		// ºÙ¾î¼­ ¿À´Â ÆÐÅ¶ÀÌ´Ù.. ·çÇÁ¸¦ µ¹¸é¼­ ÇÑ¹ø´õ ÆÄ½ÌÇØ¾ß ÇÑ´Ù!!!

	void	MsgRecv_ItemRepair(Packet& pkt);			// Item Repair Result.. 
	void	MsgRecv_ItemCountChange(Packet& pkt);		// Item Count Change..
	void	MsgRecv_ItemDestroy(Packet& pkt);			// Item Count Change..

	void	MsgRecv_WareHouse(Packet& pkt);			// º¸°üÇÔ °ü·Ã ÆÐÅ¶..
	void	MsgRecv_WareHouseOpen(Packet& pkt);		// º¸°üÇÔ ¿ÀÇÂ..

	void	MsgRecv_Anvil(Packet& pkt);			// Anvil Mesajlarını handle et
	void	MsgRecv_AnvilItemUpgradeOpen(Packet& pkt);		// Anvil ekranını aç ve inventory yükle

	void	MsgRecv_NpcChangeOpen(Packet& pkt);		// Class Change¿Í ÃÊ±âÈ­..
	void	MsgRecv_AllPointInit(Packet& pkt);			// All Point ÃÊ±âÈ­..
	void	MsgRecv_SkillPointInit(Packet& pkt);		// Skill Point ÃÊ±âÈ­..
	void	MsgRecv_PointChangePriceQueryRequest(Packet& pkt);		// °¡°Ý¿¡ ´ëÇÑ ÀÀ´ä ÆÐÅ¶..

	void	MsgRecv_NoahChange(Packet& pkt);			// ³ë¾Æ º¯°æ..
	void	MsgRecv_WarpList(Packet& pkt);				// ¿öÇÁ ¸®½ºÆ®..
	//	void	MsgRecv_ServerCheckAndRequestConcurrentUserCount(Packet& pkt);			// ¼­¹ö IP ¿Í Æ÷Æ®¸¦ ¹Þ¾Æ µ¿Á¢ÀÚ¸¦ Ã¼Å©ÇØ º»´Ù..
	//	void	MsgRecv_ConcurrentUserCountAndSendServerCheck(Packet& pkt);

		//knights...
	void	MsgRecv_Knights_Create(Packet& pkt);
	void	MsgRecv_Knights_Withdraw(Packet& pkt);
	void	MsgRecv_Knights_Join(Packet& pkt);
	void	MsgRecv_Knights_Leave(Packet& pkt);
	void	MsgRecv_Knights_AppointViceChief(Packet& pkt);
	void	MsgRecv_Knights_MemberInfoAll(Packet& pkt);
	void	MsgRecv_Knights_GradeChangeAll(Packet& pkt);
	void	MsgRecv_Knights_Duty_Change(Packet& pkt);
	void	MsgRecv_Knigts_Join_Req(Packet& pkt);

public:
	void	ProcessUIKeyInput(bool bEnable = true);
	bool	OnMouseMove(POINT ptCur, POINT ptPrev);
	bool	OnMouseLbtnDown(POINT ptCur, POINT ptPrev);
	bool	OnMouseLBtnPressd(POINT ptCur, POINT ptPrev);
	bool	OnMouseLBtnPress(POINT ptCur, POINT ptPrev);
	bool	OnMouseLDBtnPress(POINT ptCur, POINT ptPrev);
	bool	OnMouseRbtnDown(POINT ptCur, POINT ptPrev);
	bool	OnMouseRBtnPressd(POINT ptCur, POINT ptPrev);
	bool	OnMouseRBtnPress(POINT ptCur, POINT ptPrev);
	bool	OnMouseRDBtnPress(POINT ptCur, POINT ptPrev);

	// added by d5dl2
	bool	OnMouseMBtnPress(POINT ptCur, POINT ptPrev);

	void	ControlViewVDegree(int16_t sValue);
	void	PlayBGM_Town();
	void	PlayBGM_Battle();

	void	DoCommercialTransaction(int iTradeID);								// »ó°Å·¡..

	const __InfoPartyOrForce* PartyOrForceConditionGet(bool& bIAmLeader, bool& bIAmMember, int& iMemberIndex, class CPlayerBase*& pTarget);
	void						TargetSelect(int iID, bool bMustAlive);
	void						TargetSelect(class CPlayerNPC* pTarget);

	void	CommandToggleUIChat();
	void	CommandToggleUIMsgWnd();

	bool	CommandToggleUIInventory();
	bool	CommandToggleUIState();
	bool	CommandToggleAttackContinous();
	bool	CommandToggleMoveContinous();
	bool	CommandToggleWalkRun();
	bool	CommandToggleUISkillTree();
	bool	CommandToggleUIMiniMap();
	bool	CommandToggleCmdList();
	bool	OpenCmdEdit(std::string msg);

	void	CommandMove(e_MoveDirection eMD, bool bStartOrEnd); // ¿òÁ÷ÀÌ´Â ¹æÇâ(ÀüÈÄÁø, ¸ØÃã), ¿òÁ÷ÀÌ±â ½ÃÀÛÇÏ´Â°¡?
	void	CommandEnableAttackContinous(bool bEnable, CPlayerBase* pTarget);
	void	CommandCameraChange(); // Ä«¸Þ¶ó ½ÃÁ¡ ¹Ù²Ù±â..
	void	CommandSitDown(bool bLimitInterval, bool bSitDown, bool bImmediately = false);

	void	CommandTargetSelect_NearstEnemy(); // °¡Àå °¡±î¿î Àû Å¸°Ù Àâ±â..
	void	CommandTargetSelect_NearstOurForce(); // °¡Àå °¡±î¿î ÆÄÆ¼ Å¸°ÙÀâ±â..

	void	CloseUIs(); // °¢Á¾ »ó°Å·¡, ¿öÇÁµîµî... UI ´Ý±â..

	void	NoahTrade(uint8_t bType, uint32_t dwGoldOffset, uint32_t dwGold);

	void	MsgOutput(const std::string& szMsg, D3DCOLOR crMsg);

	void	InitZone(int iZone, const __Vector3& vPosPlayer);
	void	InitUI();
	void	InitPlayerPosition(const __Vector3& vPos); // ÇÃ·¹ÀÌ¾î À§Ä¡ ÃÊ±âÈ­.. ÀÏÀ¸ÄÑ ¼¼¿ì°í, ±âº»µ¿ÀÛÀ» ÃëÇÏ°Ô ÇÑ´Ù.

	void	MsgSend_Continous();												// Æ¯Á¤ Á¶°Ç(?)ÇÏ¿¡¼­ ¼­¹ö¿¡°Ô Á¤±âÀûÀ¸·Î ¸Þ½ÃÁö¸¦ º¸³¿..
	void	MsgSend_Attack(int iTargetID, float fInterval, float fDistance);	// °ø°Ý ÆÐÅ¶ ³¯¸®±â - Å×ÀÌºíÀÇ °ø°Ý ÁÖ±â¸¦ °°ÀÌ Áà¼­ ÇØÅ·À» ¸·´Â´Ù.
	void	MsgSend_Move(bool bMove, bool bContinous);							// ¼­¹ö¿¡°Ô ¿òÁ÷ÀÓ ÆÐÅ¶À» ³¯¸°´Ù.. // ¿òÁ÷ÀÌ´Â°¡ ? ÁÖ±âÀûÀ¸·Î ¿òÁ÷ÀÌ´Â °Ç°¡?
	void	MsgSend_Rotation();													// ¼­¹ö¿¡°Ô È¸Àü ÆÐÅ¶À» ³¯¸°´Ù..
	void	MsgSend_Chat(enum e_ChatMode eMode, const std::string& szChat);		// ¼­¹ö¿¡°Ô Ã¤ÆÃ ¸Þ½ÃÁö¸¦ ³¯¸°´Ù..
	void	MsgSend_ChatSelectTarget(const std::string& szTargetID); // ÀÏ´ëÀÏ Ã¤ÆÃ »ó´ë Á¤ÇÏ±â.
	void	MsgSend_Regen();
	bool	MsgSend_RequestItemBundleOpen(CPlayerNPC* pCorpse); // ¾ÆÀÌÅÛ »óÀÚ¸¦ ¿­°Å³ª ½ÃÃ¼¸¦ µÚÁø´Ù..
	void	MsgSend_RequestTargetHP(int16_t siIDTarget, uint8_t byUpdateImmediately); // 0x00 - Á¡Â÷ ´Ã¾î³ª°Ô²û.. 0x01 - Áï½Ã ¾÷µ¥ÀÌÆ®..
	void	MsgSend_GameStart();
	bool	MsgSend_NPCEvent(int16_t siIDTarget);
	void	MsgSend_NPCInRequest(int iID); // NPC Á¤º¸°¡ ¾øÀ» °æ¿ì ¿äÃ»ÇÑ´Ù..
	void	MsgSend_UserInRequest(int iID); // User Á¤º¸°¡ ¾øÀ» °æ¿ì ¿äÃ»ÇÑ´Ù..
	void	MsgSend_Warp(); // ¿öÇÁ?? - Á¸Ã¼ÀÎÁö°¡ µÉ¼öµµ ÀÖ´Ù..
	void	MsgSend_StateChange(enum e_SubPacket_State eSP, int iState);
	void	MsgSend_PerTradeReq(int iDestID, bool bNear = true);
	void	MsgSend_SpeedCheck(bool bInit = false);

	void	MsgSend_PartyOrForcePermit(int iPartyOrForce, bool bYesNo); // iPartyOrForce 1 : Party, 2:Force
	void	MsgSend_PartyOrForceLeave(int iPartyOrForce); // iPartyOrForce 1 : Party, 2:Force
	bool	MsgSend_PartyOrForceCreate(int iPartyOrForce, const std::string& szID); // iPartyOrForce 1 : Party, 2:Force

	void	MsgSend_ObjectEvent(int iEventID, int iNPCID);		// ¿ÀºêÁ§Æ®¿¡ ¼³Á¤µÇ¾î ÀÖ´Â ÀÌº¥Æ® ¿äÃ»..
	void	MsgSend_Weather(int iWeather, int iPercent);
	void	MsgSend_Time(int iHour, int iMin);
	void	MsgSend_Administrator(enum e_SubPacket_Administrator eSP, const std::string& szID);

	void	MsgSend_KnightsJoin(int iTargetID);
	void	MsgSend_KnightsLeave(std::string& szName);
	void	MsgSend_KnightsWithdraw();
	void	MsgSend_KnightsAppointViceChief(std::string& szName);
	void	MsgSend_KnightsJoinReq(bool bJoin);
	void	MsgSend_PerTradeBBSReq(std::string szName, int iDestID);
	void	MsgSend_CharacterSelect(); // virtual


	void	ProcessPlayerInclination();				// °æ»ç Ã³¸®..(°¡¸¸È÷ ÀÖ¾îµµ °æ»ç°¡ ±ÞÇÏ¸é ¹Ì²ô·¯Áü..).
	void	ProcessLocalInput(uint32_t dwMouseFlags);	// Å°º¸µå ´­¸°°ÍÀ» Ã³¸®ÇÑ´Ù..
	void	ParseChattingCommand(const std::string& szCmd);


	void	UpdateUI_PartyOrForceButtons(); // Ä¿¸Çµå ÁÙ¿¡ ÀÖ´Â ÆÄÆ¼ ¹öÆ°À» »óÈ²¿¡ µû¶ó ¾÷µ¥ÀÌÆ® ÇØÁØ´Ù.
	void	UpdateUI_MiniMap();
	void	UpdateUI_TargetBar();
	void	UpdateBGM();
	void	UpdateCameraAndLight();

	void	RenderTarget();

	void	Init();									// UI ¿Í UI ¸®¼Ò½ºµîÀ» ÀÐ´Â´Ù.
	void	Release();								// Release..	
	void	ReleaseUIs();
	void	ReleaseSound();

	void	Tick();								// Àâ´ÙÇÑ °è»ê..
	void	Render();							// ·»´õ¸µ..

	CGameProcMain();									// »ý¼ºÀÚ.
	virtual ~CGameProcMain();							// ¼Ò¸êÀÚ.
};

#endif // !defined(AFX_GAMEPROCMAIN_H__E1C4F2CC_5AF3_4417_8917_A52CD5523DB3__INCLUDED_)
