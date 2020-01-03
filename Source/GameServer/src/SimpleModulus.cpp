/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Decrypted Packets For Zodea GameServer
 * Coded By:	WolF & M.E.S
 */
#include "stdafx.h"


CSimpleModulus	g_SimpleModulusCS;
CSimpleModulus	g_SimpleModulusSC;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD CSimpleModulus::s_dwSaveLoadXOR[4] = { 0x3F08A79B, 0xE25CC287, 0x93D27AB9, 0x20DEA7BF };
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CSimpleModulus::CSimpleModulus()
{ 
	this->Init(); 
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CSimpleModulus::~CSimpleModulus()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
VOID CSimpleModulus::Init()
{
	memset(this->m_dwEncryptionKey, 0, sizeof(this->m_dwEncryptionKey));
	// -----
	memset(this->m_dwModulus, 0, sizeof(this->m_dwModulus));
	// -----
	memset(this->m_dwDecryptionKey, 0, sizeof(this->m_dwDecryptionKey));
} 
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CSimpleModulus::Encrypt(void* lpDest, void* lpSource, int iSize)
{
	int		iOriSize;
	int		iTempSize2;	
	int		iTempSize		= iSize;
	int		iDec			= ((iSize+7)/8);
	UCHAR * lpTempDest		= (LPBYTE)lpDest;
	UCHAR * lpTempSource	= (LPBYTE)lpSource;
	// -----
	iSize = (iDec + iDec*4)*2 + iDec;
	// -----
	if (lpDest != NULL)
	{
		iOriSize = iTempSize;
		// -----
		for (int i = 0; i < iTempSize; i += 8, iOriSize -= 8, lpTempDest +=11)
		{
			iTempSize2 = iOriSize;
			if (iOriSize >= 8)
				iTempSize2 = 8;

			this->EncryptBlock(lpTempDest, lpTempSource+i, iTempSize2);
		}
	}
	// -----
	return iSize;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CSimpleModulus::Decrypt(void* lpDest, void* lpSource, int iSize)
{
	int iResult = 0;
	int iDecLen = 0;
	// -----
	if (lpDest == NULL)
	{
		return iSize*8/11;
	}
	// -----
	LPBYTE lpTempDest	= (LPBYTE)lpDest;
	LPBYTE lpTempSrc	= (LPBYTE)lpSource;
	// -----
	if ( iSize > 0 )
	{
		while ( iDecLen < iSize )
		{
			int iTempResult = this->DecryptBlock(lpTempDest, lpTempSrc);
			// -----
			if ( iResult < 0 )
			{
				return iResult;
			}
			// -----
			iResult		+= iTempResult;
			iDecLen		+= 11;
			lpTempSrc	+= 11;
			lpTempDest	+= 8;
		}
	}
	// -----
	return iResult;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CSimpleModulus::EncryptBlock(void*lpDest,void*lpSource,int iSize)
{
	DWORD	dwEncBuffer[4];
	DWORD	dwEncValue	= 0;
	LPBYTE	lpEncDest	= (LPBYTE)lpDest;
	LPBYTE	lpEncSource	= (LPBYTE)lpSource;
	int		iBitPos		= 0;
	BYTE	btCheckSum	= 0xF8;
	// -----
	memset(lpEncDest, 0, 11);
	// -----
	for ( int i=0;i<4;i++)
	{
		dwEncBuffer[i]=((this->m_dwXORKey[i]^((WORD*)lpEncSource)[i]^dwEncValue)*this->m_dwEncryptionKey[i])%this->m_dwModulus[i];
		dwEncValue=dwEncBuffer[i]&0xFFFF;
	}
	// -----
	for (int i=0;i<3;i++)
	{
		dwEncBuffer[i] = dwEncBuffer[i] ^ this->m_dwXORKey[i] ^ ( dwEncBuffer[i+1] & 0xFFFF );
	}
	// -----
	for (int i=0;i<4;i++)
	{
		iBitPos = this->AddBits(lpDest, iBitPos, &dwEncBuffer[i], 0, 16);
		iBitPos = this->AddBits(lpDest, iBitPos, &dwEncBuffer[i], 22, 2);
	}
	// -----
	for (int i=0;i<8;i++)
		btCheckSum ^= lpEncSource[i];
	// -----
	((LPBYTE)&dwEncValue)[1] = btCheckSum ; 
	((LPBYTE)&dwEncValue)[0] = btCheckSum ^ iSize ^ 0x3D; 
	// -----
	return this->AddBits(lpDest, iBitPos, &dwEncValue, 0, 16);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CSimpleModulus::DecryptBlock(void*lpDest,void*lpSource)
{
	DWORD	Temp1;
	DWORD	dwDecBuffer[4]	= {0};
	int		iBitPosition	= 0;
	LPBYTE	lpDecDest		= (LPBYTE)lpDest;
	LPBYTE	lpDecSource		= (LPBYTE)lpSource;
	DWORD	Temp			= 0;
	BYTE	btCheckSum		= 0xF8;
	// -----
	memset(lpDest, 0, 8);
	// -----
	for(int i=0;i<4;i++)
	{
		this->AddBits(&dwDecBuffer[i], 0, lpDecSource, iBitPosition, 16);
		iBitPosition += 16;
		this->AddBits(&dwDecBuffer[i], 22, lpDecSource, iBitPosition, 2);
		iBitPosition += 2;
	}
	// -----
	for(int i=2;i>=0;i--)
	{
		dwDecBuffer[i] = dwDecBuffer[i] ^ this->m_dwXORKey[i] ^ (dwDecBuffer[i+1]&0xFFFF);
	}
	// -----
	for(int i=0;i<4;i++)
	{
		Temp1 = (( this->m_dwDecryptionKey[i] * (dwDecBuffer[i])) % ( this->m_dwModulus[i])) ^ this->m_dwXORKey[i]^Temp;
		Temp = dwDecBuffer[i]&0xFFFF;
		((WORD*)lpDecDest)[i] = (WORD)(Temp1);
	}
	// -----
	dwDecBuffer[0]=0;
	// -----
	this->AddBits(&dwDecBuffer[0], 0, lpDecSource, iBitPosition, 16);
	// -----
	((LPBYTE)dwDecBuffer)[0]=((LPBYTE)dwDecBuffer)[1]^ ((LPBYTE)dwDecBuffer)[0]^0x3D;
	// -----
	for(int i=0;i<8;i++)
		btCheckSum ^= lpDecDest[i];
	// -----
	if( btCheckSum != ((LPBYTE)dwDecBuffer)[1] )
		return -1;
	// -----
	return ((LPBYTE)dwDecBuffer)[0];
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CSimpleModulus::AddBits(void*lpDest,int iDestBitPos,void*lpSource,int iBitSourcePos,int iBitLen)	// Completed Emulated
{
	int iSourceBufferBitLen = iBitLen + iBitSourcePos ;
	int iTempBufferLen		= this->GetByteOfBit(iSourceBufferBitLen-1);
	// -----
	iTempBufferLen			+= 1 - this->GetByteOfBit(iBitSourcePos);
	// -----
	UCHAR * pTempBuffer		= new UCHAR[iTempBufferLen+1];
	// -----
	memset(pTempBuffer, 0, iTempBufferLen+1);
	memcpy(pTempBuffer, (UCHAR *)lpSource+ this->GetByteOfBit(iBitSourcePos), iTempBufferLen);
	// -----
	if ( (iSourceBufferBitLen%8 ) != 0 )
	{
		pTempBuffer[iTempBufferLen - 1] &= 255 << (8 - (iSourceBufferBitLen%8));
	}
	// -----
	int iShiftLeft			= (iBitSourcePos%8);
	int iShiftRight			= (iDestBitPos%8);
	// -----
	this->Shift(pTempBuffer, iTempBufferLen, -iShiftLeft);
	this->Shift(pTempBuffer, iTempBufferLen+1, iShiftRight);
	// -----
	int iNewTempBufferLen	= (( iShiftRight <= iShiftLeft )?0:1) + iTempBufferLen;
	UCHAR * TempDist		= (UCHAR *)lpDest + this->GetByteOfBit(iDestBitPos);
	// -----
	for ( int i=0;i<iNewTempBufferLen;i++)
	{
		TempDist[i] |= pTempBuffer[i];
	}
	// -----
	delete [] pTempBuffer;
	// -----
	return iDestBitPos + iBitLen;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
VOID CSimpleModulus::Shift(void*lpBuff,int iSize,int ShiftLen) // Need To be Checked
{
	UCHAR * TempBuff = (UCHAR *)lpBuff;
	// -----
	if ( ShiftLen != 0  )	
	{
		if ( ShiftLen > 0 )
		{
			if ( (iSize -1 ) > 0 )
			{
				for ( int i=(iSize-1); i>0 ; i--)
				{
					TempBuff[i] = (TempBuff[i-1]<<((8 - ShiftLen))) | (TempBuff[i]>>ShiftLen );
				}
			}
			// -----
			TempBuff[0] >>=  ShiftLen;
		}
		else
		{
			ShiftLen = - ShiftLen;
			// -----
			if ( (iSize-1) > 0 )
			{
				for ( int i=0; i<(iSize-1) ;i++)
				{
					TempBuff[i] = (TempBuff[i+1]>>((8 - ShiftLen))) | (TempBuff[i]<<ShiftLen );
				}
			}
			// -----
			TempBuff[iSize-1] <<= ShiftLen;
		}
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CSimpleModulus::GetByteOfBit(int btByte)
{
	return btByte >> 3;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSimpleModulus::SaveAllKey(LPSTR lpszFileName )
{
	return this->SaveKey(lpszFileName, 4370, TRUE, TRUE, TRUE, TRUE);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSimpleModulus::LoadAllKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, TRUE, TRUE, TRUE);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSimpleModulus::SaveEncryptionKey(LPSTR lpszFileName)
{
	return this->SaveKey(lpszFileName, 4370, TRUE, TRUE, FALSE, TRUE);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSimpleModulus::LoadEncryptionKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, TRUE, FALSE ,TRUE);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSimpleModulus::SaveDecryptionKey(LPSTR lpszFileName)
{
	return this->SaveKey(lpszFileName, 4370, TRUE, FALSE, TRUE, TRUE);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSimpleModulus::LoadDecryptionKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, FALSE ,TRUE ,TRUE);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSimpleModulus::SaveKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bSaveModulus, BOOL bSaveEncKey, BOOL bSaveDecKey, BOOL bSaveXORKey)
{
	ENCDEC_FILEHEADER HeaderBuffer;
	// -----
	int		iSize;
	DWORD	XORTable[4];
	HANDLE	hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0x80, NULL);
	// -----
	if ( hFile == INVALID_HANDLE_VALUE ) return FALSE;
	// -----
	HeaderBuffer.sFileHeader	= wFileHeader;
	HeaderBuffer.dwSize			= ( bSaveModulus + bSaveEncKey + bSaveDecKey + bSaveXORKey ) * sizeof(XORTable) + sizeof(ENCDEC_FILEHEADER);
	// -----
	WriteFile( hFile, &HeaderBuffer, sizeof(ENCDEC_FILEHEADER), (ULONG*)&iSize, NULL);
	// -----
	if ( bSaveModulus != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwModulus[n] ^ s_dwSaveLoadXOR[n];
		}
		// -----
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}
	// -----
	if ( bSaveEncKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwEncryptionKey[n] ^ s_dwSaveLoadXOR[n];
		}
		// -----
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}
	// -----
	if ( bSaveDecKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwDecryptionKey[n] ^ s_dwSaveLoadXOR[n];
		}
		// -----
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}
	// -----
	if ( bSaveXORKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwXORKey[n] ^ s_dwSaveLoadXOR[n];
		}
		// -----
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}
	// -----
	CloseHandle(hFile);
	// -----
	return TRUE;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSimpleModulus::LoadKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bLoadModulus, BOOL bLoadEncKey, BOOL bLoadDecKey, BOOL bLoadXORKey)
{
	ENCDEC_FILEHEADER HeaderBuffer;
	// -----
	int		iSize;
	DWORD	XORTable[4];
	HANDLE	hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0x80, NULL);
	// -----
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		ReadFile( hFile, &HeaderBuffer, sizeof(ENCDEC_FILEHEADER), (ULONG*)&iSize, NULL);
	}
	// -----
	if ( HeaderBuffer.sFileHeader == wFileHeader  )
	{
		if ( (HeaderBuffer.dwSize) == (int)((( bLoadModulus + bLoadEncKey + bLoadDecKey + bLoadXORKey )*sizeof(XORTable))+sizeof(ENCDEC_FILEHEADER)) )
		{
			if ( bLoadModulus != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				// -----
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwModulus[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			// -----
			if ( bLoadEncKey != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwEncryptionKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			// -----
			if ( bLoadDecKey != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwDecryptionKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			// -----
			if ( bLoadXORKey != FALSE )
			{
				ReadFile(hFile, XORTable,sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwXORKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			// -----
			CloseHandle(hFile);
			// -----
			return TRUE;
		}
	}
	else
	{
		CloseHandle(hFile);
	}
	// -----
	return FALSE;
}