#include "GInclude.h"
#include "TTimerManger.h"
#include "TServerManager.h"

//-------------------------------------------------------------------------------------------------------------------------------------------
// # Made by  : [RCZ]ShadowKing

CTimerManager::CTimerManager(void)
{

}
//-------------------------------------------------------------------------------------------------------------------------------------------

CTimerManager::~CTimerManager(void)
{

}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CTimerManager::OnSecond()
{
	for(int i = 0; i != MAX_OBJECT; i++)
	{
		if(g_ServerManager.isConnected(i) == true)
		{
			if(g_ServerManager.m_ActiveTime[i] <= 0)
			{
				g_Log.LogAdd(g_Colors.Blue(), "[CTimerManager][OnSecond] :: Idle time exepted closing index %d", i);
				g_ServerManager.CloseClient(i);
			}
			else g_ServerManager.m_ActiveTime[i]--;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------