#pragma once

class AFX_EXT_CLASS CPWThread
{
public:
	CPWThread( int nIndex, void* pThreadCallFunc, void* pParent );
	~CPWThread(void);

	int																		GetID();
	void*																	GetParent();
	void																	CloseThread();
	bool																	IsThreadWorksRun();
	void																	Begin();

	int																		GetThreadProcessSleepTime();
	void																	SetThreadProcessSleepTime( int nSleepTime );

protected:
	int								m_nThreadId;
	HANDLE							m_hThread_WORKS;
	bool							m_bIsThreadWorksRun;
	int								m_nThreadProcessSleepTime;				// M-Sec

	void							*m_pThreadCallFunc;
	void							*m_pParent;

};

