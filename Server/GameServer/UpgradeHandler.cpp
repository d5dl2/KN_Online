﻿#include "stdafx.h"
#include "../shared/DateTime.h"
#include <cmath>

// Some item ID definitions
#define MIN_ITEM_ID 100000000
#define MAX_ITEM_ID 999999999

#define MAGE_EARRING 310310004
#define WARRIOR_EARRING 310310005
#define ROGUE_EARRING 310310006
#define PRIEST_EARRING 310310007

#define UPGRADE_RUBY_EARRING_MIN 310110005
#define UPGRADE_RUBY_EARRING_MAX 310110007

#define UPGRADE_PEARL_EARRING_MIN 310150005
#define UPGRADE_PEARL_EARRING_MAX 310150007

#define SHADOW_PIECE			700009000

/**
* @brief	Packet handler for the assorted systems that
* 			were deemed to come under the 'upgrade' system.
*
* @param	pkt	The packet.
*/
void CUser::ItemUpgradeProcess(Packet& pkt)
{
	uint8_t opcode = pkt.read<uint8_t>();
	switch (opcode)
	{
	case ITEM_UPGRADE:
		ItemUpgrade(pkt);
		break;

	case ITEM_ACCESSORIES:
		ItemUpgradeAccessories(pkt);
		break;

	case ITEM_BIFROST_EXCHANGE:
		BifrostPieceProcess(pkt);
		break;

	case ITEM_UPGRADE_REBIRTH:
		ItemUpgradeRebirth(pkt);
		break;

	case ITEM_SEAL:
		ItemSealProcess(pkt);
		break;

	case ITEM_CHARACTER_SEAL:
		CharacterSealProcess(pkt);
		break;

	case ITEM_SPECIAL_EXCHANGE:
		SpecialItemExchange(pkt);
		break;
	}
}

