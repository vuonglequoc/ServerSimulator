//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU NSerialCheck Functions & Structures					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef NSERIALCHECK_H
#define NSERIALCHECK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class NSerialCheck
{

public:

	NSerialCheck();
	virtual ~NSerialCheck();

	void init();
	BOOL InCheck(BYTE serial);
	int GetSerial();
	BYTE GetSendSerial();

private:

	BYTE m_serial;	// 4
	BYTE m_SendSeiral;	// 5
};

#endif