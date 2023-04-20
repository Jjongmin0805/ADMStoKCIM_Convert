#pragma once

#include "PString.h"
#include "PLocker.h"

typedef								hash_map< unsigned int, CPLocker* >		HMAPLOCK;

class AFX_EXT_CLASS CPSM_Util
{
public:
	static CPSM_Util*														Instance( void );
	static void																Close(void);

protected :
	static CPSM_Util*														m_pMngData;

//////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	CPSM_Util(void);
	~CPSM_Util(void);

	void																	SetLodFileName( char* szFile );
	void																	SetLodFileNameOfProcessPath( CPString szPath );
	CPString																GetMyPath( CPString szFile );
	CPString																GetMyPath( CPString szDic, CPString szFile );
	void																	SetMyPath( char* szArgv );
	void																	WriteDataFile( CPString szFileName, unsigned char* pData, unsigned int unSize, CPString szWriteMode = "w" );
	void																	LogNoWriteMessage( const char *msg, ... );
	void																	LogWriteMessage( const char *msg, ... );
	void																	LogNoViewWriteMessage( const char *msg, ... );
	bool																	IsFolder( CPString szFolder, bool bMakeFolder );

	bool																	RUN_DataCompress( unsigned char* pSourceData, int unSourceSize, unsigned char*& pCompressData, int& unCompressSize );
	bool																	RUN_DataUncompress( unsigned char *pSourceData, int unSourceSize, unsigned char *&pUnCompressData, int &unUnCompressSize );
	void																	RUN_FreeCompressData( unsigned char *pBuff );
	void																	MakeMulti_Upper( char *szValue );
	void																	MakeMulti_Lower( char *szValue );

	bool																	CreateThread( void *pThreadHandle, void *pThreadFounc, void *pFouncParam );
	bool																	TerminateThread( void *pThreadHandle );

	unsigned int															GetNextNullLockerIndex( unsigned int unBeginLockIndex );
	bool																	IsLocker( unsigned int unLockIndex );
	void																	RemoveLocker( unsigned int unLockIndex );
	void																	InsertLocker( unsigned int unLockIndex );
	void																	Lock( unsigned int nIndex );
	void																	Unlock( unsigned int nIndex );

	bool																	ReRun_Process( DWORD ulProcessID, CPString szName, CPString szParameters, bool bIsInMyPath, bool bIsShow );
	bool																	Run_MypathProcess( CPString szName, CPString szParameters, bool bIsShow );
	bool																	Run_Process( CPString szPathFileName, CPString szParameters, bool bIsShow );
	int																		Run_Process_WaitReturnValue( CPString szPathFileName, CPString szParameters, bool bInclodeMyPath, int nTimeoutSec, bool bIsShow );
	bool																	IsProcessLive( CPString szProcessName );
	bool																	IsProcessModule( DWORD dwPID, CPString szProcessName );
	bool																	Process_Kill( DWORD ulProcessID, CPString szProcessName );
	bool																	Process_Kill( CPString szProcessName );
	bool																	Process_Kill( DWORD ulProcessID );
	bool																	DeleteAllFilesInFolder( char* szDeleteFolder, char* szFileExtend = NULL, bool bDoAsk = false );
	bool																	CopyAllFilesInFolder( char* szSourceFolder, char* szTargetFolder, bool bDoOverWrite = true );
	bool																	MoveAllFilesInFolder( char* szSourceFolder, char* szTargetFolder );

	char*																	CharReplace( char* szData, char* szSource, char* szTarget );
	int																		CharFind( char* szData, char chFind );
	int																		CharFind( char* szData, char* szFind );
	bool																	WriteToFile( const char* szFilename, const char* pBuffer, int nLength );
	bool																	ReadFromFile( const char* szFilename, char* pBuffer, int nLength );

	char*																	GetLogLastMsg();
	void																	SetLogLastMsg( CPString szMsgData );

	int																		doZip( TCHAR *lpszSourcePath, TCHAR *lpszTargetPath );
	int																		doUnzip( TCHAR *lpszSourcePath, TCHAR *lpszTargetPath );
	bool																	IsFileDirectory( char *szPath, bool bIsLastFile );
	bool																	IsFileDirectory( char *szPath, bool bIsLastFile, string &szRetPath );
	bool																	IsFileDirectory( wchar_t *lpszPath, bool bIsLastFile, wstring &szRetPath );
	bool																	CreateRealDirectory( const wchar_t* szPath );
	bool																	CreateRealDirectory( const char* szPath );
	void																	CheckCreateRealDirectory( char* szPath );
	int																		GetDirectoryList( const wchar_t *dir_name, VECWSTR &vecFileList, VECWSTR &vecPathFileList );
	int																		GetDirectoryList( const char *dir_name, VECSTR &vecFileList, VECSTR &vecPathFileList );
	time_t																	GetDataTime2TimeT( int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond );
	time_t																	GetAddMonthDate( time_t tmCalcuTime, int nAddMonth );
	time_t																	GetAddYearDate( time_t tmCalcuTime, int nAddYear );
	bool																	IsCheckMyPRG();
	CPString																GetProcessName( DWORD dwPID );
	DWORD																	GetOtherProcessPID( DWORD dwSearchPID, CPString szProcessName );
	HWND																	GetProcessID2HWND( DWORD PID );
	int																		CheckValue_FPClassify( double dValue );
	string																	string_format( const string szFormat, ... );

protected:
	void																	ClearLocker();
	CPLocker*																GetLockerInfor( unsigned int unLockIndex );

protected:
	CPString						m_szMyPath;
	HMAPLOCK						m_hmapLocker;

	char							*m_pReplaceBufferData, m_szLogLastMsg[ 64 ];
	char							m_szLogFileName[ 128 ];
	int								m_nReplaceBuffferDataSize;
};

#define								theUtil									CPSM_Util::Instance()
#define								LOGNOOUT								theUtil->LogNoWriteMessage
#define								LOGOUT									theUtil->LogWriteMessage
#define								LOGNOVIEW								theUtil->LogNoViewWriteMessage
#define								PASSERT( x ) \
    do {\
        if( !( x ) ) {\
            LOGOUT("* Err : Assertion failed: %s (%s:%d)", #x, __FILE__, __LINE__);\
            ULONG_PTR extra_info [1];\
			extra_info[ 0 ] = ( ULONG_PTR )( #x );\
			RaiseException( 0x40000015, EXCEPTION_NONCONTINUABLE, 1, extra_info );\
        }\
    } while (false)
