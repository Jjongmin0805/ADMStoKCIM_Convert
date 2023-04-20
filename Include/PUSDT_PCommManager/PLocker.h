#pragma once

#ifdef COMPILEMODE_LINUX
#include <pthread.h>
#include <semaphore.h>
#endif
class AFX_EXT_CLASS CPLocker
{
public:
	CPLocker( unsigned int unIndex );
	~CPLocker(void);

	int																		GetIndex();

	void																	Lock();
	void																	Unlock();
	bool																	IsTryLock();
	int																		GetSemValue();

protected:
	int								m_unLockIndex;
	bool							m_bReult;

#ifdef COMPILEMODE_LINUX
//	Mutex
//	pthread_mutex_t					m_lock;

	sem_t							m_Lock;
#else
	CRITICAL_SECTION				m_Lock;
#endif

};

