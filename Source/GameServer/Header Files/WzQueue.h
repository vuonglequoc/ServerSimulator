//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU WzQueue Structures									//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef WZQUEUE_H
#define WZQUEUE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct _ListNode
{
	_ListNode * lpNodePre;
	_ListNode * lpNodeNext;
	char* pHeapMemory;
	UINT nSize;
	BOOL bIsDataSending;
	int iBytesSended;
	BYTE Headcode;
	UINT uIndex;

}LISTNODE, * LPLISTNODE;

class WZQueue
{
public:
	WZQueue(UINT nMaxNode);
	~WZQueue();
	UINT GetCount();
	int AddToQueue(LPBYTE const pObject , UINT nSize, BYTE headcode, int uindex);
	LPLISTNODE AddToQueueList(LPBYTE pObject, UINT nSize, BYTE headcode, int uindex);
	int GetFromQueue(LPBYTE pObject, UINT* pSize, BYTE* headcode,  int* uindex);
	void Init();
	void Free();
	int StartQ();
	int NextQ();
	LPLISTNODE GetCurData(LPBYTE pObject, UINT* pSize, BYTE * headcode, int* uindex);
	int Pop( LPLISTNODE pCur, LPBYTE pObject, int nOfs, int* nSize, int* sentbytes);
	void DeleteNode( LPLISTNODE pCur);
	void SetCurDataSending( LPLISTNODE pNode);

private:
	LPLISTNODE GetHeadNode();
	int AddTail( LPLISTNODE pNewNode);
	void DeleteAllNodes();

private:
	UINT m_Count;
	UINT m_Max_Node;
	LPLISTNODE m_pHead;
	LPLISTNODE m_pTail;
	LPLISTNODE m_pCur;
	CRITICAL_SECTION m_CriticalSection;
};

#endif