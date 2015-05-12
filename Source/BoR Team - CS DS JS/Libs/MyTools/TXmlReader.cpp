#include "TXmlReader.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
//# Made by Leo123[BoR]
//-------------------------------------------------------------------------------------------------------------------------------------------

xmlReader::xmlReader(char * szFileName)
{
	memset(m_szFileName, 0, sizeof(m_szFileName));
	memcpy(m_szFileName, szFileName , sizeof(m_szFileName));;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

xmlReader::~xmlReader(void)
{

}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool xmlReader::Load()
{
	bool bReturn = false;
	// ----
	xmlInitMemory();
	// ----
	m_hFile = xmlParseFile(m_szFileName);
	// ----
	if(m_hFile)
	{
		m_thisNode = xmlDocGetRootElement(m_hFile);
		m_nextNode = m_thisNode->next;
		// ----
		bReturn = true;
	}
	else
	{
		MessageBoxA(0, "error", "[xmlReader::Load]", MB_OK);
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void xmlReader::UnLoad()
{
	xmlFreeDoc(m_hFile);
	xmlCleanupMemory();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool xmlReader::EnterNode(char * szNodeName)
{
	bool bReturn = false;
	// ----
	xmlNodePtr subChild = m_thisNode->children;
	// ----
	while(subChild)
	{
		if(xmlStrcmp(subChild->name, (const xmlChar*)szNodeName) == 0)
		{
			m_thisNode = subChild;
			m_nextNode = m_thisNode->next;
			// ----
			bReturn = true;
			break;
		}
		// ----
		subChild = subChild->next;
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void xmlReader::LeaveNode()
{
	m_thisNode = m_thisNode->parent;
	m_nextNode = m_thisNode->next;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool xmlReader::GetBool(char * szName)
{
	bool bReturn = false;
	// ----
	xmlNodePtr subChild = m_thisNode->children;
	// ----
	while(subChild)
	{
		if(xmlStrcmp(subChild->name, (const xmlChar*)szName) == 0)
		{
			if(!strcmpi((char*)subChild->children->content, "true"))  bReturn = true;
			if(!strcmpi((char*)subChild->children->content, "false")) bReturn = false;
			// ----
			break;
		}
		// ----
		subChild = subChild->next;
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

int xmlReader::GetInt(char * szName)
{
	int iReturn = false;
	// ----
	xmlNodePtr subChild = m_thisNode->children;
	// ----
	while(subChild)
	{
		if(xmlStrcmp(subChild->name, (const xmlChar*)szName) == 0)
		{
			iReturn = atoi((char*)subChild->children->content);
			// ----
			break;
		}
		// ----
		subChild = subChild->next;
	}
	// ----
	return iReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

const char* xmlReader::GetStr(char * szName)
{
	const char * cReturn = NULL;
	xmlNodePtr subChild = m_thisNode->children;
	// ----
	while(subChild)
	{
		if(xmlStrcmp(subChild->name, (const xmlChar*)szName) == 0)
		{
			int Len = strlen((char *)subChild->children->content);
			// ----
			cReturn = (const char*)subChild->children->content;
			// ----
			break;
		}
		// ----
		subChild = subChild->next;
	}
	// ----
	return cReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool xmlReader::GetBoolProp(xmlNodePtr pNode, char *szName)
{
	bool bReturn = false;
	// ----
	char* sValue = (char*)xmlGetProp(pNode, (xmlChar*)szName);
	// ----
	if(sValue != NULL)
	{
		if(!strcmpi(sValue, "true"))  bReturn = true;
		if(!strcmpi(sValue, "false")) bReturn = false;
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

int xmlReader::GetIntProp(xmlNodePtr pNode, char *szName)
{
	int iReturn = -1;
	// ----
	char* sValue = (char*)xmlGetProp(pNode, (xmlChar*)szName);
	// ----
	if(sValue != NULL)
	{
		iReturn = atoi(sValue);
	}
	// ----
	return iReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

const char* xmlReader::GetStrProp(xmlNodePtr pNode, char *szName)
{
	return (const char*)xmlGetProp(pNode, (xmlChar*)szName);
}
//-------------------------------------------------------------------------------------------------------------------------------------------