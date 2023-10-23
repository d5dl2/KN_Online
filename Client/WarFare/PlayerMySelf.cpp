// PlayerMgr.cpp: implementation of the CPlayerMySelf class.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
#include "PlayerMySelf.h"

#include "resource.h"
#include "PacketDef.h"
#include "PlayerOtherMgr.h"

#include "N3WorldManager.h"
#include "GameProcMain.h"
#include "UIInventory.h"
#include "MagicSkillMng.h"

#include "GameEng.h"

#include "DFont.h"
#include "N3Camera.h"
#include "N3ShapeMgr.h"
#include "N3SndObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayerMySelf::CPlayerMySelf()
{
	m_ePlayerType = PLAYER_MYSELF; // Player Type ... Base, NPC, OTher, MySelf

	m_bRunning = false;				// ¶Ù´ÂÁö..
	m_bMoveContinous = false;		// °è¼Ó ¿òÁ÷ÀÌ´ÂÁö..
	m_bAttackContinous = false;		// °è¼Ó °ø°ÝÇÏ´Â »óÅÂÀÎÁö..
	m_bSitDown = false;				// ¾É¾ÆÀÖ´Â »óÅÂÀÎÁö....
	m_bRecruitParty = false;		// ÆÄÆ¼¸ðÁýÁß??
	
	m_bStun = false;				// ±âÀý..
	m_fStunTime = 0.0f;				// ±âÀýÇÑ ½Ã°£..


	m_fAttackTimeRecent = CN3Base::TimeGet();	// ÃÖ±Ù¿¡ °ø°ÝÇÑ ½Ã°£..
	m_bTempMoveTurbo = false;

	m_InfoExt.Init();

	// Inventory..
	m_ChrInv.PartAlloc(PART_POS_COUNT);
	m_ChrInv.PlugAlloc(PLUG_POS_COUNT);

	m_iSendRegeneration = 0;	// ÇÑ¹ø º¸³»¸é ´Ù½Ã Á×À»¶§±îÁö ¾Èº¸³»´Â ÇÃ·¡±×	

	m_dwMagicID = 0xffffffff;
	m_fCastingTime = 0.0f;
	m_pObjectTarget = NULL; // Å¸°Ù ¿ÀºêÁ§Æ® Æ÷ÀÎÅÍ..
}

CPlayerMySelf::~CPlayerMySelf()
{
}



//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////



