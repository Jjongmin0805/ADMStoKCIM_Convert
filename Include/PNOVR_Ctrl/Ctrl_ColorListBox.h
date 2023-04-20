#pragma once


// CCtrl_ColorListBox

class AFX_EXT_CLASS CCtrl_ColorListBox : public CListBox
{
	DECLARE_DYNAMIC(CCtrl_ColorListBox)

public:
	CCtrl_ColorListBox();
	virtual ~CCtrl_ColorListBox();

public:
	int																						AddString(LPCTSTR lpszText, COLORREF rgb=0);
	int																						InsertString(int nIndex, LPCTSTR lpszText, COLORREF rgb=0);
	void																					DrawBackGround();
	void																					SetColor( COLORREF crtext, COLORREF bk ){ m_crText = crtext; m_crBk = bk; };
	void																					SetSelectColor( COLORREF crtext, COLORREF bk ){ m_crSelectText = crtext; m_crSelectBk = bk; };
	void																					SetItemHeight( UINT unHeight ){ m_unHeight = unHeight; };
	void																					SetFrontMargin( int nMargin ){ m_nFrontMargin = nMargin; };
	void																					SetVarData( int nVarData ){ m_nTempVarData = nVarData; };
	int																						GetVarData(){ return m_nTempVarData; };
	virtual void																			DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void																			MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

protected:
	virtual BOOL																			PreCreateWindow(CREATESTRUCT& cs);

	int								m_nTempVarData;
	int								m_nFrontMargin;
	CFont							m_Font;
	COLORREF						m_crText, m_crBk;
	COLORREF						m_crSelectText, m_crSelectBk;
	UINT							m_unHeight;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void				OnPaint();
	afx_msg BOOL				OnEraseBkgnd(CDC* pDC);
	afx_msg int					OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void				OnLButtonDown(UINT nFlags, CPoint point);
};


