//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef CSERVERCONFIG_H
#define CSERVERCONFIG_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#define MAX_OBJECT_IN_GROUP 20
#define MAX_GROUPS			18				// client shows only 7 || ServerList.bmd -> 18 groups
#define DATA_PATCH			"config.xml"
//-------------------------------------------------------------------------------------------------------------------------------------------

struct _SERVER
{
	USHORT		ServerCode;
	bool		Status;
	char		IP[30];
	USHORT		Port;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

struct _SERVERS
{
	USHORT		Count;
	USHORT		Group;
	_SERVER		Server[MAX_OBJECT_IN_GROUP];
};
//-------------------------------------------------------------------------------------------------------------------------------------------

class CServersConfig
{
public:	
	CServersConfig(void);
	~CServersConfig(void);
	// ----
	void		SetDefault();
	// ----
	bool		Read();
	// ----
	_SERVER*	GetServerByCode(int Code);
	_SERVERS*	GetGroupByIndex(int Index);
	// ----
	int inline	GetServersCount(){ return m_TotalServerCount; };
	// ----
	USHORT		m_TotalServerCount;
	USHORT		m_nTotalGroupsCount;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

extern CServersConfig g_ServersConfig;
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* CSERVERCONFIG_H */
//-------------------------------------------------------------------------------------------------------------------------------------------