void CPlayerMySelf::Release()
{
	m_bTargetOrPosMove = false;
	m_iMoveTarget = -1;

	m_bRunning = false;				// ¶Ù´ÂÁö..
	m_bMoveContinous = false;		// °è¼Ó ¿òÁ÷ÀÌ´ÂÁö..
	m_bAttackContinous = false;		// °è¼Ó °ø°ÝÇÏ´Â »óÅÂÀÎÁö..
	m_bSitDown = false;				// ¾É¾ÆÀÖ´Â »óÅÂÀÎÁö....
	m_bRecruitParty = false;		// ÆÄÆ¼¸ðÁýÁß??

	m_bStun = false;				// ±âÀý..
	m_fStunTime = 0.0f;				// ±âÀýÇÑ ½Ã°£..

	m_fAttackTimeRecent = CN3Base::TimeGet();	// ÃÖ±Ù¿¡ °ø°ÝÇÑ ½Ã°£..
	m_bTempMoveTurbo = false;

	m_InfoExt.Init();

	// Inventory..
	m_ChrInv.Release();
	m_ChrInv.PartAlloc(PART_POS_COUNT);
	m_ChrInv.PlugAlloc(PLUG_POS_COUNT);

	m_pObjectTarget = NULL; // Å¸°Ù ¿ÀºêÁ§Æ® Æ÷ÀÎÅÍ..

	CPlayerBase::Release();
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

void CPlayerMySelf::SetMoveTargetID(int iID)
{
	m_bTargetOrPosMove	= true;
	m_iMoveTarget		= iID;
}

void CPlayerMySelf::SetMoveTargetPos(__Vector3 vPos)
{
	m_vTargetPos		= vPos;
	m_bTargetOrPosMove	= true;
	m_iMoveTarget		= -1;
}

void CPlayerMySelf::Tick()
{
	BOOL	bAnim = TRUE;

	if(	PSA_DEATH == m_eState)  // Á×´Â »óÅÂÀÌ°í... Á×´Â ¿¡´Ï¸ÞÀÌ¼ÇÀÌ ³¡³ª¸é.. // ÇÑ¹ø º¸³»¸é ´Ù½Ã Á×À»¶§±îÁö ¾Èº¸³»´Â ÇÃ·¡±×
	{
		if(0 == m_iSendRegeneration) 
		{
//			std::string szMsg; ::_LoadStringFromResource(IDS_REGENERATION, szMsg);
//			CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_REGENERATION);
//			CLogWriter::Write("Dead!!!");
			m_iSendRegeneration = 1;
		}
		return;
	}

	if(m_fTimeAfterDeath > 0) // °­Á¦·Î Á×¿©¾ß ÇÑ´Ù..
	{
		if(m_fTimeAfterDeath > 3.0f)
			this->Action(PSA_DYING, false); // 3 ÃÊ°¡ Áö³ª¾ß Á×´Â´Ù.
		CPlayerBase::Tick();  // È¸Àü, ÁöÁ¤µÈ ¿¡´Ï¸ÞÀÌ¼Ç Tick ¹× »ö±ò ÁöÁ¤ Ã³¸®.. µîµî..
		return;
	}
	
	if(IsDead()) // Á×Àº »óÅÂ¸é µ¹¾Æ°£´Ù.
	{
		CGameProcedure::s_pProcMain->CommandEnableAttackContinous(false, NULL);
		CPlayerBase::Tick();  // È¸Àü, ÁöÁ¤µÈ ¿¡´Ï¸ÞÀÌ¼Ç Tick ¹× »ö±ò ÁöÁ¤ Ã³¸®.. µîµî..
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// ³» ÇÃ·¹ÀÌ¾î´Â ¿òÁ÷ÀÌ´Â°Ô ´Ù¸¥³Ñµé°ú ´Ù¸£±â ¶§¹®¿¡ Æ¯º°ÇÏ°Ô Ã³¸®..
	if(	PSM_WALK == m_eStateMove ||
		PSM_WALK_BACKWARD  == m_eStateMove ||
		PSM_RUN == m_eStateMove ) // ¾ÕµÚ·Î °É¾î°¡°Å³ª ´Þ·Á°¡¸é.
	{
		this->MoveSpeedCalculationAndCheckCollision(); // ¿òÁ÷ÀÌ´Â ¼Óµµ ¹× Ãæµ¹Ã¼Å©...
		if(0 == m_fMoveSpeedPerSec) // ¼Óµµ°¡ ¾øÀ¸¸é Ãæµ¹Ã¼Å© °á°ú ¸ø°¡´Â °Å´Ù...
		{
			this->ActionMove(PSM_STOP);
			CGameProcedure::s_pProcMain->MsgSend_Move(false, false); // Á¤Áö ÆÐÅ¶ º¸³»±â..
		}

		__Vector3 vPos = this->Position();
		CPlayerBase::m_pSnd_MyMove = m_pSnd_Move;
		if(m_pSnd_Move && m_Chr.NeedPlaySound0()) 
			m_pSnd_Move->Play(&vPos);
		if(m_pSnd_Move && m_Chr.NeedPlaySound1()) 
			m_pSnd_Move->Play(&vPos);

		TargetOrPosMove();
	}
	else 
	{
		CPlayerBase::m_pSnd_MyMove = NULL;
		m_fMoveSpeedPerSec = 0;
	}
	// ³» ÇÃ·¹ÀÌ¾î´Â ¿òÁ÷ÀÌ´Â°Ô ´Ù¸¥³Ñµé°ú ´Ù¸£±â ¶§¹®¿¡ Æ¯º°ÇÏ°Ô Ã³¸®..
	////////////////////////////////////////////////////////////////////////////////

	CPlayerBase* pTarget = NULL;
//	if(true == m_bAttackContinous || m_iSkillStep > 0)
	if(true == m_bAttackContinous && m_iSkillStep <= 0)
	{
		pTarget = TargetPointerCheck(false);
		if(NULL == pTarget)
		{
			CGameProcedure::s_pProcMain->CommandEnableAttackContinous(false, NULL);
		}
		else
		{
			float fTime = CN3Base::TimeGet();
			
			// È°½î±â, ¼®±Ã ½î±â µî ½ºÅ³·Î Ã³¸®ÇÑ´Ù..
			if(	(m_pItemPlugBasics[PLUG_POS_LEFTHAND] && ITEM_CLASS_BOW == m_pItemPlugBasics[PLUG_POS_LEFTHAND]->byClass ) || 
				(m_pItemPlugBasics[PLUG_POS_LEFTHAND] && ITEM_CLASS_BOW_LONG == m_pItemPlugBasics[PLUG_POS_LEFTHAND]->byClass ) || 
				(m_pItemPlugBasics[PLUG_POS_RIGHTHAND] && ITEM_CLASS_BOW_CROSS == m_pItemPlugBasics[PLUG_POS_RIGHTHAND]->byClass) )
			{
				__TABLE_UPC_SKILL* pSkill = s_pTbl_Skill.Find(102003); // OKÇU R ÝLE ARCHERY ATIYOR
				if(pSkill)
				{
					if(pTarget->IsAlive())//ÀÓ½Ã À¯Àú°¡ »ì¾Æ ÀÖ´Â »óÅÂ¿¡¼­¸¸...
					{
						float fInterval = (pSkill->iCastTime / 10.f) + 0.15f;
						if(m_fAttackDelta > 0) fInterval /= m_fAttackDelta;
						if(fTime > m_fAttackTimeRecent + fInterval) // °ø°Ý °£°ÝÀÌ ³ÑÀ¸¸é.. °ø°Ý!
						{
							if(CGameProcedure::s_pProcMain->m_pMagicSkillMng->MsgSend_MagicProcess(m_iIDTarget, pSkill))
							{// ½ºÅ³ ÆÐÅ¶ º¸³»±â¿¡ ¼º°øÇÏ¸é.
								//TRACE("%.2f\n", fTime - m_fAttackTimeRecent);
							}
							m_fAttackTimeRecent = fTime;	// ÃÖ±Ù¿¡ °ø°ÝÇÑ ½Ã°£..
						}
					}
				}
			}
			else if( m_pItemPlugBasics[PLUG_POS_RIGHTHAND] && ITEM_CLASS_LAUNCHER == m_pItemPlugBasics[PLUG_POS_RIGHTHAND]->byClass ) // ÅõÃ¢¿ë ¾ÆÀÌÅÛÀÌ¸é..
			{
				__TABLE_UPC_SKILL* pSkill = s_pTbl_Skill.Find(102009); // ½ºÅ³ Å×ÀÌºí¿¡¼­ ±âº» È° ½ºÅ³À» Ã£°í
				if(pSkill && fTime > m_fAttackTimeRecent + (pSkill->iCastTime / 10.f)) // °ø°Ý °£°ÝÀÌ ³ÑÀ¸¸é.. °ø°Ý!
				{
					if(pTarget->IsAlive())//ÀÓ½Ã À¯Àú°¡ »ì¾Æ ÀÖ´Â »óÅÂ¿¡¼­¸¸
					{
						CGameProcedure::s_pProcMain->m_pMagicSkillMng->MsgSend_MagicProcess(m_iIDTarget, pSkill); // ½ºÅ³ ÆÐÅ¶ º¸³»±â¿¡ ¼º°øÇÏ¸é.
						//TRACE("%.2f\n", fTime - m_fAttackTimeRecent);
						m_fAttackTimeRecent = fTime;	// ÃÖ±Ù¿¡ °ø°ÝÇÑ ½Ã°£..
					}
				}
			}
			else // °Á °ø°ÝÀÌ¸é..
			{
				float fIntervalTable = 1.0f;
				if(m_pItemPlugBasics[PLUG_POS_RIGHTHAND] && m_pItemPlugExts[PLUG_POS_RIGHTHAND]) // °ø°Ý °£°Ý Á¤ÀÇ
				{
					fIntervalTable = (m_pItemPlugBasics[PLUG_POS_RIGHTHAND]->siAttackInterval / 100.0f)
						* (m_pItemPlugExts[PLUG_POS_RIGHTHAND]->siAttackIntervalPercentage / 100.0f); // 100 Àº 1ÃÊ 110 ÀÌ¸é 1.1 ÃÊµî.. ÆÛ¼¾Æ®·Î °£´Ù...
				}
				
				float fInterval = fIntervalTable;
				if(m_fAttackDelta > 0) fInterval /= m_fAttackDelta; // ½ºÅ³ÀÌ³ª ¸¶¹ý¿¡ ÀÇÇØ º¯ÇÏ´Â °ø°Ý ¼Óµµ.. 1.0 ÀÌ ±âº»ÀÌ°í Å¬¼ö·Ï ´õ »¡¸® °ø°ÝÇÑ´Ù.

				if(	fTime > m_fAttackTimeRecent + fInterval) // °ø°Ý °£°ÝÀÌ ³ÑÀ¸¸é.. °ø°Ý!
				{
					bool bCastingNow = CGameProcedure::s_pProcMain->m_pMagicSkillMng->IsCasting();
					if(false == bCastingNow) // Ä³½ºÆÃ ÁßÀÌ¸é °ø°Ý ÆÐÅ¶À» º¸³»Áö ¾Ê´Â´Ù.
					{
						bool bAttackable = IsAttackableTarget(pTarget);
						if(bAttackable) // °ø°Ý °¡´É..
						{
							float fDistance = s_pPlayer->DistanceExceptRadius(pTarget); // °ø°Ý°Å¸®
							
							CGameProcedure::s_pProcMain->MsgSend_Attack(pTarget->IDNumber(), fIntervalTable, fDistance);
							if(m_iSkillStep == 0 && PSA_ATTACK != m_eState && m_fFlickeringFactor == 1.0f) // ½ºÅ³À» ¾²´Â°Ô ¾Æ´Ñµ¥ °ø°ÝÇÏÁö ¾ÊÀ¸¸é..
								this->Action(PSA_ATTACK, true, pTarget); // °ø°Ý ÁßÀÌ¾Æ´Ï¸é °ø°ÝÇÑ´Ù..
							
							m_fAttackTimeRecent = fTime;	// ÃÖ±Ù¿¡ °ø°ÝÇÑ ½Ã°£..
						}
						else 
						{
							if(CGameProcedure::s_pEng->ViewPoint() == VP_THIRD_PERSON)
							{
								CGameProcedure::s_pPlayer->RotateTo(pTarget); // ¹æÇâÀ» µ¹¸°´Ù.

								float fDist = (pTarget->Position() - m_Chr.Pos()).Magnitude(); // °ø°Ý °Å¸®¸¦ ±¸ÇÏ°í..
								float fDistLimit = this->AttackableDistance(pTarget);

								if(fDist > fDistLimit && !m_bTargetOrPosMove)
								{
									CGameProcedure::s_pProcMain->CommandMove(MD_FOWARD, true);
									s_pPlayer->SetMoveTargetID(pTarget->IDNumber());
								}
							}
							if(m_iSkillStep == 0 && PSA_SITDOWN != m_eState) // ½ºÅ³À» ¾²´Â°Ô ¾Æ´Ñµ¥ ¾É¾ÆÀÖ´Â »óÅÂ°¡ ¾Æ´Ï¸é..
								this->Action(PSA_BASIC, true); // ±âº»ÀÚ¼¼..
						}
					}
				}
			}

			if(fTime == m_fAttackTimeRecent) // °ø°ÝÇßÀ¸¸é..
				CGameProcedure::s_pProcMain->CommandSitDown(false, false); // ÀÏÀ¸ÄÑ ¼¼¿î´Ù.. ¾É¾ÆÀÖ´Â »óÅÂ¿¡¼­ ¹ö±×°¡ ÀÖ´Ù..
		}
	}


	if(m_bStun) // ±âÀýÇØ ÀÖÀ¸¸é..
	{
		m_fStunTime -= s_fSecPerFrm; // ±âÀý ½Ã°£ Ã³¸®..
		if(m_fStunTime < 0) this->StunRelease(); // ±âÀý Ç®¾îÁÖ±â..
	}


	if(PSA_ATTACK == m_eState || m_iSkillStep != 0) // °ø°Ý ÁßÀÌ°Å³ª ½ºÅ³ »ç¿ëÁßÀÌ¸é..
	{
		if(!pTarget) pTarget = TargetPointerCheck(false); // Å¸°Ù Æ÷ÀÎÅÍ¸¦ ¾ò¾î¿Â´Ù..
		CPlayerBase::ProcessAttack(pTarget); // °ø°Ý¿¡ °üÇÑ ·çÆ¾ Ã³¸®.. ¿¡´Ï¸ÞÀÌ¼Ç ¼¼ÆÃ°ú Ãæµ¹¸¸ Ã³¸®ÇÒ»Ó ÆÐÅ¶Àº Ã³¸® ¾ÈÇÑ´Ù..
	}

	if(m_dwMagicID != 0xffffffff) 
		m_fCastingTime += CN3Base::s_fSecPerFrm;

	CPlayerBase::Tick(); // È¸Àü, ÁöÁ¤µÈ ¿¡´Ï¸ÞÀÌ¼Ç Tick ¹× »ö±ò ÁöÁ¤ Ã³¸®.. µîµî..
}

void CPlayerMySelf::Render(float fSunAngle)
{
	// °­Á¦·Î LOD ·¹º§ + 1
	m_Chr.m_nLOD--;
	if(m_Chr.m_nLOD < 0) m_Chr.m_nLOD = 0;

	int iLODDeltaPrev = CN3Chr::LODDelta(); // ³»Ä³¸¯Àº Á»´õ Á¤±³ÇÏ°Ô ±×¸®ÀÚ..
	CN3Chr::LODDeltaSet(0);
	CPlayerBase::Render(fSunAngle);
	CN3Chr::LODDeltaSet(iLODDeltaPrev);
}




bool CPlayerMySelf::ToggleAttackContinous()
{
	if(!IsAlive()) return false;

	if(false == m_bAttackContinous) // Å¸°ÙÀÌ ÀÖ´ÂÁö º»´Ù..
	{
		CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(m_iIDTarget, true);
		if(pTarget) //  && !IsOutOfAttackRange(pTarget)) // Å¸°ÙÀÌ ÀÖ°í °ø°Ý °¡´ÉÇÑ ¹üÀ§¿¡ ÀÖÀ»¶§¸¸
		{
			this->m_bAttackContinous = true;
		}
	}
	else
	{
		m_bAttackContinous = false;
	}

	return m_bAttackContinous;
}

bool CPlayerMySelf::ToggleRunMode()
{
	m_bRunning = !m_bRunning; // ¶Ù´ÂÁö..
	if(true == m_bRunning)
	{
		if(PSM_WALK == m_eStateMove) 
		{
			m_eStateMove = PSM_RUN;
			m_Chr.AniCurSet(ANI_RUN);
		}
	}
	else
	{
		if(PSM_RUN == m_eStateMove) 
		{
			m_eStateMove = PSM_WALK;
			m_Chr.AniCurSet(ANI_WALK);
		}
	}

	return m_bRunning;
}

void CPlayerMySelf::ToggleMoveMode()
{
	m_bMoveContinous = !m_bMoveContinous; // °è¼Ó ¿òÁ÷ÀÌ´ÂÁö..
	
	int nAni = ANI_BREATH;
	if(m_bMoveContinous) // °è¼Ó ¿òÁ÷¿©¾ß ÇÏ¸é..
	{
		if(m_bRunning) // ¶Ù¸é..
		{
			m_eStateMove = PSM_RUN;
			nAni = ANI_RUN;
		}
		else
		{
			m_eStateMove = PSM_WALK;
			nAni = ANI_WALK;
		}

		float fSpeed = this->MoveSpeedCalculationAndCheckCollision(); // ¿òÁ÷ÀÌ´Â ¼Óµµ ¹× Ãæµ¹Ã¼Å©...
		if(0 == fSpeed) // °¥¼ö ¾øÀ¸¸é...
		{
			m_bMoveContinous = false;
			m_eStateMove = PSM_STOP;
			m_eState = PSA_BASIC;
			nAni = this->JudgeAnimationBreath(); // Å¸°ÙÀÌ ÀÖÀ¸¸é. µé°í ÀÖ´Â ¹«±â¿¡ µû¶ó ±âº» ¿¡´Ï¸ÞÀÌ¼Ç ÆÇ´Ù.. ¾øÀ¸¸é °Á ±âº» ¿¡´Ï¸ÞÀÌ¼Ç
		}
	} 
	else
	{
		m_eStateMove = PSM_STOP;
		m_eState = PSA_BASIC;
		nAni = this->JudgeAnimationBreath(); // Å¸°ÙÀÌ ÀÖÀ¸¸é. µé°í ÀÖ´Â ¹«±â¿¡ µû¶ó ±âº» ¿¡´Ï¸ÞÀÌ¼Ç ÆÇ´Ù.. ¾øÀ¸¸é °Á ±âº» ¿¡´Ï¸ÞÀÌ¼Ç
	}

	this->AnimationClear();
	m_Chr.AniCurSet(nAni); // ÅëÂ°·Î °È°í µÚ±â, ¸ØÃß±â Àû¿ë
}

__Vector3 CPlayerMySelf::NextPos(float fTimeAfter)
{
	__Matrix44 mtxRot = m_Chr.Rot();
	__Vector3 vDir(0,0,1);
	__Vector3 vNextPos = m_Chr.Pos() + ((vDir * mtxRot) * m_fMoveSpeedPerSec * fTimeAfter);

	return vNextPos;
}

void CPlayerMySelf::RotAdd(const float fRotRadianPerSec)			// y ÃàÀ» ±âÁØÀ¸·Î ÃÊ´ç È¸ÀüÇÏ´Â ¼Óµµ¸¦ ³Ö¾îÁØ´Ù.
{
	m_fYawCur += fRotRadianPerSec * s_fSecPerFrm;

	if(m_fYawCur >= D3DXToRadian(180.0f) * 2) m_fYawCur -= D3DXToRadian(180.0f) * 2;
	if(m_fYawCur <= -D3DXToRadian(180.0f) * 2) m_fYawCur += D3DXToRadian(180.0f) * 2;
	m_fYawToReach = m_fYawCur; // ¹Ù·Î µ¹¸°´Ù..
}

void CPlayerMySelf::InventoryChrRender(const RECT& Rect)
{
/*
	uint32_t dwUsefog = TRUE;
	CN3Base::s_lpD3DDev->GetRenderState( D3DRS_FOGENABLE , &dwUsefog );

	uint32_t dwUseLighting=TRUE;
	CN3Base::s_lpD3DDev->GetRenderState( D3DRS_LIGHTING, &dwUseLighting );

	int	bLight[8];
	for ( int i = 0; i < 8; i++ )	CN3Base::s_lpD3DDev->GetLightEnable(i, &bLight[i]);

	if (dwUseLighting) CN3Base::s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	if (dwUsefog) CN3Base::s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	// set render states
	for ( i = 1; i < 8; i++ )	CN3Base::s_lpD3DDev->LightEnable(i, FALSE);
	CN3Base::s_lpD3DDev->LightEnable(0, TRUE);

	D3DLIGHT8 lgt0;
	
	memset(&lgt0, 0, sizeof(D3DLIGHT8));
	lgt0.Type = D3DLIGHT_POINT;
	lgt0.Attenuation0 = 0.5f;
	lgt0.Range = 100.0f;
	lgt0.Position = __Vector3(0.0f, 2.0f, -10.0f);
	lgt0.Diffuse.r = 220/255.0f; lgt0.Diffuse.g = 255/255.0f; lgt0.Diffuse.b = 220/255.0f;
	CN3Base::s_lpD3DDev->SetLight(0, &lgt0);

	//Ä³¸¯ÅÍ Âï±â..
	//
	__Matrix44 mtxproj, mtxview, mtxworld, mtxprojback, mtxviewback, mtxworldback;

	CN3Base::s_lpD3DDev->GetTransform( D3DTS_PROJECTION, &mtxprojback );
	CN3Base::s_lpD3DDev->GetTransform( D3DTS_VIEW, &mtxviewback );
	CN3Base::s_lpD3DDev->GetTransform( D3DTS_WORLD, &mtxworldback );

	D3DXMatrixOrthoLH(&mtxproj, 12.0f, 9.0f, 0, 100);  
    CN3Base::s_lpD3DDev->SetTransform( D3DTS_PROJECTION, &mtxproj );

    D3DXMatrixLookAtLH( &mtxview, &D3DXVECTOR3( 0.0f, 2.0f,-10.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
    CN3Base::s_lpD3DDev->SetTransform( D3DTS_VIEW, &mtxview );

	mtxworld.Identity();
	CN3Base::s_lpD3DDev->SetTransform( D3DTS_WORLD, &mtxworld );


///////////////////////////////////////////////////////////////
// 2d ÁÂÇ¥ ±¸ÇÔ..

	// 2d -> 3d..
	__Vector3 vPos;
	vPos.x = (float)((Rect.right - Rect.left)/2.0f + Rect.left);
	vPos.y = (float)(Rect.bottom) - (Rect.bottom - Rect.top)/16.0f;
	vPos.z = 0.11f;

	float fWidth, fHeight;
	fWidth = s_CameraData.vp.Width;	fHeight = s_CameraData.vp.Height;	fWidth *= 0.5f;	fHeight *= 0.5f;

	vPos.x = (vPos.x - fWidth)/fWidth;
	vPos.y = (fHeight - vPos.y)/fHeight;

	__Matrix44 mtxProjInv, mtxViewInv;
	D3DXMatrixInverse(&mtxProjInv, NULL, &mtxproj);
	D3DXMatrixInverse(&mtxViewInv, NULL, &mtxview);

	vPos *= mtxProjInv;
	vPos *= mtxViewInv;

	m_ChrInv.PosSet(vPos.x, vPos.y, 1.0f);
	__Quaternion qt;
	qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(198.0f));
	m_ChrInv.RotSet(qt);

	CGameProcedure::s_pEng->ClearZBuffer(NULL);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_ChrInv.m_nLOD = 0;
	m_ChrInv.Render();

	CN3Base::s_lpD3DDev->SetTransform( D3DTS_PROJECTION, &mtxprojback );
	CN3Base::s_lpD3DDev->SetTransform( D3DTS_VIEW, &mtxviewback );
	CN3Base::s_lpD3DDev->SetTransform( D3DTS_WORLD, &mtxworldback );

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_AMBIENT, 0x00000000);

	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, dwUseLighting );
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE , dwUsefog );
	for ( i = 0; i < 8; i++ )	CN3Base::s_lpD3DDev->LightEnable(i, bLight[i]);
*/
	// ¾Æ·¡·Î dino¼öÁ¤
	// backup render state
	DWORD dwLighting;
	D3DLIGHT9 BackupLight0;

	s_lpD3DDev->GetRenderState( D3DRS_LIGHTING, &dwLighting );
	BOOL bLight[8];
	for ( int i = 0; i < 8; ++i )	s_lpD3DDev->GetLightEnable(i, &(bLight[i]));
	s_lpD3DDev->GetLight(0, &BackupLight0);

	// set render state
	if (TRUE != dwLighting) s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	for ( int i = 1; i < 8; ++i )	s_lpD3DDev->LightEnable(i, FALSE);
	s_lpD3DDev->LightEnable(0, TRUE);

	// 0¹ø light ¼³Á¤
	D3DLIGHT9 Light0;
	memset(&Light0, 0, sizeof(D3DLIGHT9));
	Light0.Type = D3DLIGHT_POINT;
	Light0.Attenuation0 = 0.5f;
	Light0.Range = 100.0f;
	Light0.Position = __Vector3(0.0f, 2.0f, 10.0f);
	Light0.Diffuse.r = 220/255.0f; Light0.Diffuse.g = 255/255.0f; Light0.Diffuse.b = 220/255.0f;
	s_lpD3DDev->SetLight(0, &Light0);

	// Ä³¸¯ÅÍ À§Ä¡¿Í ¹æÇâ ¼¼ÆÃ
	m_ChrInv.PosSet(__Vector3(0,0,0));
	__Quaternion qt;
	qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(18.0f));	// ¾à°£ ºñ½ºµëÇÏ°Ô ¼­ÀÖ°Ô..
	m_ChrInv.RotSet(qt);

	// render
	RenderChrInRect(&m_ChrInv, Rect);

	// restore
	if (TRUE != dwLighting) s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, dwLighting );
	for (int i = 0; i < 8; ++i )	s_lpD3DDev->LightEnable(i, bLight[i]);
	s_lpD3DDev->SetLight(0, &BackupLight0);
}

