//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include "../../libs/IOCP/TIOCP.h"
//-------------------------------------------------------------------------------------------------------------------------------------------

#define MAX_USER		(MAX_OBJECT * 1000)
#define SLOT_FREE		-1
//-------------------------------------------------------------------------------------------------------------------------------------------

#include "GProtocol.h"
//-------------------------------------------------------------------------------------------------------------------------------------------

struct tagSERVER_DATA
{
	void	Clear();
	char	m_szServerName[50];
	int		m_Index;
	int		m_ServerCode;
	int		m_Port;
	int		m_ConnectedCount;
	int		m_MaxUser;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

struct tagUSER_DATA
{
	void	Clear();
	char	m_szAccountID[10];
	int		m_ServerIndex;
	int		m_ServerCode;
	bool	m_bRequestMapSvrMove;
	int		m_nMoveMapTime;
	int		iUserNumber;
	short	nNumber;
	DWORD	BillCheckTimer;
	DWORD	BillCheckSendTimer;
	int		iJoinAuthCode1;
	int		iJoinAuthCode2;
	int		iJoinAuthCode3;
	int		iJoinAuthCode4;
	WORD	wPrevMapSvrCode;
	WORD	wMapNumber;
	BYTE	btX;
	BYTE	btY;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

class CServerManager : public IOCP
{
public:
	CServerManager(void);
	~CServerManager(void);
	// ----
	void	Init();
	void	Disconnect		(int aIndex){ServerDel(aIndex);};
	// ----
	void	OnError			(const char * szMessage, ...);
	void	OnMessage		(const char * szMessage, ...);
	void	OnRecv			(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
	void	OnSearch		(int aIndex);
	void	OnConnect		(int aIndex);
	void	OnDisconnect	(int aIndex);
	// ----
	bool	IsCodeExist		(int ServerCode);
	bool	AddServer		(int ServerIndex,int ServerCode, char * szServerName, int Port);
	void	DelServer		(int ServerCode);
	int		GetPortByCode	(int ServerCode);
	// ----
	bool	AddUser			(int ServerIndex, char * szAccountID, char * szIP, short nNumber);
	void	DelUser			(const char * szAccountID);
	bool	CheckUser		(const char * szAccountID); 
	bool	UserExist		(const char * szAccountID);
	int		GetUserIndexByID(const char * szAccountID);
	void	DelUserByIndex	(int nIndex);
	// ----
	void	StuckAccountScan();
	// ----
	int		m_ServerConnectedCount;
	int		m_ServerLoggedCount;
	int		m_UserConnectedCount;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

extern CServerManager	g_ServerManager;
extern bool				g_UseMD5;
extern tagSERVER_DATA	m_ServObj[MAX_OBJECT];
extern tagUSER_DATA		m_UserObj[MAX_USER];
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* SERVERMANAGER_H */
//-------------------------------------------------------------------------------------------------------------------------------------------
