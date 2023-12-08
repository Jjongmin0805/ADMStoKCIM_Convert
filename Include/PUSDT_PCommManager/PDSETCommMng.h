#pragma once

#include "PDSETComm_SVR.h"
#include "PDSETComm_CNT.h"

class CPDSETClientPeer
{
public:
	CPDSETClientPeer(void)
	{
		SetCheckUpdateTime();
	};
	CPDSETClientPeer( string szId ) : m_szIdentity( szId )
	{
		SetCheckUpdateTime();
	};
	CPDSETClientPeer( char *pStrId ) : m_szIdentity( pStrId )
	{
		SetCheckUpdateTime();
	};

	~CPDSETClientPeer(void)
	{
		//
	};
	void SetCheckUpdateTime(void)
	{
		m_pLastUpdateTm														= localtime( &m_LastUpdateTime );
	};
	string GetStrLastupdateTime(void)
	{
		char						szOutputMsg[ 1024 ]						= { 0, };
		strftime( szOutputMsg, sizeof( szOutputMsg ), "%Y/%m/%d %H:%M:%S", m_pLastUpdateTm );
		return string( szOutputMsg );
	};

	string							m_szIdentity;
	time_t							m_LastUpdateTime;
	tm								*m_pLastUpdateTm;
};
typedef								hash_map< string, CPDSETClientPeer* >	HMAPCLIENTPEER;

class CPDSETCommData;

class AFX_EXT_CLASS CPDSETCommMng
{
public:
	static CPDSETCommMng*													InitInstance( void );
	static void																Close( void ) ;

protected:
	static CPDSETCommMng*			m_pMngData;

//////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	CPDSETCommMng(void);
	~CPDSETCommMng(void);

	void																	Remove_lstPDSETCommData_SND();

	void																	SetIdentity( char* szId );
	char*																	GetIdentity();
	void																	SetIDPARAM( char* szIdParamData );
	CPString																GetIDPARAM();
	void																	SetMakeProcessIdentity();

	void																	PUTDATA_PDSETCOMM_SND( CPDSETCommData* pCommData );
	CPDSETCommData*															GETDATA_PDSETCOMM_SND();
	void																	PUTDATA_PDSETCOMM_RCV( CPDSETCommData* pCommData );
	CPDSETCommData*															GETDATA_PDSETCOMM_RCV();

	void																	Insert_ClientId( string szClientId );
	void																	Remove_ClientId( string szClientId );
	bool																	IsClientId( string szClientId );
	void																	SetHeartbeat_ClientId( string szClientId );
	int																		GetClientCount();
	int																		GetClientPeerID( vector< string > &vecClientIDs );
	time_t																	GetClientLastUpdate( string szClientId );

	int																		GetCommPort();
	void																	Begin_Server();
	void																	Begin_Client();
	void																	SetSendCompleteCheck( short snSeqNo );				// 송신후 수신 여부를 확인하는 ack의 seqno를 저장하는 함수
	short																	GetSendCompleteCheck();				// 송신후 수신 여부를 확인하는 ack의 seqno를 얻어오는 함수
	bool																	IsClientBegin();
	bool																	SEND_SVRCommData( const char* szCNTId, const char* szMessage, short snSeqNo = 0 );
	bool																	SEND_SVRCommData( const char* szCNTId, unsigned int unBuffSize, char* pBuff, short snSeqNo = 0 );
	bool																	SEND_SVRCommACK( const char* szCNTId, short snSeqNo );

	bool																	SEND_CNTCommData( const char* szMessage );
	bool																	SEND_CNTCommData( unsigned int unBuffSize, char* pBuff );
	void																	SEND_CNTCloseCMDData();

private:
	void																	Init_Comm();

	void																	RemoveComms( int nMode );
	void																	Remove_ClientId();
	void																	Remove_lstPDSETCommData();
	void																	Remove_lstPDSETCommData_RCV();

	CPDSETClientPeer*														GetClientId( string szClientId );

private:
	list< CPDSETCommData* >			m_lstPDSETCommData_SND, m_lstPDSETCommData_RCV;
	HMAPCLIENTPEER					m_hmapClientPeers;
	CPDSETComm_SVR					*m_pCommSVR;
	CPDSETComm_CNT					*m_pCommCNT;

	int								m_nCommMode;
	char							m_szIdentity[ 64 ];
	CPString						m_szIDPARAM;									// ID외에 추가로 송신해야하는 Data
	int								m_nCommPort;
	int								m_nSNDLockerId, m_nRCVLockerId;
	short							m_snSendCompleteCheck;								// 송신후 정상적으로 수신 여부를 확인하고 있는 ack의 seqno를 확인하는 변수
};

#define								thePDSETCommMNG							CPDSETCommMng::InitInstance()
