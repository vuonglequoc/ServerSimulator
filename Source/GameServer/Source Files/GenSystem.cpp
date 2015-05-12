//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Gen System
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/GenSystem.h"
#include "../Header Files/Defines.h"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/Functions.h"
#include "../Header Files/SendProto.H"


GenSystem Gens;
// ------------------------------------------------------------
GenSystem::GenSystem(void)
{
	return;
}
// ------------------------------------------------------------
GenSystem::~GenSystem(void)
{
	return;
}
// ------------------------------------------------------------
void GenSystem::Init()
{
	Gens.NeedContForLvlUp[14]		= GetPrivateProfileInt("GenSystem","GenSystemContUp13",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[13]		= GetPrivateProfileInt("GenSystem","GenSystemContUp12",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[12]		= GetPrivateProfileInt("GenSystem","GenSystemContUp11",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[11]		= GetPrivateProfileInt("GenSystem","GenSystemContUp10",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[10]		= GetPrivateProfileInt("GenSystem","GenSystemContUp9",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[9]		= GetPrivateProfileInt("GenSystem","GenSystemContUp8",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[8]		= GetPrivateProfileInt("GenSystem","GenSystemContUp7",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[7]		= GetPrivateProfileInt("GenSystem","GenSystemContUp6",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[6]		= GetPrivateProfileInt("GenSystem","GenSystemContUp5",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[5]		= GetPrivateProfileInt("GenSystem","GenSystemContUp4",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[4]		= GetPrivateProfileInt("GenSystem","GenSystemContUp3",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[3]		= GetPrivateProfileInt("GenSystem","GenSystemContUp2",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[2]		= GetPrivateProfileInt("GenSystem","GenSystemContUp1",50000,GENS_FILE_PATH);
	Gens.NeedContForLvlUp[1]		= 0;
	Gens.NeedContForLvlUp[0]		= 0;
	// ----
	Gens.Enabled					= GetPrivateProfileInt("GenSystem","Enabled",0,GENS_FILE_PATH);
	Gens.MinLevel					= GetPrivateProfileInt("GenSystem","MinLevelToJoin",0,GENS_FILE_PATH);
}
// ------------------------------------------------------------
void GenSystem::GenSystemManager (LPOBJ lpObj, unsigned char * aRecv) 
{
	switch ( aRecv[3] )
	{
	case 1:
		JoinGen(lpObj,aRecv[4]);
		break;
	case 3:
		DeleteGen(lpObj);
		break;
	case 9:
		RankingGen(lpObj,aRecv[4]);
		break;
	case 11:
		GenCheck(lpObj);
		break;
	}
	return;
}
// ------------------------------------------------------------
void GenSystem::JoinGen(LPOBJ lpObj, int Type)
{
	if( lpObj->GenFamily == Type )
	{
		BYTE Packet[6] = { 0xC1, 0x06, 0xF8, 0x02, 0x01, 0x00 } ; //Can't Join Gen - Is Already Joined
		gSendProto.DataSend(lpObj->m_Index, &Packet[0], Packet[1]);
		return;
	}
	else if( lpObj->GenFamily != Type && lpObj->GenFamily != 0 )
	{
		BYTE Packet[5] = { 0xC1, 0x05 ,0xF8 ,0x0A ,0x05 } ; //You are a member of other Gens
		gSendProto.DataSend(lpObj->m_Index, &Packet[0], Packet[1]);
		return;
	}
	else if( lpObj->Level < 50 )
	{
		BYTE Packet[6] = { 0xC1, 0x06, 0xF8, 0x02, 0x03, 0x00 } ; //Can't Join Gen Need Lvl 50
        gSendProto.DataSend(lpObj->m_Index, &Packet[0], Packet[1]);
		return;
	}
	else if ( lpObj->PartyNumber != -1 )
    {
		BYTE Packet[6] = { 0xC1, 0x06, 0xF8, 0x02, 0x06, 0x00 } ; //Can't Join Gen - Can't Join Gen in Party
        gSendProto.DataSend(lpObj->m_Index, &Packet[0], Packet[1]);
		return;
	}
	lpObj->GenFamily = Type;
	lpObj->GenLevel = 14;
	lpObj->GenContribution = 0;
	lpObj->GenNextContribution = Gens.NeedContForLvlUp[14];
	BYTE Packet[6] = { 0xC1, 0x06, 0xF8, 0x02, 0x00, Type } ; //Joined Gen
	gSendProto.DataSend(lpObj->m_Index, &Packet[0], Packet[1]);
	return;
}
// ------------------------------------------------------------
void GenSystem::DeleteGen(LPOBJ lpObj)
{
	if (lpObj->GenFamily == 0)
	{
		BYTE Packet[5] = {0xc1, 0x05, 0xF8, 0x04, 0x01}; // You have not Joined The Gens
		gSendProto.DataSend(lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	else if ( (lpObj->GenFamily == 1 && lpObj->GenNPC == 544 ) || (lpObj->GenFamily == 2 && lpObj->GenNPC == 543))
	{
		BYTE Packet[5] = {0xc1, 0x05, 0xF8, 0x04, 0x03}; // You are a member of the different Gens
		gSendProto.DataSend(lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	else if ( lpObj->GuildStatus == 128 )
	{
		BYTE Packet[5] = {0xC1, 0x05, 0xF8, 0x04, 0x02}; // The Guild Master Cannot Leave The Gens
		gSendProto.DataSend(lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	lpObj->GenContribution = 0;
	lpObj->GenFamily = 0;
	lpObj->GenLevel = 0;
	lpObj->GenNextContribution = 0;
	lpObj->GenRanking = 0;
	lpObj->GenReward = 0;
	BYTE Packet[5] = {0xc1, 0x05, 0xF8, 0x04, 0x00}; // Successfull Delete Gens
	gSendProto.DataSend(lpObj->m_Index, Packet, Packet[1]);
	return;
}
// ------------------------------------------------------------
void GenSystem::RankingGen(LPOBJ lpObj,int Type)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	// ----
	if ( lpObj->GenFamily == 0 )
	{
		BYTE Packet[5] = { 0xC1, 0x05, 0xF8, 0x0A, 0x06 }; // You have not Joined The Gens
		gSendProto.DataSend (lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	else if ( lpObj->GenFamily != Type )
	{
		BYTE Packet[5] = { 0xC1, 0x05 ,0xF8 ,0x0A ,0x05 }; //You are a member of other Gens
		gSendProto.DataSend (lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	else if (time.wDay > 10)
	{
		BYTE Packet[5] = { 0xC1, 0x05 ,0xF8 ,0x0A ,0x01 }; //Ranking rewards are given of first week of each month
		gSendProto.DataSend (lpObj->m_Index ,Packet, Packet[1]);
		return;
	}
	else if (lpObj->GenReward == 1)
	{
		BYTE Packet[5] = { 0xC1, 0x05 ,0xF8 ,0x0A ,0x04 }; //The reward has already been handed out
		gSendProto.DataSend (lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	else if (lpObj->GenLevel > 8)
	{
		BYTE Packet[5] = { 0xC1, 0x05 ,0xF8 ,0x0A ,0x02 }; //You are not eligible for reward. Try harder later
		gSendProto.DataSend (lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	else if (CheckInventoryEmptySpace(lpObj,1,1) == 0)
	{
		BYTE Packet[5] = { 0xC1, 0x05 ,0xF8 ,0x0A ,0x03 }; //You don't have enought space for the item
		gSendProto.DataSend (lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	int ItemIndex=14*512+141;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	// ----
	switch (lpObj->GenLevel)
	{
	case 1:
	case 2:
		ItemSerialCreateSend(lpObj->m_Index,235,0,0,ItemIndex,0,0,0,0,0,lpObj->m_Index,0,0, Socket); // Winning Jewelry Box
		break;
	case 3:
	case 4:
		ItemSerialCreateSend(lpObj->m_Index,235,0,0,ItemIndex+1,0,0,0,0,0,lpObj->m_Index,0,0, Socket); // Sophisticated Jewelry Box
		break;
	case 5:
	case 6:
		ItemSerialCreateSend(lpObj->m_Index,235,0,0,ItemIndex+2,0,0,0,0,0,lpObj->m_Index,0,0, Socket); // Metal Jewelry Box
		break;
	case 7:
	case 8:
		ItemSerialCreateSend(lpObj->m_Index,235,0,0,ItemIndex+3,0,0,0,0,0,lpObj->m_Index,0,0, Socket); // Old Jewelry Box
		break;
	}
	// ----
	lpObj->GenReward = 1;
	BYTE Packet[5] = { 0xC1, 0x05 ,0xF8 ,0x0A ,0x00 }; //Congratulations You has been rewarded
	gSendProto.DataSend (lpObj->m_Index, Packet, Packet[1]);
	return;
}
// ------------------------------------------------------------
void GenSystem::SendLogo(LPOBJ lpObj,unsigned char * aSend)
{
	WORD Size = (16*aSend[4])+6;
	BYTE Packet[10000];
	Packet[0] = 0xC2;
	Packet[1] = HIBYTE(Size);
	Packet[2] = LOBYTE(Size);
	Packet[3] = 0xF8;
	Packet[4] = 0x05;
	Packet[5] = aSend[4];

	int EffectCount = 0;
	for(int x=0;x<aSend[4];x++)
	{
		int Inc = 16*x;
		int Inc2 = (36*x)+EffectCount;
		BYTE PID1 = aSend[5+Inc2];
		BYTE PID2 = aSend[6+Inc2];
		WORD PlayerID = MAKE_NUMBERW(PID1,PID2);
		if(PlayerID >= OBJMAX)PlayerID -= OBJMIN;
		if(PlayerID < OBJMIN || PlayerID >= OBJMAX)
		{
			Packet[6+Inc] = 0x00;
			Packet[14+Inc] = 0x00;
		}
		else
		{
			LPOBJ lpOtherObj = &gObj[PlayerID];
			Packet[6+Inc] = lpOtherObj->GenFamily;
			Packet[14+Inc] = lpOtherObj->GenLevel;
		}
		Packet[7+Inc] = aSend[5+Inc2];
		Packet[8+Inc] = aSend[6+Inc2];
		Packet[9+Inc] = 0x00;
		Packet[10+Inc] = 0x00;
		Packet[11+Inc] = 0x00;
		Packet[12+Inc] = 0x00;
		Packet[13+Inc] = 0x00;
		Packet[15+Inc] = 0x00;
		Packet[16+Inc] = 0x00;
		Packet[17+Inc] = 0x00;
		Packet[18+Inc] = 0x00;
 		Packet[19+Inc] = 0x00;
		Packet[20+Inc] = 0x00;
		Packet[21+Inc] = 0x00;

		EffectCount += aSend[40+Inc2];
	}
	gSendProto.DataSend(lpObj->m_Index, Packet, Size);
	return;
}
// ------------------------------------------------------------
void GenSystem::GenCheck(LPOBJ lpObj)
{
	if ( lpObj->GenFamily == 0 )
	{
		return;
	}
	PMSG_GEN_CHECK pMsg;
	// ----
	lpObj->GenNextContribution = Gens.NeedContForLvlUp[lpObj->GenLevel];
	this->GenLevelDec(lpObj);
	this->GenLevelInc(lpObj);
	// ----
	pMsg.c = 0xC1;
	pMsg.size = 0x18;
	pMsg.headcode = 0xF8;
	pMsg.subcode = 0x07;

	pMsg.Family = lpObj->GenFamily;
	pMsg.Ranking = lpObj->GenRanking;
	pMsg.Level = lpObj->GenLevel;
	pMsg.Cont = lpObj->GenContribution;
	if ( lpObj->GenLevel > 1 )
	{
		pMsg.NextCont = lpObj->GenNextContribution;
	}
	else
	{
		pMsg.NextCont = 0xFFFFFFFF;
	}
	
	gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg,pMsg.size);
	return;
}
// ------------------------------------------------------------
void GenSystem::GenLevelInc(LPOBJ lpObj)
{
	while ( true )
	{
		if (lpObj->GenLevel > 1)
		{
			if (lpObj->GenContribution >= Gens.NeedContForLvlUp[lpObj->GenLevel] )
			{
				lpObj->GenLevel -= 1;
				lpObj->GenNextContribution = Gens.NeedContForLvlUp[lpObj->GenLevel];
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	return;
}
// ------------------------------------------------------------
void GenSystem::GenLevelDec(LPOBJ lpObj)
{
	while ( true )
	{
		if (lpObj->GenLevel < 14)
		{
			if (lpObj->GenContribution < Gens.NeedContForLvlUp[lpObj->GenLevel+1] )
			{
				lpObj->GenLevel += 1;
				lpObj->GenNextContribution = Gens.NeedContForLvlUp[lpObj->GenLevel];
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	return;
}
// ------------------------------------------------------------
bool GenSystem::IsGenPVP(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	if ( ( lpObj->MapNumber == MAP_INDEX_VULCAN && lpTargetObj->MapNumber == MAP_INDEX_VULCAN ) ||
		 ( lpObj->MapNumber == MAP_INDEX_ATLANS && lpTargetObj->MapNumber == MAP_INDEX_ATLANS ) ||
		 ( lpObj->MapNumber == MAP_INDEX_AIDA && lpTargetObj->MapNumber == MAP_INDEX_AIDA ) ||
		 ( lpObj->MapNumber == MAP_INDEX_KANTURU1 && lpTargetObj->MapNumber == MAP_INDEX_KANTURU1 ) ||
		 ( lpObj->MapNumber == MAP_INDEX_KANTURU2 && lpTargetObj->MapNumber == MAP_INDEX_KANTURU2 ) ||
		 ( lpObj->MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS && lpTargetObj->MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS ) )
	{
		if ( (lpObj->GenFamily != lpTargetObj->GenFamily) && lpObj->GenFamily > 0 && lpTargetObj->GenFamily > 0)
		{
			return true;
		}
	}
	return false;
}
// ------------------------------------------------------------
void GenSystem::GenAddAndDecContribution(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	// lpObj = Killed User - Dec Contribution
	// lpTargetObj = Killer User - Inc Contribution
	// ---- 
	// If Killed or Killer User is Master
	if ( gUser.gObjIsMasteringLevel(lpObj) == true || gUser.gObjIsMasteringLevel(lpTargetObj) == true )
	{
		lpTargetObj->GenContribution += 5; // Increase Contribution
		// ----
		if ( lpObj->GenContribution >= 3 )
		{
			lpObj->GenContribution -= 3; // Decrease Contribution
		}
		else
		{
			lpObj->GenContribution = 0; // Decrease Contribution
		}
	}
	// ---- Killed User Lvl Higher than Killer User Lvl
	else if ( lpObj->Level > lpTargetObj->Level )
	{
		if ( lpObj->Level - lpTargetObj->Level <= 10 )
		{
			lpTargetObj->GenContribution += 5; // Increase Contribution
			// ----
			if ( lpObj->GenContribution >= 3 )
			{
				lpObj->GenContribution -= 3; // Decrease Contribution
			}
			else
			{
				lpObj->GenContribution = 0; // Decrease Contribution
			}
		}
		else if ( lpObj->Level - lpTargetObj->Level >= 11 && lpObj->Level - lpTargetObj->Level <= 30 )
		{
			lpTargetObj->GenContribution += 6; // Increase Contribution
			// ----
			if ( lpObj->GenContribution >= 3 )
			{
				lpObj->GenContribution -= 3; // Decrease Contribution
			}
			else
			{
				lpObj->GenContribution = 0; // Decrease Contribution
			}
		}
		else if ( lpObj->Level - lpTargetObj->Level >= 31 && lpObj->Level - lpTargetObj->Level <= 50 )
		{
			lpTargetObj->GenContribution += 6; // Increase Contribution
			// ----
			if ( lpObj->GenContribution >= 2 )
			{
				lpObj->GenContribution -= 2; // Decrease Contribution
			}
			else
			{
				lpObj->GenContribution = 0; // Decrease Contribution
			}
		}
		else if ( lpObj->Level - lpTargetObj->Level >= 51 )
		{
			lpTargetObj->GenContribution += 7; // Increase Contribution
			// ----
			if ( lpObj->GenContribution >= 1 )
			{
				lpObj->GenContribution -= 1; // Decrease Contribution
			}
			else
			{
				lpObj->GenContribution = 0; // Decrease Contribution
			}
		}
	}
	// ---- Killed User Lvl Lower than Killer User Lvl
	else if ( lpObj->Level < lpTargetObj->Level )
	{
		if ( lpTargetObj->Level - lpObj->Level <= 10 )
		{
			lpTargetObj->GenContribution += 5; // Increase Contribution
			// ----
			if ( lpObj->GenContribution >= 3 )
			{
				lpObj->GenContribution -= 3; // Decrease Contribution
			}
			else
			{
				lpObj->GenContribution = 0; // Decrease Contribution
			}
		}
		else if ( lpTargetObj->Level - lpObj->Level >= 11 && lpTargetObj->Level - lpObj->Level <= 30 )
		{
			lpTargetObj->GenContribution += 3; // Increase Contribution
			// ----
			if ( lpObj->GenContribution >= 3 )
			{
				lpObj->GenContribution -= 3; // Decrease Contribution
			}
			else
			{
				lpObj->GenContribution = 0; // Decrease Contribution
			}
		}
		else if ( lpTargetObj->Level - lpObj->Level >= 31 && lpTargetObj->Level - lpObj->Level <= 50 )
		{
			lpTargetObj->GenContribution += 2; // Increase Contribution
			// ----
			if ( lpObj->GenContribution >= 3 )
			{
				lpObj->GenContribution -= 3; // Decrease Contribution
			}
			else
			{
				lpObj->GenContribution = 0; // Decrease Contribution

			}
		}
		else if ( lpTargetObj->Level - lpObj->Level >= 51 )
		{
			lpTargetObj->GenContribution += 1; // Increase Contribution
			// ----
			if ( lpObj->GenContribution >= 3 )
			{
				lpObj->GenContribution -= 3; // Decrease Contribution
			}
			else
			{
				lpObj->GenContribution = 0; // Decrease Contribution
			}
		}
	}
	// ---- Killed User Lvl same than Killer User Lvl
	else
	{
		lpTargetObj->GenContribution += 5; // Increase Contribution
		// ----
		if ( lpObj->GenContribution >= 3 )
		{
			lpObj->GenContribution -= 3; // Decrease Contribution
		}
		else
		{
			lpObj->GenContribution = 0; // Decrease Contribution
		}
	}
	// ----
	Gens.GenLevelInc(lpTargetObj);
	Gens.GenLevelDec(lpObj);
}
// ------------------------------------------------------------
bool GenSystem::IsGenZone(LPOBJ lpObj)
{
	if ( lpObj->MapNumber == MAP_INDEX_VULCAN ||
		 lpObj->MapNumber == MAP_INDEX_ATLANS ||
		 lpObj->MapNumber == MAP_INDEX_AIDA  ||
		 lpObj->MapNumber == MAP_INDEX_KANTURU1 ||
		 lpObj->MapNumber == MAP_INDEX_KANTURU2  ||
		 lpObj->MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS )
	{
		return true;
	}
	return false;
}
// ------------------------------------------------------------
void GenSystem::GenRanking()
{
}