/**
* @brief	Packet handler for the standard item upgrade system.
*
* @param	pkt	The packet.
*/
void CUser::ItemUpgrade(Packet& pkt, uint8_t nUpgradeType)
{
	Packet result(WIZ_ITEM_UPGRADE);
	result << nUpgradeType;	

	_ITEM_DATA* pOriginItem;
	_ITEM_TABLE* proto;
	int32_t nItemID[10]; int8_t bPos[10];
	uint16_t sNpcID;
	int8_t bType = UpgradeTypeNormal, bResult = UpgradeNoMatch, ItemClass = 0;
	bool Trina = false, Karivdis = false, Accessories = false;
	DateTime time;

	if (isDead() || isTrading() || isStoreOpen() || isMerchanting() || isSellingMerchant() || isBuyingMerchant() || isMining())
	{
		bResult = UpgradeTrading;
		goto fail_return;
	}

	pkt >> bType;
	result << bType;
	pkt >> sNpcID;
	for (int i = 0; i < 10; i++)
	{
		pkt >> nItemID[i] >> bPos[i];
		if (bPos[i] != -1 && bPos[i] >= HAVE_MAX)
			return;
	}

	pOriginItem = GetItem(SLOT_MAX + bPos[0]);
	if (pOriginItem->nNum != nItemID[0]
		|| (proto = g_pMain->GetItemPtr(nItemID[0])) == nullptr)
		goto fail_return;
	else if (pOriginItem->isRented()
		|| pOriginItem->isSealed())
	{
		bResult = UpgradeRental;
		goto fail_return;
	}

	for (int x = 0; x < 10; x++)
	{
		if (bPos[x] != -1
			&& (nItemID[x] > 0
				&& nItemID[x] != GetItem(SLOT_MAX + bPos[x])->nNum))
			goto fail_return;
	}

	{
		int nReqOriginItem = nItemID[0] % 1000;
		_ITEM_UPGRADE* pUpgrade = nullptr;
		foreach_stlmap(itr, g_pMain->m_ItemUpgradeArray)
		{
			pUpgrade = itr->second;
			if (pUpgrade->sOriginItem != nReqOriginItem)
				continue;

			if ((nItemID[0] / MIN_ITEM_ID) != pUpgrade->nIndex / 100000
				&& pUpgrade->nIndex < 300000)
				continue;

			if (nItemID[1] == 700002000 || nItemID[2] == 700002000 || nItemID[1] == 379258000 || nItemID[2] == 379258000)
			{
				Trina = true;
				Karivdis = true;
			}
			if (pUpgrade->bRateType == LowClass)
			{
				if (
					nItemID[1] == 379221000 ||
					nItemID[1] == 379222000 ||
					nItemID[1] == 379223000 ||
					nItemID[1] == 379224000 ||
					nItemID[1] == 379225000 ||
					nItemID[1] == 379226000 ||
					nItemID[1] == 379227000 ||
					nItemID[1] == 379228000 ||
					nItemID[1] == 379229000 ||
					nItemID[1] == 379230000 ||
					nItemID[1] == 379231000 ||
					nItemID[1] == 379232000 ||
					nItemID[1] == 379233000 ||
					nItemID[1] == 379234000 ||
					nItemID[1] == 379235000 ||
					nItemID[1] == 379255000 ||
					nItemID[2] == 379221000 ||
					nItemID[2] == 379222000 ||
					nItemID[2] == 379223000 ||
					nItemID[2] == 379224000 ||
					nItemID[2] == 379225000 ||
					nItemID[2] == 379226000 ||
					nItemID[2] == 379227000 ||
					nItemID[2] == 379228000 ||
					nItemID[2] == 379229000 ||
					nItemID[2] == 379230000 ||
					nItemID[2] == 379231000 ||
					nItemID[2] == 379232000 ||
					nItemID[2] == 379233000 ||
					nItemID[2] == 379234000 ||
					nItemID[2] == 379235000 ||
					nItemID[2] == 379255000)
					ItemClass = LowClass;
				else
					continue;
			}

			if (pUpgrade->bRateType == MiddleClass)
			{
				if (
					nItemID[1] == 379205000 ||
					nItemID[1] == 379206000 ||
					nItemID[1] == 379208000 ||
					nItemID[1] == 379209000 ||
					nItemID[1] == 379210000 ||
					nItemID[1] == 379211000 ||
					nItemID[1] == 379212000 ||
					nItemID[1] == 379213000 ||
					nItemID[1] == 379214000 ||
					nItemID[1] == 379215000 ||
					nItemID[1] == 379216000 ||
					nItemID[1] == 379217000 ||
					nItemID[1] == 379218000 ||
					nItemID[1] == 379219000 ||
					nItemID[1] == 379220000 ||
					nItemID[2] == 379205000 ||
					nItemID[2] == 379206000 ||
					nItemID[2] == 379208000 ||
					nItemID[2] == 379209000 ||
					nItemID[2] == 379210000 ||
					nItemID[2] == 379211000 ||
					nItemID[2] == 379212000 ||
					nItemID[2] == 379213000 ||
					nItemID[2] == 379214000 ||
					nItemID[2] == 379215000 ||
					nItemID[2] == 379216000 ||
					nItemID[2] == 379217000 ||
					nItemID[2] == 379218000 ||
					nItemID[2] == 379219000 ||
					nItemID[2] == 379220000
					)
					ItemClass = MiddleClass;
				else
					continue;
			}

			if (pUpgrade->bRateType == HighClass)
			{
				if (
					nItemID[1] == 379021000 ||
					nItemID[1] == 379022000 ||
					nItemID[1] == 379023000 ||
					nItemID[1] == 379024000 ||
					nItemID[1] == 379025000 ||
					nItemID[1] == 379030000 ||
					nItemID[1] == 379031000 ||
					nItemID[1] == 379032000 ||
					nItemID[1] == 379033000 ||
					nItemID[1] == 379034000 ||
					nItemID[1] == 379035000 ||
					nItemID[1] == 379138000 ||
					nItemID[1] == 379139000 ||
					nItemID[1] == 379140000 ||
					nItemID[1] == 379141000 ||
					nItemID[1] == 379016000 ||
					nItemID[2] == 379021000 ||
					nItemID[2] == 379022000 ||
					nItemID[2] == 379023000 ||
					nItemID[2] == 379024000 ||
					nItemID[2] == 379025000 ||
					nItemID[2] == 379030000 ||
					nItemID[2] == 379031000 ||
					nItemID[2] == 379032000 ||
					nItemID[2] == 379033000 ||
					nItemID[2] == 379034000 ||
					nItemID[2] == 379035000 ||
					nItemID[2] == 379138000 ||
					nItemID[2] == 379139000 ||
					nItemID[2] == 379140000 ||
					nItemID[2] == 379141000 ||
					nItemID[1] == 379016000
					)
					ItemClass = HighClass;
				else
					continue;
			}

			if (pUpgrade->bRateType == 4)
			{
				if (
					nItemID[1] == 379256000 ||
					nItemID[1] == 379257000 ||
					nItemID[2] == 379256000 ||
					nItemID[2] == 379257000)
					ItemClass = 4;
				else
					continue;
			}

			if (nItemID[1] == 700002000 && nItemID[2] != 379021000 || nItemID[1] == 379258000 && nItemID[2] != 379257000)
				goto fail_return;

			//if (proto->m_bClass == 8)
			//{
			//	Accessories = true;
			//	if (pUpgrade->bRateType != 8)//|| (pUpgrade->bOriginType != -1 && pUpgrade->bOriginType != 23 && proto-> != pUpgrade->bOriginType))
			//		continue;
			//}

			/*if (proto->m_bClass != 4 && (nItemID[1] == 379257000 || nItemID[2] == 379257000))
				goto fail_return;*/

			if (bPos[0] == bPos[1] || bPos[1] == bPos[2] || bPos[0] == bPos[2])
				continue;

			/*if (!(proto->m_bClass <= ItemClass) && !Accessories)
				continue;*/

			/*if (((nItemID[1] == 700002000 || nItemID[2] == 700002000) && (ItemClass != 3 && pUpgrade->bRateType != 3))
				|| (nItemID[1] == 379258000 || nItemID[2] == 379258000) && (ItemClass != 4 && pUpgrade->bRateType != 4))
				continue;*/

			if (pUpgrade->bOriginType != -1
				&& pUpgrade->nIndex < 200000 && pUpgrade->nIndex >= 100000)
			{
				switch (pUpgrade->bOriginType)
				{
				case 0:
					if (!proto->isDagger())
						continue;
					break;

				case 1:
					if (proto->m_bKind != 21)
						continue;
					break;

				case 2:
					if (proto->m_bKind != 22)
						continue;
					break;

				case 3:
					if (proto->m_bKind != 31)
						continue;
					break;

				case 4:
					if (proto->m_bKind != 32)
						continue;
					break;

				case 5:
					if (proto->m_bKind != 41)
						continue;
					break;

				case 6:
					if (proto->m_bKind != 42)
						continue;
					break;

				case 7:
					if (proto->m_bKind != 51)
						continue;
					break;

				case 8:
					if (proto->m_bKind != 52)
						continue;
					break;

				case 9:
					if (proto->m_bKind != 70 && proto->m_bKind != 71)
						continue;
					break;

				case 10:
					if (proto->m_bKind != 110)
						continue;
					break;

				case 11:
					if ((nItemID[0] / 10000000) != 19)
						continue;
					break;

				case 12:
					if (proto->m_bKind != 60)
						continue;
					break;

				case 13:
					if (proto->m_bKind != 210 && proto->m_bKind != 220 && proto->m_bKind != 230 && proto->m_bKind != 240)
						continue;
					break;

				case 14:
					if (proto->m_bKind != 11)
						continue;
					break;

					/*	case 15:
							if (!proto->isJamadar())
								continue;*/
				}
			}

			if ((nItemID[0] / MIN_ITEM_ID) != (pUpgrade->nIndex / 100000)
				&& ((pUpgrade->nIndex / 100000) == 1
					|| (pUpgrade->nIndex / 100000) == 2))
				continue;


			bool isValidMatch = true;
			for (int x = 1; x < MAX_ITEMS_REQ_FOR_UPGRADE; x++)
			{
				if (bPos[x] == -1
					|| pUpgrade->nReqItem[x - 1] == 0)
					continue;

				_ITEM_DATA* pItem = GetItem(SLOT_MAX + bPos[x]);

				if (pItem == nullptr
					|| nItemID[x] != pItem->nNum
					|| (nUpgradeType != ITEM_ACCESSORIES
						&& nItemID[x] != pUpgrade->nReqItem[x - 1]))
				{
					if (!Trina || !Karivdis)
						isValidMatch = false;
					break;
				}
			}

			if (!isValidMatch)
				continue;

			if (!hasCoins(pUpgrade->nReqNoah))
			{
				bResult = UpgradeNeedCoins;
				goto fail_return;
			}

			bResult = UpgradeSucceeded;
			break;
		}

		if (bResult != UpgradeSucceeded
			|| pUpgrade == nullptr)
			goto fail_return;

		int rand = myrand(0, myrand(9000, 10000)), GenRate = pUpgrade->sGenRate;
		SendUpgradeRate(GenRate, rand);
		/*if(Trina || Karivdis)
		{
			if (Trina)
			{
				GenRate = (pUpgrade->sGenRate + (pUpgrade->sGenRate * g_pMain->m_upgrade1) / 100);
				if(GenRate>10000)
					GenRate=10000;
			}
			if (Karivdis)
			{
				GenRate = (pUpgrade->sGenRate + (pUpgrade->sGenRate * g_pMain->m_upgrade2) / 100);
				if(GenRate>10000)
					GenRate=10000;
			}
		}
		else
			GenRate = pUpgrade->sGenRate;*/

		if (bType == UpgradeTypeNormal
			&& GenRate < rand)
		{
			bResult = UpgradeFailed;
			memset(pOriginItem, 0, sizeof(_ITEM_DATA));

			ItemUpgradeNotice(proto, UpgradeFailed);
			GoldLose(pUpgrade->nReqNoah, true);
		}
		else
		{

			if (isDead() || isTrading() || isStoreOpen() || isMerchanting() || isSellingMerchant() || isBuyingMerchant() || isMining())
			{
				bResult = UpgradeTrading;
				goto fail_return;
			}
			int nNewItemID = pOriginItem->nNum + pUpgrade->nGiveItem;

			_ITEM_TABLE* newProto = g_pMain->GetItemPtr(nNewItemID);
			if (newProto == nullptr)
			{
				bResult = UpgradeNoMatch;
				goto fail_return;
			}

			if (bType != UpgradeTypePreview)
			{
				pOriginItem->nNum = nNewItemID;
				pOriginItem->sDuration = newProto->m_sDuration;

				ItemUpgradeNotice(newProto, UpgradeSucceeded);

				GoldLose(pUpgrade->nReqNoah, true);
			}

			nItemID[0] = nNewItemID;
		}

		if (bType != UpgradeTypePreview) {
			int nMaxItemRemove = 0;
			if (nUpgradeType != ITEM_ACCESSORIES)
				nMaxItemRemove = MAX_ITEMS_REQ_FOR_UPGRADE;
			else
				nMaxItemRemove = 5;


			for (int i = 1; i < nMaxItemRemove; i++)
			{
				if (bPos[i] == -1
					|| bPos[i] >= HAVE_MAX)
					continue;

				_ITEM_DATA* pItem = GetItem(SLOT_MAX + bPos[i]);
				if (pItem->nNum == 0
					|| pItem->sCount == 0)
					continue;


				pItem->sCount--;
				if (pItem->sCount == 0 && pItem->nNum == nItemID[i])
					memset(pItem, 0, sizeof(pItem));
			}
		}
	}

	result << bResult;
	foreach_array(i, nItemID)
		result << nItemID[i] << bPos[i];
	Send(&result);

	result.Initialize(WIZ_OBJECT_EVENT);
	result << uint8_t(OBJECT_ANVIL) << bResult << sNpcID;
	SendToRegion(&result);

	return;
fail_return:
	result << bResult;

	foreach_array(i, nItemID)
		result << nItemID[i] << bPos[i];

	Send(&result);
}

