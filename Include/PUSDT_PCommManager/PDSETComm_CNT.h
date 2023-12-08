#pragma once

class CPWThread;
typedef								vector<CPWThread*>						VECTHREAD;

class CPDSETComm_CNT
{
public:
	CPDSETComm_CNT(void);
	~CPDSETComm_CNT(void);

	bool																	Begin();					// 통신 시작 함수
	CPLocker*																GetLockSend();
	CPLocker*																GetLockRecv();
	socket_t*																GetCommSocket();			// 통신의 송신 zmq socket를 얻는 함수
	bool																	IsBegin();

	bool																	SEND_CommData( const char* szMessage );				// 송신 데이터를 처리하는 함수 1
	bool																	SEND_CommData( unsigned int unBuffSize, const char* pBuff );		// 송신 데이터를 처리하는 함수 1
	void																	SendCloseComm();

protected :
	void																	CloseThread();									// 쓰레드를 정지 또는 종료하는 함수
	void																	RunThread();									// 쓰레드를 수행하는 함수
	void																	CloseComm();									// 통신 Socket 종료 함수

	void																	SendRegistComm();

	void																	Thread_Run();									// 쓰레드를 수행하는 함수
	static UINT WINAPI														SENDThread_Process( LPVOID pParam );			// 송신 쓰레드 함수
	static UINT WINAPI														RECVThread_Process( LPVOID pParam );			// 수신 쓰레드 함수

	bool																	RECV_CommData( socket_t* pCommSocket );			// 수신 데이터의 header를 해석하기 위한 함수
	bool																	SEND_CommAck( short snSeqNo );					// 송신 데이터를 처리하는 함수 1
	bool																	CheckSendTimeout( short snSeqNo );				// 송신 무한 처리를 하지 않도록 타임아웃을 감시하는 함수
	void																	GetSeqNo();										// 송신. 수신에 사용되는 seq number를 부여하는 함수

protected : 
	short							m_snSendSeqNo;							// 송신에 필요한 seq number를 부여하기 위한 변수
	context_t						p_context;								// zmq에서 사용하고 context 변수
	socket_t						*m_pCOMMSOCK;							// 송신과 수신 전용으로 사용하고자 하는 zmq socket 변수
	VECTHREAD						m_vecCommThreads;

	bool							m_bBegin;
	CPLocker						*m_pLock_Send, *m_pLock_Recv;
};