void CPlayerMySelf::InventoryChrTick()
{
	// Inventory Animation Tick..
	m_ChrInv.PosSet(m_Chr.Pos());
	m_ChrInv.Tick();
}



void CPlayerMySelf::InventoryChrAnimationInitialize()
{
	int iAniTmp = m_ChrInv.AniCtrl()->Count() - 1;
	m_ChrInv.AniCurSet(iAniTmp, false, 0);
	m_ChrInv.AniCurSet(ANI_BREATH, false, 0);
	m_ChrInv.Tick(); // ÇÑ¹ø ÇØÁØ´Ù..
}

CN3CPlugBase* CPlayerMySelf::PlugSet(e_PlugPosition ePos, const std::string& szFN, __TABLE_ITEM_BASIC* pItemBasic, __TABLE_ITEM_EXT* pItemExt)
{
	int iJoint = 0;
	if(PLUG_POS_RIGHTHAND == ePos)
	{
		m_pItemPlugBasics[ePos] = pItemBasic;
		m_pItemPlugExts[ePos] = pItemExt;
		iJoint = m_pLooksRef->iJointRH;
	}
	else if(PLUG_POS_LEFTHAND == ePos)
	{
		m_pItemPlugBasics[ePos] = pItemBasic;
		m_pItemPlugExts[ePos] = pItemExt;
		if(pItemBasic)
		{
			if(pItemBasic->byClass == ITEM_CLASS_SHIELD) iJoint = m_pLooksRef->iJointLH2; // ¹æÆÐÀÎ °æ¿ì
			else iJoint = m_pLooksRef->iJointLH; // ¿Þ¼Õ ³¡..
		}
	}
	else if (PLUG_POS_KNIGHTS_GRADE == ePos)
	{
		m_pItemPlugBasics[ePos] = pItemBasic;
		m_pItemPlugExts[ePos] = pItemExt;
		iJoint = m_pLooksRef->iJointRH - 2; // ¿À¸¥ÂÊ ÆÈ¸ñ
	}
	else if (PLUG_POS_BACK == ePos)
	{
		//m_pItemBasicPlugRefs[PLUG_POS_BACK] = pItem;
	}
	else { __ASSERT(0, "Invalid Plug Item position"); }

	CN3CPlugBase* pPlug = m_ChrInv.PlugSet(ePos, szFN);
	if(NULL == pPlug) return NULL;

	if(PLUG_POS_LEFTHAND == ePos || PLUG_POS_RIGHTHAND == ePos) // Å°¿¡ ºñ·ÊÇØ¼­ Å©°Ô Å°¿î´Ù. ±âº»Å°´Â 1.8 ¹ÌÅÍÀÌ´Ù. 
	{
		float fScale = m_Chr.Height() / 1.8f;
		fScale *= pPlug->Scale().y / m_Chr.Scale().y;
		pPlug->ScaleSet(__Vector3(fScale, fScale, fScale));
		pPlug->m_nJointIndex = iJoint; // °üÀý ¹øÈ£ ¼¼ÆÃ..
	}
//	else if(PLUG_POS_BACK == ePos)
//	{
//		CN3CPlug_Cloak *pPlugCloak = (CN3CPlug_Cloak*)pPlug;
//		pPlugCloak->GetCloak()->SetPlayerBase(this);
//	}

	this->SetSoundPlug(pItemBasic);
	return CPlayerBase::PlugSet(ePos, szFN, pItemBasic, pItemExt);
}