void CUser::SendUpgradeRate(uint16_t rate, int randomed)
{
	Packet result(WIZ_ITEM_UPGRADE);
	result << (uint8_t)ITEM_UPGRADE_RATE;
	result << rate;
	result << randomed;
	Send(&result);
}

/**
* @brief	Upgrade notice.
*
* @param	pItem	The item.
*/
void CUser::ItemUpgradeNotice(_ITEM_TABLE* pItem, uint8_t UpgradeResult)
{
	bool bSendUpgradeNotice = false;
	std::string sUpgradeNotice;

	// Notice is only rebirth upgrade a Offical stuff.
	if (pItem->m_ItemType == 11 || pItem->m_ItemType == 12 || true /* always send notice */)
		bSendUpgradeNotice = true;

	if (bSendUpgradeNotice)
	{
		if (UpgradeResult == 0)
			sUpgradeNotice = string_format("%s has failed to upgrade %s.", GetName().c_str(), pItem->m_sName.c_str());
		else if (UpgradeResult == 1)
			sUpgradeNotice = string_format("%s has succeeded to upgrade %s.", GetName().c_str(), pItem->m_sName.c_str());

		//g_pMain->SendAnnouncement(sUpgradeNotice.c_str());
		g_pMain->SendNotice(sUpgradeNotice.c_str());
	}
}

