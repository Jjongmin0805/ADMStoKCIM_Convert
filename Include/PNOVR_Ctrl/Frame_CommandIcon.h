#pragma once

class CCtrl_ColorMenu;

typedef								vector< CCtrl_ColorMenu* >									VECCOLORMENU;

class CFrame_CommandIcon
{
public:
	CFrame_CommandIcon( UINT unUnitID, int nIconSymbol, int nType, int nDefaultStatus, CString szTitle );
	~CFrame_CommandIcon(void);

	UINT																						GetUnitID();
	void																						SetRect( CRect rectPos );
	int																							GetType();
	void																						SetStatus( int nStatus );
	int																							GetStatus();
	CString																						GetTitle();
	CRect																						GetPosition();
	void																						SetPosition( CRect rectPos );
	void																						DrawCMDIcon( Graphics& graphics );
	void																						SetSelect( BOOL bSelect );
	BOOL																						GetSelect();
	void																						AddMenu( UINT unID, TCHAR* szString, UINT unFlag, int nType, int nDefault, Bitmap* pIcon = NULL );

	int																							CreateSubMenu();
	void																						AddSubMenu( int nSubMenuIndex, UINT unID, TCHAR* szString, UINT unFlag, int nType, int nDefault, Bitmap* pIcon = NULL );
	void																						AddPopupMenu( int nSubMenuIndex, TCHAR* szString );
	void																						OpenMenu();
	void																						OpenMenu( CWnd* pMainParent );
	int																							GetMenuValue( UINT unID );
	void																						SetMenuValue( UINT unID, int nValue );

protected:
	void																						CreateControl();

protected:
	UINT							m_unUnitID; 
	int								m_nIconSymbolIndex;
	int								m_nType;
	int								m_nStatus;
	BOOL							m_bSelect;
	CString							m_szTitle;
	CRect							m_rectPos;

	CCtrl_ColorMenu*				m_pMenu;
	VECCOLORMENU					m_vecSubMenu;
};

typedef								vector< CFrame_CommandIcon* >								VECCMDICON;