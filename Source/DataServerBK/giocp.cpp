// Giocp.cpp
//------------------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
//------------------------------------------

// GS-N 0.99.60T 0x00473020 - Status : Completed :)
//	GS-N	1.00.18	JPN	0x00489FD0	-	Completed
#include "stdafx.h"
#include "giocp.h"
#include "logproc.h"
#include "eDataServer.h"
#include "ServerEngine.h"
#include "CSprotocol.h"


DWORD g_dwServerCount=0;
HANDLE g_CompletionPort[MAX_SERVER_TYPE];
DWORD g_dwThreadCount[MAX_SERVER_TYPE];
CRITICAL_SECTION criti[MAX_SERVER_TYPE];
HANDLE g_ThreadHandles[MAX_SERVER_TYPE][MAX_IO_THREAD_HANDLES];
int g_ServerPort[MAX_SERVER_TYPE];
HANDLE g_IocpThreadHandle[MAX_SERVER_TYPE];
SOCKET g_Listen[MAX_SERVER_TYPE] = {INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET};

BOOL IocpServerStart()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	//LogAddC(2, "Initializing...");

	/*if ( CreateGIocp(g_FSGateServerListPort, ST_FSGATE) == TRUE )
		LogAddTD("[IOCP] ST_FSGATE Start! PORT [ %d ]", g_FSGateServerListPort); */
	if (g_UseJoinServer == TRUE)
	{
		if ( CreateGIocp(g_JoinServerListPort, ST_JOINSERVER) == TRUE )
			LogAddTD("[IOCP] ST_JOINSERVER Start! PORT [ %d ]", g_JoinServerListPort);

		Sleep(100);
	}
	/*if ( CreateGIocp(g_ConnectServerListPort, ST_CONNECTSERVER) == TRUE	)	// Need some adjust
		LogAddTD("[IOCP] ST_CONNECTSERVER Start! PORT [ %d ]", g_ConnectServerListPort);*/

	if (g_UseDataServer1 == TRUE)
	{
		if ( CreateGIocp(g_DataServer1ListPort, ST_DATASERVER1) == TRUE )
		LogAddTD("[IOCP] ST_DATASERVER1 Start! PORT [ %d ]", g_DataServer1ListPort);

		Sleep(100);
	}
	if (g_UseDataServer2 == TRUE)
	{
		if ( CreateGIocp(g_DataServer2ListPort, ST_DATASERVER2) == TRUE )
		LogAddTD("[IOCP] ST_DATASERVER2 Start! PORT [ %d ]", g_DataServer2ListPort);

		Sleep(100);
	}
	if (g_UseExDataServer == TRUE)
	{
		if ( CreateGIocp(g_ExDataServerListPort, ST_EXDATASERVER) == TRUE )
		LogAddTD("[IOCP] ST_EXDATASERVER Start! PORT [ %d ]", g_ExDataServerListPort);

		Sleep(100);
	}
	/*if ( CreateGIocp(g_ChatServerListPort, ST_CHATSERVER) == TRUE )	// Don't Connect Directly
		LogAddTD("[IOCP] ST_CHATSERVER Start! PORT [ %d ]", g_ChatServerListPort);*/

	if (g_UseExDataServer == TRUE)
	{
		if ( CreateGIocp(g_EventServerListPort, ST_EVENTSERVER) == TRUE )
			LogAddTD("[IOCP] ST_EVENTSERVER Start! PORT [ %d ]", g_EventServerListPort);

		Sleep(100);
	}
	if (g_UseRankingServer == TRUE)
	{
		if ( CreateGIocp(g_RankingServerListPort, ST_RANKINGSERVER) == TRUE )
			LogAddTD("[IOCP] ST_RANKINGSERVER Start! PORT [ %d ]", g_RankingServerListPort);

		Sleep(100);
	}
	if (g_UseCashShopServer == TRUE)
	{
		if ( CreateGIocp(g_CashShopServerListPort, ST_CASHSHOPSERVER) == TRUE )
			LogAddTD("[IOCP] ST_CASHSHOPSERVER Start! PORT [ %d ]", g_CashShopServerListPort);

		Sleep(100);
	}
	return TRUE;
}



