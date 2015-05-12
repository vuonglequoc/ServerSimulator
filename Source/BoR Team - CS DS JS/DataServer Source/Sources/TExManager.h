/*//-----------------------------------------------------------------------------------------------------------
#ifndef TEXMANAGER_H
#define TEXMANAGER_H
//-----------------------------------------------------------------------------------------------------------

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
//-----------------------------------------------------------------------------------------------------------

struct tagUSER_DATA
{
	void	Clear();
	char	m_szAccountID[10];
	int		m_ServerIndex;
	int		m_ServerCode;
};
//-----------------------------------------------------------------------------------------------------------

class TExManager : public IOCP
{
public:
	TExManager(void);
	~TExManager(void);
	// ----
	void Init();
	// ----
	void OnError(const char * szMessage, ...);
	void OnMessage(const char * szMessage, ...);
	void OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
	void OnSearch(int aIndex);
	void OnConnect(int aIndex);
	void OnDisconnect(int aIndex);
	// ----
	bool IsCodeExist		(int ServerCode);
	bool AddServer		(int ServerIndex,int ServerCode, char * szServerName, int Port);
	void DelServer		(int ServerCode);
	// ----
	void DelUserByIndex(int nIndex);
};
//-----------------------------------------------------------------------------------------------------------

extern TExManager	g_TExManager[];
extern int			g_ConnectedCount; // ??
//-----------------------------------------------------------------------------------------------------------
#endif /* SERVERMANAGER_H */
//-----------------------------------------------------------------------------------------------------------
*/