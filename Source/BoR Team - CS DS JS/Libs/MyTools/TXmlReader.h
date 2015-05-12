//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef XMLREADER_H
#define XMLREADER_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include <libxml/parser.h>
#include <windows.h>
#include <string.h>
#include <assert.h>
//-------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_TEXT_LEN 1024
//-------------------------------------------------------------------------------------------------------------------------------------------

class xmlReader
{
private:
	char m_szFileName[MAX_TEXT_LEN];
public:
	xmlNodePtr m_thisNode;
	xmlNodePtr m_nextNode;
	xmlDocPtr  m_hFile;
	// ----
	xmlReader(char * szFileName);
	~xmlReader(void);
	// ----
	bool		Load		();
	void		UnLoad		();
	bool		EnterNode	(char * szNodeName);
	void		LeaveNode	();
	bool		GetBool		(char * szName);
	int			GetInt		(char * szName);
	const char*	GetStr		(char * szName);
	// ----
	bool		GetBoolProp	(xmlNodePtr pNode, char * szName);
	int			GetIntProp	(xmlNodePtr pNode, char * szName);
	const char*	GetStrProp	(xmlNodePtr pNode, char * szName);
};

//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* XML_READER_H */
//-------------------------------------------------------------------------------------------------------------------------------------------