#include "../Header Files/stdafx.h"
#include "../Header Files/PKSystem.h"
#include "../Header Files/Defines.h"

CPKSystem gPKSystem;

CPKSystem::CPKSystem(void)
{
	return;
}

CPKSystem::~CPKSystem(void)
{
	return;
}

void CPKSystem::Init()
{
	gPKSystem.Enabled				= GetPrivateProfileInt(GS_INFO_HDR,"PKEnabled",0,RFile.szRFileName[0]);
	gPKSystem.ItemDrop				= GetPrivateProfileInt(GS_INFO_HDR,"PKItemDrop",0,RFile.szRFileName[0]);
	gPKSystem.IncreaseLevel			= GetPrivateProfileInt(GS_INFO_HDR,"PKIncreaseLevel",0,RFile.szRFileName[0]);
	gPKSystem.MurderSystem			= GetPrivateProfileInt(GS_INFO_HDR,"MurderEnabled",0,RFile.szRFileName[0]);
	gPKSystem.HeroSystem			= GetPrivateProfileInt(GS_INFO_HDR,"HeroEnabled",0,RFile.szRFileName[0]);
	gPKSystem.PKTime				= GetPrivateProfileInt(GS_INFO_HDR,"NeededTimeToReducePK",0,RFile.szRFileName[0]);

	return;
}

void CPKSystem::PKDownTime(LPOBJ lpObj, int TargetLevel)
{
	if(lpObj->m_PK_Level == 3)
	{
		return;
	}

	unsigned int dwtime = ((GetTickCount() - lpObj->m_dwPKTimer) / 1000) * gPKSystem.PKTime;

	if(dwtime > 5)
	{
		dwtime = 2;
		lpObj->m_dwPKTimer = GetTickCount();
	}
	else
	{
		lpObj->m_dwPKTimer = dwtime * 1000;
	}

	if(TargetLevel < 2)
	{
		lpObj->m_PK_Time += dwtime;
	}
	else
	{
		lpObj->m_PK_Time += TargetLevel;
	}

	if(lpObj->m_PK_Level < 3)
	{
		if(lpObj->m_PK_Time > 3600)
		{
			lpObj->m_PK_Time = 0;
			lpObj->m_PK_Count += 1;

			if(lpObj->m_PK_Count > 100)
			{
				lpObj->m_PK_Count = 100;
			}

			lpObj->m_PK_Level += 1;

			GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
		}
	}
	else if(lpObj->m_PK_Level > 3)
	{
		if(lpObj->m_PK_Count > 3)
		{
			if(lpObj->m_PK_Time > 3600)
			{
				lpObj->m_PK_Time = 0;
				lpObj->m_PK_Count -= 1;

				if(lpObj->m_PK_Count <= 0)
				{
					lpObj->m_PK_Count = 0;
					lpObj->m_PK_Level = 3;
				}
				else if(lpObj->m_PK_Count == 2)
				{
					lpObj->m_PK_Level = 5;
				}
				else if(lpObj->m_PK_Count == 1)
				{
					lpObj->m_PK_Level = 4;
				}

				if(lpObj->m_PK_Level <= 5)
				{
					GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
				}

				if(lpObj->m_PK_Level == 4)
				{
					//GCServerMsgStringSend(lMsg.Get(1137),lpObj->m_Index,1);
				}
			}
		}
		else if(lpObj->m_PK_Time > 10800)
		{
			lpObj->m_PK_Time = 0;
			lpObj->m_PK_Count -= 1;

			if(lpObj->m_PK_Count <= 0)
			{
				lpObj->m_PK_Count = 0;
				lpObj->m_PK_Level = 3;
			}
			else if(lpObj->m_PK_Count == 2)
			{
				lpObj->m_PK_Level = 5;
			}
			else if(lpObj->m_PK_Count == 1)
			{
				lpObj->m_PK_Level = 4;
			}

			if(lpObj->m_PK_Level <= 5)
			{
				GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
			}

			if(lpObj->m_PK_Level == 4)
			{
				//GCServerMsgStringSend(lMsg.Get(1137),lpObj->m_Index,1);
			}
		}
	}
}