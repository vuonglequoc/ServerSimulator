//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Network Class.
//----------------------------------------------------------------------------------
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "../Header Files/StdAfx.H"
#include "../Header Files/Defines.H"


struct _PER_IO_CONTEXT
{ 
	WSAOVERLAPPED		Overlapped;
	WSABUF				wsabuf; 
	// -----
	UCHAR				Buffer[MAX_IO_BUFFER_SIZE];
	UCHAR				BufferSecond[MAX_IO_BUFFER_SIZE];
	// -----
	int					nSecondOfs;
	int					nTotalBytes;
	int					nSentBytes;
	BYTE				IOOperation;
	BYTE				nWaitIO;
}; 
// ---------------------------------------------------------------------------------------------------------------------------------------------------
struct _PER_SOCKET_CONTEXT
{
	int					nIndex;
	int					dwIOCount;
	// -----
	SOCKET				m_socket;
	_PER_IO_CONTEXT		IOContext[2];
};
// ---------------------------------------------------------------------------------------------------------------------------------------------------
class CNetwork
{
public:
	CNetwork();
	virtual ~CNetwork();
	// -----
	bool				Initialize();
	bool				StartWinSocket();
	bool				OpenSocket();
	bool				CreateListenSocket();
	bool				CreateSrvWorker(int ServerPort);
	bool				UpdateIOCompletionPort(int Socket, int ClientIndex);
	bool				RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int aIndex);
	bool				AcceptClientSocket();
	bool				DataSend(int aIndex, UCHAR * lpMsg, DWORD dwSize);
	bool				IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext);
	bool				IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext);
	void				CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext);
	void				CloseClient(int aIndex);
	void				ResponErrorCloseClient(int index);
	void				CloseNetwork();
	DWAPI				IocpServerWorker(LPVOID Param);
	DWAPI				SrvWorkerThread(LPVOID Param);
	// -----
private:
	WSAData				wsData;
	SYSTEM_INFO			SysInfo;
	CRITICAL_SECTION	CS;
	// -----
	DWORD				m_ThreadID;
	SOCKET				m_Listen;
	HANDLE				CIOCPort;
	HANDLE				GIocpThreadHandle;
	HANDLE				m_hCmport;
	HANDLE				m_hThread[MAX_IO_THREAD_HANDLES];
	// -----
	int					m_StartWSA;
	int					m_BindSocket;
	int					m_ListenSocket;
	int					m_RecvData;
	int					m_SendData;
	int					m_ServerPort;
	int					m_ClientSocket;
	int					m_ClientIndex;
	UINT				m_NumOfThreads;
	UINT				m_ThreadsCount;
	UCHAR *				m_SendBuff;
};
// ---------------------------------------------------------------------------------------------------------------------------------------------------
extern CNetwork gNet;
// ---------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// ---------------------------------------------------------------------------------------------------------------------------------------------------