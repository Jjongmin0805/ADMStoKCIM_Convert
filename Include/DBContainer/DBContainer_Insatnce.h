#pragma once

#include <atldbcli.h>
#include <atldbsch.h>
#include "DBContainer_Object.h"

class AFX_EXT_CLASS CDBContainer_Insatnce
{
public:
	CDBContainer_Insatnce(void);
	~CDBContainer_Insatnce(void);
	// CDBContainer_Insatnce를 광역으로 사용할 수 있도록 지정
	static CDBContainer_Insatnce*															Instance( void );
	static void																				Close(void);

protected : 
	static CDBContainer_Insatnce*	m_pMngData;												//싱글톤을 전역으로 사용하기 위하여 설정

public:
	// DB 설정 읽기
	DB_VECSTR																				LoadingDBConfig();

	// 저장위치 설정( 전위치, 후위치 )
	_tstring																				GetMyPath( _tstring szPath, _tstring szFile = _T("") );
	// 문자열을 소문자로 변경( 문자열 )
	bool																					TranformLow( wchar_t* szValue );
	// 문자열을 대문자로 변경( 문자열 )
	bool																					TranformHigh( wchar_t* szValue );
	// str Trim
	void																					strTrim( _tstring& szSource, _tstring szTarget, _tstring szRep );
	// DB 연결 여부 설정
	void																					SetConnectStatus( int nIndex, BOOL bStatus );
	// DB 연결 여부
	BOOL																					IsConnect( int nIndex );
	// DB 연결( DB 정보 )
	BOOL																					ConnectDB( bool bAutoReconnect = false );
	// DB 연결
	BOOL																					ConnectDB( int nIndex );
	// DB 연결 해제
	BOOL																					DisConnectDB( int nIndex );
	// DB 재연결
	BOOL																					ReConnectDB( int nIndex );


	// DB DataSource 얻기
	CDataSource*																			GetDataSource( int nIndex );
	// DB Session 얻기
	CSession*																				GetSession( int nIndex );
	// DB Object 갯수 얻기
	int																						GetDBObjectCount();
	// 연결 종류 얻기(MSSQL:1, Orcle:2, ADO:3)
	int																						GetSessionConnectSourceMode( int nIndex );
	bool																					IsCheckDBConnect( int nIndex );

	// 마지막 오류 메세지 남기기
	CString																					GetLastErrMsg();
	void																					SetLastErrMsg( int nIndex, CString szErrMsg );

	void																					SetLogMutexName( _tstring szName );
	_tstring																				GetLogFileName();
	void																					SetLogFileName( _tstring szFileName );
	void																					WriteLog( const wchar_t *msg, ... );
	void																					WriteLog( const char *msg, ... );
	void																					WriteLog2File( char* szFilePath, char* szMsg );

	bool																					SetValue2String( unsigned long long* ullDblData, int nDataSize, char* pRtBuff );
	bool																					SetValue2String( unsigned long long* ullDblData, int nDataSize, wchar_t* pRtBuff );
	bool																					SetValue2String( double* pDblData, int nDataSize, char* pRtBuff );
	bool																					SetValue2String( double* pDblData, int nDataSize, wchar_t* pRtBuff );
	bool																					SetValue2String( int* pDblData, int nDataSize, char* pRtBuff );
	bool																					SetValue2String( int* pDblData, int nDataSize, wchar_t* pRtBuff );

	bool																					SetStrings2Value( char* pRtBuff, unsigned long long* pDblData, int nDataSize );
	bool																					SetStrings2Value( wchar_t* pRtBuff, unsigned long long* pDblData, int nDataSize );
	bool																					SetStrings2Value( char* pRtBuff, double* pDblData, int nDataSize );
	bool																					SetStrings2Value( wchar_t* pRtBuff, double* pDblData, int nDataSize );
	bool																					SetStrings2Value( char* pRtBuff, int* pDblData, int nDataSize );
	bool																					SetStrings2Value( wchar_t* pRtBuff, int* pDblData, int nDataSize );

