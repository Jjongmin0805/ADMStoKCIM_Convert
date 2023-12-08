#pragma once

class CPWThread;
typedef								vector<CPWThread*>						VECTHREAD;

class CPDSETComm_SVR
{
public:
	CPDSETComm_SVR(void);
	~CPDSETComm_SVR(void);

	bool																	Begin();
	CPLocker*																GetLockSendRecv();
	socket_t*																GetCommSocket();

	bool																	SEND_CommData( const char* szCNTId, const char* szMessage );
	bool																	SEND_CommData( const char* szCNTId, const char* szMessage, short snSeqNo );
	bool																	SEND_CommData( const char* szCNTId, unsigned int unBuffSize, char* pBuff );
	bool																	SEND_CommData( const char* szCNTId, unsigned int unBuffSize, char* pBuff, short snSeqNo );
	bool																	SEND_CommAck( const char* szCNTId, short snSeqNo );

protected : 
	void																	CloseThread();
	void																	RunThread();
	static UINT WINAPI														SENDThread_Process( LPVOID pParam );
	static UINT WINAPI														RECVThread_Process( LPVOID pParam );

	bool																	RECV_CommData( socket_t* pCommSocket );
	bool																	CheckSendTimeout( short snSeqNo );
	void																	GetSeqNo();

protected : 
	short							m_snSendSeqNo;
	context_t						p_context;
	socket_t						*m_pCOMMSOCK;
	VECTHREAD						m_vecCommThreads;

	CPLocker						*m_pLock_SendRecv;
};



