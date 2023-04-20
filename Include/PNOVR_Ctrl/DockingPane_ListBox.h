#pragma once

class CAlramList : public CListBox
{
	// 생성입니다.
public:
	CAlramList();

	// 구현입니다.
public:
	virtual ~CAlramList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	DECLARE_MESSAGE_MAP()
};

// CDockingPane_ListBox


typedef								map< CString, CAlramList*>										MAPDOCALRAMLST;
class AFX_EXT_CLASS CDockingPane_ListBox : public CDockablePane
{
	DECLARE_DYNAMIC(CDockingPane_ListBox)

public:
	CDockingPane_ListBox();
	virtual ~CDockingPane_ListBox();

	BOOL																							CreateSingleTab( CWnd* pParentWnd, TCHAR* szTitle, const RECT& rect, UINT unID, UINT unListID );
	BOOL																							CreateMultiTab( CWnd* pParentWnd, TCHAR* szTitle, const RECT& rect, UINT unID, UINT unTABID );
	void																							AddString( TCHAR* szMsg );
	void																							AddString( CString szTabName, TCHAR* szMsg );
	void																							AdjustHorzScroll( CListBox& wndListBox );

	virtual	void																					ShowPane( BOOL bShow, BOOL bDelay = FALSE, BOOL bActivate = TRUE );

	void																							AddGroup( CString szListName, UINT unListID );
	CAlramList*																						GetFirstAlramList();
	CAlramList*																						GetAlramList( CString szTabName );

protected:
	void																							DeleteAll();

protected:
	CMFCTabCtrl*					m_pWndTab;
//	CAlramList						m_wndMsgLst;
	MAPDOCALRAMLST					m_mapWndMsgLst;
	UINT							m_unType;
	UINT							m_unListID;
	UINT							m_unTabeID;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