BOOL CreateGIocp(int server_port, eSERVER_TYPE eServerType)
{
	//IocpServerParameter pIocpServerParameter = {g_dwServerCount, eServerType};
	DWORD dwPar = 0;
	dwPar = g_dwServerCount & 0xFFFF;
	dwPar |= (eServerType & 0xFFFF ) << 16;

	DWORD ThreadID;
	g_ServerPort[g_dwServerCount] = server_port;
	g_IocpThreadHandle[g_dwServerCount] = CreateThread( NULL, 0,
											(LPTHREAD_START_ROUTINE)IocpServerWorker,
											(LPVOID)dwPar, 0, &ThreadID);

	if ( g_IocpThreadHandle[g_dwServerCount] == NULL )
	{
		LogAdd("[IOCP] CreateThread() failed with error %d - ServerCount : %d", GetLastError(), g_dwServerCount);
		return FALSE;
	}
	
	g_dwServerCount++;

	if ( g_dwServerCount >= MAX_SERVER_TYPE )
	{
		MessageBox(NULL, "[IOCP] MAX_SERVER_TYPE reached!", "Error", MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}


void DestroyGIocp()
{
	for(DWORD I=0;I<g_dwServerCount;I++)
	{
		closesocket(g_Listen[g_dwServerCount]);

		for (DWORD dwCPU=0; dwCPU < g_dwThreadCount[g_dwServerCount];dwCPU++ )
			TerminateThread(g_ThreadHandles[g_dwServerCount][dwCPU] , 0);

		TerminateThread(g_IocpThreadHandle[g_dwServerCount], 0);

		if ( g_CompletionPort[g_dwServerCount] != NULL )
		{
			CloseHandle(g_CompletionPort[g_dwServerCount]);
			g_CompletionPort[g_dwServerCount]=NULL;
		} 
	}
}


BOOL CreateListenSocket(DWORD dwServerCountIndex)
{
	sockaddr_in InternetAddr;
	int nRet;

	g_Listen[dwServerCountIndex] = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if ( g_Listen[dwServerCountIndex] == INVALID_SOCKET )
	{
		LogAdd("WSASocket() failed with error %d", WSAGetLastError() );
		return FALSE;
	}

	InternetAddr.sin_family=AF_INET;
	InternetAddr.sin_addr.S_un.S_addr=htonl(0);
	InternetAddr.sin_port=htons(g_ServerPort[dwServerCountIndex]);
	nRet=bind(g_Listen[dwServerCountIndex], (sockaddr*)&InternetAddr, 16);
	
	if ( nRet == -1 )
	{
		MsgBox("bind error : eDataServer can't be launched twice");
		SendMessage(ghWnd, WM_CLOSE, 0,0);	// Kill aplication
		return FALSE;
	}

	nRet=listen(g_Listen[dwServerCountIndex], 5);
	if (nRet == -1)
	{
		LogAdd("listen() failed with error %d", WSAGetLastError());
		return FALSE;
	}

	return TRUE;
}


unsigned long __stdcall IocpServerWorker(DWORD pIocpServerParameter)
{
	SYSTEM_INFO SystemInfo;
	DWORD ThreadID;
	SOCKET Accept;
	int nRet;
	int ClientIndex;
	sockaddr_in cAddr;
	in_addr cInAddr;
	int cAddrlen;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	int RecvBytes;
	unsigned long Flags;
	
	cAddrlen=16;
	lpPerSocketContext=0;
	Flags=0;
	DWORD dwServerCountIndex = pIocpServerParameter & 0xFFFF;

	InitializeCriticalSection(&criti[(pIocpServerParameter >> 16)]);
	GetSystemInfo(&SystemInfo);

	g_dwThreadCount[dwServerCountIndex] = SystemInfo.dwNumberOfProcessors * 2;
	__try
	{

		g_CompletionPort[dwServerCountIndex] = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

		if ( g_CompletionPort[dwServerCountIndex] == NULL )
		{
			LogAdd("CreateIoCompletionPort failed with error: %d", GetLastError());
			__leave;
		}

		for ( DWORD n = 0; n<g_dwThreadCount[dwServerCountIndex]; n++ )
		{
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerWorkerThread, (LPVOID)g_CompletionPort[dwServerCountIndex], 0, &ThreadID);

			if ( hThread == NULL )
			{
				LogAdd("CreateThread() failed with error %d", GetLastError() );
				__leave;
			}

			g_ThreadHandles[dwServerCountIndex][n] = hThread;
			CloseHandle(hThread);
		}

		if ( CreateListenSocket(dwServerCountIndex) == FALSE )
		{

		}
		else
		{
			while ( true )
			{
				Accept = WSAAccept(g_Listen[dwServerCountIndex], (sockaddr*)&cAddr, &cAddrlen, NULL, 0 );

				if ( Accept == -1 )
				{
					EnterCriticalSection(&criti[(pIocpServerParameter >> 16)]);
					LogAdd("WSAAccept() failed with error %d", WSAGetLastError() );
					LeaveCriticalSection(&criti[(pIocpServerParameter >> 16)]);
					continue;
				}

				EnterCriticalSection(&criti[(pIocpServerParameter >> 16)]);
				memcpy(&cInAddr, &cAddr.sin_addr  , sizeof(cInAddr) );

				ClientIndex = gObjServerAddSearch();

				if ( ClientIndex == -1 )
				{
					LogAddL("error-L2 : ServerIndex = -1");
					closesocket(Accept);
					LeaveCriticalSection(&criti[(pIocpServerParameter >> 16)]);
					continue;
				}

				if (UpdateCompletionPort(Accept, ClientIndex, dwServerCountIndex) == 0 )
				{
					LogAddL("error-L1 : %d %d CreateIoCompletionPort failed with error %d", Accept, ClientIndex, GetLastError() );
					closesocket(Accept);
					LeaveCriticalSection(&criti[(pIocpServerParameter >> 16)]);
					continue;
				}

				if (gObjServerAdd(Accept, inet_ntoa(cInAddr), ClientIndex, eSERVER_TYPE(pIocpServerParameter >> 16 ) ) == -1 )
				{
					LogAddL("error-L1 : %d %d gObjAdd() failed with error %d", Accept, ClientIndex, GetLastError() );
					LeaveCriticalSection(&criti[(pIocpServerParameter >> 16)]);
					closesocket(Accept);
					continue;
				}
				
				memset(&g_Server[ClientIndex].PerSocketContext->IOContext[0].Overlapped, 0, sizeof(WSAOVERLAPPED));
				memset(&g_Server[ClientIndex].PerSocketContext->IOContext[1].Overlapped, 0, sizeof(WSAOVERLAPPED));

				g_Server[ClientIndex].PerSocketContext->IOContext[0].wsabuf.buf = (char*)&g_Server[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].wsabuf.len = MAX_IO_BUFFER_SIZE;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].nTotalBytes = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].nSentBytes = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].nSecondOfs = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].IOOperation = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].wsabuf.buf = (char*)g_Server[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].wsabuf.len = MAX_IO_BUFFER_SIZE;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].nTotalBytes = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].nSentBytes = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].nWaitIO = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].nSecondOfs = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].IOOperation = 1;
				g_Server[ClientIndex].PerSocketContext->m_socket = Accept;
				g_Server[ClientIndex].PerSocketContext->nIndex = ClientIndex;

				nRet = WSARecv(Accept, &g_Server[ClientIndex].PerSocketContext->IOContext[0].wsabuf , 1, (unsigned long*)&RecvBytes, &Flags, 
						&g_Server[ClientIndex].PerSocketContext->IOContext[0].Overlapped, NULL);

				if ( nRet == -1 )
				{
					if ( WSAGetLastError() != WSA_IO_PENDING )
					{
						LogAddL("error-L1 : WSARecv() failed with error %d", WSAGetLastError() );
						g_Server[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
						CloseClient(g_Server[ClientIndex].PerSocketContext, 0);
						LeaveCriticalSection(&criti[(pIocpServerParameter >> 16)]);
						continue;
					}
				}

				g_Server[ClientIndex].PerSocketContext->IOContext[0].nWaitIO  = 1;
				g_Server[ClientIndex].PerSocketContext->dwIOCount++;

				LeaveCriticalSection(&criti[(pIocpServerParameter >> 16)]);
				//SCPJoinResultSend(ClientIndex, 1);
			}


		}
	}
	__finally
	{
		
		if ( g_CompletionPort[dwServerCountIndex] != NULL )
		{
			for ( DWORD i = 0 ; i < g_dwThreadCount[dwServerCountIndex] ; i++ )
			{
				PostQueuedCompletionStatus( g_CompletionPort[dwServerCountIndex] , 0, 0, 0);
			}
		}

		if ( g_CompletionPort[dwServerCountIndex] != NULL )
		{
			CloseHandle(g_CompletionPort[dwServerCountIndex]);
			g_CompletionPort[dwServerCountIndex] = NULL;
		}
		if ( g_Listen[dwServerCountIndex] != INVALID_SOCKET )
		{
			closesocket( g_Listen[dwServerCountIndex]);
			g_Listen[dwServerCountIndex] = INVALID_SOCKET;
		}
	}
	return TRUE;
}


