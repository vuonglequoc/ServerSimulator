//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Network Class.
//----------------------------------------------------------------------------------
#include "StdAfx.H"
#include "Network.H"
#include "Spe.H"
#include "Functions.H"


CNetwork gNet;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CNetwork::CNetwork()
{
	InitializeCriticalSection(&CS);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CNetwork::~CNetwork()
{
	this->CloseNetwork();
	// -----
	DeleteCriticalSection(&CS);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::Initialize()
{
	bool bReturn	= true;
	// -----
	if ( !StartWinSocket() ) bReturn = false;
	// -----
	if( !CreateListenSocket() ) bReturn = false;
	// -----
	m_SendBuff		= new UCHAR[MAX_SENDBUFF_SIZE];
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::StartWinSocket()
{
	bool bReturn	= true;
	// -----
	// -- Use The WinSocket DLL By the Process.
	// -----
	m_StartWSA	= WSAStartup(MAKEWORD(2,2), &this->wsData);
	// -----
	if ( m_StartWSA == SOCKET_ERROR )
	{
		CLog.Error("[NETWORK] Could not Start Up Winsock (DLL).");
		// -----
		bReturn = false;
	}
	else
	{
		CLog.Info("[NETWORK] WinSock Started Successfully.");
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::OpenSocket()
{
	bool bReturn	= true;
	// -----
	// -- Create Socket Of The Service Provider
	// -----
	m_Listen	= WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	// -----
	if ( m_Listen == SOCKET_ERROR ) 
	{
		CLog.Error("[NETWORK] Could not Create the Socket.");
		// -----
		bReturn = false;
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::CreateListenSocket()
{
	bool bReturn	= true;
	// -----
	sockaddr_in SrvAddr	= {0};
	// -----
	if ( !OpenSocket() ) bReturn = false;
	// -----
	// -- Fill Address Structure with Appropriate Data.
	// -----
	SrvAddr.sin_family		= AF_INET;
	SrvAddr.sin_port		= htons(this->m_ServerPort);
	SrvAddr.sin_addr.s_addr	= INADDR_ANY;
	// -----
	// -- Bind Local Address with the Socket.
	// -----
	m_BindSocket	= bind(m_Listen, (sockaddr*)&SrvAddr, sizeof(SrvAddr));
	// -----
	if ( m_BindSocket == SOCKET_ERROR )
	{
		//CLog.MsgBox("Terminate Zodea GameServer", "GameServer can not be Lanched Twice!", MB_ICONERROR|MB_OK);
		CLog.MsgBox("Terminate MuBK GameServer", "GameServer can not be Lanched Twice!", MB_ICONERROR|MB_OK);
		// -----
		ExitProcess(0);
		// -----
		bReturn = false;
	}
	// -----
	// -- Mark the Socket for Listening For the incoming Connections.
	// -----
	m_ListenSocket	= listen(m_Listen, 5);
	// -----
	if ( m_ListenSocket == SOCKET_ERROR ) 
	{
		CLog.Error("[NETWORK] Marking Socket For Listening Failed.");
		// -----
		bReturn = false;
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::CreateSrvWorker(int ServerPort)
{
	bool bReturn		= true;
	// -----
	this->m_ThreadID	= 0;
	// -----
	this->m_ServerPort	= ServerPort;	
	// -----
	GIocpThreadHandle	= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)gNet.IocpServerWorker, NULL, 0, &m_ThreadID);
	// -----
	if ( GIocpThreadHandle == NULL )
	{
		CLog.Error("[NETWORK] ServerWorker Failed to be Created.");
		// -----
		bReturn = false;
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::UpdateIOCompletionPort(int Socket, int ClientIndex)
{
	bool bReturn		= true;
	// -----
	_PER_SOCKET_CONTEXT * lpPerSocketContext = NULL;
	// -----
	// -- Create Input/Output Completion Port and binds to the Socket
	// -----
	CIOCPort = CreateIoCompletionPort((HANDLE) Socket, m_hCmport, ClientIndex, NULL);
	// -----
	if( CIOCPort == NULL )
	{
		bReturn = false;
	}
	// ----
	gObj[ClientIndex].PerSocketContext->dwIOCount = 0;
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int aIndex)
{
	bool bReturn		= true;
	// -----
	UCHAR * RecvBuff	= lpIOContext->Buffer;
	int		lOfs		= 0;
	int		size		= 0;
	BYTE	headcode	= 0;
	BYTE	xcode		= 0;
	// -----
	UCHAR	byDec[MAX_SENDBUFF_SIZE];
	// -----
	if(lpIOContext->nSentBytes < 3) 
	{
		return TRUE;
	}
	// -----
	while(true)
	{
		if((RecvBuff[lOfs] == 0xC1) || (RecvBuff[lOfs] == 0xC3))
		{
			UCHAR * pBuf;
			// -----
			pBuf		= &RecvBuff[lOfs];
			size		= pBuf[1];
			headcode	= pBuf[2];
			xcode		= RecvBuff[lOfs];
		}
		// -----
		else if((RecvBuff[lOfs+0] == 0xC2) || (RecvBuff[lOfs+0] == 0xC4))
		{
			UCHAR * pBuf;
			// -----
			pBuf		= &RecvBuff[lOfs];
			size		= pBuf[1] * 256;
			size	   |= pBuf[2];
			headcode	= pBuf[3];
			xcode		= RecvBuff[lOfs];
		}
		else
		{
			CLog.Error("[NETWORK] Parse Header Packet Fail size [%d] headcode (%x) xcode (%x)", size, headcode, xcode);
			// ----
			lpIOContext->nSentBytes = 0;
			// ----
			return false;
			// ----
			break;
		}
		// -----
		if(size <= 0)
		{
			CLog.Error("[NETWORK] Receive Data Parsing Size [%d].", size);
			// ----
			return false;
		}
		// -----
		if ( size <= lpIOContext->nSentBytes )
		{
			if ( xcode == 0xC3 )
			{
				int Retn = g_SimpleModulusCS.Decrypt( &byDec[2], &RecvBuff[lOfs+2], size-2);
				// -----
				if ( Retn >= 0 )
				{
					UCHAR * pDecBuf	= &byDec[1];
					// -----
					headcode		= pDecBuf[2];
					BYTE subhead	= byDec[2];
					byDec[1]		= 0xC1;
					byDec[2]		= (Retn&0xFF)+2;
					// -----
					CStreamPacketEngine PacketStream;
					// -----
					PacketStream.Clear();
					// -----
					if ( !(PacketStream.AddData(&byDec[1], Retn+2)) )
					{
						CLog.Error( "[NETWORK] CStreamPacketEngine Add Data Error - IP:(%s) Account:(%s) Name:(%s) HEAD:[%x] Status:(%s).", 
							gObj[aIndex].Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].Name, headcode, GetStatus(gObj[aIndex].Connected));
						// -----
						return false;
					}
					// -----
					if ( PacketStream.ExtractPacket(byDec) != 0 )
					{
						CLog.Error( "[NETWORK] CStreamPacketEngine Extract Packet Error - IP:(%s) Account:(%s) Name:(%s) HEAD:[%x] Status:(%s).", 
							gObj[aIndex].Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].Name, headcode, GetStatus(gObj[aIndex].Connected));
						// -----
						return false;
					}
					// -----
					ProtocolCore(headcode, byDec, Retn, aIndex, 1, subhead);
				}
			}
			// -----
			else if ( xcode == 0xC4 )
			{
				int Retn = g_SimpleModulusCS.Decrypt( &byDec[3], &RecvBuff[lOfs+3], size-3);
				// -----
				if ( Retn >= 0 )
				{
					UCHAR* pDecBuf	= &byDec[1];
					// -----
					BYTE hcode		= byDec[3];
					byDec[1]		= 0xC2;
					WORD size		= (Retn &0xFFFF)+3;
					byDec[2]		= SET_NUMBERH(size);
					byDec[3]		= SET_NUMBERL(size); 
					// -----
					CStreamPacketEngine PacketStream;
					// -----
					PacketStream.Clear();
					// -----
					if ( !(PacketStream.AddData(&byDec[1], Retn+3)) )
					{
						CLog.Error( "[NETWORK] CStreamPacketEngine Add Data Error - IP:(%s) Account:(%s) Name:(%s) HEAD:[%x] Status:(%s).", 
							gObj[aIndex].Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].Name, headcode, GetStatus(gObj[aIndex].Connected));
						// -----
						return false;
					}
					// -----
					if ( PacketStream.ExtractPacket(byDec) != 0 )
					{
						CLog.Error( "[NETWORK] CStreamPacketEngine Extract Packet Error - IP:(%s) Account:(%s) Name:(%s) HEAD:[%x] Status:(%s).", 
							gObj[aIndex].Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].Name, headcode, GetStatus(gObj[aIndex].Connected));
						// -----
						return false;
					}
					// -----
					headcode = pDecBuf[2];
					// -----
					ProtocolCore(headcode, byDec, Retn, aIndex, 1, hcode);
				}
			}
			// -----
			else
			{
				CStreamPacketEngine CSTPESrv;
				// -----
				CSTPESrv.Clear();
				// -----
				if ( !(CSTPESrv.AddData(&RecvBuff[lOfs], size)) )
				{
					CLog.Error( "[NETWORK] CStreamPacketEngine Add Data Error - IP:(%s) Account:(%s) Name:(%s) HEAD:[%x] Status:(%s).", 
						gObj[aIndex].Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].Name, headcode, GetStatus(gObj[aIndex].Connected));
					// -----
					return false;
				}
				// -----
				if ( CSTPESrv.ExtractPacket(byDec) != 0 )
				{
					CLog.Error( "[NETWORK] CStreamPacketEngine Extract Packet Error - IP:(%s) Account:(%s) Name:(%s) HEAD:[%x] Status:(%s).", 
						gObj[aIndex].Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].Name, headcode, GetStatus(gObj[aIndex].Connected));
					// -----
					return false;
				}
				// -----
				ProtocolCore(headcode, byDec, size, aIndex, 0, -1);
			}
			// -----
			lOfs					 += size;
			lpIOContext->nSentBytes	 -= size;
			// -----
			if(lpIOContext->nSentBytes <= 0)
			{
				break;
			}
		}
		// -----
		else if(lOfs > 0)
		{
			if(lpIOContext->nSentBytes < 1)
			{
				CLog.Error("[NETWORK] Recieve Data BuffLen [1].");
				// -----
				break;
			}
			// -----
			if(lpIOContext->nSentBytes < MAX_IO_BUFFER_SIZE) 
			{
				memcpy(RecvBuff, &RecvBuff[lOfs], lpIOContext->nSentBytes);
				// -----
				CLog.Error("[RecvDataParse] Message Copy [%d].", lpIOContext->nSentBytes);
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
	return true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::AcceptClientSocket()
{
	bool bReturn				= true;
	// -----
	sockaddr_in		cAddr		= {0};
	in_addr			cInAddr		= {0};
	int				cAddrLen	= 16;
	ULONG			RecvBytes	= 0;
	ULONG			Flags		= 0;
	// -----
	while(true)
	{
		// -- Accept the Client on the Socket
		// -----
		m_ClientSocket = WSAAccept(m_Listen, (sockaddr*)&cAddr, &cAddrLen, NULL, 0);
		// -----
		if( m_ClientSocket == SOCKET_ERROR )
		{
			CLog.Error("[NETWORK] Failed to Accept the Client to the Socket.");
			// -----
			continue;
		}
		// -----
		memcpy(&cInAddr, &cAddr.sin_addr, sizeof(cInAddr));
		// -----
		EnterCriticalSection(&CS);
		// -----
		m_ClientIndex = gObjAddSearch(m_ClientSocket, inet_ntoa(cInAddr));
		// -----
		if( m_ClientIndex == SOCKET_ERROR )
		{
			CLog.Error("[NETWORK] ClientIndex [SOCKET_ERROR] - Client Index Could not be Added.");
			// -----
			closesocket(m_ClientSocket);
			// -----
			LeaveCriticalSection(&CS);
			// -----
			continue;
		}
		// -----
		if( !(UpdateIOCompletionPort(m_ClientSocket, m_ClientIndex)) )
		{
			CLog.Error("[NETWORK] Update IO Completion Port Failed Socket[%d] Index[%d].", m_ClientSocket, m_ClientIndex);
			// -----
			closesocket(m_ClientSocket);
			// -----
			LeaveCriticalSection(&CS);
			// -----
			continue;
		}
		// -----
		if( gObjAdd(m_ClientSocket, inet_ntoa(cInAddr), m_ClientIndex) == SOCKET_ERROR )
		{
			CLog.Error("[NETWORK] Could not Add Client - Socket[%d] Index[%d].", m_ClientSocket, m_ClientIndex);
			// -----
			closesocket(m_ClientSocket);
			// -----
			LeaveCriticalSection(&CS);
			// -----
			continue;
		}
		// -----
		_PER_IO_CONTEXT * IOContextSend = &gObj[m_ClientIndex].PerSocketContext->IOContext[0];
		_PER_IO_CONTEXT * IOContextRecv = &gObj[m_ClientIndex].PerSocketContext->IOContext[1];
		// -----
		memset(&IOContextSend->Overlapped, 0, sizeof(OVERLAPPED));
		memset(&IOContextRecv->Overlapped, 0, sizeof(OVERLAPPED));
		// -----
		IOContextSend->wsabuf.buf			= (LPSTR)IOContextSend->Buffer;
		IOContextSend->wsabuf.len			= MAX_IO_BUFFER_SIZE;
		IOContextSend->nTotalBytes			= 0;
		IOContextSend->nSentBytes			= 0;
		IOContextSend->nWaitIO				= 0;
		IOContextSend->nSecondOfs			= 0;
		IOContextSend->IOOperation			= OP_SEND;
		// -----
		IOContextRecv->wsabuf.buf			= (LPSTR)IOContextSend->Buffer;
		IOContextRecv->wsabuf.len			= MAX_IO_BUFFER_SIZE;
		IOContextRecv->nTotalBytes			= 0;
		IOContextRecv->nSentBytes			= 0;
		IOContextRecv->nWaitIO				= 0;
		IOContextRecv->nSecondOfs			= 0;
		IOContextRecv->IOOperation			= OP_RECV;
		// -----
		gObj[m_ClientIndex].PerSocketContext->m_socket	= m_ClientSocket;
		gObj[m_ClientIndex].PerSocketContext->nIndex	= m_ClientIndex;
		// -----
		// -- Receive Data From Client
		// -----
		m_RecvData = WSARecv(m_ClientSocket, &IOContextSend->wsabuf, 1, (LPDWORD)&RecvBytes, &Flags, &IOContextSend->Overlapped, NULL);
		// -----
		if( m_RecvData == SOCKET_ERROR )
		{
			if( WSAGetLastError() != WSA_IO_PENDING )
			{
				CLog.Error("[NETWORK] Failed to Receive Data From Client.");
				// -----
				gObj[m_ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
				// -----
				CloseClient(gObj[m_ClientIndex].PerSocketContext);
				// -----
				LeaveCriticalSection(&CS);
				// -----
				continue;
			}
		}
		// -----
		gObj[m_ClientIndex].PerSocketContext->IOContext[0].nWaitIO  = 1;
		gObj[m_ClientIndex].PerSocketContext->dwIOCount++;
		// -----
		LeaveCriticalSection(&CS);
		// -----
		SCPJoinResultSend(m_ClientIndex, 1);
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::DataSend(int aIndex, UCHAR * lpMsg, DWORD dwSize)
{
	ULONG SendBytes;
	UCHAR *				SendBuf;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	// -----
	if ( aIndex < OBJ_STARTUSERINDEX )
	{
		return true;
	}
	// -----
	EnterCriticalSection(&CS);
	// -----
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		CLog.Error("[DataSend] Index(%d) %x %x %x.", dwSize, lpMsg[0], lpMsg[1], lpMsg[2]);
		// -----
		LeaveCriticalSection(&CS);
		// -----
		return false;
	}
	// -----
	if(lpMsg[0] == 0xC3 || lpMsg[0] == 0xC4)
	{
		int		Retn;
		BYTE	btsize;
		// -----
		if(lpMsg[0] == 0xC3)
		{
			btsize			= lpMsg[1];
			lpMsg[1]		= gNSerialCheck[aIndex].GetSendSerial();
			Retn			= g_SimpleModulusSC.Encrypt(&m_SendBuff[2], &lpMsg[1], dwSize-1);
			m_SendBuff[0]	= 0xC3;
			m_SendBuff[1]	= Retn + 2;
			SendBuf			= m_SendBuff;
			dwSize			= Retn + 2;
			lpMsg[1]		= btsize;
		}
		else
		{
			btsize			= lpMsg[2];
			lpMsg[2]		= gNSerialCheck[aIndex].GetSendSerial();
			Retn			= g_SimpleModulusSC.Encrypt(&m_SendBuff[3], &lpMsg[2], dwSize-2);
			m_SendBuff[0]	= 0xC4;
			m_SendBuff[1]	= SET_NUMBERH(Retn+3);
			m_SendBuff[2]	= SET_NUMBERL(Retn+3);
			SendBuf			= m_SendBuff;
			dwSize			= Retn + 3;
			lpMsg[2]		= btsize;
		}
	}
	else
	{
		SendBuf = lpMsg;
	}
	// -----
	if( gObj[aIndex].Connected < PLAYER_CONNECTED )
	{
		LeaveCriticalSection(&CS);
		// -----
		return false;
	}
	// -----
	lpPerSocketContext = gObj[aIndex].PerSocketContext;
	// -----
	if( dwSize > sizeof(lpPerSocketContext->IOContext[0].Buffer))
	{
		CLog.Error("[DataSend] Error Max Message [%d].", dwSize);
		// -----
		CloseClient(aIndex);
		// -----
		LeaveCriticalSection(&CS);
		// -----
		return false;
	}
	// -----
	_PER_IO_CONTEXT  * lpIoCtxt = &lpPerSocketContext->IOContext[1];
	// -----
	if(lpIoCtxt->nWaitIO > 0)
	{
		if( ( lpIoCtxt->nSecondOfs + dwSize ) > MAX_IO_BUFFER_SIZE-1 )
		{
			CLog.Error("[DataSend] [%s][%s] Index[%d] MAX BUFFER OVER %d %d %d", gObj[aIndex].AccountID, gObj[aIndex].Name, aIndex,
				lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize);
			// -----
			lpIoCtxt->nWaitIO = 0;
			// -----
			CloseClient(aIndex);
			// -----
			LeaveCriticalSection(&CS);
			// -----
			return true;
		}
		// -----
		memcpy( &lpIoCtxt->BufferSecond[lpIoCtxt->nSecondOfs], SendBuf, dwSize);
		// -----
		lpIoCtxt->nSecondOfs += dwSize;
		// -----
		LeaveCriticalSection(&CS);
		// -----
		return true;
	}
	// -----
	lpIoCtxt->nTotalBytes = 0;
	// -----
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		// -----
		lpIoCtxt->nTotalBytes	= lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs	= 0;
	}
	// -----
	if ( (lpIoCtxt->nTotalBytes+dwSize) > MAX_IO_BUFFER_SIZE-1 )
	{
		CLog.Error("[DataSend] [%s][%s] Index[%d] MAX BUFFER OVER %d %d %d", gObj[aIndex].AccountID, gObj[aIndex].Name, aIndex,
				lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize);
		// -----
		lpIoCtxt->nWaitIO = 0;
		// -----
		CloseClient(aIndex);
		// -----
		LeaveCriticalSection(&CS);
		// -----
		return false;
	}
	// -----
	memcpy( &lpIoCtxt->Buffer[lpIoCtxt->nTotalBytes], SendBuf, dwSize);
	// -----
	lpIoCtxt->nTotalBytes	+= dwSize;
	lpIoCtxt->wsabuf.buf	= (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes	= 0;
	lpIoCtxt->IOOperation	= OP_RECV;
	// -----
	if( WSASend( gObj[aIndex].m_socket, &lpIoCtxt->wsabuf , 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == SOCKET_ERROR )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )	
		{
			lpIoCtxt->nWaitIO = 0;
			// -----
			if ( lpIoCtxt->wsabuf.buf[0] == 0xC1 )
			{
				CLog.Error("[DataSend] WSASend(%d) Failed with Error [%x][%x] [%d] (%s).", aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), gObj[aIndex].Ip_addr);
			}
			// -----
			else if ( lpIoCtxt->wsabuf.buf[0] == 0xC2 )
			{
				CLog.Error("[DataSend] WSASend(%d) Failed with Error [%x][%x] [%d] (%s).", aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), gObj[aIndex].Ip_addr);
			}
			// -----
			CloseClient(aIndex);
			// -----
			LeaveCriticalSection(&CS);
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
	LeaveCriticalSection(&CS);
	// -----
	return true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	bool bReturn				= true;
	// -----
	EnterCriticalSection(&CS);
	// -----
	int	aIndex					= lpPerSocketContext->nIndex;
	_PER_IO_CONTEXT * lpIoCtxt	= &lpPerSocketContext->IOContext[1];
	ULONG SendBytes				= 0;
	// -----
	if(lpIoCtxt->nWaitIO > 0)
	{
		LeaveCriticalSection(&CS);
		// -----
		bReturn = false;
	}
	// -----
	lpIoCtxt->nTotalBytes		= 0;
	// -----
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		// -----
		lpIoCtxt->nTotalBytes	= lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs	= 0;
	}
	else
	{
		LeaveCriticalSection(&CS);
		// -----
		bReturn = false;
	}
	// -----
	lpIoCtxt->wsabuf.buf		= (LPSTR)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len		= lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes		= 0;
	lpIoCtxt->IOOperation		= OP_RECV;
	// -----
	// -- Send Data to Client
	// -----
	m_SendData = WSASend(gObj[aIndex].m_socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL);
	// -----
	if( m_SendData == SOCKET_ERROR )
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
			CLog.Error("[NETWORK] Failed to Send Data to Client (%s).", gObj[aIndex].Ip_addr);
			// -----
			CloseClient(aIndex);
			// -----
			LeaveCriticalSection(&CS);
			// -----
			bReturn = false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount++;
	}
	// -----
	lpIoCtxt->nWaitIO = 1;
	// -----
	LeaveCriticalSection(&CS);
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CNetwork::IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	bool bReturn				= true;
	// -----
	EnterCriticalSection(&CS);
	// -----
	int	aIndex					= lpPerSocketContext->nIndex;
	_PER_IO_CONTEXT * lpIoCtxt	= &lpPerSocketContext->IOContext[1];
	ULONG SendBytes				= 0;
	// -----
	if((lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes) < 0)
	{
		LeaveCriticalSection(&CS);
		// -----
		bReturn = false;
	}
	// -----
	lpIoCtxt->wsabuf.buf	= (LPSTR)&lpIoCtxt->Buffer[lpIoCtxt->nSentBytes];
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes;
	lpIoCtxt->IOOperation	= OP_RECV;
	// -----
	// -- Send Data to Client
	// -----
	m_SendData = WSASend(gObj[aIndex].m_socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL);
	// -----
	if( m_SendData == SOCKET_ERROR )
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
			CLog.Error("[NETWORK] Failed to Send Data to Client (%s).", gObj[aIndex].Ip_addr);
			// -----
			CloseClient(aIndex);
			// -----
			LeaveCriticalSection(&CS);
			// -----
			bReturn = false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount++;
	}
	// -----
	lpIoCtxt->nWaitIO = 1;
	// -----
	LeaveCriticalSection(&CS);
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNetwork::CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	int aIndex = lpPerSocketContext->nIndex;
	// -----
	if ( aIndex >= OBJMIN && aIndex < OBJMAX )
	{
		if ( gObj[aIndex].m_socket != INVALID_SOCKET )
		{
			if ( closesocket(gObj[aIndex].m_socket) == SOCKET_ERROR )
			{
				if (WSAGetLastError() != WSAENOTSOCK)
				{
					return;
				}
			}
			// -----
			gObj[aIndex].m_socket = INVALID_SOCKET;
		}
		// -----
		gObjDel(aIndex);
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNetwork::CloseClient(int aIndex)
{
	if ( aIndex < 0 || aIndex >= OBJMAX )
	{
		CLog.Error("[CloseClient] Index Error [%d].", aIndex);
		// -----
		return;
	}
	// -----
	if ( gObj[aIndex].Connected == PLAYER_EMPTY )
	{
		CLog.Error("[CloseClient] Connect State Error [%s].", GetStatus(gObj[aIndex].Connected));
		// -----
		return;
	}
	// -----
	EnterCriticalSection(&CS);
	// -----
	if ( gObj[aIndex].m_socket != INVALID_SOCKET )
	{
		closesocket(gObj[aIndex].m_socket);
		// -----
		gObj[aIndex].m_socket = INVALID_SOCKET;
	}
	else
	{
		CLog.Error("[CloseClient] INVALID SOCKET [%d].", gObj[aIndex].m_socket);
	}
	// -----
	LeaveCriticalSection(&CS);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNetwork::ResponErrorCloseClient(int aIndex)
{
	if (aIndex < 0 || aIndex >= OBJMAX)
	{
		CLog.Error("[CloseClient] Index Error [%d].", aIndex);
		// -----
		return;
	}
	// -----
	if ( gObj[aIndex].Connected == PLAYER_EMPTY )
	{
		CLog.Error("[CloseClient] Connect State Error [%s].", GetStatus(gObj[aIndex].Connected));
		// -----
		return;
	}
	// -----
	EnterCriticalSection(&CS);
	// -----
	closesocket(gObj[aIndex].m_socket);
	// -----
	gObj[aIndex].m_socket = INVALID_SOCKET;
	// -----
	if ( gObj[aIndex].m_socket == INVALID_SOCKET )
	{
		CLog.Error("[CloseClient] INVALID SOCKET [%d].", gObj[aIndex].m_socket);
	}
	// -----
	gObjDel(aIndex);
	// -----
	LeaveCriticalSection(&CS);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNetwork::CloseNetwork()
{
	DWORD szCPU		= 0;
	// -----
	for ( DWORD i = 0; i != m_NumOfThreads; i++ ) { PostQueuedCompletionStatus(m_hCmport, 0, 0, 0); }
	// -----
	do { TerminateThread(m_hThread[szCPU] , 0); szCPU++; } while (szCPU < m_ThreadsCount);
	// -----
	CloseHandle(m_hCmport);
	closesocket(m_Listen);
	closesocket(m_ClientSocket);
	// -----
	this->m_hCmport			= NULL;
	this->m_StartWSA		= INVALID_SOCKET;
	this->m_BindSocket		= INVALID_SOCKET;
	this->m_ListenSocket	= INVALID_SOCKET;
	this->m_RecvData		= INVALID_SOCKET;
	this->m_Listen			= INVALID_SOCKET;
	this->m_ClientIndex		= INVALID_SOCKET;
	// -----
	WSACleanup();
	// -----
	delete[] m_SendBuff;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD CNetwork::IocpServerWorker(LPVOID Param)
{
	_PER_SOCKET_CONTEXT *	lpPerSocketContext	= NULL;
	// -----
	GetSystemInfo(&gNet.SysInfo);
	// -----
	// -- Get Number Of Process
	// -----
	gNet.m_NumOfThreads = gNet.SysInfo.dwNumberOfProcessors * 2;
	// -----
	__try
	{	
		gNet.m_hCmport = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
		// -----
		if ( gNet.m_hCmport == NULL )
		{
			CLog.Error("[NETWORK] Create IO Completion Port Failed Socket[%d] Index[%d].", lpPerSocketContext->m_socket, lpPerSocketContext->nIndex);
			// -----
			__leave;
		}
		// -----
		for ( DWORD i = 0; i != gNet.m_NumOfThreads; i++ )
		{
			HANDLE hThread = CreateThread(NULL, 0, gNet.SrvWorkerThread, gNet.m_hCmport, 0, &gNet.m_ThreadID);
			// -----
			if ( hThread == NULL )
			{
				CLog.Error("[NETWORK] Create Thread Failed to be created.");
				// -----
				__leave;
			}
			// -----
			gNet.m_hThread[i] = hThread;
			// -----
			CloseHandle(hThread);
		}
		// -----
		if ( gNet.CreateListenSocket() )
		{
			gNet.AcceptClientSocket();
		}
	}
	// -----
	__finally
	{
		gNet.CloseNetwork();
		// -----
		CLog.Error("[NETWORK] IocpServerWorker Failed - Error: [%d]", GetLastError());
	}
	// -----
	return TRUE;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD CNetwork::SrvWorkerThread(LPVOID Param)
{
	DWORD ClientIndex							= SOCKET_ERROR;
	DWORD dwIoSize								= 0;
	DWORD dwSendNumBytes						= 0;
	ULONG RecvBytes								= 0;
	ULONG Flags									= 0;
	BOOL bSuccess								= FALSE;
	// -----
	LPOVERLAPPED lpOverlapped					= {0};
	_PER_SOCKET_CONTEXT * lpPerSocketContext	= {0};
	_PER_IO_CONTEXT * lpIOContext				= {0};
	// -----
	while(true)
	{
		// -- Dequeue I/O Completion Packet from I/O Completion Port.
		// -- If there is no Completion Packet Queued, The function waits for a pending I/O operation binded to Completion Port to complete.
		// -- [ WAIT FOR EVER ]
		// -----
		bSuccess = GetQueuedCompletionStatus(gNet.m_hCmport, &dwIoSize, &ClientIndex, &lpOverlapped, -1);
		// -----
		if(!bSuccess)
		{
			if(lpOverlapped != 0)
			{
				DWORD aError = GetLastError();
				// ----
				if( (aError != ERROR_SEM_TIMEOUT)			&& (aError != ERROR_NETNAME_DELETED)  &&
					(aError != ERROR_CONNECTION_ABORTED)	&& (aError != ERROR_OPERATION_ABORTED) )
				{
					EnterCriticalSection(&gNet.CS);
					// -----
					CLog.Error("[NETWORK] Failed With Get Queue Completion Status.");
					// -----
					LeaveCriticalSection(&gNet.CS);
					// -----
					return FALSE;
				}
			}
		}
		// -----
		EnterCriticalSection(&gNet.CS);
		// -----
		lpPerSocketContext = gObj[ClientIndex].PerSocketContext;
		// -----
		lpPerSocketContext->dwIOCount--;
		// -----
		if( dwIoSize == NULL )
		{
			CLog.LogOut("Connection Closed, dwIoSize[0] Index[%d].", lpPerSocketContext->nIndex);
			// -----
			gNet.CloseClient(lpPerSocketContext);
			// -----
			LeaveCriticalSection(&gNet.CS);
			// -----
			continue;
		}
		// -----
		lpIOContext = (_PER_IO_CONTEXT *)lpOverlapped;
		// -----
		if( lpIOContext == NULL )
		{
			continue;
		}
		// -----
		if( lpIOContext->IOOperation == OP_RECV )
		{
			lpIOContext->nSentBytes += dwIoSize;
			// -----	
			if(lpIOContext->nSentBytes >= lpIOContext->nTotalBytes)
			{
				lpIOContext->nWaitIO = 0;
				// -----
				if( lpIOContext->nSecondOfs > 0)
				{
					gNet.IoSendSecond(lpPerSocketContext);
				}
			}
			else
			{
				gNet.IoMoreSend(lpPerSocketContext);
			}
		}
		// -----
		else if( lpIOContext->IOOperation == OP_SEND )
		{
			RecvBytes				 = 0;
			lpIOContext->nSentBytes += dwIoSize;
			// -----
			if( !(gNet.RecvDataParse(lpIOContext, lpPerSocketContext->nIndex)) )
			{
				CLog.Error("[NETWORK] Failed to Receive Data - Index: [%d].", lpPerSocketContext->nIndex);
				// -----
				gNet.CloseClient(lpPerSocketContext);
				// -----
				LeaveCriticalSection(&gNet.CS);
				// -----
				continue;
			}
			// -----
			lpIOContext->nWaitIO	= 0;
			Flags					= 0;
			// -----
			memset(&lpIOContext->Overlapped, 0, sizeof (WSAOVERLAPPED));
			// -----
			lpIOContext->wsabuf.len		= MAX_IO_BUFFER_SIZE - lpIOContext->nSentBytes;
			lpIOContext->wsabuf.buf		= (LPSTR)&lpIOContext->Buffer[lpIOContext->nSentBytes];
			lpIOContext->IOOperation	= OP_SEND;
			// -----
			// -- Receive Data From Client
			// -----
			gNet.m_RecvData = WSARecv(lpPerSocketContext->m_socket, &lpIOContext->wsabuf, 1, (LPDWORD)&RecvBytes, &Flags, &lpIOContext->Overlapped, NULL);
			// -----
			if( gNet.m_RecvData == SOCKET_ERROR )
			{
				if( WSAGetLastError() != WSA_IO_PENDING )
				{
					CLog.Error("[NETWORK] Failed to Receive Data From Client.");
					// -----
					gNet.CloseClient(lpPerSocketContext);
					// -----
					LeaveCriticalSection(&gNet.CS);
					// -----
					continue;
				}
			}
			// -----
			lpPerSocketContext->dwIOCount++;
			lpIOContext->nWaitIO = 1;
		}
		// -----
		LeaveCriticalSection(&gNet.CS);
	}
	// -----
	return TRUE;
}
