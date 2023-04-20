#pragma once


// CCtrl_ColorComboBox

class AFX_EXT_CLASS CCtrl_ColorComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CCtrl_ColorComboBox)

public:
	CCtrl_ColorComboBox();
	virtual ~CCtrl_ColorComboBox();

	void																					SetTextColor( COLORREF _Color ){ m_sttTextColor = _Color; };
	void																					SetBkColor( COLORREF _Color ){ m_sttBKColor = _Color; };
	void																					SetHighLightTextColor( COLORREF _Color ){ m_sttHighLightTextColor = _Color; };
	void																					SetHighLightBKColor( COLORREF _Color ){ m_sttHighLightBKColor = _Color; };
	void																					SetHighLightNotBKColor( COLORREF _Color ){ m_sttHighLightNotBKColor = _Color; };
	void																					LineColor( COLORREF _Color ){ m_crLineColor = _Color; };
	void																					SetDataValue( int nItem, int nSubItem );
	int																						GetItem(){ return m_nItem; };
	int																						GetSubitem(){ return m_nSubItem; };

	void																					SetFontSize( int nSize );
	int																						GetFontSize();
	void																					SetFontWeight( int nWeight );
	int																						GetFontWeight();

private:
	int								m_nItem, m_nSubItem;
	COLORREF						m_sttTextColor;
	COLORREF						m_sttBKColor;
	COLORREF						m_sttHighLightTextColor;
	COLORREF						m_sttHighLightBKColor;
	COLORREF						m_sttHighLightNotBKColor;
	COLORREF						m_crLineColor;
	int								m_nViewFontSize;
	int								m_nViewFontWeight;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