/**
* @brief	Packet handler for the accessory upgrade system.
*
* @param	pkt	The packet.
*/
void CUser::ItemUpgradeAccessories(Packet& pkt)
{
	ItemUpgrade(pkt, ITEM_ACCESSORIES);
}

/**
* @brief	Packet handler for the Chaotic Generator system
* 			which is used to exchange Bifrost pieces/fragments.
*
* @param	pkt	The packet.
*/
void CUser::BifrostPieceProcess(Packet& pkt)
{
	enum ResultOpCodes
	{
		Failed = 0,
		Success = 1
	};

	enum ResultMessages
	{
		EffectNone = 0, // No effect
		EffectRed = 1, // There will be better days.
		EffectGreen = 2, // Don't be too disappointed. You're luck isn't that bad.
		EffectWhite = 3 // It must be your lucky day.
	};

	uint16_t nObjectID = 0;
	uint32_t nExchangeItemID = 0;

	pkt >> nObjectID >> nExchangeItemID;

	std::vector<uint32_t> ExchangeIndexList;
	ResultOpCodes resultOpCode = Success;
	ResultMessages resultMessage = EffectNone;
	uint32_t nItemID = 0;
	uint8_t sItemSlot = 0;
	uint8_t sExchangeItemSlot = 0;

	sExchangeItemSlot = FindSlotForItem(nExchangeItemID, 1) - SLOT_MAX;

	if (g_pMain->m_ItemExchangeArray.GetSize() > 0)
	{
		foreach_stlmap(itr, g_pMain->m_ItemExchangeArray)
		{
			if (itr->second->nOriginItemNum[0] == nExchangeItemID)
			{
				if (std::find(ExchangeIndexList.begin(), ExchangeIndexList.end(), itr->second->nIndex) == ExchangeIndexList.end())
					ExchangeIndexList.push_back(itr->second->nIndex);
			}
			else
				continue;
		}
	}

	if (ExchangeIndexList.size() > 0)
	{
		uint32_t randIndex = myrand(0, (ExchangeIndexList.size() - 1));
		uint32_t nExchangeID = ExchangeIndexList[randIndex];

		_ITEM_EXCHANGE* pExchange = g_pMain->m_ItemExchangeArray.GetData(nExchangeID);

		if (pExchange == nullptr
			|| !CheckExchange(nExchangeID)
			|| pExchange->bRandomFlag > 101
			|| !CheckExistItemAnd(pExchange->nOriginItemNum[0], pExchange->sOriginItemCount[0], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
			resultOpCode = Failed;

		if (isTrading() || isMerchanting() || isMining())
			resultOpCode = Failed;

		if (pExchange->bRandomFlag == 101 && resultOpCode == Success)
		{
			uint32_t nTotalPercent = 0;
			for (int i = 0; i < ITEMS_IN_EXCHANGE_GROUP; i++)
				nTotalPercent += pExchange->sExchangeItemCount[i];

			if (nTotalPercent > 10000)
				resultOpCode = Failed;

			if (resultOpCode == Success)
			{
				uint8_t bRandArray[10000];
				memset(&bRandArray, 0, sizeof(bRandArray));
				uint16_t sExchangeCount[ITEMS_IN_EXCHANGE_GROUP];
				memcpy(&sExchangeCount, &pExchange->sExchangeItemCount, sizeof(pExchange->sExchangeItemCount));

				int offset = 0;
				for (int n = 0, i = 0; n < ITEMS_IN_EXCHANGE_GROUP; n++)
				{
					if (sExchangeCount[n] > 0)
					{
						memset(&bRandArray[offset], n, sExchangeCount[n]);
						offset += sExchangeCount[n];
					}
				}

				uint8_t bRandSlot = bRandArray[myrand(0, 9999)];
				nItemID = pExchange->nExchangeItemNum[bRandSlot];

				sItemSlot = GetEmptySlot() - SLOT_MAX;
				RobItem(pExchange->nOriginItemNum[0], 1);
				GiveItem(nItemID, 1);

				_ITEM_TABLE* pItem = g_pMain->m_ItemtableArray.GetData(nItemID);

				if (pItem != nullptr)
				{
					if (pItem->m_ItemType == 4)
						resultMessage = EffectWhite;
					else if (pItem->m_ItemType == 5 || pItem->m_ItemType == 11 || pItem->m_ItemType == 12)
						resultMessage = EffectGreen;
					else
						resultMessage = EffectRed;
				}
			}
		}
	}

	Packet result(WIZ_ITEM_UPGRADE);
	result << (uint8_t)ITEM_BIFROST_EXCHANGE << (uint8_t)resultOpCode << nItemID << sItemSlot << nExchangeItemID << sExchangeItemSlot << (uint8_t)resultMessage;
	Send(&result);

	result.Initialize(WIZ_OBJECT_EVENT);
	result << (uint8_t)OBJECT_ARTIFACT << (uint8_t)resultMessage << nObjectID;

	if (resultOpCode != Failed)
		SendToRegion(&result);
}

/**
* @brief	Packet handler for the Special exchange system
* 			which is used to exchange Krowaz meterials.
*
* @param	pkt	The packet.
*/
void CUser::SpecialItemExchange(Packet& pkt)
{
	enum ResultOpCodes
	{
		WrongMaterial = 0,
		Success = 1,
		Failed = 2
	};

	ResultOpCodes resultOpCode = WrongMaterial;

	uint16_t sNpcID;
	uint32_t nShadowPiece;
	uint8_t nShadowPieceSlot;
	uint8_t nMaterialCount;
	uint8_t nItemSlot[10];
	uint8_t nDownFlag;
	uint32_t nItemID[10];
	uint8_t nItemCount[10];

	uint32_t nItemNumber = 0;
	uint8_t sItemSlot = 0;

	pkt >> sNpcID >> nShadowPiece >> nShadowPieceSlot >> nMaterialCount;

	for (int i = 0; i < 10; i++)
	{
		nItemID[i] = 0;
		nItemCount[i] = 0;
	}

	for (int i = 0; i < nMaterialCount; i++)
		pkt >> nItemSlot[i];

	pkt >> nDownFlag;

	for (int i = 0; i < nMaterialCount; i++)
	{
		uint8_t nReadByte;
		int nDigit = 100000000;
		nItemID[i] = 0;
		for (int x = 0; x < 9; x++)
		{
			pkt >> nReadByte;
			nItemID[i] += (nReadByte - 48) * nDigit;
			nDigit = nDigit / 10;
		}

		uint8_t nCount[3] = { 0, 0, 0 };
		pkt >> nCount[0];
		pkt >> nCount[1];
		pkt >> nCount[2];
		int nCountFinish = 0;
		nCountFinish += (nCount[0] - 48) * 100;
		nCountFinish += (nCount[1] - 48) * 10;
		nCountFinish += (nCount[2] - 48) * 1;
		nItemCount[i] = nCountFinish;
	}

	std::vector<uint32_t> ExchangeIndexList;

	if (nMaterialCount > 3) // Minimum Required : 4 Material
	{
		if (g_pMain->m_ItemExchangeArray.GetSize() > 0)
		{
			foreach_stlmap(itr, g_pMain->m_ItemExchangeArray)
			{
				if (itr->second->bRandomFlag == 102) // Special Item Exchange
				{
					if (nShadowPiece == 0 && itr->second->nOriginItemNum[0] == SHADOW_PIECE) // If Need Shadow Piece Please Set is nOriginItem1 Column... 
						continue;
					else
					{
						uint8_t nOriginItemCount = 0;
						uint8_t nMatchCount = 0;
						bool bAddArray = false;

						if (nMaterialCount == 4)
							nMatchCount = (nShadowPiece == 0 ? 4 : 5);
						else if (nMaterialCount == 5)
							nMatchCount = (nShadowPiece == 0 ? 5 : 6);

						for (int i = 0; i < nMaterialCount; i++)
						{
							if (nItemID[i] != 0)
							{
								for (int x = 0; x < ITEMS_IN_ORIGIN_GROUP; x++)
								{
									if (itr->second->nOriginItemNum[x] != 0
										&& nItemID[i] == itr->second->nOriginItemNum[x])
									{
										nOriginItemCount++;
										break;
									}
								}
							}
						}

						if (nOriginItemCount == nMatchCount)
							bAddArray = true;
						else if (nOriginItemCount == nMatchCount)
							bAddArray = true;

						if (bAddArray && std::find(ExchangeIndexList.begin(), ExchangeIndexList.end(), itr->second->nIndex) == ExchangeIndexList.end())
							ExchangeIndexList.push_back(itr->second->nIndex);
					}
				}
				else
					continue;
			}
		}
	}

	if (ExchangeIndexList.size() > 0)
	{
		uint32_t randIndex = myrand(0, (ExchangeIndexList.size() - 1));
		uint32_t nExchangeID = ExchangeIndexList[randIndex];

		_ITEM_EXCHANGE* pExchange = g_pMain->m_ItemExchangeArray.GetData(nExchangeID);

		if (pExchange == nullptr
			|| !CheckExchange(nExchangeID)
			|| pExchange->bRandomFlag > 102
			|| !CheckExistItemAnd(pExchange->nOriginItemNum[0], pExchange->sOriginItemCount[0],
				pExchange->nOriginItemNum[1], pExchange->sOriginItemCount[1],
				pExchange->nOriginItemNum[2], pExchange->sOriginItemCount[2],
				pExchange->nOriginItemNum[3], pExchange->sOriginItemCount[3],
				pExchange->nOriginItemNum[4], pExchange->sOriginItemCount[4],
				pExchange->nOriginItemNum[5], pExchange->sOriginItemCount[5],
				pExchange->nOriginItemNum[6], pExchange->sOriginItemCount[6],
				pExchange->nOriginItemNum[7], pExchange->sOriginItemCount[7],
				pExchange->nOriginItemNum[8], pExchange->sOriginItemCount[8],
				pExchange->nOriginItemNum[9], pExchange->sOriginItemCount[9],
				pExchange->nOriginItemNum[10], pExchange->sOriginItemCount[10]))
		{
			resultOpCode = WrongMaterial;
		}
		else
		{
			bool bContinueExchange = true;

			for (int i = 0; i < nMaterialCount; i++)
			{
				if (!bContinueExchange)
					break;

				if (nItemID[i] != 0)
				{
					for (int x = 0; x < ITEMS_IN_ORIGIN_GROUP; x++)
					{
						if (pExchange->nOriginItemNum[x] != 0
							&& nItemID[i] == pExchange->nOriginItemNum[x]
							&& nItemCount[i] != pExchange->sOriginItemCount[x])
						{
							bContinueExchange = false;
							break;
						}
					}
				}
			}

			if (!bContinueExchange)
				resultOpCode = WrongMaterial;
			else
			{
				uint32_t nTotalPercent = 0;
				for (int i = 0; i < ITEMS_IN_EXCHANGE_GROUP; i++)
					nTotalPercent += pExchange->sExchangeItemCount[i];

				if (nTotalPercent > 10000)
					resultOpCode = WrongMaterial;
				else
				{
					uint8_t bRandArray[10000];
					memset(&bRandArray, 0, sizeof(bRandArray));
					uint16_t sExchangeCount[ITEMS_IN_EXCHANGE_GROUP];
					memcpy(&sExchangeCount, &pExchange->sExchangeItemCount, sizeof(pExchange->sExchangeItemCount));

					int offset = 0;
					for (int n = 0, i = 0; n < ITEMS_IN_EXCHANGE_GROUP; n++)
					{
						if (sExchangeCount[n] > 0)
						{
							memset(&bRandArray[offset], n, sExchangeCount[n]);
							offset += sExchangeCount[n];
						}
					}

					uint8_t bRandSlot = bRandArray[myrand(0, 9999)];
					nItemNumber = pExchange->nExchangeItemNum[bRandSlot];
					uint16_t nItemRate = pExchange->sExchangeItemCount[bRandSlot];
					int rand = myrand(0, myrand(9000, 10000));

					if (nItemRate <= rand)
						resultOpCode = Failed;
					else
					{
						sItemSlot = GetEmptySlot() - SLOT_MAX;
						GiveItem(nItemNumber, 1);
						resultOpCode = Success;
					}

					for (int i = 0; i < ITEMS_IN_ORIGIN_GROUP; i++)
					{
						if (pExchange->nOriginItemNum[i] != 0)
							RobItem(pExchange->nOriginItemNum[i], pExchange->sOriginItemCount[i]);
					}
				}
			}
		}
	}

	Packet result(WIZ_ITEM_UPGRADE);
	result << (uint8_t)ITEM_SPECIAL_EXCHANGE << (uint8_t)resultOpCode << sNpcID;

	if (resultOpCode == Success)
		result << nItemNumber << sItemSlot;

	Send(&result);

	if (resultOpCode == Success)
		ShowNpcEffect(31033, true);
	else if (resultOpCode == Failed)
		ShowNpcEffect(31034, true);
}


/**
* @brief	Packet handler for the upgrading of 'rebirthed' items.
*
* @param	pkt	The packet.
*/
void CUser::ItemUpgradeRebirth(Packet& pkt)
{
	ItemUpgrade(pkt, ITEM_UPGRADE_REBIRTH);
}
