//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H
//-------------------------------------------------------------------------------------------------------------------------------------------

class CDSManager : public IOCP
{
public:
	CDSManager(void);
	~CDSManager(void);
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

extern CDSManager		g_DSManager[];
extern int				g_ConnectedCount;
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* SERVERMANAGER_H */
//-------------------------------------------------------------------------------------------------------------------------------------------