unsigned long __stdcall ServerWorkerThread(HANDLE CompletionPortID)
{
	HANDLE CompletionPort;
	DWORD dwIoSize;
	unsigned long RecvBytes;
	unsigned long Flags;
	DWORD dwSendNumBytes;
	BOOL bSuccess;
	int nRet;
	DWORD ClientIndex;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	LPOVERLAPPED lpOverlapped;
	_PER_IO_CONTEXT * lpIOContext;
	
	CompletionPort=CompletionPortID;
	dwSendNumBytes=0;
	bSuccess=0;
	lpPerSocketContext=0;
	lpOverlapped=0;
	lpIOContext=0;
	
	while ( true )
	{
		bSuccess=GetQueuedCompletionStatus( CompletionPort, &dwIoSize, &ClientIndex, &lpOverlapped, -1); // WAIT_FOREVER

		if (bSuccess == FALSE)
		{
			if (lpOverlapped != NULL)
			{
				DWORD aError = GetLastError();
				if ( (aError != ERROR_NETNAME_DELETED) && (aError != ERROR_CONNECTION_ABORTED) && (aError != ERROR_OPERATION_ABORTED) )
				{
					//EnterCriticalSection(&criti[dwServerCountIndex]);
					LogAdd("Error Thread : GetQueueCompletionStatus( %d )", GetLastError());
					//LeaveCriticalSection(&criti[dwServerCountIndex]);
					return 0;
				}
			}
		}

		//EnterCriticalSection(&criti[dwServerCountIndex]);
		EnterCriticalSection(&criti[g_Server[ClientIndex].m_Type]);

		lpPerSocketContext=g_Server[ClientIndex].PerSocketContext;
		lpPerSocketContext->dwIOCount --;
				
		if ( dwIoSize == 0 )
		{
			LogAdd("Connection Closed, dwIoSize == 0 (Index:%d)", lpPerSocketContext->nIndex);
			CloseClient(lpPerSocketContext, 0);
			//LeaveCriticalSection(&criti[dwServerCountIndex]);
			LeaveCriticalSection(&criti[g_Server[ClientIndex].m_Type]);
			continue;
		}

		lpIOContext = (_PER_IO_CONTEXT *)lpOverlapped;

		if ( lpIOContext == 0 )
		{
			continue;
		}

		if ( lpIOContext->IOOperation == 1 )
		{
			lpIOContext->nSentBytes += dwIoSize;


			if ( lpIOContext->nSentBytes >= lpIOContext->nTotalBytes )
			{
				lpIOContext->nWaitIO = 0;
						
				if ( lpIOContext->nSecondOfs > 0)
				{
					IoSendSecond(lpPerSocketContext);
				}
			}
			else
			{
				IoMoreSend(lpPerSocketContext);
			}
			
		}
		else if ( lpIOContext->IOOperation == 0 )
		{
			RecvBytes = 0;
			lpIOContext->nSentBytes += dwIoSize;

			if ( RecvDataParse(lpIOContext, lpPerSocketContext->nIndex ) == 0 )
			{
				LogAdd("error-L1 : Socket Header error %d, %d", WSAGetLastError(), lpPerSocketContext->nIndex);
				CloseClient(lpPerSocketContext, 0);
				//LeaveCriticalSection(&criti[dwServerCountIndex]);
				LeaveCriticalSection(&criti[g_Server[ClientIndex].m_Type]);
				continue;
			}

			lpIOContext->nWaitIO = 0;
			Flags = 0;
			memset(&lpIOContext->Overlapped, 0, sizeof (WSAOVERLAPPED));
			lpIOContext->wsabuf.len = MAX_IO_BUFFER_SIZE - lpIOContext->nSentBytes;
			lpIOContext->wsabuf.buf = (char*)&lpIOContext->Buffer[lpIOContext->nSentBytes];
			lpIOContext->IOOperation = 0;

			nRet = WSARecv(lpPerSocketContext->m_socket, &lpIOContext->wsabuf, 1, &RecvBytes, &Flags,
						&lpIOContext->Overlapped, NULL);

			if ( nRet == -1 )
			{
				if ( WSAGetLastError() != WSA_IO_PENDING)
				{
					LogAdd("WSARecv() failed with error %d", WSAGetLastError() );
					CloseClient(lpPerSocketContext, 0);
					//LeaveCriticalSection(&criti[dwServerCountIndex]);
					LeaveCriticalSection(&criti[g_Server[ClientIndex].m_Type]);
					continue;
				}
			}

			lpPerSocketContext->dwIOCount ++;
			lpIOContext->nWaitIO = 1;
		}
		//LeaveCriticalSection(&criti[dwServerCountIndex]);
		LeaveCriticalSection(&criti[g_Server[ClientIndex].m_Type]);
		

	}


	return 1;
}



