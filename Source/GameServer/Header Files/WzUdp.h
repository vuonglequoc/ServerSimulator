//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU WzUdp Structures										//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef WZUDP_H
#define WZUDP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void (*UdpProtocolCore)(BYTE,LPBYTE, int);


struct PER_IO_OPERATION_DATA
{
	WSAOVERLAPPED Overlapped;
	WSABUF DataBuf;
	char Buffer[2048];
	DWORD Bytes;
	int lOfs;
};

class WzUdp
{
public:
	WzUdp();
	virtual ~WzUdp();
	BOOL Init();
	BOOL CreateSocket();
	BOOL Run();
	BOOL SendData(LPBYTE SendData, DWORD nSendDataLen);
	BOOL MuProtocolParse(LPBYTE RecvData, int& nRecvDataLen);
	BOOL Close();
	BOOL SendSet(LPSTR ip, int port);
	BOOL RecvSet(int port);
	BOOL RecvThread();
	BOOL SetProtocolCore(UdpProtocolCore pc);

private:
	int m_Port;
	HANDLE m_ThreadHandle;
	DWORD m_ThreadID;
	SOCKET m_Socket;
	LPBYTE m_Recvbuf;
	LPBYTE m_Sendbuf;
	int m_dwLength;
	int m_dwRecvOfs;
	int m_dwSendLength;
	PER_IO_OPERATION_DATA m_PerIoSendData;
	sockaddr_in	m_SockAddr;
	UdpProtocolCore ProtocolCore;
};


#endif