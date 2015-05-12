//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef CTIMER_H
#define CTIMER_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include <Windows.h>
//-------------------------------------------------------------------------------------------------------------------------------------------

#define	LPTHREAD				LPTHREAD_START_ROUTINE
//-------------------------------------------------------------------------------------------------------------------------------------------

class CTimer
{
private:
	ULONG	m_Tick;
	HANDLE	m_hThread;
	bool	m_bWorker;
	// ----
	ULONG	m_nMilliseconds;
	ULONG	m_nSeconds;
	ULONG	m_nMinutes;
	ULONG	m_nHours;
	ULONG	m_nDays;
public:
	CTimer(void);
	~CTimer(void);
	// ----
	void			Init();
	// ----
	static void		TimerWorker(LPVOID lpParam);
	// ----
	virtual void	OnSecond(){};
	virtual void	OnMinute(){};
	virtual void	OnHour(){};
	virtual void	OnDay(){};
	// ----
	inline ULONG	getMilliseconds(){return (m_Tick - GetTickCount());};
	inline ULONG	getSeconds(){return m_nSeconds;};
	inline ULONG	getMinutes(){return m_nMinutes;};
	inline ULONG	getHours(){return m_nHours;};
	inline ULONG	getDays(){return m_nDays;};
};
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------------------------------------------------