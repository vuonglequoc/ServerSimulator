#ifndef DS_SRV_MNG_HEADER
#define DS_SRV_MNG_HEADER

#include "StdAfx.H"


class DServerManager : public IOCP
{
public:
	DServerManager(void)			{}
	virtual ~DServerManager(void)	{}
	// -----
	void OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
extern DServerManager DS_SrvManager[];
// ------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* DS_SRV_MNG_HEADER */
// ------------------------------------------------------------------------------------------------------------------------------------------------------