BOOL RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex)	
{
	unsigned char* recvbuf;
	int lOfs;
	int size;
	BYTE headcode;
	BYTE xcode;

	// Check If Recv Data has More thatn 3 BYTES
	if ( lpIOContext->nSentBytes < 3 )
	{
		return TRUE;
	}

	// Initialize Variables
	lOfs=0;
	size=0;
	xcode=0;
	recvbuf = lpIOContext->Buffer;

	// Start Loop
	while ( true )
	{
		// Select packets with
		// C1 or C2 as HEader
		if ( recvbuf[lOfs] == 0xC1 ||
			 recvbuf[lOfs] == 0xC3 )
		{
			size = recvbuf[lOfs+1];
			headcode = recvbuf[lOfs+2];
			xcode = recvbuf[lOfs];
		}
		else if ( recvbuf[lOfs] == 0xC2 ||
			      recvbuf[lOfs] == 0xC4 )
		{
			size = recvbuf[lOfs+1] * 256;
			size |= recvbuf[lOfs+2];
			headcode = recvbuf[lOfs+3];
			xcode = recvbuf[lOfs];
		}
		// If HEader Differs - Second Generation Protocols
		else
		{
			protocol::MSG_HEADER * pHeader = (protocol::MSG_HEADER *)&recvbuf[lOfs];
			size = pHeader->size;
		}

		// Check Size is leess thant 0
		if ( size <= 0 )
		{
			LogAdd("error-L1 : size %d",size);
			return false;
		}

		// Check if Size is On Range
		if ( size <= lpIOContext->nSentBytes )
		{
			if ( g_Server[uIndex].m_Type == ST_CASHSHOPSERVER )
			{
				protocol::MSG_HEADER * pHeader = (protocol::MSG_HEADER *)&recvbuf[lOfs];
				g_Server[uIndex].ProtocolCore2(uIndex, pHeader->id, &recvbuf[lOfs], size);
			}
			else	// fIRST gENERATION pROTOCOLS
			{
				g_Server[uIndex].ProtocolCore1(uIndex, headcode, &recvbuf[lOfs], size);
			}

			lOfs += size;
			lpIOContext->nSentBytes  -= size;

			if ( lpIOContext->nSentBytes <= 0 )
			{
				break;
			}
		}
		else if ( lOfs > 0 )
		{
			if ( lpIOContext->nSentBytes < 1 )
			{
				LogAdd("error-L1 : recvbuflen 1 %s %d", __FILE__, __LINE__);
				break;
			}

			if ( lpIOContext->nSentBytes < MAX_IO_BUFFER_SIZE ) 
			{
				memcpy(recvbuf, &recvbuf[lOfs], lpIOContext->nSentBytes);
				LogAdd("Message copy %d", lpIOContext->nSentBytes);
			}
			break;
		
		}
		else
		{
			break;
		}
		
	}

	return true;
}











