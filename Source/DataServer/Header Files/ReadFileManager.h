//----------------------------------------------------------------------------------
// # Project:	Zodea eMU Suite
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S
// # Hint:		[Aug 10 2011] This Script For Read File Manager of the Application.
// # More Info: This Script has been Modified By WolF in [Nov 27 2011]
//----------------------------------------------------------------------------------
#ifndef RFILE_MANGER_H
#define RFILE_MANGER_H

#include "StdAfx.H"


class CRFileManager
{
public:
	void				SetInfo();
	bool				CheckFile();
	int					GetInt(char* Selection, char* Key, char* szFileName);
	void				GetString(char* Dest, char* Selection, char* Key, char* szFileName);
	// -----
	char				szRFileName[MAX_READ_FILE][256];
private:
	char				String[255];
	char				szTemp[256];
	WIN32_FIND_DATAA	m_WFD;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
extern CRFileManager RFile;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* RFILE_MANGER_H */
// -------------------------------------------------------------------------------------------------------------------------------------------------------