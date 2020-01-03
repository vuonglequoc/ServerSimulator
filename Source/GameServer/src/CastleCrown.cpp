//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Castle Crown Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "CastleCrown.h"
#include "CastleSiege.h"
#include "user.h"
#include "LogProc.H"
#include "Functions.h"


#if(GS_CASTLE)
// ---------------------------------------------------------------------------------------------------------------------------------------------------
CCastleCrown g_CsNPC_CastleCrown;
// ---------------------------------------------------------------------------------------------------------------------------------------------------
CCastleCrown::CCastleCrown()
{
	return;
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
CCastleCrown::~CCastleCrown()
{
	return;
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CCastleCrown::CastleCrownAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	if ( gObj[iIndex].Type != OBJ_NPC ||
		 gObj[iIndex].Class != 216 )
		 return;

	if ( !gObjIsConnected(g_CastleSiege.GetCrownUserIndex()) )
	{
		g_CastleSiege.ResetCrownUserIndex();
		g_CastleSiege.SetCrownAccessUserX(0);
		g_CastleSiege.SetCrownAccessUserY(0);
		g_CastleSiege.ResetCrownAccessTickCount();

		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpUserObj = &gObj[g_CastleSiege.GetCrownUserIndex()];

	if ( lpUserObj->MapNumber == MAP_INDEX_CASTLESIEGE &&
		 lpUserObj->m_btCsJoinSide >= 2 &&
		 lpUserObj->m_bCsGuildInvolved == 1 )
	{
		if ( lpUserObj->Y == g_CastleSiege.GetCrownAccessUserY() &&
			 lpUserObj->X == g_CastleSiege.GetCrownAccessUserX() )
		{
			if ( gObjIsConnected(g_CastleSiege.GetCrownUserIndex()) &&
				 gObjIsConnected(g_CastleSiege.GetCrownSwitchUserIndex(217)) &&
				 gObjIsConnected(g_CastleSiege.GetCrownSwitchUserIndex(218)) )
			{
				if ( gObj[g_CastleSiege.GetCrownUserIndex()].m_btCsJoinSide == gObj[g_CastleSiege.GetCrownSwitchUserIndex(217)].m_btCsJoinSide &&
					 gObj[g_CastleSiege.GetCrownUserIndex()].m_btCsJoinSide == gObj[g_CastleSiege.GetCrownSwitchUserIndex(217)].m_btCsJoinSide )	// #warning ??? maybe is 218??? 
				{
					DWORD dwTime = GetTickCount() - g_CastleSiege.GetCrownAccessTickCount();
		
					CLog.LogAdd("[CastleSiege] [Reg. Accumulating] Accumulated Crown AccessTime : acc(%d) + %d [%s](%s)(%s)",
						lpUserObj->m_iAccumulatedCrownAccessTime, dwTime,
						lpUserObj->GuildName, lpUserObj->AccountID, lpUserObj->Name);

				}
				else
				{
					GCAnsCsAccessCrownState(lpUserObj->m_Index, 2);
					g_CastleSiege.ResetCrownUserIndex();
					g_CastleSiege.SetCrownAccessUserX(0);
					g_CastleSiege.SetCrownAccessUserY(0);
					g_CastleSiege.ResetCrownAccessTickCount();

					CLog.LogAdd("[CastleSiege] [%s][%s] Register Castle Crown Canceled (GUILD:%s)",
						lpUserObj->AccountID, lpUserObj->Name, lpUserObj->GuildName);
				}
			}
			else
			{
				GCAnsCsAccessCrownState(lpUserObj->m_Index, 2);
				g_CastleSiege.ResetCrownUserIndex();
				g_CastleSiege.SetCrownAccessUserX(0);
				g_CastleSiege.SetCrownAccessUserY(0);
				g_CastleSiege.ResetCrownAccessTickCount();
			}
		}
		else
		{
			GCAnsCsAccessCrownState(lpUserObj->m_Index, 2);
			g_CastleSiege.ResetCrownUserIndex();
			g_CastleSiege.SetCrownAccessUserX(0);
			g_CastleSiege.SetCrownAccessUserY(0);
			g_CastleSiege.ResetCrownAccessTickCount();
		}
	}
	else
	{
		GCAnsCsAccessCrownState(lpUserObj->m_Index, 2);
		g_CastleSiege.ResetCrownUserIndex();
		g_CastleSiege.SetCrownAccessUserX(0);
		g_CastleSiege.SetCrownAccessUserY(0);
		g_CastleSiege.ResetCrownAccessTickCount();
	}
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// ---------------------------------------------------------------------------------------------------------------------------------------------------