CN3CPart* CPlayerMySelf::PartSet(e_PartPosition ePos, const std::string& szFN, __TABLE_ITEM_BASIC* pItemBasic, __TABLE_ITEM_EXT* pItemExt)
{
	if(ePos < PART_POS_UPPER || ePos >= PART_POS_COUNT)
	{
		__ASSERT(0, "Invalid Item Position");
		return NULL;
	}

	if(PART_POS_UPPER == ePos) // »óÃ¼ÀÏ °æ¿ì Æ¯º°ÇÑ Ã³¸®°¡ ÇÊ¿ä..
	{
		if(pItemBasic) // ÀÔÈ÷´Â °æ¿ì
		{
			if(pItemBasic->byIsRobeType && m_Chr.Part(PART_POS_LOWER)) // ·Îºê Å¸ÀÔÀÇ ÅëÂ¥ À­¿ÊÀÌ°í ¾Æ·¡¿¡ ¹º°¡ ÀÔ°í ÀÖÀ¸¸é..
			{
				m_ChrInv.PartSet(PART_POS_LOWER, ""); // ¾Æ·¡¸¦ ºñ¿öÁØ´Ù..
				m_Chr.PartSet(PART_POS_LOWER, "");
			}
		}
		else // »óÃ¼¸¦ ¹þ´Â °æ¿ì
		{
			if(m_pItemPartBasics[ePos] && m_pItemPartBasics[ePos]->byIsRobeType) // Àü¿¡ Âø¿ëÇß´ø°Ô ·Îºê¸é..
			{
				if(m_pItemPartBasics[PART_POS_LOWER]) // ÇÏÃ¼¿¡ ¾ÆÀÌÅÛÀÌ ÀÔÇôÀÖÀ¸¸é..
				{
					std::string szFN2;
					e_PartPosition ePartPos2 = PART_POS_UNKNOWN;
					e_PlugPosition ePlugPos2 = PLUG_POS_UNKNOWN;
					CGameProcedure::MakeResrcFileNameForUPC(m_pItemPartBasics[PART_POS_LOWER], &szFN2, NULL, ePartPos2, ePlugPos2, m_InfoBase.eRace);
					
					m_ChrInv.PartSet(PART_POS_LOWER, szFN2); // ÇÏÃ¼¿¡ ÀüÀÇ ¿ÊÀ» ÀÔÈù´Ù..
					m_Chr.PartSet(PART_POS_LOWER, szFN2);
				}
				else // ÇÏÃ¼¿¡ ÀÔ°í ÀÖ¾ú´ø ¾ÆÀÌÅÛÀÌ ¾ø´Ù¸é..
				{
					__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks.Find(m_InfoBase.eRace);	// User Player Character Skin ±¸Á¶Ã¼ Æ÷ÀÎÅÍ..
					m_ChrInv.PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER]); // ÇÏÃ¼¿¡ ±âº»¿ÊÀ» ÀÔÈù´Ù.
					m_Chr.PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER]); // ÇÏÃ¼¿¡ ±âº»¿ÊÀ» ÀÔÈù´Ù.
				}
			}
		}
	}
	else if(PART_POS_LOWER == ePos) // ÇÏÃ¼ÀÏ °æ¿ì..
	{
		// ¾ÆÀÌÅÛÀ» Âø¿ëÇÏ°Ç ¹þ°Ç°£¿¡...
		if(m_pItemPartBasics[PART_POS_UPPER] && m_pItemPartBasics[PART_POS_UPPER]->byIsRobeType) // Àü¿¡ »óÃ¼¿¡ Âø¿ëÇß´ø°Ô ·Îºê¸é..
		{
			m_pItemPartBasics[ePos] = pItemBasic; // ¾ÆÀÌÅÛ Àû¿ë
			m_pItemPartExts[ePos] = pItemExt;
			m_ChrInv.PartSet(ePos, "");
			m_Chr.PartSet(ePos, ""); // ÇÏÃ¼´Â ¹þ±â°í(?)..
			return NULL; // µ¹¾Æ°£´Ù.
		}
	}

	CN3CPart* pPart = NULL;
	if(szFN.empty()) // ÆÄÀÏ ÀÌ¸§ÀÌ ¾ø´Â°Å¸é.. ±âº» Âø¿ë..
	{
		if(PART_POS_HAIR_HELMET == ePos)
		{
			this->InitHair();
			pPart = m_Chr.Part(ePos);
		}
		else if(PART_POS_FACE == ePos)
		{
			this->InitFace();
			pPart = m_Chr.Part(ePos);
		}
		else
		{
			__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks.Find(m_InfoBase.eRace);	// Player Character Skin ±¸Á¶Ã¼ Æ÷ÀÎÅÍ..
			if(pLooks)
			{
				m_ChrInv.PartSet(ePos, pLooks->szPartFNs[ePos]);
				pPart = m_Chr.PartSet(ePos, pLooks->szPartFNs[ePos]);
				if(pPart) pPart->TexOverlapSet("");
			}
		}
	}
	else
	{
		m_ChrInv.PartSet(ePos, szFN);
		pPart = m_Chr.PartSet(ePos, szFN);
	}

	m_pItemPartBasics[ePos] = pItemBasic; // ¾ÆÀÌÅÛ Àû¿ë
	m_pItemPartExts[ePos] = pItemExt;

	return pPart;
}

