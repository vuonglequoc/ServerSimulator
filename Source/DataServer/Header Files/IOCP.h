/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [16.08.2011] This Script For IOCP Network Of The Application.
#
#############################################################################*/
#ifndef IOCP_HEADER
#define IOCP_HEADER

#include "StdAfx.H"


class IOCP
{
public:
	IOCP();
	virtual ~IOCP();
	// -----
	void				SetDefault();
	void				StartServer(UINT SrvType);
	void				DestroySocket();
	void				DataSendAll(UCHAR * lpMsg, DWORD dwSize);
	void				CloseClient(int index);
	void				ServerDel(int aIndex);
	// -----
	bool				StartNetwork(UINT NumberOfThreads);
	bool				CreateListenSocket();
	bool				UpdateIOCompletionPort(SOCKET hSock, int ClientIndex);
	bool				DataSend(int aIndex, UCHAR* lpMsg, DWORD dwSize);
	// -----
	int					ServerAddSearch();
	int					ServerAdd(int aIndex, SOCKET Socket, char * Ip);
	int					ServerSerachByIP(char * Ip);
	// -----
	VVOID				OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize){};
	VVOID				OnConnect(){};
	VVOID				OnDisconnect(int aIndex){};
	// -----
	void   inline		SetPort(UINT Port)				{ m_nPort = Port; };
	void   inline		SetName(char * Name)			{ memset(m_Name,0,sizeof(m_Name)); memcpy(m_Name,Name,sizeof(m_Name)); };
	SOCKET inline		GetSocket(int aIndex)			{ return m_Server[aIndex].m_Socket; };
	bool   inline		IsConnected(int aIndex)			{ if(m_Server[aIndex].m_State > SRVS_OFFLINE) return true; else return false; };
	// -----
private:
	OBJECTSTRUCT		m_Server[MAX_OBJECT];
	// -----
	WSAData				wsData;
	HANDLE				m_hThread[MAX_IO_THREAD_HANDLES];
	HANDLE				m_hCmport;
	HANDLE				m_ListThread;
	UINT				m_nThreadCount;
	SOCKET				m_Listen;
	TSyncCSection		TSync;
	// -----
	UINT				m_nPort;
	UINT				m_AddedCount;
	DWORD				m_ThreadID;
	char				m_Name[15];
	int					m_DelaySNet;
	// -----
	bool				RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex);
	bool				IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext);
	bool				IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext);
	// -----
	void				CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result);
	void				ResponErrorCloseClient(int index);
	// -----
	DWAPI				ListenWorker(LPVOID Param);
	DWAPI				RecvWorker(LPVOID Param);
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* IOCP_HEADER */
// -------------------------------------------------------------------------------------------------------------------------------------------------------