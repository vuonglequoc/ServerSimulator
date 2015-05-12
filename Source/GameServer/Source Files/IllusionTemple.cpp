//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: IllusionTemple Event									//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/IllusionTemple.h"
#include "../Header Files/User.h"
#include "../Header Files/GameMain.h"
#include "../Header Files/ChaosCastle.h"
#include "../Header Files/BloodCastle.h"
#include "../Header Files/prodef.h"
#include "../Header Files/Gate.h"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/ObjUseSkill.h"
#include "../Header Files/SkillHitBox.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/Tokenizer.h"
#include "../Header Files/ReadScript.h"
#include "../Header Files/Functions.H"
#include "../Header Files/Party.h"
#include "../Header Files/MoveSystem.h"
#include "../Header Files/SendProto.H"


#if(!GS_CASTLE)
CIllusionTemple	g_IllusionTemple;
// -----------------------------------------------------------------------------------------------------------------------
#pragma pack(1)

struct PMSG_ILLUSION_NPC
{
	PBMSG_HEAD	h;
	BYTE		subtype;
	BYTE		State;
	BYTE		MemberCount;
	PMSG_ILLUSION_NPC()
	{
		h.c = 0xC3;
		h.headcode = 0x30;
		h.size = sizeof(PMSG_ILLUSION_NPC);
		subtype = 0x14;
		State = 0;
		MemberCount = 0;
	}
};
// -----------------------------------------------------------------------------------------------------------------------
#pragma pack()
// -----------------------------------------------------------------------------------------------------------------------
#define IT_SKILL_SHIELD		0xD2
#define IT_SKILL_RESIST		0xD3
#define IT_SKILL_TRACK		0xD4
#define IT_SKILL_FREEZE		0xD5
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::UseSkillProc(OBJECTSTRUCT* lpObj, PMSG_ILLUSION_USESKILL* pMsg)
{
	LPOBJ lpTargetObj = &gObj[pMsg->m_TargetIndex];

	int TargePosx = lpTargetObj->X;
	int TargePosy = lpTargetObj->Y;

	int iangle = gObjUseSkill.GetAngle(lpObj->X,lpObj->Y,TargePosx,TargePosy);

	if(!(IT_FLOOR_RANGE(lpObj->m_IllusionIndex) && IT_MAP_RANGE(lpObj->MapNumber)))
	{
		GCMagicAttackNumberSend(lpObj, (BYTE)pMsg->m_SkillIndex, lpObj->m_Index, 1);
		return;
	}

	int SkillPoint = MagicDamageC.CheckKillCount(pMsg->m_SkillIndex, lpObj->m_IllusionKillCount);

	if(SkillPoint < 0)
	{
		GCMagicAttackNumberSend(lpObj, (BYTE)pMsg->m_SkillIndex, lpObj->m_Index, 1);
		CLog.LogAddC(2, "[IllusionTemple][%s] Use skill without enough points (%d)(%d)", lpObj->Name, lpObj->m_IllusionKillCount, pMsg->m_SkillIndex);
		return;
	}

	switch(pMsg->m_SkillIndex)
	{
		case IT_SKILL_SHIELD:
		{
			if(lpObj->m_IllusionShieldSkillTime > 0)
			{
				GCMagicAttackNumberSend(lpObj, AT_SKILL_ITSHIELD, lpObj->m_Index, 0);
				return;
			}

			if(lpObj->Mana < MagicDamageC.SkillGetMana(IT_SKILL_SHIELD))
			{
				GCMagicAttackNumberSend(lpObj, AT_SKILL_ITSHIELD, lpObj->m_Index, 0);
				return;
			}

			lpObj->Mana -= MagicDamageC.SkillGetMana(IT_SKILL_SHIELD);
			GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
			lpObj->m_IllusionShieldSkillTime = MagicDamageC.GetDelayTime(IT_SKILL_SHIELD);
			GCStateInfoSend(lpObj, 1, eVS_ILLUSION_PROTECTION);
			gObjSetIllusionKillCount(lpObj->m_Index, KILLCOUNT_DECREASE, MagicDamageC.GetKillCount(IT_SKILL_SHIELD));
			GCMagicAttackNumberSend(lpObj, AT_SKILL_ITSHIELD, lpObj->m_Index, 1);
		}break;

		case IT_SKILL_RESIST:
		{
			if(lpObj->Type != OBJ_USER )
			{
				CLog.LogAddC(2, "[IllusionTemple][%s] can't use Skill (%d) in normal state,only in PVP", lpObj->Name, pMsg->m_SkillIndex );
				return;
			}

			if(gObjCalDistance(lpObj,lpTargetObj) <= 4)
			{
				if(SkillElectricSparkHitBox.HitCheck(iangle,lpObj->X,lpObj->Y,lpObj->X,lpObj->Y))
				{
					lpTargetObj->m_btSkillRestraintOrder = TRUE;
					lpTargetObj->m_btSkillRestraintOrderTime = 15;
					//this->GCEffectSkillSend(aIndex,lpTargetObj->m_Index,lpMagic->m_Skill,btType);
					gObjSetPosition(lpObj->m_Index,lpObj->X,lpObj->Y);
				}
			}
		}break;

		case IT_SKILL_TRACK:
		{
			if(lpObj->m_Index != this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner)
			{
				if(OBJMAX_RANGE(this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner))
				{
					gObjSetIllusionKillCount(lpObj->m_Index, KILLCOUNT_DECREASE, MagicDamageC.GetKillCount(IT_SKILL_TRACK));
					gObjTeleport(lpObj->m_Index, gObj[this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner].MapNumber, 
						gObj[this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner].X, 
						gObj[this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner].Y); 
				}
			}
		}break;

		case IT_SKILL_FREEZE:
		{
			if(lpObj->Type != OBJ_USER )
			{
				CLog.LogAddC(2, "[IllusionTemple][%s] can't use Skill (%d) in normal state,only in PVP", lpObj->Name, pMsg->m_SkillIndex );
				return;
			}

			lpObj->iShield -= (lpObj->iMaxShield / 2);

			if( lpObj->iShield < 0 )
			{
				lpObj->iShield = 0;
			}

			GCReFillSend(lpObj->m_Index,(WORD)lpObj->Life,0xFF,0,lpObj->iShield);
			GCSendEffectInfo(lpTargetObj->m_Index, 17);
		}break;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::ProcessMonsterKiller(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpTarget)
{
	if(IT_FLOOR_RANGE(lpObj->m_IllusionIndex) == FALSE) return;
	if(lpObj->MapNumber != lpTarget->MapNumber) return;
	if(lpTarget->Type != OBJ_MONSTER) return;
	//if(lpObj->m_IllusionTeam != lpTarget->m_IllusionTeam)
	//{
		gObjSetIllusionKillCount(lpObj->m_Index, KILLCOUNT_INCREASE, this->m_iMonsterKillPoint);
	//}
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::ProcessUserKiller(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpTarget)
{
	if(IT_FLOOR_RANGE(lpObj->m_IllusionIndex) == FALSE) 
		return;

	if(lpObj->MapNumber != lpTarget->MapNumber) 
		return;

	if(lpObj->m_IllusionTeam != lpTarget->m_IllusionTeam)
	{
		gObjSetIllusionKillCount(lpTarget->m_Index, KILLCOUNT_RESET, 0);
		if(lpTarget->m_Index == this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner)
		{
			bool bFound = false;

			for(int i = 0; i < INVENTORY_SIZE; i++)
			{
				if(lpTarget->pInventory[i].IsItem())
				{
					if(lpTarget->pInventory[i].m_Type == ITEMGET(14, 64))
					{
						gObjInventoryDeleteItem(lpTarget->m_Index, i);
						GCInventoryItemDeleteSend(lpTarget->m_Index, i, 1);
						bFound = true;
					}
				}
			}

			if(!bFound)
			{
				CLog.LogAddC(2, "[IllusionTemple] Player killed, but ball not found in inventory! [BUG!?] (killer: %s, target: %s)", lpObj->Name, lpTarget->Name);
			}
			
			// Ball back to the Statue
			this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner = this->m_IllusionTemple[lpObj->m_IllusionIndex].m_Statues[rand()%MAX_IT_STATUES];

			gObjSetIllusionKillCount(lpObj->m_Index, KILLCOUNT_INCREASE, this->m_iBallKillPoint);
		}
		else
		{
			gObjSetIllusionKillCount(lpObj->m_Index, KILLCOUNT_INCREASE, this->m_iPlayerKillPoint);
		}
	}
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::ProcessDieUser(OBJECTSTRUCT* lpObj)
{
	if(IT_MAP_RANGE ( lpObj->MapNumber ) == TRUE && IT_FLOOR_RANGE ( lpObj->m_IllusionIndex ) == TRUE && 
	  (lpObj->m_IllusionTeam == IT_TEAM_RED || lpObj->m_IllusionTeam == IT_TEAM_BLUE))
	{
		unsigned char x = lpObj->X;
		unsigned char y = lpObj->Y;
		unsigned char map = lpObj->MapNumber;
		unsigned char dir = lpObj->Dir;
		short level = 0;

		if(lpObj->m_IllusionTeam == IT_TEAM_RED)
		{
			GateC.GetGate(154+lpObj->m_IllusionIndex, x, y, map, dir, level);
		}

		else if(lpObj->m_IllusionTeam == IT_TEAM_BLUE)
		{
			GateC.GetGate(148+lpObj->m_IllusionIndex, x, y, map, dir, level);
		}

		lpObj->MapNumber = map;
		lpObj->X = x;
		lpObj->Y = y;
		lpObj->Dir = dir;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
bool CIllusionTemple::NpcTalk(OBJECTSTRUCT* lpNpc, OBJECTSTRUCT* lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return true;
	}

	switch(lpNpc->Class)
	{
		case 385:
		{
			int templeIndex = this->GetObjTempleIndex(lpObj);
				
			if(templeIndex != -1)
			{
				if(templeIndex >= 0 && templeIndex < MAX_ILLUSION_TEMPLES)
				{
					if(!(this->m_IllusionTemple[templeIndex].m_TempleState == IT_STATE_OPEN))
					{
						char szBuff[100] = {0};
						
						if(this->m_IllusionTemple[templeIndex].m_TempleState == IT_STATE_CLOSED) 
						{
							sprintf(&szBuff[0], "IT Event will be Opened in %d Minute(s).", (this->m_IllusionTemple[0].m_RemainTime / 60000) - 4);
						} 
						else 
						{	
							sprintf(&szBuff[0], "IT Event is Running Now, Try again Later!", (this->m_IllusionTemple[0].m_RemainTime / 60000) - 4);
						}
						
						ChatTargetSend(lpNpc, &szBuff[0], lpObj->m_Index);
					}
					else
					{
						PMSG_ILLUSION_NPC pMsg;
						pMsg.MemberCount = this->GetUserCount(templeIndex);
						pMsg.State = 0;
						gSendProto.DataSend(lpObj->m_Index, (unsigned char*)&pMsg, pMsg.h.size);
						ChatTargetSend(lpNpc, "Illusion Temple Event Awaits You!", lpObj->m_Index);
					}
				}
				else
				{
					ChatTargetSend(lpNpc, "Sorry, You are not allowed to enter the IT Event.", lpObj->m_Index);
					CLog.LogAddC(2, "[IllusionTemple] Gate Index Error (%d)(%s).", templeIndex, &lpObj->Name[0]);
				}
			}
			else
			{
				ChatTargetSend(lpNpc, "You don't have sufficient Level to Enter IT Event.", lpObj->m_Index);
			}
		}
		return TRUE;

		case 380:
		{
			if(IT_FLOOR_RANGE(lpObj->m_IllusionIndex) && IT_MAP_RANGE(lpObj->MapNumber))
			{
				if(this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner == lpNpc->m_Index)
				{
					BYTE ItemPosition = gObjInventoryInsertItem(lpObj, 14, 64, 0);
					
					if(ItemPosition != -1)
					{
						GCInventoryItemOneSend(lpObj->m_Index, ItemPosition);
						this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner = lpObj->m_Index;
					}
					else
					{
						GCServerMsgStringSend("You do not have space to capture the artifact!", lpObj->m_Index, 1);
					} // Need To be Translated By Testers "Você não tem espaço para capturar o artefato!"
				}
				else
				{
					GCServerMsgStringSend("The artifact is not in the statue!", lpObj->m_Index, 1);
				} // Need To be Translated By Testers "O artefato não se encontra nesta estátua!"
			}
		}
		return TRUE;

		case 383: // Blue
		
		case 384:
		{
			int iTeam = lpNpc->Class == 383 ? IT_TEAM_BLUE : IT_TEAM_RED;
				
			if(IT_FLOOR_RANGE(lpObj->m_IllusionIndex) && IT_MAP_RANGE(lpObj->MapNumber))
			{
				bool bFound = false;
					
				for(int i = 0; i < INVENTORY_SIZE; i++)
				{
					if(lpObj->pInventory[i].IsItem())
					{
						if(lpObj->pInventory[i].m_Type == ITEMGET(14, 64))
						{
							gObjInventoryDeleteItem(lpObj->m_Index, i);
							GCInventoryItemDeleteSend(lpObj->m_Index, i, 1);
							bFound = true;
						}
					}
				}
				if(bFound)
				{
					this->m_IllusionTemple[lpObj->m_IllusionIndex].m_BallOwner = this->m_IllusionTemple[lpObj->m_IllusionIndex].m_Statues[rand()%MAX_IT_STATUES];
					this->m_IllusionTemple[lpObj->m_IllusionIndex].m_Teams[iTeam].m_Score++;
				}
			}
		}
		return TRUE;

		default:
			
		return TRUE;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::NpcRegister(OBJECTSTRUCT* lpObj, PMSG_IT_REGISTER* pMsg)
{
	if(lpObj->m_IfState.use > 0)
	{
		CLog.LogAdd("[IllusionTemple] Interface State Error (%d)(%d)(%s)", lpObj->m_IfState.use, lpObj->m_IfState.type, lpObj->Name); 
		return;
	}

	if(lpObj->pInventory[pMsg->TicketPosition].m_Type != ITEMGET(13, 51) || 
		lpObj->pInventory[pMsg->TicketPosition].m_Level != pMsg->TargetTemple )
	{
		GCServerMsgStringSend("You missed Illusion Temple Ticket.", lpObj->m_Index, 1);
		return;
	}

	pMsg->TargetTemple--;

	if(IT_FLOOR_RANGE(pMsg->TargetTemple) == FALSE)
	{
		CLog.LogAdd("[IllusionTemple] Index out of range (%d)(%s)", pMsg->TargetTemple, &lpObj->Name[0]);
		return;
	}

	if(this->m_IllusionTemple[pMsg->TargetTemple].m_iUserCount >= 10)
	{
		GCServerMsgStringSend("[IllusionTemple] Event is already Full!", lpObj->m_Index, 1);
		return;
	}

	ILLUSION_OBJ obj;
	obj.m_Index = lpObj->m_Index;
	obj.m_TeamIndex = -1;
	obj.m_BonusCount = 0;
	obj.m_TempleIndex = pMsg->TargetTemple;

	gObjInventoryDeleteItem(lpObj->m_Index, pMsg->TicketPosition);
	GCInventoryItemDeleteSend(lpObj->m_Index, pMsg->TicketPosition, 1);

	if ( lpObj->PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		int iPartyIndex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
		if ( iPartyIndex >= 0 )
		{
			pMsg.Number = iPartyIndex;
			CGPartyDelUser(&pMsg, lpObj->m_Index);
		}
	}

	lpObj->m_IllusionIndex = pMsg->TargetTemple;
	gObjSetIllusionKillCount(lpObj->m_Index, KILLCOUNT_RESET, 0);

	gObjMoveGate(lpObj->m_Index, 142 + pMsg->TargetTemple);
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::ProtocolCore(unsigned char* lpBuffer, int iUserIndex)
{
	PMSG_DEFAULT2* pMsg = (PMSG_DEFAULT2*)lpBuffer;

	switch(pMsg->subcode)
	{
	case IT_PROTOCOL_REGISTER:
		this->NpcRegister((OBJECTSTRUCT*)&gObj[iUserIndex], (PMSG_IT_REGISTER*)lpBuffer);
		break;
	case IT_PROTOCOL_USESKILL:
		{
			PMSG_ILLUSION_USESKILL pMsg2;
			memcpy(&pMsg2, pMsg, pMsg->h.size);
			pMsg2.m_SkillIndex = (lpBuffer[4] * 256) + lpBuffer[5];
			pMsg2.m_TargetIndex = (lpBuffer[6] * 256) + lpBuffer[7];
			this->UseSkillProc(&gObj[iUserIndex], &pMsg2);
		}
		break;
	case IT_PROTOCOL_ADDREWARD:
		//LogAddStrHexFunc("ADD REWARD!", (char*)lpBuffer, pMsg->h.size); *Disabled By WolF-Untill Add "LogAddStrHexFunc"
		break;
	default:
		CLog.LogAddC(2, "[IllusionTemple] Invalid Protocol (%02X %02X) (%s)", pMsg->h.headcode, pMsg->subcode, gObj[iUserIndex].Name);
		break;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SetTempleState(int iTempleIndex, int iTempleState)
{
	this->m_IllusionTemple[iTempleIndex].m_TempleState = iTempleState;

	switch(iTempleState)
	{
		case IT_STATE_NONE:
				CLog.LogAdd("[IllusionTemple] (%d) SetTempleState NONE", iTempleIndex+1);
			break;

		case IT_STATE_CLOSED:
			if(this->SetProc_Closed(iTempleIndex))
			{
				CLog.LogAdd("[IllusionTemple] (%d) SetTempleState CLOSED", iTempleIndex+1);
			}
			break;

		case IT_STATE_OPEN:
			if(this->SetProc_Open(iTempleIndex))
			{
				CLog.LogAdd("[IllusionTemple] (%d) SetTempleState OPEN", iTempleIndex+1);
			}
			break;

		case IT_STATE_PREPARE:
			if(this->SetProc_Prepare(iTempleIndex))
			{
				CLog.LogAdd("[IllusionTemple] (%d) SetTempleState PREPARE", iTempleIndex+1);
			}
			break;

		case IT_STATE_BATTLE:
			if(this->SetProc_Battle(iTempleIndex))
			{
				CLog.LogAdd("[IllusionTemple] (%d) SetTempleState BATTLE", iTempleIndex+1);
			}
			break;

		case IT_STATE_END:
			if(this->SetProc_End(iTempleIndex))
			{
				CLog.LogAdd("[IllusionTemple] (%d) SetTempleState END", iTempleIndex+1);
			}
			break;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SetState(int iTempleState)
{
	for(int i = 0; i < MAX_ILLUSION_TEMPLES; i++)
	{
		this->SetTempleState(i, iTempleState);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
int CIllusionTemple::GetObjTempleIndex(OBJECTSTRUCT* lpObj)
{
	if( gUser.gObjIsMasteringLevel(lpObj) )
	{
		//if(lpObj->Level >= this->m_IllusionRules[MAX_ILLUSION_TEMPLES-1].m_MinLevel && 
		//	lpObj->Level <= this->m_IllusionRules[MAX_ILLUSION_TEMPLES-1].m_MaxLevel)
		//{
			return 5;
		//}
	}

	for(int i = 0; i < MAX_ILLUSION_TEMPLES-1; i++)
	{
		if(lpObj->Level >= this->m_IllusionRules[i].m_MinLevel &&
			lpObj->Level <= this->m_IllusionRules[i].m_MaxLevel)
		{
			return i;
		}
	}
	return -1;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SendMsgWaitRoom(int iTempleIndex, int iMessageType, char* Message, ...)
{
	va_list pArguments;
	va_start ( pArguments, Message );
	
	char szBuff[300] = {0};
	vsprintf(&szBuff[0], Message, pArguments);

	va_end ( pArguments );

	//PMSG_NOTICE pMsg;
	//TNotice::MakeNoticeMsgEx(&pMsg, iMessageType, &szBuff[0]);

	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{
		if(gObj[i].MapNumber == MAP_INDEX_ILLUSION1 + iTempleIndex)
		{
			//gSendProto.DataSend(i, (BYTE*)&pMsg, pMsg.h.size);
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SendMsgTemple(int iTempleIndex, int iMessageType, char* Message, ...)
{
	va_list pArguments;
	va_start ( pArguments, Message );
	
	char szBuff[300] = {0};
	vsprintf(&szBuff[0], Message, pArguments);

	va_end ( pArguments );

	//PMSG_NOTICE pMsg;
	//TNotice::MakeNoticeMsgEx(&pMsg, iMessageType, &szBuff[0]);

	for(int i = 0; i < MAX_ILLUSION_TEAMS; i++)
	{
		for(ILLUSION_OBJIT it = this->m_IllusionTemple[iTempleIndex].m_Teams[i].m_Members.begin();
			it != this->m_IllusionTemple[iTempleIndex].m_Teams[i].m_Members.end(); it++)
		{
			//gSendProto.DataSend(it->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SendMsgTempleTeam(int iTempleIndex, int iTeamIndex, int iMessageType, char* Message, ...)
{
	va_list pArguments;
	va_start ( pArguments, Message );
	
	char szBuff[300] = {0};
	vsprintf(&szBuff[0], Message, pArguments);

	va_end ( pArguments );

	//PMSG_NOTICE pMsg;
	//TNotice::MakeNoticeMsgEx(&pMsg, iMessageType, &szBuff[0]);

	for(ILLUSION_OBJIT it = this->m_IllusionTemple[iTempleIndex].m_Teams[iTeamIndex].m_Members.begin();
		it != this->m_IllusionTemple[iTempleIndex].m_Teams[iTeamIndex].m_Members.end(); it++)
	{
		//gSendProto.DataSend(it->m_Index, (BYTE*)&pMsg, pMsg.h.size);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::UserStateCheck(int iTempleIndex)
{
	if(this->m_dwUserCheckTick < GetTickCount())
	{
		if(this->m_IllusionTemple[iTempleIndex].m_TempleState != IT_STATE_CLOSED && 
			this->m_IllusionTemple[iTempleIndex].m_TempleState != IT_STATE_OPEN)
		{
			for(int iTeamIndex = 0; iTeamIndex < MAX_ILLUSION_TEAMS; iTeamIndex++)
			{
				for(ILLUSION_OBJIT it = this->m_IllusionTemple[iTempleIndex].m_Teams[iTeamIndex].m_Members.begin();
					it != this->m_IllusionTemple[iTempleIndex].m_Teams[iTeamIndex].m_Members.end(); it++)
				{
					if(gObj[it->m_Index].MapNumber != (MAP_INDEX_ILLUSION1 + iTempleIndex) 
						|| gObjIsConnected(&gObj[it->m_Index]) == FALSE)
					{
						CLog.LogAdd("[IllusionTemple] %s UserOut (%d)", gObj[it->m_Index].Name, iTempleIndex+1);
						if(it->m_Index == this->m_IllusionTemple[it->m_TempleIndex].m_BallOwner)
						{
							this->m_IllusionTemple[it->m_TempleIndex].m_BallOwner = this->m_IllusionTemple[it->m_TempleIndex].m_Statues[rand()%MAX_IT_STATUES];
						}
						if(gObjIsConnected(it->m_Index))
						{
							for(int i = 0; i < INVENTORY_SIZE; i++)
							{
								if(gObj[it->m_Index].pInventory[i].IsItem())
								{
									if(gObj[it->m_Index].pInventory[i].m_Type == ITEMGET(14, 64))
									{
										gObjInventoryDeleteItem(it->m_Index, i);
										GCInventoryItemDeleteSend(it->m_Index, i, 1);
									}
								}
							}
							gObj[it->m_Index].m_Change = -1;
							gObjViewportListProtocolCreate(&gObj[it->m_Index]);
						}
						this->m_IllusionTemple[iTempleIndex].m_Teams[iTeamIndex].m_Members.erase(it);
						this->UserStateCheck(iTempleIndex);
						return;
					}
				}
			}
		}
		if(iTempleIndex == MAX_ILLUSION_TEMPLES-1)
		{
			this->m_dwUserCheckTick = GetTickCount() + 2000;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)
{
	for ( int i=OBJ_STARTUSERINDEX; i<OBJMAX; i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				gSendProto.DataSend(i, lpMsg, iSize);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::Load(const char* szFile)
{
	this->m_IllusionLoaded = false;

	TokenizerGroup* group = (new Tokenizer())->ParseFile(szFile);

	SMDFile = fopen(szFile, "r");

	if (SMDFile == NULL)
	{
		CLog.MsgBox("[IllusionTemple] Event file Load Fail [%s]", szFile);
		return;
	}

	if(group == NULL)
	{
		CLog.LogAddC(2, "[IllusionTemple] Failed to load 'IllusionTemple.dat'");
		return;
	}

	//-- Schedule Section
	TokenizerSection* section = group->GetSection(0);

	if(section == NULL)
	{
		CLog.LogAddC(2, "[IllusionTemple] Failed to load Event Start Times");
		return;
	}

	ILLUSION_START_TIME	sTime;

	for(int i = 0; i < section->RowCount; i++)
	{
		if(section->Rows[i]->ColumnCount < 2)
		{
			CLog.LogAddC(2, "[IllusionTemple] Failed to load event Start Times (wrong column count)");
			return;
		}
		sTime.m_iStartHour = section->Rows[i]->GetInt(0);
		sTime.m_iStartMinute = section->Rows[i]->GetInt(1);
		this->m_IllusionOpenTime.push_back(sTime);
	}
	
	//-- Rules Section
	section = group->GetSection(1);

	if(section == NULL)
	{
		CLog.LogAddC(2, "[IllusionTemple] Failed to load Rule Info");
		return;
	}

	for(int i = 0; i < section->RowCount; i++)
	{
		if(section->Rows[i]->ColumnCount < 3)
		{
			CLog.LogAddC(2, "[IllusionTemple] Failed to load Event Rule Info (wrong column count)");
			return;
		}
		if(section->Rows[i]->GetInt(0) >= 0 && section->Rows[i]->GetInt(0) < MAX_ILLUSION_TEMPLES)
		{			
			this->m_IllusionRules[section->Rows[i]->GetInt(0)].m_MinLevel = section->Rows[i]->GetInt(1);
			this->m_IllusionRules[section->Rows[i]->GetInt(0)].m_MaxLevel = section->Rows[i]->GetInt(2);
		}
	}

	//-- Awards Section
	section = group->GetSection(2);

	if(section == NULL)
	{
		CLog.LogAddC(2, "[IllusionTemple] Failed to load reward info");
		return;
	}

	for(int i = 0; i < section->RowCount; i++)
	{
		if(section->Rows[i]->ColumnCount < 4)
		{
			CLog.LogAddC(2, "[IllusionTemple] Failed to load reward info (wrong column count)");
			return;
		}

		if(section->Rows[i]->GetInt(0) >= 0 && section->Rows[i]->GetInt(0) < MAX_ILLUSION_TEMPLES)
		{	
			this->m_IllusionRewards[section->Rows[i]->GetInt(0)].m_ItemType = section->Rows[i]->GetInt(1, 12);
			this->m_IllusionRewards[section->Rows[i]->GetInt(0)].m_ItemIndex = section->Rows[i]->GetInt(2, 15);
			this->m_IllusionRewards[section->Rows[i]->GetInt(0)].m_ItemLevel = section->Rows[i]->GetInt(3, 0);
		}
	}

	//-- Skill Points Section
	section = group->GetSection(3);

	if(section == NULL)
	{
		CLog.LogAddC(2, "[IllusionTemple] Failed to load Skill Points Info");
		return;
	}

	this->m_iMonsterKillPoint = 2;
	this->m_iPlayerKillPoint = 5;
	this->m_iBallKillPoint = 10;

	for(int i = 0; i < section->RowCount; i++)
	{
		if(section->Rows[i]->ColumnCount < 2)
		{
			CLog.LogAddC(2, "[IllusionTemple] Failed to load Skill Points Info (wrong column count)");
			return;
		}

		switch(section->Rows[i]->GetInt(0))
		{
		case 0:
			this->m_iMonsterKillPoint = section->Rows[i]->GetInt(1);
			break;
		case 1:
			this->m_iPlayerKillPoint = section->Rows[i]->GetInt(1);
			break;
		case 2:
			this->m_iBallKillPoint = section->Rows[i]->GetInt(1);
			break;
		}
	}

	//-- Comp Section
	section = group->GetSection(4);

	if(section == NULL)
	{
		CLog.LogAddC(2, "[IllusionTemple] Failed to load Comp Info");
		return;
	}

	for(int i = 0; i < section->RowCount; i++)
	{
		if(section->Rows[i]->ColumnCount < 3)
		{
			CLog.LogAddC(2, "[IllusionTemple] Failed to load Comp Info (wrong column count)");
			return;
		}

		if(section->Rows[i]->GetInt(0) >= 0 && section->Rows[i]->GetInt(0) < MAX_ILLUSION_TEMPLES)
		{	
			this->m_IllusionTemple[section->Rows[i]->GetInt(0)].m_ScoreFactorA = section->Rows[i]->GetInt(0, 0);
			this->m_IllusionTemple[section->Rows[i]->GetInt(0)].m_ScoreFactorB = section->Rows[i]->GetInt(2, 0);
		}
	}

	CLog.LogAdd("[%s] File Successfully Loaded!", szFile);
	this->m_IllusionLoaded = true;
	
	delete group;
	delete section;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SynchronizeTemple(int iTempleIndex)
{
	if (!(iTempleIndex >= 0 && iTempleIndex < MAX_ILLUSION_TEMPLES)) return;

	if ( this->m_IllusionOpenTime.empty() != false )
	{
		CLog.LogAddC(2, "[IllusionTemple] (%d) No Schedule TimeData - IllusionTemple Terminated (m_IllusionOpenTime.empty())", iTempleIndex);
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	BOOL bTIME_CHANGED = FALSE;

	for ( std::list<ILLUSION_START_TIME>::iterator it = this->m_IllusionOpenTime.begin() ; it != this->m_IllusionOpenTime.end() ; it++ )
	{
		ILLUSION_START_TIME* stITime = &(*it);
		
		if ( (sysTime.wHour * 60 + sysTime.wMinute) < (stITime->m_iStartHour * 60 + stITime->m_iStartMinute) )
		{
			if ( ( iMIN_HOUR * 60 + iMIN_MINUTE ) > ( stITime->m_iStartHour * 60 + stITime->m_iStartMinute ) )
			{
				bTIME_CHANGED = TRUE;
				iMIN_HOUR = stITime->m_iStartHour;
				iMIN_MINUTE = stITime->m_iStartMinute;
			}
		}
	}

	if ( bTIME_CHANGED == FALSE )
	{
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;
		for ( std::list<ILLUSION_START_TIME>::iterator it = this->m_IllusionOpenTime.begin(); it != this->m_IllusionOpenTime.end() ; it++ )
		{
			ILLUSION_START_TIME * stITime = &(*it);
			if ( ( iMIN_HOUR * 60 + iMIN_MINUTE ) > ( stITime->m_iStartHour * 60 + stITime->m_iStartMinute ) )
			{
				bTIME_CHANGED = 2;
				iMIN_HOUR = stITime->m_iStartHour;
				iMIN_MINUTE = stITime->m_iStartMinute;
			}
		}
	}

	switch ( bTIME_CHANGED )
	{
		case 1:
			this->m_IllusionTemple[iTempleIndex].m_RemainTime = (((iMIN_HOUR * 60 * 60) + (iMIN_MINUTE * 60)) - ((sysTime.wHour * 60 * 60) + (sysTime.wMinute * 60) + (sysTime.wSecond))) * 1000;
			break;

		case 2:
			this->m_IllusionTemple[iTempleIndex].m_RemainTime = ((((iMIN_HOUR+24) * 60 * 60) + (iMIN_MINUTE * 60)) - ((sysTime.wHour * 60 * 60) + (sysTime.wMinute * 60) + (sysTime.wSecond))) * 1000;
			break;

		default:
			CLog.LogAddC(2, "[IllusionTemple] (%d) No Schedule TimeData - Illusion Temple Terminated (bTIME_CHANGED=%d)", iTempleIndex + 1, bTIME_CHANGED);
			return;

	}

	this->m_IllusionTemple[iTempleIndex].m_TickCount = GetTickCount();

	CLog.LogAdd("[IllusionTemple] (%d) Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		iTempleIndex+1, this->m_IllusionTemple[iTempleIndex].m_RemainTime / 60000, iMIN_HOUR, iMIN_MINUTE);

}
// -----------------------------------------------------------------------------------------------------------------------
int CIllusionTemple::IsObjectInEvent(int iUserIndex)
{
	if(OBJMAX_RANGE(iUserIndex) == FALSE)
	{
		return FALSE;
	}
	return this->IsObjectInEvent(&gObj[iUserIndex]);
}
// -----------------------------------------------------------------------------------------------------------------------
int CIllusionTemple::IsObjectInEvent(OBJECTSTRUCT* lpObj)
{
	if(IT_FLOOR_RANGE(lpObj->m_IllusionIndex) == TRUE && IT_MAP_RANGE(lpObj->MapNumber) == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::Synchronize()
{
	if(!this->m_IllusionActive)
	{
		for(int i = 0; i < MAX_ILLUSION_TEMPLES; i++)
		{
			this->SynchronizeTemple(i);
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::Run()
{
#if(GS_CASTLE)
	return;
#endif

	if(!this->m_IllusionLoaded)
	{
		return;
	}

	if(this->m_IllusionInit == FALSE)
	{
		this->SetState(IT_STATE_CLOSED);
		this->m_IllusionInit = TRUE;
	}
	
	EnterCriticalSection(&this->m_Critical);

	for(int i = 0; i < MAX_ILLUSION_TEMPLES; i++)
	{
		this->UpdateTemple(i);
	}

	LeaveCriticalSection(&this->m_Critical);
}
// -----------------------------------------------------------------------------------------------------------------------
bool CIllusionTemple::UpdateTemple(int iTempleIndex)
{
	this->UserStateCheck(iTempleIndex);

	switch(this->m_IllusionTemple[iTempleIndex].m_TempleState)
	{
		case IT_STATE_CLOSED:
			this->DoProc_Closed(iTempleIndex);
			break;

		case IT_STATE_OPEN:
			this->DoProc_Open(iTempleIndex);
			break;

		case IT_STATE_PREPARE:
			this->DoProc_Prepare(iTempleIndex);
			break;

		case IT_STATE_BATTLE:
			this->DoProc_Battle(iTempleIndex);
			break;

		case IT_STATE_END:
			this->DoProc_End(iTempleIndex);
			break;
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SendDataWaitRoom(int iTempleIndex, BYTE* szBuffer, int iSize)
{
	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{
		if(gObj[i].MapNumber == MAP_INDEX_ILLUSION1 + iTempleIndex)
		{
			gSendProto.DataSend(i, (BYTE*)szBuffer, iSize);
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SendDataTemple(int iTempleIndex, BYTE* szBuffer, int iSize)
{
	for(int i = 0; i < MAX_ILLUSION_TEAMS; i++)
	{
		for(ILLUSION_OBJIT it = this->m_IllusionTemple[iTempleIndex].m_Teams[i].m_Members.begin();
			it != this->m_IllusionTemple[iTempleIndex].m_Teams[i].m_Members.end(); it++)
		{
			gSendProto.DataSend(it->m_Index, szBuffer, iSize);
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SendDataTempleTeam(int iTempleIndex, int iTeamIndex, BYTE* szBuffer, int iSize)
{
	for(ILLUSION_OBJIT it = this->m_IllusionTemple[iTempleIndex].m_Teams[iTeamIndex].m_Members.begin();
		it != this->m_IllusionTemple[iTempleIndex].m_Teams[iTeamIndex].m_Members.end(); it++)
	{
		gSendProto.DataSend(it->m_Index, szBuffer, iSize);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
#pragma pack(1)

struct PMSG_ILLUSION_RADAR
{
	unsigned short UserIndex;
	BYTE Unknow;
	BYTE X;
	BYTE Y;
	BYTE Unknow2;
};
// -----------------------------------------------------------------------------------------------------------------------
struct PMSG_ILLUSION_STATE
{
	PBMSG_HEAD2	h;
	unsigned short Timer;
	unsigned short BallOwner;
	BYTE BallOwnerX;
	BYTE BallOwnerY;
	BYTE RedPoints;
	BYTE BluePoints;
	BYTE RadarColor;
	BYTE PlayerCount;
	PMSG_ILLUSION_RADAR	Radar[5];
};
// -----------------------------------------------------------------------------------------------------------------------
#pragma pack()

void CIllusionTemple::UpdateTempleStatus(int iTempleIndex)
{

	if(IT_FLOOR_RANGE(iTempleIndex) == FALSE)
		return;

	ILLUSION_TEMPLE_DATA* lpTemple = &this->m_IllusionTemple[iTempleIndex];

	PMSG_ILLUSION_STATE	pMsg;
	C1SubHeadSet((BYTE*)&pMsg, 0xBF, 0x01, sizeof(pMsg));

	pMsg.Timer = ((WORD)(lpTemple->m_TickCount - GetTickCount())) / 1000;
	
	// Arrange
	pMsg.BallOwner = this->m_IllusionTemple[iTempleIndex].m_BallOwner;
	pMsg.BallOwnerX = (this->m_IllusionTemple[iTempleIndex].m_BallOwner != -1 ? gObj[this->m_IllusionTemple[iTempleIndex].m_BallOwner].X : 0);
	pMsg.BallOwnerY = (this->m_IllusionTemple[iTempleIndex].m_BallOwner != -1 ? gObj[this->m_IllusionTemple[iTempleIndex].m_BallOwner].Y : 0);

	pMsg.BluePoints = lpTemple->m_Teams[IT_TEAM_BLUE].m_Score;
	pMsg.RedPoints = lpTemple->m_Teams[IT_TEAM_RED].m_Score;

	for(int team = 0; team < MAX_ILLUSION_TEAMS; team++)
	{
		int iCount = 0;
		pMsg.RadarColor = team;
		ZeroMemory(&pMsg.Radar[0], sizeof(PMSG_ILLUSION_RADAR) * 5);
		for(ILLUSION_OBJIT it = lpTemple->m_Teams[team].m_Members.begin();
			it != lpTemple->m_Teams[team].m_Members.end(); it++)
		{
			pMsg.Radar[iCount].UserIndex = it->m_Index;
			pMsg.Radar[iCount].X = (BYTE)gObj[it->m_Index].X;
			pMsg.Radar[iCount].Y = (BYTE)gObj[it->m_Index].Y;
			pMsg.Radar[iCount].Unknow = 0;
			pMsg.Radar[iCount].Unknow2 = 0;
			iCount++;
		}
		pMsg.PlayerCount = iCount;
		pMsg.h.size = 14 + (iCount * sizeof(PMSG_ILLUSION_RADAR));
		this->SendDataTempleTeam(iTempleIndex, team, (BYTE*)&pMsg, pMsg.h.size);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
int CIllusionTemple::GetUserCount(int iTempleIndex)
{
	if(IT_FLOOR_RANGE(iTempleIndex) == FALSE)
		return 0;

	int iCount = 0;
	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{
		if(IT_MAP_RANGE(gObj[i].MapNumber) == TRUE && gObjIsConnected(i) == TRUE)
		{
			if(iTempleIndex == (gObj[i].MapNumber - MAP_INDEX_ILLUSION1))
			{
				iCount++;
			}
		}
	}

	return iCount;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::SetTempleMonsters(int iTempleIndex)
{
	if ( IT_FLOOR_RANGE(iTempleIndex) == FALSE )
	{
		return;
	}

	for(int iStatueIndex = 0; iStatueIndex < MAX_IT_STATUES; iStatueIndex++)
	{
		int result = 0;
		result = gObjAddMonster(iTempleIndex + MAP_INDEX_ILLUSION1);
		if ( result >= 0 && result < OBJMAX)
		{
			gObj[result].m_PosNum = -1;
			gObj[result].X = s_itStatuePosition[iStatueIndex].PositionX;
			gObj[result].Y = s_itStatuePosition[iStatueIndex].PositionY;
			gObj[result].MapNumber = MAP_INDEX_ILLUSION1 + iTempleIndex;
			gObj[result].TX = s_itStatuePosition[iStatueIndex].PositionX;
			gObj[result].TY = s_itStatuePosition[iStatueIndex].PositionY;
			gObj[result].m_OldX = s_itStatuePosition[iStatueIndex].PositionX;
			gObj[result].m_OldY = s_itStatuePosition[iStatueIndex].PositionY;
			gObj[result].Dir = s_itStatuePosition[iStatueIndex].Direction;
			gObj[result].StartX = s_itStatuePosition[iStatueIndex].PositionX;
			gObj[result].StartY = s_itStatuePosition[iStatueIndex].PositionY;
			gObjSetMonster(result, 380);
			gObj[result].MaxRegenTime = 200;
			this->m_IllusionTemple[iTempleIndex].m_Statues[iStatueIndex] = result;
		}
		else
		{
			this->SetTempleState(iTempleIndex, IT_STATE_CLOSED);
			CLog.LogAddC(2, "[IllusionTemple] Statue Setup Error! (Statue:%d, Temple:%d)", iStatueIndex, iTempleIndex + 1);
			return;
		}
	}

	this->m_IllusionTemple[iTempleIndex].m_BallOwner = this->m_IllusionTemple[iTempleIndex].m_Statues[rand()%2];

	for ( int n = 0; n < gMSetBase.m_Count; n++)
	{
		if ( IT_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) == TRUE )
		{

			if ( (gMSetBase.m_Mp[n].m_MapNumber - MAP_INDEX_ILLUSION1) != iTempleIndex )
			{
				continue;
			}

			if(gMSetBase.m_Mp[n].m_Type == 380) continue;

			int result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);
			if ( result >= 0  && result <= OBJMAX)
			{
				gObj[result].m_PosNum = n;
				gObj[result].X = gMSetBase.m_Mp[n].m_X;
				gObj[result].Y = gMSetBase.m_Mp[n].m_Y;
				gObj[result].MapNumber = gMSetBase.m_Mp[n].m_MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = gMSetBase.m_Mp[n].m_Dir;
				gObj[result].StartX = (BYTE)gObj[result].X;
				gObj[result].StartY = (BYTE)gObj[result].Y;
				gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type);
				gObj[result].MaxRegenTime = 2000;
				this->m_IllusionTemple[iTempleIndex].m_SpawnedMonsters.push_back(result);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::UnSetTempleMonsters(int iTempleIndex)
{
	if(this->m_IllusionTemple[iTempleIndex].m_Statues[IT_STATUE_UP] >= 0 && 
		this->m_IllusionTemple[iTempleIndex].m_Statues[IT_STATUE_UP] < OBJMAX)
	{
		gObjDel(this->m_IllusionTemple[iTempleIndex].m_Statues[IT_STATUE_UP]);
	}

	if(this->m_IllusionTemple[iTempleIndex].m_Statues[IT_STATUE_DOWN] >= 0 &&
		this->m_IllusionTemple[iTempleIndex].m_Statues[IT_STATUE_DOWN] < OBJMAX)
	{
		gObjDel(this->m_IllusionTemple[iTempleIndex].m_Statues[IT_STATUE_DOWN]);
	}

	for(list<int>::iterator it = this->m_IllusionTemple[iTempleIndex].m_SpawnedMonsters.begin(); 
		it != this->m_IllusionTemple[iTempleIndex].m_SpawnedMonsters.end(); it++)
	{
		gObjDel(*it);
	}

	this->m_IllusionTemple[iTempleIndex].m_SpawnedMonsters.clear();

}
#endif
// -----------------------------------------------------------------------------------------------------------------------