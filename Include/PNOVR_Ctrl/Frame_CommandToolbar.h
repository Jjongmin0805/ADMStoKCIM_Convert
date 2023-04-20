#pragma once

class CFrame_CommandIconTool;
class CFrame_CommandRebar;

class AFX_EXT_CLASS CFrame_CommandToolbar
{
public:
	CFrame_CommandToolbar(void);
	~CFrame_CommandToolbar(void);

	BOOL																							Create( CWnd* pParentWnd );
	int																								AddCmdIcon( UINT unUnitID, int nSymbolIndex, int nType, int nDefaultStatus, CString szTitle );
	BOOL																							RemoveCmdIconUID( UINT unUnitID );
	BOOL																							RemoveCmdIconIndex( int nIndex );

	int																								AddCmdIcon_Separator();
	void																							SetCMDIconCalcuPosition();
	void																							Invalidate();
	BOOL																							GetSelectCMDIcon( int nIndex );
	void																							SetSelectCMDIcon( int nIndex, BOOL bSelect );
	int																								GetCMDIconStatus( int nIndex );
	void																							SetCMDIconStatus( int nIndex, int nSelect );
	int																								GetCMDMenuValue( int nIndex, UINT unID );
	void																							SetCMDMenuValue( int nIndex, UINT unID, int nValue );

	void																							InsertCMDIcon_Menu( int nIndex, UINT unID, TCHAR* szString, UINT unFlag, int nType, int nDefault, Bitmap* pIcon );
	void																							InsertCMDIcon_PopupMenu( int nIndex, int nSubMenuIndex, TCHAR* szString );
	int																								CreateCMDIcon_SubMenu( int nIndex );
	void																							InsertCMDIcon_SubMenu( int nIndex, int nSubMenuIndex, UINT unID, TCHAR* szString, UINT unFlag, int nType, int nDefault, Bitmap* pIcon );
	void																							OpenMenu( int nIndex );

	void																							SetCMDWeather_Activation( BOOL bActivation );
	void																							SetCMDWeather_IconIndex( int nIndex, BOOL bRefresh = TRUE );
	void																							SetCMDWeather_InforValue( CString szValue11, CString szValue12, CString szValue13, CString szValue21, CString szValue22, CString szValue23 );

protected:
	CFrameWndEx*					m_pParentFrm;
	CFrame_CommandRebar*			m_wndCTrebar;
	CFrame_CommandIconTool*			m_pWndCmdIconTool;
};

