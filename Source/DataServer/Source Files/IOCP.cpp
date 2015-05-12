#include "../Header Files/StdAfx.H"
#include "../Header Files/IOCP.H"
#include "../Header Files/Main.H"
#include "../Header Files/LogProc.H"


IOCP::IOCP()
{
	TSync.CSLock();
	// -----
	SetDefault();
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
IOCP::~IOCP()
{
	TSync.CSUnlock();
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void IOCP::SetDefault()
{
	TSync.CSLock();
	// -----
	memset(m_Name, 0, sizeof(m_Name));
	// -----
	m_Name[0]		= 'N';
	m_Name[1]		= 'O';
	m_Name[2]		= 'N';
	m_Name[3]		= 'E';
	// -----
	m_nPort			= 00000;
	m_hCmport		= 0;	
	m_nThreadCount	= 0;
	m_AddedCount	= 0;
	m_DelaySNet		= 100;
	m_Listen		= INVALID_SOCKET;
	// -----
	for(int i = 0; i != MAX_OBJECT; i++)
    {
		m_Server[i].m_Index             = SOCKET_ERROR;
		m_Server[i].m_Socket            = INVALID_SOCKET;
		m_Server[i].m_State             = SRVS_OFFLINE;
		m_Server[i].PerSocketContext	= new _PER_SOCKET_CONTEXT;
    }
	// -----
	TSync.CSUnlock();
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void IOCP::StartServer(UINT SrvType)
{
	switch (SrvType)
	{
		case ST_JOIN_SERVER:
		{
			JSrvManager.SetPort(gMainApp.MSTR.JoinSrv_Port);
			// -----
			JSrvManager.SetName("JoinServer");
			// -----
			JSrvManager.StartNetwork(2);
			// -----
		} break;
		// -----
		case ST_DATA_SERVER:
		{
			char szTemp[1024] = {0};
			// -----
			DServerManager * DS_SrvManager = new DServerManager[gMainApp.MSTR.DataSrv_Count];
			// -----
			for(int i = 0; i != gMainApp.MSTR.DataSrv_Count; i++)
			{
				sprintf(szTemp, "DataServer%d", i+1);
				// -----
				DS_SrvManager[i].SetPort((i * 2) + gMainApp.MSTR.DataSrv_Port);			
				DS_SrvManager[i].SetName(szTemp);
				// -----
				DS_SrvManager[i].StartNetwork(16);
			}
		} break;
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool IOCP::StartNetwork(UINT NumberOfThreads)
{
	bool bReturn		= true;
	// -----
	if ( WSAStartup(MAKEWORD(2,2), &this->wsData) != NULL )
	{
		CLog.Error("Could't Start Up WSA Library.");
		// -----
		bReturn			= false;
	}
	// -----
	Sleep(m_DelaySNet);
	// -----
	if ((m_hCmport = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0)) == NULL )
	{
		CLog.Error("Couldn't Create Completion Port() Failed: [%d].", GetLastError());
		// -----
		bReturn			= false;
	}
	// -----
	Sleep(m_DelaySNet);
	// -----
	if(CreateListenSocket() == false)
	{
		CLog.Error("[IOCP] Couldn't Create Socket() Failed: [%d].", GetLastError());
		// -----
		bReturn = false;
	}
	// -----
	Sleep(m_DelaySNet);
	// -----
	for (int i = 0; i != 4; i++)
	{
		if ( ( m_ListThread = CreateThread(NULL, 0, (LPTHREAD)ListenWorker, (LPVOID)this, 0, &m_ThreadID) ) == NULL )
		{
			CLog.Error("Couldn't Create ListenThread() Failed: [%d].", GetLastError());
			// -----
			bReturn = false;
		}
	}
	// -----
	Sleep(m_DelaySNet);
	// -----
	for (int i = 0 ; i != NumberOfThreads; i++)
    {
        if ( ( m_hThread[i] = CreateThread(NULL, 0, RecvWorker, (LPVOID)this, 0, &m_ThreadID) ) == NULL )
        {
			CLog.Error("Couldn't Create RecvThread() Failed: [%d].", GetLastError());
            // -----
            bReturn = false;
        }
        // -----
		m_nThreadCount++;
	}
	// -----
	Sleep(m_DelaySNet);
	// -----
	CLog.Connect("%s Started! Port [%d].", m_Name, m_nPort);
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool IOCP::CreateListenSocket()
{
	bool bReturn			= true;
	sockaddr_in	ServerAddr	= {0};
	// -----
	if ( (m_Listen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED)) != SOCKET_ERROR )
	{
		ServerAddr.sin_family				= AF_INET;
		ServerAddr.sin_addr.s_addr			= inet_addr("0.0.0.0");
		ServerAddr.sin_port					= htons(m_nPort);
		// -----
		if ( bind(m_Listen , (sockaddr*)&ServerAddr, 16) == SOCKET_ERROR )
		{
			CLog.Error("[IOCP] Bind Error() : DataServer can't be Launched Twice.");
			// -----
			SendMessage(gMainApp.m_hWnd, WM_CLOSE, 0,0);
			// -----
			bReturn = false;
		}
		else
		{
			if(listen(m_Listen, 5) == SOCKET_ERROR)
			{
				CLog.Error("[IOCP] Can't Listen Socket. Port [%d] Failed Error [%d].", m_nPort, WSAGetLastError());
				// -----
				bReturn = false;
			}
		}
	}
	else
	{
		CLog.Error("[IOCP] Couldn't Listen Socket - Failed Error [%d].", WSAGetLastError());
		// -----
		bReturn = false;
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void IOCP::DestroySocket()
{
	DWORD i			= 0;
	DWORD szCPU		= 0;
	// -----
	CloseSocket(m_Listen);
	// -----
	do { TerminateThread(m_hThread[szCPU] , 0); szCPU++; } while (szCPU < m_nThreadCount);
	// -----
	TerminateThread(m_ListThread, 0);
	// -----
	if ( m_hCmport != NULL )
	{
		CloseHandle(m_hCmport);
		// -----
		m_hCmport = NULL;
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool IOCP::UpdateIOCompletionPort(SOCKET hSock, int ClientIndex)
{
	bool bReturn = true;
	// -----
	if(CreateIoCompletionPort((HANDLE) hSock, m_hCmport, ClientIndex, 0) == NULL)
	{
		CLog.Error("[IOCP] Couldn't Update Completion Port() Failed: [%d].", GetLastError());
		// -----
		m_Server[ClientIndex].PerSocketContext->dwIOCount = 0;
		// -----
		bReturn = false;
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool IOCP::DataSend(int aIndex, UCHAR * lpMsg, DWORD dwSize)
{
	ULONG SendBytes			= NULL;
	OBJECTSTRUCT * lpObj	= & m_Server[aIndex];
	LPPERSOCKET				= lpObj->PerSocketContext;
	// -----
	if((aIndex < 0) || (aIndex > MAX_OBJECT))
	{
		CLog.Error("[IOCP] Index(%d) %x %x %x", dwSize, lpMsg[0], lpMsg[1], lpMsg[2]);
		// -----
		return false;
	}
	// -----
	if(lpObj->m_State < SRVS_ONLINE) return false;
	// -----
	if(dwSize > sizeof(lpPerSocketContext->IOContext[0].Buffer))
	{
		CLog.Error("[IOCP] Error Max msg [%d].", dwSize);
		// -----
		CloseClient(aIndex);
		// -----
		return false;
	}
	// -----
	_PER_IO_CONTEXT  * lpIoCtxt = &lpPerSocketContext->IOContext[1];
	// -----
	if(lpIoCtxt->nWaitIO > 0)
	{
		if((lpIoCtxt->nSecondOfs + dwSize) > MAX_IO_BUFFER_SIZE-1)
		{
			CLog.Error("[IOCP] (Index->%d) Max Buffer Over [%d/%d/%d].", aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize);
			// -----
			lpIoCtxt->nWaitIO = 0;
			// -----
			CloseClient(aIndex);
			// -----
			return true;
		}
		// -----
		memcpy(&lpIoCtxt->BufferSecond[lpIoCtxt->nSecondOfs], lpMsg, dwSize);
		// -----
		lpIoCtxt->nSecondOfs += dwSize;
		// -----
		return true;
	}
	// -----
	lpIoCtxt->nTotalBytes = 0;
	// -----
	if(lpIoCtxt->nSecondOfs > 0)
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		// -----
		lpIoCtxt->nTotalBytes	= lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs	= 0;
	}
	// -----
	if((lpIoCtxt->nTotalBytes + dwSize) > (MAX_IO_BUFFER_SIZE-1))
	{
		CLog.Error("[IOCP] (Index->%d) Max Buffer Over [%d/%d/%d].", aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize);
		// -----
		lpIoCtxt->nWaitIO = 0;
		// -----
		CloseClient(aIndex);
		// -----
		return false;
	}
	// -----
	memcpy( &lpIoCtxt->Buffer[lpIoCtxt->nTotalBytes], lpMsg, dwSize);
	// -----
	lpIoCtxt->nTotalBytes	+= dwSize;
	lpIoCtxt->wsabuf.buf	= (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes	= 0;
	lpIoCtxt->IOOperation	= OP_RECV;
	// -----
	if(WSASend(lpObj->m_Socket, & lpIoCtxt->wsabuf , 1, & SendBytes, 0, & lpIoCtxt->Overlapped, NULL) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)	
		{
			lpIoCtxt->nWaitIO = 0;
			// -----
			if(lpIoCtxt->wsabuf.buf[0] == 0xC1 || lpIoCtxt->wsabuf.buf[0] == 0xC2)
			{
				CLog.Error("[IOCP] WSASend[%d][%s] Failed Error: [%d]-[%x][%x]", aIndex, lpObj->m_ServerIp, WSAGetLastError(),
					(BYTE)lpIoCtxt->wsabuf.buf[0], (BYTE)lpIoCtxt->wsabuf.buf[2]);
			}
			// -----
			CloseClient(aIndex);
			// -----
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	// -----	
	lpIoCtxt->nWaitIO = 1;
	// -----
	return true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void IOCP::DataSendAll(UCHAR * lpMsg, DWORD dwSize)
{
	int i = 0;
	// -----
	while ( i != MAX_OBJECT )
	{
		DataSend(i, lpMsg, dwSize);	i++;
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void IOCP::CloseClient(int index)
{
	if (index < 0 || index >= MAX_OBJECT)
	{
		CLog.Error("[IOCP] CloseClient Index Error");
		// -----
		return;
	}
	// -----
	if (m_Server[index].m_State == SRVS_OFFLINE)
	{
		CLog.Error("[IOCP] CloseClient Connect Error");
		// -----
		return;
	}
	// -----
	if (m_Server[index].m_Socket != INVALID_SOCKET)
	{
		CloseSocket(m_Server[index].m_Socket);
		// -----
		m_Server[index].m_Socket = INVALID_SOCKET;
	}
	else
	{
		CLog.Error("[IOCP] CloseClient INVALID_SOCKET");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool IOCP::RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex)
{
	bool bReturn				= true;
	// -----
	UCHAR* recvbuf				= lpIOContext->Buffer;
	int lOfs					= 0;
	int size					= 0;
	BYTE headcode				= 0x00;
	BYTE xcode					= 0x00;
	// -----
	if(lpIOContext->nSentBytes < 3) bReturn = false;
	// -----
	while(true && bReturn)
	{
		if((recvbuf[lOfs+0] == 0xC1) || (recvbuf[lOfs+0] == 0xC3))
		{
			size		= recvbuf[lOfs+1];
			headcode	= recvbuf[lOfs+2];
			xcode		= recvbuf[lOfs+0];
		}
		// -----
		else if((recvbuf[lOfs+0] == 0xC2) || (recvbuf[lOfs+0] == 0xC4))
		{
			size		= recvbuf[lOfs+1] * 256;
			size		|=recvbuf[lOfs+2];
			headcode	= recvbuf[lOfs+3];
			xcode		= recvbuf[lOfs+0];
		}
		// -----
		else
		{
			CLog.Error("[IOCP] Parse Header Packet Failed Size (%d) headcode %x xcode %x", size, headcode, xcode);
			// -----
			bReturn = false;
			// -----
			break;
		}
		// -----
		if(size <= 0)
		{
			CLog.Error("[IOCP] Recv Data Parse Size (%d).", size);
			// -----
			bReturn = false;
			// -----
			break;
		}
		// -----
		if(size <= lpIOContext->nSentBytes)
		{
			OnRecv(uIndex, headcode, &recvbuf[lOfs], size);
			// -----
			lOfs					+= size;
			lpIOContext->nSentBytes	-= size;
			// -----
			if(lpIOContext->nSentBytes <= 0)
			{
				break;
			}
		}
		else if(lOfs > 0)
		{
			if(lpIOContext->nSentBytes < 1)
			{
				CLog.Error("[IOCP] Recv Data Parse [RecvBuffLen < 1].");
				// -----
				break;
			}
			// -----
			if(lpIOContext->nSentBytes < MAX_IO_BUFFER_SIZE) 
			{
				memcpy(recvbuf, &recvbuf[lOfs], lpIOContext->nSentBytes);
				// -----
				CLog.Error("[IOCP] Recv Data Parse Message Copy [%d].", lpIOContext->nSentBytes);
			}
			// -----
			break;
		}
		else
		{
			break;
		}
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool IOCP::IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	OBJECTSTRUCT * lpObj		= & m_Server[lpPerSocketContext->nIndex];
	ULONG SendBytes				= NULL;
	_PER_IO_CONTEXT * lpIoCtxt	= &lpPerSocketContext->IOContext[1];
	// -----
	if(lpIoCtxt->nWaitIO > 0) return false;
	// -----
	lpIoCtxt->nTotalBytes = 0;
	// -----
	if(lpIoCtxt->nSecondOfs > 0)
	{
		// -----
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		// -----
		lpIoCtxt->nTotalBytes	= lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs	= 0;
	}
	else
	{
		return false;
	}
	// -----
	lpIoCtxt->wsabuf.buf	= (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes	= 0;
	lpIoCtxt->IOOperation	= OP_RECV;
	// -----
	if(WSASend(lpObj->m_Socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			CLog.Error("[IOCP] WSASend [%s] Failed: [%d].", lpObj->m_ServerIp, WSAGetLastError());
			// -----
			CloseClient(lpObj->m_Index);
			// -----
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount++;
	}
	// -----
	lpIoCtxt->nWaitIO = 1;
	// -----
	return true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool IOCP::IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	OBJECTSTRUCT * lpObj		= & m_Server[lpPerSocketContext->nIndex];
	ULONG SendBytes				= NULL;
	_PER_IO_CONTEXT * lpIoCtxt	= &lpPerSocketContext->IOContext[1];
	// -----
	if (lpIoCtxt->nWaitIO > 0)
	{
		return false;
	}
	// -----
	if((lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes) < 0) return false;
	// -----
	lpIoCtxt->wsabuf.buf	= (char*)&lpIoCtxt->Buffer[lpIoCtxt->nSentBytes];
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes;
	lpIoCtxt->IOOperation	= OP_RECV;
	// -----
	if (WSASend(lpObj->m_Socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			CLog.Error("[IOCP] WSASend [%s] Failed: [%d].", lpObj->m_ServerIp, WSAGetLastError());
			// -----
			CloseClient(lpObj->m_Index);
			// -----
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount++;
	}
	// -----
	lpIoCtxt->nWaitIO = 1;
	// -----
	return true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void IOCP::CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result)
{
	int index = lpPerSocketContext->nIndex ;
	// -----
	if (index >= 0 && index <= MAX_OBJECT)
	{
		if (m_Server[index].m_Socket != INVALID_SOCKET)
		{
			if (CloseSocket(m_Server[index].m_Socket) == -1)
			{
				if (WSAGetLastError() != WSAENOTSOCK) return;
			}
			// -----
			m_Server[index].m_Socket = INVALID_SOCKET;
		}
		// -----
		ServerDel(index);
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void IOCP::ResponErrorCloseClient(int index)
{
	if((index < 0) || (index >= MAX_OBJECT))
	{
		CLog.Error("[IOCP] CloseClient Index Error");
		// -----
		return;
	}
	// -----
	if (m_Server[index].m_State == SRVS_OFFLINE)
	{
		CLog.Error("[IOCP] CloseClient Connect Error");
		// -----
		return;
	}
	// -----
	CloseSocket(m_Server[index].m_Socket);
	// -----
	m_Server[index].m_Socket = INVALID_SOCKET;
	// -----
	if (m_Server[index].m_Socket == INVALID_SOCKET)
	{
		CLog.Error("[IOCP] CloseClient INVALID_SOCKET");
	}
	// -----
	ServerDel(index);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int IOCP::ServerAddSearch()
{
	int iReturn			= -1;
	// -----
	if(m_Server == NULL) return iReturn;
	// -----
	for(int i = 0 ; i < MAX_OBJECT; i++)
	{
		if(m_Server[i].m_Index == -1 )
		{
			iReturn		= i;
			// -----
			break;
		}
	}
	// -----
	return iReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int IOCP::ServerAdd(int ServerIndex, SOCKET Socket, char * Ip)
{
	memcpy(m_Server[ServerIndex].m_ServerIp, Ip, 16);
	// -----
	m_Server[ServerIndex].m_Index           = ServerIndex;
	m_Server[ServerIndex].m_State           = SRVS_ONLINE;
	m_Server[ServerIndex].m_Socket          = Socket;
	// -----
	m_AddedCount++;
	// -----
	OnConnect();
	// -----
	return ServerIndex;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void IOCP::ServerDel(int aIndex)
{
	m_Server[aIndex].m_Index   = -1;
	m_Server[aIndex].m_Socket  = INVALID_SOCKET;
	m_Server[aIndex].m_State   = SRVS_OFFLINE;
	// -----
	memset(m_Server[aIndex].m_ServerIp, 0, sizeof(m_Server[aIndex].m_ServerIp));
	// -----
	m_AddedCount--;
	// -----
	OnDisconnect(aIndex);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int IOCP::ServerSerachByIP(char * Ip)
{
	int iReturn = SOCKET_ERROR;
	// -----
	for(int i = 0 ; i != MAX_OBJECT ; i++)
	{
		OBJECTSTRUCT * lpObj = &m_Server[i];
		// -----
		if(lpObj->m_State = SRVS_OFFLINE)
		{
			continue;
		}
		else if(strcmp(Ip, lpObj->m_ServerIp) == 0)
		{
			iReturn = i;
			// -----
			break;
		}
	}
	// -----
	return iReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD IOCP::ListenWorker(LPVOID Param)
{
	IOCP *	lpThis		= (IOCP *)Param;
	// -----
	int	ClientIndex		= SOCKET_ERROR;
	SOCKET AcceptSocket	= INVALID_SOCKET;
	// -----
	sockaddr_in cAddr	= {0};
	in_addr	cInAddr		= {0};
	int	cAddrlen		= 16;
	// -----
	int	RecvBytes		= 0;
	ULONG Flags			= 0;		
	// -----
	__try
	{
		while(true)
		{
			if(lpThis->m_AddedCount >= MAX_OBJECT)
			{
				Sleep(1);
				// -----
				continue;
			}
			// -----
			AcceptSocket = WSAAccept(lpThis->m_Listen, (sockaddr*)&cAddr, &cAddrlen, NULL, 0);
			// -----
			if(AcceptSocket == SOCKET_ERROR)
			{
				CLog.Error("[IOCP] WSAAccept() Failed: [%d].", WSAGetLastError());
				// -----
				continue;
			}
			// ----
			memcpy(&cInAddr, &cAddr.sin_addr  , sizeof(cInAddr));
			// -----
			ClientIndex = lpThis->ServerAddSearch();
			// -----
			if(ClientIndex == SOCKET_ERROR)
			{
				CLog.Error("[IOCP] ClientIndex = SOCKET_ERROR (MAX).");
				// -----
				CloseSocket(AcceptSocket);
				// -----
				continue;
			}
			// -----
			if(lpThis->UpdateIOCompletionPort(AcceptSocket, ClientIndex) == false)
			{
				CLog.Error("[IOCP] CreateIoCompletionPort Failed: [%d]. Socket [%d] Index [%d].", GetLastError(), AcceptSocket, ClientIndex);
				// -----
				CloseSocket(AcceptSocket);
				// -----
				continue;
			}
			// -----
			if(lpThis->ServerAdd(ClientIndex, AcceptSocket , inet_ntoa(cInAddr)) == SOCKET_ERROR)
			{
				CLog.Error("[IOCP] CreateIoCompletionPort Failed: [%d]. Socket [%d] Index [%d].", GetLastError(), AcceptSocket, ClientIndex);
				// -----
				CloseSocket(AcceptSocket);
				// -----
				continue;
			}
			// -----
			_PER_IO_CONTEXT * IOContext0 = & lpThis->m_Server[ClientIndex].PerSocketContext->IOContext[0];
			_PER_IO_CONTEXT * IOContext1 = & lpThis->m_Server[ClientIndex].PerSocketContext->IOContext[1];
			// -----
			memset(& IOContext0->Overlapped, 0, sizeof(WSAOVERLAPPED));
			memset(& IOContext1->Overlapped, 0, sizeof(WSAOVERLAPPED));
			// -----
			IOContext0->wsabuf.buf		= (char*)&IOContext0->Buffer;
			IOContext0->wsabuf.len		= MAX_IO_BUFFER_SIZE;
			IOContext0->nTotalBytes		= 0;
			IOContext0->nSentBytes		= 0;
			IOContext0->nWaitIO			= 0;
			IOContext0->nSecondOfs		= 0;
			IOContext0->IOOperation		= 0;
			// -----
			IOContext1->wsabuf.buf		= (char*)IOContext0->Buffer;
			IOContext1->wsabuf.len		= MAX_IO_BUFFER_SIZE;
			IOContext1->nTotalBytes		= 0;
			IOContext1->nSentBytes		= 0;
			IOContext1->nWaitIO			= 0;
			IOContext1->nSecondOfs		= 0;
			IOContext1->IOOperation		= 1;
			// -----
			lpThis->m_Server[ClientIndex].PerSocketContext->m_socket	= AcceptSocket;
			lpThis->m_Server[ClientIndex].PerSocketContext->nIndex		= ClientIndex;
			// -----
			if(WSARecv(AcceptSocket, & IOContext0->wsabuf , 1, (ULONG*)& RecvBytes,& Flags,
				& IOContext0->Overlapped, NULL) == SOCKET_ERROR)
			{
				if(WSAGetLastError() != WSA_IO_PENDING)
				{
					CLog.Error("[IOCP] WSARecv() Failed: [%d].", WSAGetLastError());
					// -----
					lpThis->m_Server[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
					// -----
					lpThis->CloseClient(lpThis->m_Server[ClientIndex].PerSocketContext, 0);
					// -----
					continue;
				}
			}
			// -----
			lpThis->m_Server[ClientIndex].PerSocketContext->IOContext[0].nWaitIO  = 1;
			lpThis->m_Server[ClientIndex].PerSocketContext->dwIOCount++;
		}
	}
	// -----
	__finally
	{
		CLog.Error("[IOCP] ListenWorker() Failed: [%d]", GetLastError());
	}
	// -----
	return TRUE;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD IOCP::RecvWorker(LPVOID Param)
{
	IOCP *	lpThis							= (IOCP *)Param;
	BOOL bSuccess							= FALSE;
	DWORD ClientIndex						= SOCKET_ERROR;
	DWORD dwIoSize							= 0;
	DWORD dwWSARet							= 0;
	// -----
	LPOVERLAPPED lpOverlapped				= {0};
	_PER_SOCKET_CONTEXT *lpPerSocketContext = {0};
	_PER_IO_CONTEXT * lpIOContext			= {0};
	// -----
	ULONG RecvBytes							= 0;
	ULONG Flags								= 0;
	// ----
	while(true)
	{
		bSuccess = GetQueuedCompletionStatus( lpThis->m_hCmport, & dwIoSize, & ClientIndex, & lpOverlapped, -1);
		// -----
		if(bSuccess == 0)
		{
			if(lpOverlapped != 0)
			{
				DWORD aError = GetLastError();
				// -----
				if((aError		!= ERROR_SEM_TIMEOUT) 
					&& (aError	!= ERROR_NETNAME_DELETED) 
					&& (aError	!= ERROR_CONNECTION_ABORTED)
					&& (aError	!= ERROR_OPERATION_ABORTED))
				{
					CLog.Error("[IOCP] GetQueueCompletionStatus() Failed: [%d].", GetLastError());
					// -----
					return FALSE;
				}
			}
		}
		// -----
		lpPerSocketContext = lpThis->m_Server[ClientIndex].PerSocketContext;
		// -----
		lpPerSocketContext->dwIOCount--;
		// -----
		if(dwIoSize == NULL)
		{
			lpThis->CloseClient(lpPerSocketContext, 0);
			// -----
			continue;
		}
		// -----
		lpIOContext = (_PER_IO_CONTEXT *)lpOverlapped;
		// ----
		if(lpIOContext == 0)
		{
			continue;
		}
		// -----
		if(lpIOContext->IOOperation == OP_RECV)
		{
			lpIOContext->nSentBytes += dwIoSize;
			// -----
			if(lpIOContext->nSentBytes >= lpIOContext->nTotalBytes)
			{
				lpIOContext->nWaitIO = 0;
				// -----
				if( lpIOContext->nSecondOfs > 0)
				{
					lpThis->IoSendSecond(lpPerSocketContext);
				}
			}
			else
			{
				lpThis->IoMoreSend(lpPerSocketContext);
			}
		}
		// -----
		else if(lpIOContext->IOOperation == OP_SEND)
		{
			RecvBytes = 0;
			// -----
			lpIOContext->nSentBytes += dwIoSize;
			// -----
			if(lpThis->RecvDataParse(lpIOContext, lpPerSocketContext->nIndex ) == false)
			{
				CLog.Error("[IOCP] [%d] Socket Header, Failed: [%d].", lpPerSocketContext->nIndex, WSAGetLastError());
				// -----
				continue;
			};
			// -----
			lpIOContext->nWaitIO	= 0;
			Flags					= 0;
			// -----
			memset(&lpIOContext->Overlapped, 0, sizeof (WSAOVERLAPPED));
			// -----
			lpIOContext->wsabuf.len		= MAX_IO_BUFFER_SIZE - lpIOContext->nSentBytes;
			lpIOContext->wsabuf.buf		= (char*)&lpIOContext->Buffer[lpIOContext->nSentBytes];
			lpIOContext->IOOperation	= OP_SEND;
			// -----
			dwWSARet = WSARecv(lpPerSocketContext->m_socket, &lpIOContext->wsabuf, 1, &RecvBytes, &Flags, &lpIOContext->Overlapped, NULL);
			// -----
			if(dwWSARet == SOCKET_ERROR)
			{
				if( WSAGetLastError() != WSA_IO_PENDING)
				{
					CLog.Error("[IOCP] WSARecv() Failed: [%d].", WSAGetLastError());
					// -----
					lpThis->CloseClient(lpPerSocketContext, 0);
					// -----
					continue;
				}
			}
			// -----
			lpPerSocketContext->dwIOCount++;
			lpIOContext->nWaitIO = 1;
		}
	}
	// -----
	return TRUE;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------