#pragma once
class AFX_EXT_CLASS CUtil_Manager
{
public:
	CUtil_Manager(void);
	~CUtil_Manager(void);

	static CUtil_Manager*																		instance( void );

protected :
	static CUtil_Manager*			m_pMngData;

//////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	CString																						GetModulePath();
	CSize																						FormatText(CString& sText, CDC* pDC, int nWidth);
	COLORREF																					Darker(COLORREF crBase, float fFactor);
	COLORREF																					Lighter(COLORREF crBase, float fFactor);
	_tstring																					GetMyPath( _tstring szPath, _tstring szFile );

	// =====================
	// Command Flag
	// ---------------------
	// "MakeDate", "UpdateDate"
	// ---------------------
	// "FileVersion", "InternalName", "OriginalFilename", "ProductVersion", "ProductName", "Comments", "CompanyName"
	// =====================
	_tstring																					GetFilePropertyInfor( _tstring szFileName, _tstring szFlag );

	BOOL																						GetProcessModule( DWORD dwPID, wstring sProcessName );
	BOOL																						KillProcess( wstring szProcessName );
	BOOL																						GetProcessLive( wstring sProcessName );
	CString																						GetProcessName( DWORD dwPID );
	DWORD																						GetOtherProcessPID( DWORD dwSearchPID, wstring sProcessName );
	HWND																						GetProcessID2HWND( DWORD PID );
	bool																						SetCheckMyPRG();

	COLORREF																					GetColorPicker( CWnd* pParent, CRect rectViewPos, int nValign );			// Valign = 1(상단), 2(하단)
	BOOL																						IsSystemCheck();
	int																							ParseTokenString( vector<CString> &vecSTRValue, CString szData, CString szDiv );

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Log Write
public:
	void																						SetLogFileName( _tstring szFileName );
	_tstring																					GetLogFileName();
	void																						WriteLog( const wchar_t *msg, ... );
	void																						WriteLog( const char *msg, ... );

protected :
	void																						WriteLog2File( char* szFilePath, char* szMsg );

	_tstring						m_szLogFileName;
	HANDLE							m_hLockMutex_Log;


};

#define								thePNO_UTILmng												CUtil_Manager::instance()
#define								PNOLog														thePNO_UTILmng->WriteLog