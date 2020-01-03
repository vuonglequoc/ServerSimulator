//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Gate System Functions & Structures					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef GATE_H
#define GATE_H

#define MAX_GATES	512

class GCGateSystem
{
public:
	GCGateSystem(void);
	~GCGateSystem(void);

	void	InitGate();
	void	LoadGate(const char *filename);
	void	LoadGateB(LPSTR Buffer, int iSize);
	void	InsertGate(int iIndex,int iType,int iMapNum, int iX1,int iY1,int iX2,int iY2, int iTarget, int iDir,int iLevel);
	bool	IsGate(int GateNumber);
	int		GetGate(int GateIndex, BYTE& CordX, BYTE& CordY, BYTE& MapNumber, BYTE& Dir, short& Level);
	int		GetLevel(int GateNumber);
	bool	IsInGate(int aIndex, int GateNumber);
	bool	CheckGateLevel(int aIndex, int GateNumber);

	short iIndex[MAX_GATES];
	unsigned char iType[MAX_GATES];
	unsigned char iMapNum[MAX_GATES];
	unsigned char iX1[MAX_GATES];
	unsigned char iY1[MAX_GATES];
	unsigned char iX2[MAX_GATES];
	unsigned char iY2[MAX_GATES];
	short iTargetGate[MAX_GATES];
	unsigned char iDir[MAX_GATES];
	short iLevel[MAX_GATES];
};

extern GCGateSystem GateC;

#endif