bool CPlayerMySelf::InitChr(__TABLE_PLAYER_LOOKS* pTbl)
{
	CPlayerBase::InitChr(pTbl);

	m_ChrInv.JointSet(pTbl->szJointFN);
	m_ChrInv.AniCtrlSet(pTbl->szAniFN);
	
	float fScale = 2.1f / m_Chr.Height();
	m_ChrInv.ScaleSet(fScale, fScale, fScale); // ÀÎº¥Åä¸® Ã¢¿¡ µé¾î°¡°Ô Å©±â¸¦ ÁÙ¿©ÁØ´Ù..

	return true;
}

float CPlayerMySelf::AttackableDistance(CPlayerBase* pTarget)
{
	if(NULL == pTarget) return 0;

	float fDistLimit = (m_Chr.Radius() + pTarget->Radius())/2.0f; // °ø°Ý °Å¸®Á¦ÇÑ..
	if(m_pItemPlugBasics[0])
		fDistLimit += m_pItemPlugBasics[0]->siAttackRange / 10.0f; // ¾ÆÀÌÅÛÀ» µé°í ÀÖÀ¸¸é..
	else if(m_pItemPlugBasics[1] && ITEM_POS_TWOHANDLEFT == m_pItemPlugBasics[1]->byAttachPoint) // ¿Þ¼Õ¿¡ ÅõÇÚµå·¹ÇÁÆ®(È°) ¾ÆÀÌÅÛÀ» µé°í ÀÖÀ¸¸é..
		fDistLimit += m_pItemPlugBasics[1]->siAttackRange / 10.0f;

	return fDistLimit;
}

float CPlayerMySelf::DistanceExceptRadius(CPlayerBase* pTarget)
{
	if(NULL == pTarget) return 0;

	float fDist = (pTarget->Position() - m_Chr.Pos()).Magnitude();
	float fDistRadius = (m_Chr.Radius() + pTarget->Radius())/2.0f; // °ø°Ý °Å¸®Á¦ÇÑ..

	return fDist - fDistRadius;
}

