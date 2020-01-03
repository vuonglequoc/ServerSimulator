#include "StdAfx.H"
#include "ReadFileManager.H"
#include "LogProc.H"

CRFileManager RFile;

void CRFileManager::SetInfo()
{
	sprintf( szRFileName[0],  ".\\DataServer.conf" );

	if ( this->CheckFile() == true )
	{
		CLog.Load("File Manager Successfully Loaded.");
	}
}
bool CRFileManager::CheckFile()
{
	for( int n=0; n<MAX_READ_FILE; n++ )
	{
		if( FindFirstFile ( szRFileName[n] , &m_WFD ) == INVALID_HANDLE_VALUE )
		{
			CLog.Error("File not found '%s'...!", szRFileName[n]);
			return false;
		}
	}
	return true;
}
int CRFileManager::GetInt(char* Selection, char* Key, char* szFileName)
{
	return GetPrivateProfileIntA(Selection, Key, NULL, szFileName);
}
void CRFileManager::GetString(char* Dest, char* Selection, char* Key, char* szFileName)
{
	GetPrivateProfileStringA( Selection, Key, NULL, this->String, sizeof(this->String), szFileName );
	memcpy( Dest,this->String, sizeof(this->String));
	ZeroMemory( this->String, sizeof(this->String));
}
