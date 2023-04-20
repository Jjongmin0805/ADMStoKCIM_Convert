#pragma once

// CFrame_MenuBar_Ctrl

#define					MB_TRACKPOPUPMENU		( WM_USER + 1102 )

struct MENUTOOLBAR_DATA
{
	MENUTOOLBAR_DATA( void ) : wdVersion( 0 ), wdWidth( 0 ), wdHeight( 0 ), wdItemCount( 0 ) {};
	~MENUTOOLBAR_DATA( void ) {};

	WORD						wdVersion;
	WORD						wdWidth;
	WORD						wdHeight;
	WORD						wdItemCount;

	WORD*						Items(){ return (WORD*)(this+1); }
};

class AFX_EXT_CLASS CFrame_MenuBar_Ctrl : public CToolBarCtrl
{
	DECLARE_DYNAMIC(CFrame_MenuBar_Ctrl)

public:
	CFrame_MenuBar_Ctrl();
	virtual ~CFrame_MenuBar_Ctrl();

	void						SetColorBK( COLORREF _color )				{ m_colorBK = _color; }
	void						SetColorHot( COLORREF _color )				{ m_colorHot = _color; }
	void						SetColorSelected( COLORREF _color )			{ m_colorChecked = _color; }
	void						SetColorChecked( COLORREF _color )			{ m_colorSelected = _color; }
	void						SetColorLine( COLORREF _color )				{m_colorLine = _color; }

	BOOL						LoadMenu( CMenu *pMenu );
	void						TrackPopupMenu( void );
	BOOL						OnMenuInput( MSG* pMsg );
	static LRESULT CALLBACK		MenuInputFilter( int nCode, WPARAM wParam, LPARAM lParam );
	CRect						GetDrawItemTrect(){ return m_DrawItemTrect; };
	BOOL						SetEnableMenu( int nMainIndex, int nSubIndex, UINT unID, BOOL bEnable );

protected:
	void						ItemDraw( CDC *pDC, CRect rectDraw, UINT uState, CString szName );
	CRect						MaxMergeRect( CRect firstRect, CRect secondRect );

	CMenu*						m_pThisMenu;
	int							m_nItemCount;
	int							m_nPressed;
	CPoint						m_ptMouse;
	CRect						m_DrawItemTrect;
	int							m_nThisToolbarChildSysIdx;

private:
	COLORREF					m_colorBK, m_colorHot, m_colorChecked, m_colorSelected, m_colorLine;

protected:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg LRESULT OnPopupMenu( WPARAM wParam, LPARAM lParam );
	afx_msg void OnExitMenuLoop( BOOL bTrackPopupMenu );
	afx_msg void OnEnterMenuLoop( BOOL bTrackPopupMenu );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
 	afx_msg void OnMenuClick(UINT nID);
// 	afx_msg void OnUpdateMenuClick(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
};


