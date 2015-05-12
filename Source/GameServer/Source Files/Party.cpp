#include "../Header Files/stdafx.h"
#include "../Header Files/Party.h"
#include "../Header Files/User.h"
#include "../Header Files/Packets.h"
#include "../Header Files/Functions.h"
#include "../Header Files/IllusionTemple.h"
#include "../Header Files/ChaosCastle.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/GameMain.h"
#include "../Header Files/Notice.H"
#include "../Header Files/SendProto.H"


PartyClass		gParty;
// ------------------------------------------------------------------------------------------------------------------------------------------------------
PartyClass::PartyClass()
{
	// -----
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
PartyClass::~PartyClass()
{
	// -----
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool PartyClass::IsParty(int Number)
{
	if(Number < 0 || Number >= OBJMAX) return false;
	// -----
	if(m_PartyInfo[Number].Count == 0) return false;
	// -----
	return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
int PartyClass::GetCount(int Number)
{
	if(IsParty(Number) == 0) return false;
	// -----
	return m_PartyInfo[Number].Count;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool PartyClass::GetIndexUser(int Number, int aIndex, int& UserNum,  int& DBNum)
{
	if ( IsParty(Number) == FALSE ) return false;
	// -----
	if ( aIndex < 0 || aIndex >= MAX_USER_IN_PARTY ) return false;
	// -----
	UserNum = m_PartyInfo[Number].Number[aIndex];
	DBNum	= m_PartyInfo[Number].DbNumber[aIndex];
	// -----
	return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
int PartyClass::GetIndex(int Number, int UserNum, int DBNum)
{
	if ( IsParty(Number) == FALSE ) return FALSE;
	// -----
	for ( int n=0; n<MAX_USER_IN_PARTY; n++)
	{
		if ( m_PartyInfo[Number].Number[n] == UserNum || m_PartyInfo[Number].DbNumber[n] == DBNum )
		{
			return n;
		}
	}
	// -----
	return -1;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
int PartyClass::Create(int usernumber, int dbnumber, int level)
{
	int n = m_PartyCount;
	// -----
	int LoopCount = 0;
	// -----
	while ( true )
	{
		if ( m_PartyInfo[n].Count == 0 )
		{
			m_PartyInfo[n].Count = 1;
			m_PartyInfo[n].Number[0] = usernumber;
			m_PartyInfo[n].DbNumber[0] = dbnumber;
			// -----
			for ( int i=1;i<MAX_USER_IN_PARTY;i++)
			{
				m_PartyInfo[n].DbNumber[i] = -1;
				m_PartyInfo[n].Number[i] = -1;
			}
			// -----
			m_PartyCount++;
			m_PartyInfo[n].m_MaxLevel = 0;
			m_PartyInfo[n].m_MinLevel = 0;
			SetLevel(n, level);
			// -----
			if ( m_PartyCount >= OBJMAX )
			{
				m_PartyCount=0;
			}
			// -----
			return n;
		}
		// -----
		n++;
		// -----
		if ( n >= OBJMAX )
		{
			n=0;
		}
		// -----
		LoopCount++;
		// -----
		if ( LoopCount >= OBJMAX )
		{
			break;
		}
	}
	// -----
	return -1;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool PartyClass::Destroy(int Number)
{
	if ( IsParty(Number) == FALSE ) return false;
	// -----
	m_PartyInfo[Number].Count		= 0;
	m_PartyInfo[Number].Number[0]	= -1;
	m_PartyInfo[Number].DbNumber[0] = -1;
	m_PartyCount--;
	// -----
	return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
int PartyClass::AddMember(int Number, int UserNum, int DBNum, int Level)
{
	if ( IsParty(Number) == FALSE ) return -1;
	// -----
	for ( int i = 0; i<MAX_USER_IN_PARTY ; i++)
	{
		if ( (m_PartyInfo[Number].Number[i] >= 0) && (m_PartyInfo[Number].Number[i] == UserNum) && (m_PartyInfo[Number].DbNumber[i] == DBNum) )
		{
			return -2;
		}
	}
	// -----
	for (int i = 0; i<MAX_USER_IN_PARTY ; ++i)
	{
		if ( m_PartyInfo[Number].Number[i] == -1 )
		{
			m_PartyInfo[Number].Number[i]	= UserNum;
			m_PartyInfo[Number].DbNumber[i] = DBNum;
			m_PartyInfo[Number].Count++;
			// -----
			SetLevel(Number, Level);
			// -----
			return i;
		}
	}
	// -----
	return -1;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
int PartyClass::DelMember(int Number, int UserNum, int DBNum)
{
	if ( IsParty(Number) == FALSE ) return -1;
	// -----
	for ( int i = 0 ; i<MAX_USER_IN_PARTY ; i++ )
	{
		if ( (m_PartyInfo[Number].Number[i] == UserNum) && (m_PartyInfo[Number].DbNumber[i] == DBNum) )
		{
			m_PartyInfo[Number].Number[i]	= -1;
			m_PartyInfo[Number].DbNumber[i] = -1;
			// -----
			m_PartyInfo[Number].Count--;
			// -----
			return i;
		}
	}
	// -----
	return -1;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void PartyClass::DelMember(int Number, int aIndex)
{
	if ( IsParty(Number) == FALSE ) return;
	// -----
	if ( aIndex < 0 || aIndex >= MAX_USER_IN_PARTY ) return;
	// -----
	if ( m_PartyInfo[Number].Number[aIndex] >= 0 )
	{
		m_PartyInfo[Number].Number[aIndex]		= -1;
		m_PartyInfo[Number].DbNumber[aIndex]	= -1;
		// -----
		m_PartyInfo[Number].Count--;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool PartyClass::SetLevel(int Number, int Level)
{
	if ( m_PartyInfo[Number].m_MaxLevel  < Level )
	{
		m_PartyInfo[Number].m_MaxLevel  = Level;
	}
	// -----
	if ( m_PartyInfo[Number].m_MinLevel  == 0 || m_PartyInfo[Number].m_MinLevel > Level )
	{
		m_PartyInfo[Number].m_MinLevel = Level;
	}
	// -----
	return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool PartyClass::GetLevel(int Number, int& MaxLevel, int& MinLevel)
{
	if ( IsParty(Number) == FALSE ) return false;
	// -----
	MaxLevel = m_PartyInfo[Number].m_MaxLevel;
	MinLevel = m_PartyInfo[Number].m_MinLevel;
	// -----
	return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
int PartyClass::GetPartyCount(int Number)
{
	if ( IsParty( Number ) == FALSE ) return -1;
	// -----
	return m_PartyInfo[Number].Count;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void PartyClass::Paint(int Number)
{
	if (IsParty(Number) == FALSE ) return;
	// -----
	int aIndex = 0;
	// -----
	wsprintf( m_szTempPaint, "Member (%d) :", m_PartyInfo[Number].Count);
	// -----
	for ( int i = 0; i< MAX_USER_IN_PARTY ; i++ )
	{
		if ( (m_PartyInfo[Number].Number[i] != 0 ) && (m_PartyInfo[Number].DbNumber[i] != 0 ) )
		{
			aIndex = m_PartyInfo[Number].Number[i];
			// -----
			if ( !gObjIsConnected(aIndex) ) continue;
			// -----
			strcat(m_szTempPaint, "[");
			strcat(m_szTempPaint, gObj[aIndex].AccountID);
			strcat(m_szTempPaint, "][");
			strcat(m_szTempPaint, gObj[aIndex].Name);
			strcat(m_szTempPaint, "], ");
		}
	}
	// -----
	CLog.LogAddC(TColor.Green(), PartyModule, m_szTempPaint);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void PartyClass::PartyMemberLifeSend(int Number)
{
	if ( IsParty(Number) == FALSE ) return;
	// -----
	PMSG_DEFAULT_COUNT	pCount;
	PMSG_PARTYLIFEALL	pList;
	// -----
	int		aIndex;
	int		lOfs = 0;
	char	SendBuff[512];
	// -----
	C1HeadSet((LPBYTE)&pCount, 0x44, sizeof(pCount));
	// -----
	pCount.Count = m_PartyInfo[Number].Count;
	// -----
	lOfs += sizeof(pCount);
	// -----
	for ( int i = 0; i<MAX_USER_IN_PARTY ; i++ )
	{
		if ( (m_PartyInfo[Number].Number[i] >= 0 ) && (m_PartyInfo[Number].DbNumber[i] >= 0) )
		{
			aIndex = m_PartyInfo[Number].Number[i];
			// -----
			if ( gObjIsConnected(aIndex) != PLAYER_EMPTY )
			{
				pList.Number  = (i*16) & 0xF0 ;
				pList.Number |= (BYTE)(gObj[aIndex].Life / ((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) / 10)) & 0x0F;
				// -----
				memcpy(&SendBuff[lOfs], &pList, 1);
				// -----
				lOfs++;
			}
			else
			{
				m_PartyInfo[Number].Number[i]		= -1;
				m_PartyInfo[Number].DbNumber[i]	= -1;
				// -----
				m_PartyInfo[Number].Count--;
			}
		}
	}
	// -----
	pCount.h.size = lOfs;
	// -----
	memcpy(SendBuff, &pCount, sizeof(pCount));
	// -----
	for ( int i=0 ; i<MAX_USER_IN_PARTY ; i++ )
	{
		aIndex = m_PartyInfo[Number].Number[i];
		// -----
		if ( aIndex >= 0 )
		{
			gSendProto.DataSend(aIndex, (unsigned char*)SendBuff, lOfs);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex)
{
	int number =  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	PMSG_PARTYREQUESTSEND pMsg;

	if ( number < 0 || number > OBJMAX-1)
		return;

	if (!PacketCheckTime(&gObj[aIndex]))
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		::GCResultSend(aIndex, 0x41, 0x03);
		return;
	}

	if ( gObj[aIndex].CloseCount >= 0 || gObj[number].CloseCount >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( gObj[number].GenFamily != gObj[aIndex].GenFamily )
	{
		::GCServerMsgStringSend("Can't make party with different gen member", aIndex, 1);
		return;
	}

	if ( gUser.ChaosCastleMapRange(gObj[aIndex].MapNumber) == true )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
		return;
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		::GCServerMsgStringSend("Can't make party in Illusion Temple", aIndex, 1);
		return;
	}

#if (FOREIGN_GAMESERVER==1)
	if ( szAuthKey[4] != AUTHKEY4 )
		DestroyGIocp();
#endif

	//LogAddL("Party result : %d %d %d %d", gObj[aIndex].CloseCount, gObj[number].CloseCount, gObj[aIndex].Connected, gObj[number].Connected);
	//LogAddL("%s %s", gObj[aIndex].Name, gObj[number].Name);

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
		
	if ( gObj[number].m_IfState.use > 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( (gObj[number].m_Option &1) != TRUE )
	{
		::GCResultSend(aIndex, 0x41, 0x01);
		return;
	}

	if ( gObj[aIndex].PartyNumber >= 0 )
	{
		int iUserIndex;
		int iDbNumber;
		gParty.GetIndexUser(gObj[aIndex].PartyNumber, 0, iUserIndex, iDbNumber);
		
		if ( iUserIndex < 0 || ( iUserIndex != aIndex && gObj[aIndex].DBNumber != iDbNumber )  )
		{
			::GCResultSend(aIndex, 0x41, 0x00);
			return;

		}
	}

	if ( gObj[aIndex].PartyTargetUser >= 0 )
		return;

	if ( gObj[number].PartyNumber >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x04);
		return;
	}

	if ( gObj[number].PartyTargetUser >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	int maxlevel = 0;
	int minlevel = 0;

	/*if ( gObj[number].PartyNumber >= 0 )
	{
		if ( gParty.GetLevel( gObj[number].PartyNumber, maxlevel, minlevel) == TRUE )
		{
			int limmaxlevel;
			int limmaxlevel2;

			if ( maxlevel > gObj[number].Level )
				limmaxlevel = maxlevel;
			else
				limmaxlevel = gObj[number].Level;

			if ( maxlevel < gObj[number].Level )
				limmaxlevel2 = maxlevel;
			else
				limmaxlevel2 = gObj[number].Level;

			if ( (limmaxlevel - limmaxlevel2) > MAX_PARTY_LEVEL_DIFFERENCE )
			{
				::GCResultSend(aIndex, 0x41, 0x05);
				// Restricted level for the party is %d
				// DEATHWAY FIX HERE
				LogAdd(lMsg.Get(MSGGET(2, 189)), gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
				return;
			}

			int limminlevel;
			int limminlevel2;

			if ( maxlevel > gObj[number].Level )
				limminlevel = minlevel;
			else
				limminlevel = gObj[number].Level;

			if ( maxlevel < gObj[number].Level )
				limminlevel2 = minlevel;
			else
				limminlevel2 = gObj[number].Level;

			if ( (limminlevel - limminlevel2) > MAX_PARTY_LEVEL_DIFFERENCE )
			{
				::GCResultSend(aIndex, 0x41, 0x05);
				// Restricted level for the party is %d
				// DEATHWAY FIX HERE
				LogAdd(lMsg.Get(MSGGET(2, 189)), gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
				return;
			}
		}
	}
	else
	{
		short sMaxMinLevel[2];

		if ( gObj[aIndex].Level > gObj[number].Level )
			sMaxMinLevel[1] = gObj[aIndex].Level;
		else
			sMaxMinLevel[1] = gObj[number].Level;
		
		if ( gObj[aIndex].Level < gObj[number].Level )
			sMaxMinLevel[0] = gObj[aIndex].Level;
		else
			sMaxMinLevel[0] = gObj[number].Level;
		
		if ( (sMaxMinLevel[1]- sMaxMinLevel[0]) >MAX_PARTY_LEVEL_DIFFERENCE)
		{
			::GCResultSend(aIndex, 0x41, 0x05);
			// Restricted level for the party is %d
			// DEATHWAY FIX HERE
			LogAdd(lMsg.Get(MSGGET(2, 189)), gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
			return;
		}
	}*/

	gObj[aIndex].m_IfState.use = TRUE;
	gObj[aIndex].m_IfState.type = 2;
	gObj[aIndex].m_IfState.state = 0;
	gObj[number].m_IfState.use = TRUE;
	gObj[number].m_IfState.type = 2;
	gObj[number].m_IfState.state = 0;
	gObj[aIndex].TargetNumber = number;
	gObj[number].TargetNumber = aIndex;
	gObj[aIndex].m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();
	gObj[aIndex].PartyTargetUser = number;
	
	C1HeadSet((LPBYTE)&pMsg, 0x40, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	gSendProto.DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
}
/*void CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex)
{
	int Number =  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	// -----
	LPOBJ lpObj			= &gObj[aIndex];
	LPOBJ TargetpObj	= &gObj[Number];
	// ------
	PMSG_PARTYREQUESTSEND pMsg;
	// -----
	if( Number < 0 || Number > OBJMAX-1) return;
	// -----
	if( !PacketCheckTime(lpObj) ) return;
	// -----
	if( !gObjIsConnected(TargetpObj) )
	{
		GCResultSend(aIndex, 0x41, 0x03);
		return;
	}
	// -----
	if( lpObj->CloseCount >= 0 || TargetpObj->CloseCount >= 0 )
	{
		GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
	// -----
	if( CC_MAP_RANGE(lpObj->MapNumber) )
	{
		SendMsg.MessageOut(aIndex, "You can't form a Party in ChaosCastle.");
		return;
	}
	// -----
	if( IT_MAP_RANGE(lpObj->MapNumber) )
	{
		SendMsg.MessageOut(aIndex, "You can't form a Party in IllusionTemple.");
		return;
	}
	// -----
	if( lpObj->GenFamily != TargetpObj->GenFamily )
	{
		SendMsg.MessageOut(aIndex, "You can't form a Party with different Gen Member");
		return;
	}
	// -----
	if( lpObj->m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
	// -----	
	if( TargetpObj->m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
	// -----
	if( (TargetpObj->m_Option &1) != TRUE )
	{
		GCResultSend(aIndex, 0x41, 0x01);
		return;
	}
	// -----
	if( lpObj->PartyNumber >= 0 )
	{
		int iUserIndex;
		int iDbNumber;
		// -----
		gParty.GetIndexUser(lpObj->PartyNumber, 0, iUserIndex, iDbNumber);
		// -----
		if( iUserIndex < 0 || ( iUserIndex != aIndex && lpObj->DBNumber != iDbNumber ) )
		{
			GCResultSend(aIndex, 0x41, 0x00);
			return;
		}
	}
	// -----
	if( lpObj->PartyTargetUser >= 0 ) return;
	// -----
	if( TargetpObj->PartyNumber >= 0 )
	{
		GCResultSend(aIndex, 0x41, 0x04);
		return;
	}
	// -----
	if( TargetpObj->PartyTargetUser >= 0 )
	{
		GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
	// -----
	int MaxLevel = 0;
	int MinLevel = 0;
	// -----
	if( TargetpObj->PartyNumber >= 0 )
	{
		if( gParty.GetLevel( TargetpObj->PartyNumber, MaxLevel, MinLevel) == TRUE )
		{
			int limmaxlevel;
			int limmaxlevel2;

			if( MaxLevel > TargetpObj->Level )
			{
				limmaxlevel = MaxLevel;
			}
			else
				limmaxlevel = TargetpObj->Level;

			if( MaxLevel < TargetpObj->Level )
				limmaxlevel2 = MaxLevel;
			else
				limmaxlevel2 = TargetpObj->Level;

			if( (limmaxlevel - limmaxlevel2) > MAX_PARTY_LEVEL_DIFFERENCE )
			{
				GCResultSend(aIndex, 0x41, 0x05);
				// Restricted level for the party is %d
				// DEATHWAY FIX HERE
				CLog.LogAdd(lMsg.Get(MSGGET(2, 189)), lpObj->Name, lpObj->Level, TargetpObj->Name, TargetpObj->Level);
				return;
			}

			int limminlevel;
			int limminlevel2;

			if( MaxLevel > TargetpObj->Level )
				limminlevel = MinLevel;
			else
				limminlevel = TargetpObj->Level;

			if( MaxLevel < TargetpObj->Level )
				limminlevel2 = MinLevel;
			else
				limminlevel2 = TargetpObj->Level;

			if( (limminlevel - limminlevel2) > MAX_PARTY_LEVEL_DIFFERENCE )
			{
				GCResultSend(aIndex, 0x41, 0x05);
				// Restricted level for the party is %d
				// DEATHWAY FIX HERE
				CLog.LogAdd(lMsg.Get(MSGGET(2, 189)), lpObj->Name, lpObj->Level, TargetpObj->Name, TargetpObj->Level);
				return;
			}
		}
	}
	else
	{
		short sMaxMinLevel[2];

		if( gObj[aIndex].Level > TargetpObj->Level )
			sMaxMinLevel[1] = gObj[aIndex].Level;
		else
			sMaxMinLevel[1] = TargetpObj->Level;
		
		if( gObj[aIndex].Level < TargetpObj->Level )
			sMaxMinLevel[0] = gObj[aIndex].Level;
		else
			sMaxMinLevel[0] = TargetpObj->Level;
		
		if( (sMaxMinLevel[1]- sMaxMinLevel[0]) >MAX_PARTY_LEVEL_DIFFERENCE)
		{
			GCResultSend(aIndex, 0x41, 0x05);
			// Restricted level for the party is %d
			// DEATHWAY FIX HERE
			CLog.LogAdd(lMsg.Get(MSGGET(2, 189)), lpObj->Name, lpObj->Level, TargetpObj->Name, TargetpObj->Level);
			return;
		}
	}

	lpObj->m_IfState.use = TRUE;
	lpObj->m_IfState.type = 2;
	lpObj->m_IfState.state = 0;
	TargetpObj->m_IfState.use = TRUE;
	TargetpObj->m_IfState.type = 2;
	TargetpObj->m_IfState.state = 0;
	lpObj->TargetNumber = Number;
	TargetpObj->TargetNumber = aIndex;
	lpObj->m_InterfaceTime = GetTickCount();
	TargetpObj->m_InterfaceTime = GetTickCount();
	lpObj->PartyTargetUser = Number;
	
	C1HeadSet((LPBYTE)&pMsg, 0x40, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	gSendProto.DataSend(Number, (LPBYTE)&pMsg, pMsg.h.size);
}*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CGPartyRequestResultRecv(PMSG_PARTYREQUESTRESULT * lpMsg, int aIndex)
{
	int number;
	BYTE result = false;
	char szTemp[256];
	int pnumber = -1;

	if( !PacketCheckTime(&gObj[aIndex]))
		return;

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if( number < 0 || number > OBJMAX-1)
		return;

	if( !gObjIsConnected(&gObj[aIndex]))
		return;

	if( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x41, 0x04);
	}
	else
	{
		result = true;
	}

	if( gObj[number].MapNumber != gObj[aIndex].MapNumber )
	{
		result = false;
		GCResultSend(number, 0x41, 0x00);
	}

	if( gUser.ChaosCastleMapRange(gObj[aIndex].MapNumber) == true )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}

	if( lpMsg->Result == 0 )
	{
		result = false;
		::GCResultSend(number, 0x41, 0x01);
	}

	if( gObj[number].m_IfState.use == 0 || gObj[number].m_IfState.type != 2)
	{
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}	
	
	if( gObj[aIndex].m_IfState.use == 0 || gObj[aIndex].m_IfState.type != 2)
	{
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}

	if( result == 1 )
	{
		if( gObj[number].PartyNumber < 0 )
		{
			gObj[number].PartyNumber = gParty.Create(number, gObj[number].DBNumber, gObj[number].Level);
		}

		if( gObj[number].PartyNumber >= 0  )
		{
			pnumber = gObj[number].PartyNumber;
			int iPartyPos = gParty.AddMember(gObj[number].PartyNumber, aIndex, gObj[aIndex].DBNumber, gObj[aIndex].Level);

			if( iPartyPos >= 0 )
			{
				gObj[aIndex].PartyNumber = gObj[number].PartyNumber;
				result = true;
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 83)), gObj[aIndex].Name);
				::GCServerMsgStringSend(szTemp, number, 1);
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 83)), gObj[number].Name);
				::GCServerMsgStringSend(szTemp, aIndex, 1);

				gParty.Paint(pnumber);
			}
			else if( iPartyPos == -1 )
			{
				GCResultSend(number, 0x41, 2);
				GCResultSend(aIndex, 0x41, 2);
			}
			else if( iPartyPos == -2 )
			{
				result = 2;
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 84)), gObj[aIndex].Name);
				GCServerMsgStringSend(szTemp, aIndex, 1);
			}
		}
	}

	if( gObj[aIndex].m_IfState.use !=  0 && gObj[aIndex].m_IfState.type == 2)
	{
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].PartyTargetUser = -1;
	}

	if( gObj[number].m_IfState.use !=  0 && gObj[number].m_IfState.type == 2)
	{
		gObj[number].m_IfState.use = 0;
		gObj[number].PartyTargetUser = -1;
	}

	if( pnumber >= 0 )
	{
		CGPartyListAll(pnumber);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CGPartyList(int aIndex)
{
	PMSG_PARTYLISTCOUNT pCount;
	PMSG_PARTYLIST pList;
	BYTE sendbuf[512];
	int lOfs = 0;

	if( !PacketCheckTime(&gObj[aIndex]))
		return;

	C1HeadSet((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Count = 0;
	pCount.Result = false;

	int number;
	int pnumber = gObj[aIndex].PartyNumber ;

	if( pnumber >= 0 )
		pCount.Count = gParty.GetCount(pnumber);

	lOfs += sizeof(pCount);

	if( pCount.Count != 0 )
	{
		for ( int n=0;n<MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyInfo[pnumber].Number[n];

			if( number >= 0 )
			{
				if( gObjIsConnected(&gObj[number]) == TRUE )
				{
					memset(&pList, 0, sizeof(pList));
					memcpy(pList.szId, gObj[number].Name, MAX_ACCOUNT_LEN);
					pList.Number = n;
					pList.MapNumber = gObj[number].MapNumber;
					pList.X = gObj[number].X;
					pList.Y = gObj[number].Y;
					pList.Life = gObj[number].Life;
					pList.MaxLife = gObj[number].MaxLife + gObj[number].AddLife;

					memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
					lOfs += sizeof(pList);
				}
			}
		}

		pCount.Result = true;
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	
	gSendProto.DataSend(aIndex, (LPBYTE)&sendbuf, lOfs);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CGPartyListAll(int pnumber)
{
	PMSG_PARTYLISTCOUNT pCount;
	PMSG_PARTYLIST pList;
	BYTE sendbuf[512];
	int lOfs = 0;
	int number;

	if( pnumber < 0 )
		return;

	if( gParty.IsParty(pnumber) == FALSE )
		return;

	C1HeadSet((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Result = false;
	pCount.Count = gParty.GetCount(pnumber);

	if( pCount.Count == 0 )
		return;

	pCount.Result = true;
	lOfs += sizeof(pCount);

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyInfo[pnumber].Number[n];

		if( number >= 0 )
		{
			if( gObjIsConnected(&gObj[number]) == TRUE )
			{
				memset(&pList, 0, sizeof(pList));
				memcpy(pList.szId, gObj[number].Name, MAX_ACCOUNT_LEN);
				pList.Number = n;
				pList.MapNumber = gObj[number].MapNumber;
				pList.X = gObj[number].X;
				pList.Y = gObj[number].Y;
				pList.Life = gObj[number].Life;
				pList.MaxLife =  gObj[number].MaxLife;

				memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
				lOfs += sizeof(pList);
			}
		}
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyInfo[pnumber].Number[n];

		if( number >= 0 )
		{
			gSendProto.DataSend(number, sendbuf, lOfs);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CGPartyDelUser(PMSG_PARTYDELUSER * lpMsg, int aIndex)
{
	int usernumber = -1;
	int dbnumber = -1;

	if( gObj[aIndex].PartyNumber < 0 )
	{
		CLog.LogAdd("No Party %s %s", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	if( !gParty.GetIndexUser(gObj[aIndex].PartyNumber, lpMsg->Number, usernumber, dbnumber))
	{
		CLog.LogAdd("User Not found");
		return;
	}

	if( usernumber < 0 )
	{
		CLog.LogAdd("User Not found");
		return;
	}

	int count;
	int number;
	int pnumber = gObj[aIndex].PartyNumber;

	if( pnumber < 0 )
	{
		CLog.LogAdd("%s %d", __FILE__, __LINE__);
		return;
	}

	count = gParty.GetCount(pnumber);

	if( count < 1 )
	{
		CLog.LogAdd("%s %d", __FILE__, __LINE__);
		return;
	}

	if( lpMsg->Number == 0 || count <= 2 )
	{
		for ( int n=0;n<MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyInfo[pnumber].Number[n];

			if( number >= 0 )
			{
				gParty.DelMember(pnumber, n);
				gObj[number].PartyNumber = -1;
				gObj[number].PartyTargetUser = -1;
				GCPartyDelUserSend(number);
			}
		}

		gParty.Destroy(pnumber);
	}
	else
	{
		gParty.DelMember(pnumber, lpMsg->Number);
		count = gParty.GetCount(gObj[aIndex].PartyNumber);
		gObj[usernumber].PartyNumber = -1;
		gObj[usernumber].PartyTargetUser = -1;
		GCPartyDelUserSend(usernumber);
		CGPartyListAll(pnumber);
	}		
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void GCPartyDelUserSend(int aIndex)
{
	PMSG_PARTYDELUSERSEND pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x43, sizeof(pMsg));

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------