BOOL DataSend(int aIndex, unsigned char* lpMsg, DWORD dwSize)
{
	unsigned long SendBytes;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	unsigned char * SendBuf;
	
	//EnterCriticalSection(&criti);
	EnterCriticalSection(&criti[g_Server[aIndex].m_Type]);

	if ( aIndex < 0 || aIndex > (g_dwMaxServerGroups-1) )
	{
		LogAdd("error-L2 : Index(%d) %x %x %x ", dwSize, lpMsg[0], lpMsg[1], lpMsg[2]);
		//LeaveCriticalSection(&criti);
		LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
		return false;
	}

	SendBuf = lpMsg;

	if ( g_Server[aIndex].m_State < SS_CONNECTED )
	{
		//LeaveCriticalSection(&criti);
		LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
		return FALSE;
	}

	lpPerSocketContext= g_Server[aIndex].PerSocketContext;

	if ( dwSize > sizeof(lpPerSocketContext->IOContext[0].Buffer))
	{
		LogAdd("Error : Max msg(%d) %s %d", dwSize, __FILE__, __LINE__);
		CloseClient(aIndex);
		//LeaveCriticalSection(&criti);
		LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
		return false;
	}

	_PER_IO_CONTEXT  * lpIoCtxt;

	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		if ( ( lpIoCtxt->nSecondOfs + dwSize ) > MAX_IO_BUFFER_SIZE-1 )
		{
			LogAdd("(%d)error-L2 MAX BUFFER OVER %d %d %d",
				aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize);

			lpIoCtxt->nWaitIO = 0;
			CloseClient(aIndex);
			//LeaveCriticalSection(&criti);
			LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
			return true;
		}

		memcpy( &lpIoCtxt->BufferSecond[lpIoCtxt->nSecondOfs], SendBuf, dwSize);
		lpIoCtxt->nSecondOfs += dwSize;
		//LeaveCriticalSection(&criti);
		LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
		return true;
	}

	lpIoCtxt->nTotalBytes = 0;
	
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs = 0;
	}

	if ( (lpIoCtxt->nTotalBytes+dwSize) > MAX_IO_BUFFER_SIZE-1 )
	{
		LogAdd("(%d)error-L2 MAX BUFFER OVER %d %d %d",
			aIndex, lpIoCtxt->nTotalBytes, dwSize);

		lpIoCtxt->nWaitIO = 0;
		CloseClient(aIndex);
		//LeaveCriticalSection(&criti);
		LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
		return FALSE;
	}

	memcpy( &lpIoCtxt->Buffer[lpIoCtxt->nTotalBytes], SendBuf, dwSize);
	lpIoCtxt->nTotalBytes += dwSize;
	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = 1;
	

	if ( WSASend( g_Server[aIndex].m_Socket, &lpIoCtxt->wsabuf , 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{

		if ( WSAGetLastError() != WSA_IO_PENDING )	
		{
			lpIoCtxt->nWaitIO = 0;
			

			if ( lpIoCtxt->wsabuf.buf[0] == 0xC1 )
			{
				LogAdd("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), g_Server[aIndex].m_ServerIp);
			}
			else if ( lpIoCtxt->wsabuf.buf[0] == 0xC2 )
			{
				LogAdd("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[3], WSAGetLastError(), g_Server[aIndex].m_ServerIp);
			}
			CloseClient(aIndex);
			//LeaveCriticalSection(&criti);
			LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	//LeaveCriticalSection(&criti);
	LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
	return true;
}





BOOL IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	unsigned long SendBytes;
	int aIndex;
	_PER_IO_CONTEXT * lpIoCtxt;

	//EnterCriticalSection(&criti);
	aIndex = lpPerSocketContext->nIndex;
	EnterCriticalSection(&criti[g_Server[aIndex].m_Type]);


	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		//LeaveCriticalSection(&criti);
		LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
		return false;
	}

	lpIoCtxt->nTotalBytes = 0;
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs = 0;
	}
	else
	{
		//LeaveCriticalSection(&criti);
		LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = 1;

	if ( WSASend(g_Server[aIndex].m_Socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			LogAdd("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), g_Server[aIndex].m_ServerIp);
			CloseClient(aIndex);
			//LeaveCriticalSection(&criti);
			LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	lpIoCtxt->nWaitIO = 1;
	//LeaveCriticalSection(&criti);
	LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
	
	return true;
}


BOOL IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	unsigned long SendBytes;
	int aIndex;
	_PER_IO_CONTEXT * lpIoCtxt;

	//EnterCriticalSection(&criti);

	aIndex = lpPerSocketContext->nIndex;
	EnterCriticalSection(&criti[g_Server[aIndex].m_Type]);
	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( (lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes) < 0 )
	{
		//LeaveCriticalSection(&criti);
		LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer[lpIoCtxt->nSentBytes];
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes;
	lpIoCtxt->IOOperation = 1;

	if ( WSASend(g_Server[aIndex].m_Socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			LogAdd("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), g_Server[aIndex].m_ServerIp);
			CloseClient(aIndex);
			//LeaveCriticalSection(&criti);
			LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	//LeaveCriticalSection(&criti);
	LeaveCriticalSection(&criti[g_Server[aIndex].m_Type]);
	return true;
}


BOOL UpdateCompletionPort(SOCKET sd, int ClientIndex, DWORD dwServerTypeCount)
{
	_PER_SOCKET_CONTEXT * lpPerSocketContext = NULL;
	HANDLE cp = CreateIoCompletionPort((HANDLE) sd, g_CompletionPort[dwServerTypeCount], ClientIndex, 0);

	if ( cp == NULL )
	{
		LogAdd("CreateIoCompletionPort: %d", GetLastError() );
		return FALSE;
	}

	g_Server[ClientIndex].PerSocketContext->dwIOCount = 0;
	return TRUE;
}


void CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result)
{
	int index = -1;
	index = lpPerSocketContext->nIndex ;

	if ( index >= 0 && index <= g_dwMaxServerGroups )
	{
		if ( g_Server[index].m_Socket != INVALID_SOCKET )
		{
			if (closesocket(g_Server[index].m_Socket) == -1 )
			{
				if ( WSAGetLastError() != WSAENOTSOCK )
				{
					return;
				}
			}

			g_Server[index].m_Socket = INVALID_SOCKET;
		}

		gObjServerDel(index);
	}
}



void CloseClient(int index)
{
	if ( index < 0 || index >= g_dwMaxServerGroups )
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}

	if ( g_Server[index].m_State == SS_CLOSED )
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}

	//EnterCriticalSection(&criti);
	EnterCriticalSection(&criti[g_Server[index].m_Type]);

	if ( g_Server[index].m_Socket != INVALID_SOCKET )
	{
		closesocket(g_Server[index].m_Socket );
		g_Server[index].m_Socket = INVALID_SOCKET;
	}
	else
	{
		LogAdd("error-L1 : CloseClient INVALID_SOCKET");
	}

	//LeaveCriticalSection(&criti);
	LeaveCriticalSection(&criti[g_Server[index].m_Type]);
}

void ResponErrorCloseClient(int index)
{
	if ( index < 0 || index >= g_dwMaxServerGroups )
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}

	if ( g_Server[index].m_State == SS_CLOSED )
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}

	//EnterCriticalSection(&criti);
	EnterCriticalSection(&criti[g_Server[index].m_Type]);
	closesocket(g_Server[index].m_Socket);
	g_Server[index].m_Socket = INVALID_SOCKET;

	if ( g_Server[index].m_Socket == INVALID_SOCKET )
	{
		LogAdd("error-L1 : CloseClient INVALID_SOCKET");
	}

	gObjServerDel(index);
	//LeaveCriticalSection(&criti);
	LeaveCriticalSection(&criti[g_Server[index].m_Type]);
}
