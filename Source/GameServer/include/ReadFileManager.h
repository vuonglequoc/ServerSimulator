/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Read File Manager For Zodea GameServer
 * Coded By:	WolF & M.E.S
 */
#ifndef R_FILE_MANG_HEADER
#define R_FILE_MANG_HEADER


#define	MAX_FILE_NAME		64
#define	MAX_FILE_LEN		256
// -------------------------------------------------------------------------------------------------------------------------------------------------------
class CRFileManager
{
public:
	CRFileManager();
	virtual ~CRFileManager();
	// -----
	void	RFSetInfo(int iCountryCode);
	void	RFCheckFile();
	int		GetInt(char* Selection, char* Key, char* szFileName);
	void	GetString(char* Dest, char* Selection, char* Key, char* szFileName);
	// -----
	char	szRFCountryName[4];
	char	szRFileName[MAX_FILE_NAME][256];
private:
	char *	szLanguage;
	char	String[255];
	char	szTemp[256];
};
extern CRFileManager RFile;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------------------------------------------------------------------