#include "GInclude.h"
#include "TServersConfig.h"
#include "Main.h"

//-------------------------------------------------------------------------------------------------------------------------------------------
//# Made by Leo123[BoR]

//-------------------------------------------------------------------------------------------------------------------------------------------

CServersConfig g_ServersConfig;
_SERVERS m_Servers[MAX_GROUPS];
//-------------------------------------------------------------------------------------------------------------------------------------------

CServersConfig::CServersConfig(void)
{
	// ----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

CServersConfig::~CServersConfig(void)
{
	// ----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServersConfig::SetDefault()
{
	for(int i = 0; i != MAX_GROUPS; i++)
	{
		m_Servers[i].Count = 0;
		m_Servers[i].Group = 0;
		// ----
		for(int n = 0; n != MAX_OBJECT_IN_GROUP; n++)
		{
			memset(m_Servers[i].Server[n].IP, 0, sizeof(m_Servers[i].Server[n].IP));
			// ----
			m_Servers[i].Server[n].Port			= 0;
			m_Servers[i].Server[n].ServerCode	= 0;
			m_Servers[i].Server[n].Status		= false;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool CServersConfig::Read()
{
	bool bReturn		= true;
	// ----
	SetDefault();
	// ----
	USHORT GroupCount	= 0;
	USHORT ServerCount	= 0;
	// ----
	m_TotalServerCount	= 0;
	// ----
	g_Log.LogAdd(g_Colors.Black(), "[CServersConfig][Read] :: Trying to read config : %s", DATA_PATCH);
	// ----
	if(g_xmlConfig.Load() == true)
	{
		if(g_xmlConfig.EnterNode("ConnectServer"))
		{
			xmlNodePtr ServersNode = g_xmlConfig.m_thisNode->children;
			// ----
			while(ServersNode)
			{
				if(xmlStrEqual(ServersNode->name, (const xmlChar*)"ServersGroup"))
				{
					ServerCount = 0;
					// ----
					xmlNodePtr subChild = ServersNode->children;
					// ----
					while(subChild)
					{
						if(xmlStrEqual(subChild->name, (const xmlChar*)"Server"))
						{
							_SERVER * lpServer = & m_Servers[GroupCount].Server[ServerCount];
							// ----
							lpServer->ServerCode = ServerCount + (GroupCount * 20);
							// ----
							memset(lpServer->IP, 0 , sizeof(lpServer->IP));
							memcpy(lpServer->IP, g_xmlConfig.GetStrProp(subChild, "Ip"), sizeof(lpServer->IP));
							// ----
							lpServer->Port		= g_xmlConfig.GetIntProp(subChild, "Port");
							lpServer->Status	= g_xmlConfig.GetBoolProp(subChild, "Show");
							// ----
							ServerCount++;
							m_TotalServerCount++;
						}
						// ----
						subChild = subChild->next;
					}
					// ----
					m_Servers[GroupCount].Count = ServerCount;
					m_Servers[GroupCount].Group = GroupCount;
					// ----
					GroupCount++;
				}
				// ----
				ServersNode = ServersNode->next;
			}
		}
	}
	else
	{
		bReturn = false;
		// ----
		g_Log.LogAdd(g_Colors.Red(), "[CServersConfig][Read] :: Failed to read config : %s", DATA_PATCH);
	}
	// ----
	if(bReturn) for(USHORT i = 0; i != GroupCount; i++)
	{
		_SERVERS * lpGroup = &m_Servers[i];
		// ----
		for(USHORT n = 0; n != lpGroup->Count; n++)
		{
			_SERVER * lpServer = &lpGroup->Server[n];
			// ----
			g_Log.LogAdd(g_Colors.Green(), "[CServersConfig][Read] :: Server [%d/%d] [Code(%d),IP(%s),PORT(%d),Status(%d)]",
				lpGroup->Group, n , lpServer->ServerCode, lpServer->IP , lpServer->Port, lpServer->Status);
		}
	}
	// ----
	g_Log.LogAdd(g_Colors.Black(), "[CServersConfig][Read] :: Config has been readed: %s", DATA_PATCH);
	g_Log.LogAdd(g_Colors.Purple(), "[CServersConfig][Read] :: Total server count GroupCount : %d", GroupCount);
	m_nTotalGroupsCount = GroupCount;
	// ----
	g_xmlConfig.UnLoad();
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
// UNFISNIHED !

_SERVER	* CServersConfig::GetServerByCode(int Code)
{
	_SERVER * lpReturn = {0};
	// ----
	for(USHORT i = 0 ; i != MAX_GROUPS ; i++)
	{
		for(USHORT n = 0 ; n != MAX_OBJECT_IN_GROUP ; n++)
		{
			if(m_Servers[i].Server[n].ServerCode == Code)
			{
				lpReturn = &m_Servers[i].Server[n];
				// ----
				return lpReturn;
			}
		}
	}
	// ----
	return NULL;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

_SERVERS * CServersConfig::GetGroupByIndex(int Index)
{
	return & m_Servers[Index];
}
//-------------------------------------------------------------------------------------------------------------------------------------------