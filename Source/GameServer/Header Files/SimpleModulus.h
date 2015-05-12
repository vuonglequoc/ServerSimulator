/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Decrypted Packets For Zodea GameServer
 * Coded By:	WolF & M.E.S
 */
#ifndef SIMPLE_MODULUS_HEADER
#define SIMPLE_MODULUS_HEADER


#pragma pack(1)
struct ENCDEC_FILEHEADER
{
	short	sFileHeader;
	int		dwSize;
};
// -----
typedef DWORD dwXORTable[4];
#pragma pack()
// -------------------------------------------------------------------------------------------------------------------------------------------------------
class CSimpleModulus
{
public:
	CSimpleModulus();
	virtual ~CSimpleModulus();
	// -----
	void	Init();
	DWORD	m_dwModulus[4];
	DWORD	m_dwEncryptionKey[4];
	DWORD	m_dwDecryptionKey[4];
	DWORD	m_dwXORKey[4];
	int		Encrypt(void * lpDest, void * lpSource, int iSize);
	int		Decrypt(void * lpDest, void * lpSource, int iSize);
	BOOL	SaveAllKey(LPSTR lpszFileName);
	BOOL	LoadAllKey(LPSTR lpszFileName);
	BOOL	SaveEncryptionKey(LPSTR lpszFileName);
	BOOL	LoadEncryptionKey(LPSTR lpszFileName);
	BOOL	SaveDecryptionKey(LPSTR lpszFileName);
	BOOL	LoadDecryptionKey(LPSTR lpszFileName);
	// -----
protected:
	static	DWORD s_dwSaveLoadXOR[4];
	int		EncryptBlock(void*, void*, int);
	int		DecryptBlock(void*, void*);
	int		CSimpleModulus::AddBits(void* lpDest, int iDestBitPos, void* lpSource, int iBitSourcePos, int iBitLen);
	void	Shift(void* lpBuff, int iSize, int ShiftLen);
	int		GetByteOfBit(int);
	BOOL	SaveKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bSaveModulus, BOOL bSaveEncKey, BOOL bSaveDecKey, BOOL bSaveXORKey);
	BOOL	LoadKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bLoadModulus, BOOL bLoadEncKey, BOOL bLoadDecKey, BOOL bLoadXORKey);
};
extern CSimpleModulus	g_SimpleModulusCS;
extern CSimpleModulus	g_SimpleModulusSC;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------------------------------------------------------------------