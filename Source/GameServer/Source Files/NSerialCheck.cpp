//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU NSerialCheck Functions & Structures					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/NSerialCheck.h"


NSerialCheck::NSerialCheck()
{
	this->init();
}

NSerialCheck::~NSerialCheck()
{
	return;
}

void NSerialCheck::init()
{
	this->m_serial=-1;
	this->m_SendSeiral=0;
}

BOOL NSerialCheck::InCheck(unsigned char serial)
{
	BYTE m_s=this->m_serial;
	m_s++;

	if ( m_s  == serial )
	{
		this->m_serial = serial;
		return TRUE;
	}

	return FALSE;
}

int NSerialCheck::GetSerial()
{
	return this->m_serial;
}

BYTE NSerialCheck::GetSendSerial()
{
	return this->m_SendSeiral++;
}