//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu State Info Structure & Functions				//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_STATE_INFO_H
#define KANTURU_STATE_INFO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKanturuStateInfo
{
public:
	CKanturuStateInfo();
	virtual ~CKanturuStateInfo();

	void ResetTimeInfo()
	{
		this->SetStateInfo(0);
		this->SetCondition(0);
		this->SetValue(0);
	}


	void SetStateInfo(int iStateTimeInfo)
	{
		this->m_iState = iStateTimeInfo;
	}

	void SetCondition(int iCondition)
	{
		this->m_iCondition = iCondition;
	}


	int GetCondition()
	{
		return this->m_iCondition;
	}

	void SetValue(int iValue)
	{
		this->m_iValue = iValue;
	}

	int GetValue()
	{
		return this->m_iValue;
	}
	void SetAppliedTime()
	{
		this->m_dwAppliedTickCount = GetTickCount();
	}

	int GetAppliedTime()
	{
		return this->m_dwAppliedTickCount;
	}

	int GetElapsedTime()
	{
		int iElapsedTime = GetTickCount() - this->GetAppliedTime();
		return iElapsedTime;
	}

	int GetRemainTime()
	{
		int iRemainTime = 0;

		if ( this->GetCondition() == 1 )
		{
			iRemainTime = this->m_iValue - this->GetElapsedTime();

			if ( iRemainTime <= 0 )
			{
				return 0;
			}

			iRemainTime /= 1000;
		}

		return iRemainTime;
	}

	BOOL IsTimeOut()
	{
		if ( this->GetRemainTime() <= 0 )
		{
			return TRUE;
		}

		return FALSE;
	}
private:

	int m_iState;
	int m_iCondition;
	int m_iValue;
	DWORD m_dwAppliedTickCount;
};


#endif