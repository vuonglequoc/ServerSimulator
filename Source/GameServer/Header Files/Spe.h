/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Stream Packet Engine Of Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#ifndef SPE_HEADER
#define SPE_HEADER

#include "StdAfx.H"
#include "LogProc.H"


class CStreamPacketEngine
{
public:
	CStreamPacketEngine() { this->Clear(); };
	// -----
	virtual ~CStreamPacketEngine() { return; };
	// -----
	void Clear() { this->m_wSize = 0; };
	// -----
protected:
	void XorData(int iStart, int iEnd, int iDir)
	{
		if ( iStart < iEnd  )
		{
			CLog.Error("[StreamPacketEngine] XorData Error Start::[%d], End::[%d]", iStart, iEnd);
			return;
		}
		// -----
		BYTE byXorFilter[32] = { 0xAB, 0x11, 0xCD, 0xFE, 0x18, 0x23, 0xC5, 0xA3, 0xCA, 0x33, 0xC1, 0xCC, 0x66, 0x67, 0x21, 0xF3, 0x32, 0x12, 0x15, 
								 0x35, 0x29, 0xFF, 0xFE, 0x1D, 0x44, 0xEF, 0xCD, 0x41, 0x26, 0x3C, 0x4E, 0x4D };
		// -----
		for ( int i = iStart; i != iEnd; i+=iDir )
		{
			this->m_byBuffer[i] ^= this->m_byBuffer[i-1] ^ byXorFilter[ i%32 ];
		}
	}
	// -----
public:
	bool AddData(void* pSrc, WORD wSize )
	{
		if ( ((this->m_wSize + wSize) >=  2048) || (wSize == NULL) )
		{
			CLog.Error("[StreamPacketEngine] Adding Buffer Size Error [%d].", this->m_wSize + wSize);
			// -----
			int iSize = 2048;
			// -----
			return false;
		}
		// -----
		memcpy((void*)&this->m_byBuffer[this->m_wSize], pSrc, wSize);
		// -----
		this->m_wSize += wSize;
		// -----
		return true;
	}
	// -----
	int ExtractPacket(void* pTar)
	{
		WORD wSize;
		BYTE byTemp[2048];
		// -----
		switch ( this->m_byBuffer[0] )
		{
			case 0xC1:
				wSize = this->m_byBuffer[1];
				break;
				// -----
			case 0xC2:
				wSize = this->m_byBuffer[1] * 256 + this->m_byBuffer[2];
				break;
				// -----
			default:
				return TRUE;
				break;
		}
		// -----
		if ( this->m_wSize < wSize )
		{
			return 2;
		}
		// -----
		BYTE byXorFilter[32] = { 0xAB, 0x11, 0xCD, 0xFE, 0x18, 0x23, 0xC5, 0xA3, 0xCA, 0x33, 0xC1, 0xCC, 0x66, 0x67, 0x21, 0xF3, 0x32, 0x12, 0x15, 
								 0x35, 0x29, 0xFF, 0xFE, 0x1D, 0x44, 0xEF, 0xCD, 0x41, 0x26, 0x3C, 0x4E, 0x4D };
		// -----
		this->XorData(wSize-1, (m_byBuffer[0] == 0xC1 ? 2 : 3), -1);
		// -----
		memcpy(pTar, this->m_byBuffer, wSize);
		// -----
		this->m_wSize	-=	wSize;
		// -----
		memcpy(byTemp, &this->m_byBuffer[wSize], this->m_wSize);
		memcpy(this->m_byBuffer, byTemp, this->m_wSize);
		// -----
		return FALSE;
	}
	// -----
private:
	WORD	m_wSize;
	BYTE	m_byBuffer[2048];
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------------------------------------------------------------------------------