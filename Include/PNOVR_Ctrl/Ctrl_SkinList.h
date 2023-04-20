#pragma once


// CCtrl_SkinList

#define LCSB_CLIENTDATA 1
#define LCSB_NCOVERRIDE 2

class CCtrl_SkinHeader;
class CCtrl_SkinScrollbar_Hori;
class CCtrl_SkinScrollbar_Vert;
class CCtrl_ColorEdit;
class CSkinCtrl_CellInfor
{
public:
	CSkinCtrl_CellInfor() : m_nItem( 0 ), m_nSubItem( 0 ), m_nType( SKINLISTCTRLTYPE_NORMAL ), m_nValue( 0 ), m_dwItemData( 0 )
	{
		m_crTxt																				= RGB(   4,   4,   4 );
		m_crBkg																				= RGB( 255, 255, 255 );
		m_TypeDrawRect																		= CRect( 0, 0, 0, 0 );
	};
	~CSkinCtrl_CellInfor()
	{
		//
	};

public:
	int								m_nItem, m_nSubItem;
	int								m_nType;												// 표현 모드 - 0:일반텍스트, 1:체크박스
	int								m_nValue;												// 값 저장
	COLORREF						m_crTxt, m_crBkg;
	CRect							m_TypeDrawRect;
	DWORD							m_dwItemData;

};
typedef vector<CSkinCtrl_CellInfor*>														VECITEMINFOR;

class AFX_EXT_CLASS CCtrl_SkinList : public CListCtrl
{
	DECLARE_DYNAMIC(CCtrl_SkinList)

public:
	CCtrl_SkinList();
	virtual ~CCtrl_SkinList();

	BOOL															CreateList( const RECT& rect, CWnd* pParentWnd, UINT nListCtrlID );
	void															Init();
	void															EnableHighlighting( HWND hWnd, int row, bool bHighlight );
	bool															IsRowSelected( HWND hWnd, int row );
	bool															IsRowHighlighted( HWND hWnd, int row );
	void															SetScrollBars_Realignment();
	void															HideScrollBars( int nBarType );
	void															PositionScrollBars( int nBarType = -1 );

	void															SetRowFontInfor();
	void															SetRowFontInfor( wchar_t* szName, int nSize );
	void															AddColumnItemData( const TCHAR** pszTitle, const int* arrWidth, const unsigned int* arrMask );
	void															AddColumnItemData( vector<_tstring> pszTitle, const int* arrWidth, const unsigned int* arrMask );
	int																Lst_InsertItem( int nItem, LPCTSTR lpszItem = _T("") );
	BOOL															Lst_SetItemText( int nItem, int nSubItem, int nValueItem, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL															Lst_SetItemText( int nItem, int nSubItem, double dValueItem, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL															Lst_SetItemText( int nItem, int nSubItem, LPCTSTR lpszText, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL															Lst_SetItemCheckBox( int nItem, int nSubItem, int nDefaultValue );
	BOOL															Lst_SetItemEdit( int nItem, int nSubItem, int nValueItem, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL															Lst_SetItemEdit( int nItem, int nSubItem, double dValueItem, COLORREF crText = -1, COLORREF crBackground = -1 );
	BOOL															Lst_SetItemEdit( int nItem, int nSubItem, LPCTSTR lpszText, COLORREF crText = -1, COLORREF crBackground = -1 );
	int																GetColumns();
	void															ResizeListColumn();
	void															ResizeListColumn( const int* arrWidth );
	void															SetEnableMove(){ m_bIsMove = true; };
	void															SetChangeFocusPosition( int nItemCur = -1 );
	CString															GetItemValue( int nItem, int nSubItem );
	int																GetItemSubValue( int nItem, int nSubItem );
	BOOL															Lst_SetItemData( int nItem, DWORD dwData );
	DWORD															Lst_GetItemData( int nItem );
	void															Lst_SortItem( int nSubItem, BOOL bSort );
	BOOL															Lst_SetCurSel( int nItem );
	BOOL															ShowWindow( int nCmdShow );
	void															SetMoveLastRecord( bool bMove ){ m_bMoveLastRecord = bMove; };

protected:
	void															UpdateSubItem( int nItem, int nSubItem );
	void															DrawUnderLine( CDC* pDC, int nItem, int nSubItem, CRect drawRect, COLORREF crUnderLine );
	void															DrawText( CDC* pDC, int nItem, int nSubItem, CRect drawRect, CString szValue, COLORREF crTxt, COLORREF crBkg );
	void															DrawCheckBox( CDC* pDC, int nItem, int nSubItem, CRect drawRect, CString szValue, COLORREF crTxt, COLORREF crBkg );
	static int CALLBACK												CompareNumericFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

public:
	VECITEMINFOR													m_vecItemInfor;
	int																m_nListCtrlID;
	CCtrl_SkinHeader*												m_pSkinHeaderCtrl;
	CCtrl_SkinScrollbar_Hori*										m_pSkinScrollbar_Hori;
	CCtrl_SkinScrollbar_Vert*										m_pSkinScrollbar_Vert;
	CCtrl_ColorEdit*												m_pEdit;

	bool															m_bSBCheck_Horz, m_bSBCheck_Vert;					// 스크롤바 업데이트 유무
	bool															m_bHeaderTracking, m_bHeaderTrackingUpdate;			// header Tracking 
	int																m_nHeaderTracking_ResizeCheck;						// header Tracking 넓이 변경시 변경크기 감시용
	int																m_nAllListContextAlign;								// -1 : No Align, 0 : LEFT, 1: CENTER, 2:RIGHT
	int																m_nTrackingItem;									// Tracking 위치 저장용
	bool															m_bTracking;										// Tracking 사용 유무 확인
	int*															m_nHeaderInfor_ColSize;								// Header Column별 사이즈 저장
	int																m_nListFontSize;									// Child Ctrl의 폰트와도 동일하게 적용

	bool															m_bIsMove;
	bool															m_bRefreshHideScrollBars;
	bool															m_bMoveLastRecord;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnEditReturn( WPARAM wParam, LPARAM lParam );
};


