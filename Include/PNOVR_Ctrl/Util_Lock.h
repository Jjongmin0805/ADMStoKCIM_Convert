#pragma once
class AFX_EXT_CLASS CUtil_Lock
{
public:
	CUtil_Lock( int nMode );
	CUtil_Lock( int nMode, TCHAR* szName );
	~CUtil_Lock(void);

	void																							Lock( void );
	void																							UnLock( void );

protected:
	int								m_nMode;

	HANDLE							m_hLock_Mutex;
	CRITICAL_SECTION				m_Lock_CS;
};

