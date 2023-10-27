#pragma once

#include "GameProcedure.h"
#include <set>

typedef std::set<int>::iterator it_ID;
typedef std::pair<it_ID, bool> pair_ID;

class CGameProcFxEditor : public CGameProcedure
{
	friend class CGameProcedure;
protected:
	std::set<int> m_SetNPCID;
	std::set<int> m_SetUPCID;

public:

	class CUIInventory* m_pUIInventory;			// ÀÎº¥Åä¸®
	class CUIVarious* m_pUIVar;				// Ä³¸¯ÅÍ »óÅÂÃ¢, ±â»ç´Ü °ü¸® µîÀÌ ÆäÀÌÁö·Î µé¾î°£ ´Ù¿ëµµ UI
	class CUIChat* m_pUIChatDlg;			// Main Chat Window
	class CUIMessageWnd* m_pUIMsgDlg;			// °ÔÀÓ ¸Þ½ÃÁö Ãâ·Â »óÀÚ.

	// Folded UI
	class CUIChat2* m_pUIChatDlg2;			// Main Chat window toggle off window
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
	class CUIDisguiseRing* m_pUIDisguiseRingDlg;

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
	void	ItemUpgradeOpen(uint16_t npcId);	// Anvil ekranını aç ve inventory yükle

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

	void	ProcessPlayerInclination();				// °æ»ç Ã³¸®..(°¡¸¸È÷ ÀÖ¾îµµ °æ»ç°¡ ±ÞÇÏ¸é ¹Ì²ô·¯Áü..).
	void	ProcessLocalInput(uint32_t dwMouseFlags);	// Å°º¸µå ´­¸°°ÍÀ» Ã³¸®ÇÑ´Ù..
	void	ParseChattingCommand(const std::string& szCmd);


	void	UpdateUI_PartyOrForceButtons(); // Ä¿¸Çµå ÁÙ¿¡ ÀÖ´Â ÆÄÆ¼ ¹öÆ°À» »óÈ²¿¡ µû¶ó ¾÷µ¥ÀÌÆ® ÇØÁØ´Ù.
	void	UpdateUI_MiniMap();
	void	UpdateUI_TargetBar();
	void	UpdateBGM();
	void	UpdateCameraAndLight();

	void	RenderTarget();

	void	CreateDummyCharacter();
	void	Init();									// UI ¿Í UI ¸®¼Ò½ºµîÀ» ÀÐ´Â´Ù.
	void	Release();								// Release..	
	void	ReleaseUIs();
	void	ReleaseSound();

	void	Tick();								// Àâ´ÙÇÑ °è»ê..
	void	Render();							// ·»´õ¸µ..

	CGameProcFxEditor();									// »ý¼ºÀÚ.
	virtual ~CGameProcFxEditor();							// ¼Ò¸êÀÚ.
};

