#include "stdafx.h"
#include "Packets.h"
#include "User.h"
#include "LogProc.H"


bool PacketCheckTime(LPOBJ lpObj)
{
	if( (GetTickCount() - lpObj->m_PacketCheckTime) < 300 ) return false;
	// ------
	lpObj->m_PacketCheckTime = GetTickCount();
	// ------
	return true;
}

bool DataEncryptCheck(int aIndex, BYTE protoNum, BOOL Encrypt)
{
	if ( Encrypt == FALSE )
	{
		CLog.LogAdd("Error-L1 : Not Encrypt %s %d", gObj[aIndex].AccountID, protoNum);
		gNet.CloseClient(aIndex);
		return false;
	}
	// ------
	return true;
}

void ToHex(unsigned char * out, unsigned char * data, int len)
{
	std::string s,t;
	char cBuff[5000];
	for(int i=0;i<len;i++)
	{
		wsprintf(cBuff,"%02X,",data[i]);
		s=cBuff;
		t+=s;
	}
	memcpy(out,t.c_str(),t.length());
}