bool CPlayerMySelf::IsAttackableTarget(CPlayerBase* pTarget, bool bMesureAngle)
{
	if(m_fFlickeringFactor != 1.0f) return false;	//ºÎÈ°ÇØ¼­ ±ôºýÀÌ´Â °æ¿ì´Â °ø°ÝÇÒ¼ö ¾ø´Ù.
	if(NULL == pTarget || pTarget->IsDead()) return false;  //Á×Àº »óÅÂÀÇ Ä³¸¯Àº °ø°ÝÇÏÁö ¸øÇÏ°Ô - ´Ü Á×±â Á÷ÀüÀÇ Ä³¸¯Àº Á¦¿ÜÇÑ´Ù..
	if(pTarget->m_InfoBase.eNation == m_InfoBase.eNation) return false; // °°Àº ±¹°¡ÀÌ´Ù..

	//-------------------------------------------------------------------------
	/*
	// TODO(srmeier): need to use ZoneAbilityType here
	// NOTE(srmeier): using zoneability information to determine if target is attackable
	if (!ACT_WORLD->canAttackSameNation() && (pTarget->m_InfoBase.eNation==m_InfoBase.eNation))
		return false;
	if (!ACT_WORLD->canAttackOtherNation() && (m_InfoBase.eNation == NATION_ELMORAD && pTarget->m_InfoBase.eNation == NATION_KARUS))
		return false;
	if (!ACT_WORLD->canAttackOtherNation() && (m_InfoBase.eNation == NATION_KARUS && pTarget->m_InfoBase.eNation == NATION_ELMORAD))
		return false;
	*/
	//-------------------------------------------------------------------------

	float fDist = (pTarget->Position() - m_Chr.Pos()).Magnitude(); // °ø°Ý °Å¸®¸¦ ±¸ÇÏ°í..
	float fDistLimit = this->AttackableDistance(pTarget);
	if(fDist > fDistLimit) return false; // °Å¸®°¡ ÀÏÁ¤ÀÌ»ó ¶³¾îÁ® ÀÖÀ¸¸é µ¹¾Æ°£´Ù.

	if(bMesureAngle)// °¢µµ¸¦ º»´Ù..
	{
		__Vector3 vDir = this->Direction(); vDir.y = 0; vDir.Normalize();
		__Vector3 vDirTarget = pTarget->Position() - m_Chr.Pos(); vDirTarget.y = 0; vDirTarget.Normalize();
		float fDot = vDir.Dot(vDirTarget);
		if(fDot > 0.7f) return true;
		else return false;
	}

	return true;
}

