#pragma once

// CCtrl_Label_SkinList
class CCtrl_SkinList;
class AFX_EXT_CLASS CCtrl_Label_SkinList : public CWnd
{
	DECLARE_DYNAMIC(CCtrl_Label_SkinList)

public:
	CCtrl_Label_SkinList();
	virtual ~CCtrl_Label_SkinList();

	BOOL																					Create( const RECT& rect, CWnd* pParentWnd, _tstring szTitle, UINT nListCtrlID );

	void																					AddColumnItemData( const TCHAR** pszTitle, const int* arrWidth, const unsigned int* arrMask = NULL );
	void																					AddColumnItemData( vector<_tstring> pszTitle, const int* arrWidth, const unsigned int* arrMask = NULL );
	int																						Lst_InsertItem( int nItem, LPCTSTR lpszItem = _T("") );
	BOOL																					Lst_SetItemText( int nItem, int nSubItem, LPCTSTR lpszText, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL																					Lst_SetItemCheckBox( int nItem, int nSubItem, int nDefaultValue );
	BOOL																					Lst_SetItemEdit( int nItem, int nSubItem, int nValueItem, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL																					Lst_SetItemEdit( int nItem, int nSubItem, double dValueItem, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL																					Lst_SetItemEdit( int nItem, int nSubItem, LPCTSTR lpszText, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL																					Lst_SetItemData( int nItem, DWORD dwData );
	int																						Lst_GetItemCount();
	void																					Lst_ResizeListColumn();
	void																					SetWindowResize();
	void																					Lst_ShowWindow( int nCmdShow );
	void																					Lst_DeleteAllItems();
	void																					Lst_Invalidate();
	CString																					Lst_GetItemText( int nItem, int nSubItem );
	void																					Lst_SetChangeFocusPosition( int nItemCur = -1 );
	DWORD																					Lst_GetItemData( int nItem );
	void																					Lst_SetSortItem( int nItem, BOOL bSort );
	int																						Lst_GetItemSubValue( int nItem, int nSubItem );
	BOOL																					Lst_SetCurSelItem( int nItem );
	void																					Lst_HideScrollBars( int nBarType );
	void																					Lst_MoveLastRecord( bool bMove );

protected:
	_tstring						m_szTitle;
	CRect							m_Rect;
	CCtrl_SkinList*					m_pList;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


