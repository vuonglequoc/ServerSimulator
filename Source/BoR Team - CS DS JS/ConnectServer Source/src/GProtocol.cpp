#include "GInclude.h"
#include "GProtocol.h"
#include "Main.h"
#include "TServerManager.h"
#include "TServersConfig.h"
//-----------------------------------------------------------------------------------------------------------
//# Made by [RCZ]ShadowKing && Leo123[BoR];
//-----------------------------------------------------------------------------------------------------------

void CSProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	g_Log.SetStatusText(g_Colors.Black(), g_Colors.GreenYellow(), "%s ON_GO", MAIN_PROJECT_NAME);
	// ----
	switch (HeadCode)
	{
		case 0xF4:
		{
			switch(aRecv[3])
			{
				case CS_SERVER_SELECT:
				{
					ServerInfoReq(aIndex, MAKEWORD(aRecv[4], aRecv[5]));
				}
				break;

				case CS_CLIENT_CONNECT:
				{
					ServerListReq(aIndex);
				}
				break;
			}
		}
		break;
	}
	// ----
	g_Log.SetStatusText(g_Colors.White(), g_Colors.Gray() , "%s STATE_WAIT", MAIN_PROJECT_NAME);
}
//-----------------------------------------------------------------------------------------------------------

void CSConnectResultSend(int aIndex)
{
	PMSG_HELLO pMsg = {0};
	// ----
	pMsg.h.set(0x00, sizeof(pMsg));
	pMsg.result = 0x01;
	// ----
	// # Some problems with async send.. so we use unsync..
	send(g_ServerManager.getSocket(aIndex) ,(char*) & pMsg , sizeof(pMsg) , 0);
}
//-----------------------------------------------------------------------------------------------------------

void ServerListReq(int aIndex)
{
	if(g_ServersConfig.GetServersCount() > 0)
	{
		CSServerListSend(aIndex);
	}
	else g_ServerManager.ServerDel(aIndex);
}
//-----------------------------------------------------------------------------------------------------------

void CSServerListSend(int aIndex)
{
	register size_t PacketSize	= g_ServersConfig.GetServersCount() * 4 + 7;
	UCHAR *ServerList			= new unsigned char[PacketSize]();
	USHORT Count				= 0;
	int Percent					= 0;
	// ----
	ServerList[0]				= 0xC2;
	ServerList[3]				= 0xF4;
	ServerList[4]				= 0x06;
	ServerList[5]				= 0x00;
	ServerList[6]				= 0x00;
	// ----
	for(USHORT i = 0; i != g_ServersConfig.m_nTotalGroupsCount; i++)
	{
		_SERVERS * lpGroup = g_ServersConfig.GetGroupByIndex(i);
		// ----
		for(USHORT n = 0; n != lpGroup->Count; n++)
		{
			_SERVER * lpServer = &lpGroup->Server[n];
			// ----
//			sprintf(g_SharedManager.m_Temp, "%s%d", SHARED_LOADPERCENT, lpServer->Port);
			// ----
//			Percent = g_SharedManager.ReadMemoryInt(g_SharedManager.m_Temp);
			// ----
			if((lpServer->Status) && (Percent != -1))
			{
				memcpy(&ServerList[Count * 4 + 7], &lpServer->ServerCode, 2);
				// ----
				ServerList[Count * 4 + 9] = Percent;
				// ----
				ServerList[6]++;
				Count++;
			}
			else PacketSize -= 4;
		}
	}
	// ----
	if(ServerList[6] > 0)
	{
		ServerList[1] = HIBYTE(PacketSize);
		ServerList[2] = LOBYTE(PacketSize);
		g_ServerManager.DataSend(aIndex, ServerList, PacketSize);
	}
	else
	{
		g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][CSServerListSend] :: No active server!");
		g_ServerManager.ServerDel(aIndex);
	}
	// ----
	delete [] ServerList;
	/*register size_t PacketSize	= g_ServersConfig.GetServersCount() * 4 + 7;
	USHORT Count				= 0;
	int Percent					= 0;
	// ----
	ServerList List				= {0};
	// ----
	List.SubHead				= 0x06;
	// ----
	for(USHORT i = 0; i != g_ServersConfig.m_nTotalGroupsCount; i++)
	{
		_SERVERS * lpGroup = g_ServersConfig.GetGroupByIndex(i);
		// ----
		for(USHORT n = 0; n != lpGroup->Count; n++)
		{
			_SERVER * lpServer = &lpGroup->Server[n];
			// ----
			sprintf(g_SharedManager.m_Temp, "%s%d", SHARED_LOADPERCENT, lpServer->Port);
			// ----
			Percent = g_SharedManager.ReadMemoryInt(g_SharedManager.m_Temp);
			// ----
			if((lpServer->Status) && (Percent != -1))
			{
				memcpy(&List.Servers[Count].ServerCode, &lpServer->ServerCode, 2);
				// ----
				List.Servers[Count].Percent = 0;
				List.Servers[Count].UNK = 0;
				// ----
				List.ServersCountH = SET_NUMBERH((Count + 1));
				List.ServersCountL = SET_NUMBERL((Count + 1));
				Count++;
			}
			else PacketSize -= 4;
		}
	}
	// ----
	if((Count + 1) > 0)
	{
		List.h.set(0xF4, PacketSize);
		// ----
		g_ServerManager.DataSend(aIndex, (LPBYTE)&List, PacketSize);
		g_Log.LogAdd(g_Colors.Orange(), "%d", sizeof(List));
	}
	else
	{
		g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][CSServerListSend] :: No active server!");
		g_ServerManager.ServerDel(aIndex);
	}*/
}
//-----------------------------------------------------------------------------------------------------------

void ServerInfoReq(int aIndex, USHORT ServerCode)
{
	GS_CONNECT_INFO pMsg	= {0};
	// ----
	pMsg.h.set(0xF4, sizeof(pMsg));
	pMsg.SubHead			= 0x03;
	// ----
	_SERVER * lpServer = g_ServersConfig.GetServerByCode(ServerCode);
	// ----
	if(lpServer->ServerCode == ServerCode)
	{
		memcpy(&pMsg.IP, & lpServer->IP , strlen(lpServer->IP));
		pMsg.Port		=  lpServer->Port;
		// ----
		g_ServerManager.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		// ----
		g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][ServerInfoReq] :: [%d] Server Selected!", ServerCode);
	}
}
//-----------------------------------------------------------------------------------------------------------