// TODO(Gilad): It'd probably be better to refactor this one as it modifies the m_fYNext variable which is part of the PlayerBase
// class and when the packet is sent to the game server, it reads the next Y coordinates to climb the terrain height.
// Why? Because we just checking if collision detected and it should just check if collision detected and not modify any global members
// or pointers. It would be more partical to check the whole thing if authority = GM when more code is implemented here in the future.
// Another option would be to send a packet to the server validate the collision through the game server and let the game server check 
// against wall hacking.
bool CPlayerMySelf::CheckCollision()
{
	float fSpeed = m_fMoveSpeedPerSec * CN3Base::s_fSecPerFrm; // The speed at which the current moving speed is calculated.
	if(0 == fSpeed) return false; // If it does not move, do not check collision.

	__Vector3 vPos = this->Position();

	__Vector3 vDir(0,0,1); // Find the rotation value ...
	__Matrix44 mtxRot = this->Rotation(); 
	vDir *= mtxRot; // Find the direction of rotation.
	if(fSpeed < 0)
	{
		fSpeed *= -1.0f;
		vDir *= -1.0f;
	}
	__Vector3 vPosNext = vPos + (vDir * fSpeed); // ´ÙÀ½ À§Ä¡ °è»ê..
	if (s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER && 
		false == ACT_WORLD->IsInTerrainWithTerrain(vPosNext.x, vPosNext.z, vPos) )
		return true; // °æ°è ¾È¿¡ ÀÖÁö ¾ÊÀ¸¸é..


	//////////////////////////////////
	// ´Ù¸¥ ÇÃ·¹ÀÌ¾î¿Í Ã¼Å©..
	CPlayerOther* pUPC = NULL;
	float fHeightSum, fMag;
	it_UPC it = s_pOPMgr->m_UPCs.begin(), itEnd = s_pOPMgr->m_UPCs.end();
	for(; it != itEnd; it++)
	{
		pUPC = it->second;

		if(pUPC->IsDead()) continue; //Á×¾î ÀÖ´Â »óÅÂÀÇ Ä³¸¯ÅÍ´Â Ãæµ¹Ã¼Å©¸¦ ÇÏÁö ¾Ê´Â´Ù.
		if(m_InfoBase.eNation == pUPC->m_InfoBase.eNation) continue; // °°Àº ±¹°¡...

		//-------------------------------------------------------------------------
		/*
		// TODO(srmeier): need to use ZoneAbilityType here
		// NOTE(srmeier): using zoneability information to determine if target is colliable
		if (!ACT_WORLD->canAttackSameNation() && (pUPC->m_InfoBase.eNation == m_InfoBase.eNation))
			continue;
		if (!ACT_WORLD->canAttackOtherNation() && (m_InfoBase.eNation == NATION_ELMORAD && pUPC->m_InfoBase.eNation == NATION_KARUS))
			continue;
		if (!ACT_WORLD->canAttackOtherNation() && (m_InfoBase.eNation == NATION_KARUS && pUPC->m_InfoBase.eNation == NATION_ELMORAD))
			continue;
		*/
		//-------------------------------------------------------------------------
		
		fMag = (pUPC->Position() - vPos).Magnitude();
		if(fMag < 32.0f)
		{
			fHeightSum = (pUPC->Height() + m_Chr.Height()) / 2.5f;
			if(fMag < fHeightSum) // °Å¸®°¡ Å°ÀÇ ÇÕº¸´Ù ÀÛÀ¸¸é..
			{
				float fMag2 = (pUPC->Position() - vPosNext).Magnitude(); // ´ÙÀ½À§Ä¡°¡ ´õ °¡±î¿ì¸é.
				if(s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER && fMag2 < fMag)
					return true;
			}
		}
	}
	// ´Ù¸¥ ÇÃ·¹ÀÌ¾î¿Í Ã¼Å©..
	//////////////////////////////////

//	__TABLE_ZONE* pZoneInfo = s_pTbl_Zones.Find(m_InfoExt.iZoneCur);
//	if(pZoneInfo && pZoneInfo->bNPCCollisionCheck) //this_zone

	//Àû±¹ ¿£ÇÇ¾¾´Â Ãæµ¹ Ã¼Å©¸¦ ÇÑ´Ù.
	CPlayerNPC* pNPC = NULL;
	it_NPC it_N = s_pOPMgr->m_NPCs.begin(),	it_NEnd	= s_pOPMgr->m_NPCs.end();
	for(; it_N != it_NEnd; it_N++)
	{
		pNPC = it_N->second;

		if(pNPC->m_pShapeExtraRef) continue; // ¼º¹®µîÀÇ ÇüÅÂÀÌ¸é Ãæµ¹Ã¼Å©¸¦ ÇÏÁö ¾Ê´Â´Ù..

		if(m_InfoBase.eNation == pNPC->m_InfoBase.eNation) continue; // °°Àº ±¹°¡...
		// NOTE(srmeier): I believe these are for passing through monsters and such
		if(m_InfoBase.eNation == NATION_KARUS && pNPC->m_InfoBase.eNation != NATION_ELMORAD) continue; // Àû±¹ ¿£ÇÇ¾¾´Â Ãæµ¹ Ã¼Å©¸¦ ÇÑ´Ù.
		if(m_InfoBase.eNation == NATION_ELMORAD && pNPC->m_InfoBase.eNation != NATION_KARUS) continue; // 

		//-------------------------------------------------------------------------
		/*
		// TODO(srmeier): need to use ZoneAbilityType here
		// NOTE(srmeier): using zoneability information to determine if target is colliable
		if (!ACT_WORLD->canAttackSameNation() && (pNPC->m_InfoBase.eNation == m_InfoBase.eNation))
			continue;
		if (!ACT_WORLD->canAttackOtherNation() && (m_InfoBase.eNation == NATION_ELMORAD && pNPC->m_InfoBase.eNation == NATION_KARUS))
			continue;
		if (!ACT_WORLD->canAttackOtherNation() && (m_InfoBase.eNation == NATION_KARUS && pNPC->m_InfoBase.eNation == NATION_ELMORAD))
			continue;
		*/
		//-------------------------------------------------------------------------

		fMag = (pNPC->Position() - vPos).Magnitude();
		if(fMag < 32.0f)
		{
			fHeightSum = (pNPC->Height() + m_Chr.Height()) / 2.5f;
			if(fMag < fHeightSum) // °Å¸®°¡ Å°ÀÇ ÇÕº¸´Ù ÀÛÀ¸¸é..
			{
				float fMag2 = (pNPC->Position() - vPosNext).Magnitude(); // ´ÙÀ½À§Ä¡°¡ ´õ °¡±î¿ì¸é.
				if(s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER && fMag2 < fMag)
					return true;
			}
		}
	}//for(


	// ¿ÀºêÁ§Æ®¿Í Ãæµ¹Ã¼Å©..
	__Vector3 vPos2 = vPos, vCol, vNormal;
	if (!s_pWorldMgr->IsIndoor())
		vPos2.y += 0.5f; // Ä³¸¯ÅÍ ¹ß³ôÀÌ¿¡¼­ 0.5 ¹ÌÅÍ ³ôÀÌ À§¿¡¼­ Ãæµ¹Ã¼Å©ÇÑ´Ù.
	else
		vPos2.y += 0.6f; // Ä³¸¯ÅÍ ¹ß³ôÀÌ¿¡¼­ 0.6 ¹ÌÅÍ ³ôÀÌ À§¿¡¼­ Ãæµ¹Ã¼Å©ÇÑ´Ù.	ÀÌ ÇÔ¼ö ³»¿¡¼­ ¾²´Â 0.6Àº PvsMgrÀÇ m_fVolumeOffs.. ^^
	bool bColShape = ACT_WORLD->CheckCollisionWithShape(vPos2, vDir, fSpeed, &vCol, &vNormal);
	if(s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER && bColShape) return true; // ¿ÀºêÁ§Æ®¿Í Ãæµ¹°ªÀÌ ÀÖÀ¸¸é 

	////////////////////////////////////////////////////////////////////////////////
	// Áö¸é°ú ¿ÀºêÁ§Æ®ÀÇ ³ôÀÌ°ª ±¸ÇÏ±â..
	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(vPosNext.x, vPosNext.z);		// Áö¸éÀÇ ³ôÀÌ°ª..
	float fYClimb = ACT_WORLD->GetHeightNearstPosWithShape(vPosNext, CN3Base::s_fSecPerFrm * 30.0f, &vNormal); // Ãæµ¹ Ã¼Å© ¿ÀºêÁ§Æ®ÀÇ ³ôÀÌ°ª..
	vNormal.y = 0; // ÀÌ·¡¾ß Á¤»óÀûÀÎ °æ»ç¸¦ ¾òÀ»¼ö ÀÖ´Ù..
	
	if (!s_pWorldMgr->IsIndoor())
	{
		if(fYClimb > fYTerrain && fYClimb < vPosNext.y + ((30.0f/CN3Base::s_fFrmPerSec) * 0.5f)) // Ãæµ¹ Ã¼Å© ¿ÀºêÁ§Æ® ³ôÀÌ°ªÀÌ ÀÖ°í ÁöÇüº¸´Ù ³ôÀ» °æ¿ì¸¸ ³ôÀÌ°ª Àû¿ë
		{
			if(s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER && 
				vNormal.Magnitude() > MAX_INCLINE_CLIMB && vNormal.Dot(vDir) <= 0.0f) // °æ»ç Ã¼Å©..
			{
				return true;
			}
			m_fYNext = fYClimb;
		}
		else
		{
			// ÁöÇüÀÇ °æ»ç°¡ 45 µµ ÀÌÇÏÀÎÁö Ã¼Å©
			if(s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER && 
				true == ACT_WORLD->CheckInclineWithTerrain(vPosNext, vDir, MAX_INCLINE_CLIMB))
			{
				return true;
			}
			m_fYNext = fYTerrain; // ´ÙÀ½ À§Ä¡¸¦ ¸ÂÃß°í..
		}
	}
	else	// ÀÏ´Ü..
	{
		if ((fYClimb > fYTerrain) && (fYClimb < vPosNext.y + 0.6f))
			m_fYNext = fYClimb;
		else
			m_fYNext = fYTerrain; // ´ÙÀ½ À§Ä¡¸¦ ¸ÂÃß°í..

		if ((m_fYNext > vPos.y + 0.6f) || (m_fYNext < vPos.y - 0.6f*2.0f))
		{
			m_fYNext = vPos.y;
			if (s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER) return true;
		}
	}

//	else // ¿Ã¶ó°¥¼ö ¾ø´Â °÷ÀÌ¸é ÁöÇü°úÀÇ ±â¿ï±â Ã¼Å©..
//	{
//		// ¹æÇâÀ» ±¸ÇØ¼­.. ±â¿ï±â¿¡ µû¶ó ´Ù¸¥ ¼Óµµ¸¦ Àû¿ë
//		s_pTerrain->GetNormal(vPos.x, vPos.z, vNormal);
//		vNormal.Normalize();
//		vNormal.y	= 0.0f;
//		float fM = vNormal.Magnitude();
//		float fD = vNormal.Dot(vDir);
//		if(fSpeed < 0) fD *= -1.0f;
//		if(fD < 0) fSpeed *= 1.0f - (fM / 0.7071f); // ±â¿ï±â¿¡ µû¸¥ ÆÑÅÍ Àû¿ë
//
//		vPosNext = vPos + (vDir * fSpeed); // ´ÙÀ½ À§Ä¡ °è»ê..
//		m_fYNext = s_pTerrain->GetHeight(vPosNext.x, vPosNext.z);
//	}

	this->PositionSet(vPosNext, false);

	///////////////////////////////////////////////////////////////
	// Ä³¸¯ÅÍ Ãæµ¹ Ã¼Å©..
//	int iSize = s_pOPMgr->m_OPCs.size();
//	it_UPC it = s_pOPMgr->m_OPCs.begin();
//	for( int i = 0; i < iSize; i++, it++ )
//	{
//		if ( ((*it)->Position() - vPosAfter).Magnitude() < 1.2f )
//			return vPosBefore;
//	}

	return false;
}


void CPlayerMySelf::InitFace()
{
	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks.Find(m_InfoBase.eRace);
	if(pLooks && !pLooks->szPartFNs[PART_POS_FACE].empty()) // ¾ÆÀÌÅÛÀÌ ÀÖ°í ¾ó±¼ ÀÌ¸§ÀÌ ÀÖÀ¸¸é..
	{
		char szBuff[256] = "", szDir[128] = "", szFName[128] = "", szExt[16] = "";
		::_splitpath(pLooks->szPartFNs[PART_POS_FACE].c_str(), NULL, szDir, szFName, szExt);
		sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, m_InfoExt.iFace, szExt);
		this->PartSet(PART_POS_FACE, szBuff, NULL, NULL);
	}
}