	bool																					DeleteAllFilesInFolder( TCHAR* szDeleteFolder, TCHAR* szFileExtend, bool bDoAsk = false );

	CString																					GetConfigProvider( int nIndex );
	CString																					GetConfigDataSource( int nIndex );
	CString																					GetConfigDBName( int nIndex );
	CString																					GetConfigUserID( int nIndex );
	CString																					GetConfigPassWord( int nIndex );
	CString																					GetConfigFailOver( int nIndex );
	CString																					GetConfigActiveDataSource( int nIndex );
	CString																					GetConfigServerIP( int nIndex );
	int																						GetConfigServerPort( int nIndex );
	void																					SetBCopyFolderName( CString szFolderName );
	CString																					GetBCopyFolderName();
	void																					CheckBCopyFolder();
	BOOL																					GetProcessLive( CString sProcessName );
	BOOL																					KillProcess( CString szProcessName );
	int																						MariaDB_UploadFile( int nDBSourceIndex, char *szPathFile, char *szUploadData_TableName, char *szUploadData_FieldName, char *szUploadSize_FieldName, char *szUploadData_WhereData );
	int																						MariaDB_DownloadFile( int nDBSourceIndex, char *szDownloadFile_Path, char *szDownloadFile_Name, char *szDN_DBTableName, char *szDN_DBFieldName, char *szDN_DBWhereData );
	void																					SetIsThread_CheckDBConnectStatus( bool bIsStatus );
	bool																					IsThread_CheckDBConnectStatus();
	void																					CloseThread_CheckDBConnectStatus();
	void																					RunThread_CheckDBConnectStatus();
	void																					SetAutoConnection( bool bIsAutoConnection );
	bool																					GetAutoConnection();
	MYSQL*																					GetMySQLConn( int nIndex );
	string																					StrConvert_MultiCharToUFT8( const char* pszInString );
	string																					StrConvert_UFT8ToMultiChar( const char* pszInString );
	void																					SetAutoConnection_Lock();
	void																					SetAutoConnection_Unlock();

	int MariaDB_GetTest();

protected : 
	void																					RemoveData();
	BOOL																					GetProcessModule( DWORD dwPID, CString sProcessName );
	bool																					CreateRealDirectory( const char* szPath );

	static UINT WINAPI																		RunThreadProcess_CheckDBConnectStatus( LPVOID pParam );

protected : 
	VECDBC_OBJ						m_vecObjs;

	HANDLE							m_hLockMutex_Log;							// Log 파일 보호 핸들
	_tstring						m_szLogMutexName;							// Log Lock Mutext Name
	_tstring						m_szLogFileName;							// Log 파일 이름
	int								m_nLastDBIndex;								// 마지막 선택된 DB Source Index 지정
	CString							m_szBCopyFolder;							// Binary Copy 폴더 명칭
	HANDLE							m_hThread_CheckDBConnectStatus;				// DB 연결 여부 확인 쓰레드 핸들
	bool							m_bIsThread_CheckDBConnectStatus;			// DB 연결 여부 확인 쓰레드 수행 설정
	bool							m_bAutoConnection;
	CRITICAL_SECTION				m_LockAutoConnection;
};

#define								theDBContainer											CDBContainer_Insatnce::Instance()
#define								MYPATH( a, b )											CDBContainer_Insatnce::Instance()->GetMyPath( a, b )
#define								TEXTLOW( a )											CDBContainer_Insatnce::Instance()->TranformLow( a )
#define								TEXTHIGH( a )											CDBContainer_Insatnce::Instance()->TranformHigh( a )
#define								TEXTTRIM( a, b, c )										CDBContainer_Insatnce::Instance()->strTrim( a, b, c )
#define								DBCLOG													CDBContainer_Insatnce::Instance()->WriteLog