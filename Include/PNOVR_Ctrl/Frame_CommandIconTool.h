#pragma once

// CFrame_CommandIconTool

#include "Frame_CommandIcon.h"

class AFX_EXT_CLASS CFrame_CommandIconTool : public CWnd
{
	DECLARE_DYNAMIC(CFrame_CommandIconTool)

public:
	CFrame_CommandIconTool();
	virtual ~CFrame_CommandIconTool();

	BOOL																						Create( const RECT& rect, CWnd* pParentWnd );
	int																							AddCMDIcon( UINT unUnitID, int nSymbolIndex, int nType, int nDefaultStatus, CString szTitle );
	BOOL																						RemoveCMDIconUID( UINT unUnitID );
	BOOL																						RemoveCMDIconIndex( int nIndex );
	void																						SetCalcuPosition();
	BOOL																						GetSelectCMDIcon( int nIndex );
	void																						SetSelectCMDIcon( int nIndex, BOOL bSelect );
	int																							GetCMDIconStatus( int nIndex );
	void																						SetCMDIconStatus( int nIndex, int nSelect );
	void																						SetCMDIconStartPosition();
	void																						DrawCMDIcon( int nIndex );
	void																						DrawWeatherInfor( Graphics& graphics );

	void																						InsertMenu( int nIndex, UINT unID, TCHAR* szString, UINT unFlag, int nType, int nDefault, Bitmap* pIcon );
	void																						InsertPopupMenu( int nIndex, int nSubMenuIndex, TCHAR* szString );
	int																							CreateSubMenu( int nIndex );
	void																						InsertSubMenu( int nIndex, int nSubMenuIndex, UINT unID, TCHAR* szString, UINT unFlag, int nType, int nDefault, Bitmap* pIcon );
	void																						OpenMenu( int nIndex );
	int																							GetMenuValue( int nIndex, UINT unID );
	void																						SetMenuValue( int nIndex, UINT unID, int nValue );

	void																						SetWeatherIconIndex( int nIndex );
	void																						SetWeatherInfor( CString szValue11, CString szValue12, CString szValue13, CString szValue21, CString szValue22, CString szValue23 );
	void																						WeatherInvalidateRect();

protected:
	CWnd*							m_pParent;
	CPoint							m_ptIconStart;
	CRect							m_rectCMDIconRound;
	BOOL							m_bMouseLClick;

	CRect							m_rectCMDWeatherInforRound;
	int								m_nWeatherIndex;
	CString							m_szWeatherInfor[ 2 ][ 3 ];

	VECCMDICON						m_vecIcons;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCMDMenuEventFunc( UINT nID );
};