void CPlayerMySelf::InitHair()
{
	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks.Find(m_InfoBase.eRace);
	if(pLooks && !pLooks->szPartFNs[PART_POS_HAIR_HELMET].empty()) // ¾ÆÀÌÅÛÀÌ ÀÖ°í ¾ó±¼ ÀÌ¸§ÀÌ ÀÖÀ¸¸é..
	{
		char szBuff[256] = "", szDir[128] = "", szFName[128] = "", szExt[16] = "";
		::_splitpath(pLooks->szPartFNs[PART_POS_HAIR_HELMET].c_str(), NULL, szDir, szFName, szExt);
		sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, m_InfoExt.iHair, szExt);
		this->PartSet(PART_POS_HAIR_HELMET, szBuff, NULL, NULL);
	}
	else
	{
		m_Chr.PartSet(PART_POS_HAIR_HELMET, "");
		m_ChrInv.PartSet(PART_POS_HAIR_HELMET, "");
	}
}

void CPlayerMySelf::KnightsInfoSet(int iID, const std::string& szName, int iGrade, int iRank)
{
	CPlayerBase::KnightsInfoSet(iID, szName, iGrade, iRank);

	m_InfoExt.iKnightsID = iID;
	m_InfoExt.szKnights = szName;
	m_InfoExt.iKnightsGrade = iGrade;
	m_InfoExt.iKnightsRank = iRank;

	if(m_InfoExt.szKnights.empty()) { if(m_pClanFont) delete m_pClanFont; m_pClanFont = NULL; }
	else
	{
		if(!m_pClanFont)
		{
			std::string szFontID; ::_LoadStringFromResource(IDS_FONT_ID, szFontID);
			m_pClanFont = new CDFont(szFontID, 12);
			m_pClanFont->InitDeviceObjects( s_lpD3DDev );
			m_pClanFont->RestoreDeviceObjects();
		}

		m_pClanFont->SetText(m_InfoExt.szKnights.c_str(), D3DFONT_BOLD); // ÆùÆ®¿¡ ÅØ½ºÆ® ÁöÁ¤.
		m_pClanFont->SetFontColor(KNIGHTS_FONT_COLOR);
	}
}

void CPlayerMySelf::SetSoundAndInitFont(uint32_t dwFontFlag)
{
	CPlayerBase::SetSoundAndInitFont();
	
	if(m_InfoExt.szKnights.empty()) { delete m_pClanFont; m_pClanFont = NULL; }
	else
	{
		if(!m_pClanFont)
		{
			std::string szFontID; ::_LoadStringFromResource(IDS_FONT_ID, szFontID);
			m_pClanFont = new CDFont(szFontID, 12, D3DFONT_BOLD);
			m_pClanFont->InitDeviceObjects( s_lpD3DDev );
			m_pClanFont->RestoreDeviceObjects();
		}

		m_pClanFont->SetText(m_InfoExt.szKnights.c_str()); // ÆùÆ®¿¡ ÅØ½ºÆ® ÁöÁ¤.
		m_pClanFont->SetFontColor(KNIGHTS_FONT_COLOR);
	}
}

float CPlayerMySelf::MoveSpeedCalculationAndCheckCollision() // ¼Óµµ¸¦ ±¸ÇÏ°í ±× ¼Óµµ·Î Ãæµ¹ Ã¼Å©¸¦ ÇÑ´Ù. ¸®ÅÏ°ªÀÌ 0 ÀÌ¸é Ãæµ¹ÀÌ´Ù..
{
	m_fMoveSpeedPerSec = MOVE_SPEED_WHEN_WALK;
	if(PSM_RUN == m_eStateMove) m_fMoveSpeedPerSec *= MOVE_DELTA_WHEN_RUNNING;
	else if(PSM_STOP == m_eStateMove) m_fMoveSpeedPerSec = 0.0f;
	else if(PSM_WALK_BACKWARD == m_eStateMove) m_fMoveSpeedPerSec *= -1.0f;
	m_fMoveSpeedPerSec *= m_fMoveDelta; // ÀÌµ¿ ¼Óµµ º¯ÇÏ±â¿¡ µû¶ó¼­... °öÇØÁØ´Ù.

	if(m_bTempMoveTurbo) // °³¹ß¿ë ÇÃ·¡±×...
	{
		m_fMoveSpeedPerSec *= 10.0f;
	}

	if(this->CheckCollision()) // Ãæµ¹ Ã¼Å©¸é..
	{
		if(CGameProcedure::s_pProcMain)
			CGameProcedure::s_pProcMain->CommandMove(MD_STOP, true);
		m_fMoveSpeedPerSec = 0;
	}

	// ¹æÇâÀ» ±¸ÇØ¼­.. ±â¿ï±â¿¡ µû¶ó ´Ù¸¥ ¼Óµµ¸¦ Àû¿ë
/*	__Vector3 vDir = this->Direction();
	__Vector3 vPos = m_Chr.Pos(), vNormal(0,1,0);
	s_pTerrain->GetNormal(vPos.x, vPos.z, vNormal);
	vNormal.Normalize();
	vNormal.y	= 0.0f;
	float fM = vNormal.Magnitude();
	float fD = vNormal.Dot(vDir);
	if(fSpeed < 0) fD *= -1.0f;
//	if (fM > MAX_INCLINE_CLIMB && fD <= 0.0f )
//	{
//	}
	if(fD < 0) fSpeed *= 1.0f - (fM / 0.7071f); // ±â¿ï±â¿¡ µû¸¥ ÆÑÅÍ Àû¿ë
*/	
	return m_fMoveSpeedPerSec;

}

void CPlayerMySelf::Stun(float fTime) // ÀÏÁ¤ÇÑ ½Ã°£µ¿¾È ±âÀý ½ÃÅ°±â.
{
	m_bStun = true;				// ±âÀý..
	m_fStunTime = fTime;		// ±âÀýÇÑ ½Ã°£..

	CGameProcedure::s_pProcMain->CommandEnableAttackContinous(false, NULL); // °ø°Ý ¸ØÃß°í..
	CGameProcedure::s_pProcMain->CommandMove(MD_STOP, true); // ¿òÁ÷ÀÓÀ» ¸ØÃß°Ô ÇÏ°í..
}

void CPlayerMySelf::StunRelease() // ±âÀý Ç®±â..
{
	m_bStun = false;			// ±âÀý..
	m_fStunTime = 0.0f;			// ±âÀýÇÑ ½Ã°£..
}

void CPlayerMySelf::TargetOrPosMove()
{
	if(!m_bTargetOrPosMove) return;

	m_vTargetPos.y = m_Chr.Pos().y;

	if( m_iMoveTarget >= 0 )
	{
		CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(m_iMoveTarget, true);
		if(pTarget)
		{
			m_vTargetPos = pTarget->Position();
		}
		else
		{
			this->ActionMove(PSM_STOP);
			CGameProcedure::s_pProcMain->MsgSend_Move(false, false); // Á¤Áö ÆÐÅ¶ º¸³»±â..
		}
	}

	__Vector3 vDir = m_vTargetPos - s_pPlayer->Position();
	vDir.y = 0.0f;
	vDir.Normalize();

	float fYaw = ::_Yaw2D(vDir.x, vDir.z);
	this->RotateTo(fYaw, true); // ¹æÇâÀ» µ¹¸®°í

	if( m_iMoveTarget >= 0 )
	{
		CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(m_iMoveTarget, true);

		float fDist = (pTarget->Position() - m_Chr.Pos()).Magnitude(); // °ø°Ý °Å¸®¸¦ ±¸ÇÏ°í..
		float fDistLimit = this->AttackableDistance(pTarget);

		if(fDist < fDistLimit)
		{
			CGameProcedure::s_pProcMain->CommandEnableAttackContinous(true, pTarget);
			this->ActionMove(PSM_STOP);
			CGameProcedure::s_pProcMain->MsgSend_Move(false, false); // Á¤Áö ÆÐÅ¶ º¸³»±â..
		}
	}
	else
	{
		float fDist = (m_vTargetPos - Position()).Magnitude();
		if( fDist < 0.5f )
		{
			this->ActionMove(PSM_STOP);
			CGameProcedure::s_pProcMain->MsgSend_Move(false, false); // Á¤Áö ÆÐÅ¶ º¸³»±â..
		}
	}
}
