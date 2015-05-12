//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef RSMANAGER_H
#define RSMANAGER_H
//-------------------------------------------------------------------------------------------------------------------------------------------

class TRSManager : public IOCP
{
public:
	TRSManager(void);
	~TRSManager(void);
	// ----
	void OnError(const char * szMessage, ...);
	void OnMessage(const char * szMessage, ...);
	void OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
	void OnSearch(int aIndex);
	void OnConnect(int aIndex);
	void OnDisconnect(int aIndex);
	// ----
};
//-------------------------------------------------------------------------------------------------------------------------------------------

extern TRSManager	g_TRSManager;
extern int			g_RSConnectedCount;
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* RSMANAGER_H */
//-------------------------------------------------------------------------------------------------------------------------------------------