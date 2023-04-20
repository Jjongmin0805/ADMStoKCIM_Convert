#pragma once

// CCtrl_ColorEdit

class AFX_EXT_CLASS CCtrl_ColorEdit : public CEdit
{
	DECLARE_DYNAMIC(CCtrl_ColorEdit)

public:
	CCtrl_ColorEdit();
	CCtrl_ColorEdit( COLORREF crTxt, COLORREF crBkg );
	virtual ~CCtrl_ColorEdit();

	void																					SetColor( COLORREF _text, COLORREF _bk ){ m_pColor = _text; m_pBKColor = _bk; };
	void																					SetDataValue( int nItem, int nSubItem, CString szValue );
	int																						GetItem(){ return m_nItem; };
	int																						GetSubitem(){ return m_nSubItem; };
	void																					SetFontInfor( wchar_t* szName, int nSize );

protected:
	int								m_nItem, m_nSubItem;
	COLORREF						m_pColor, m_pBKColor;
	CString							m_szValue;
	CFont*							m_pFont;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocus();
};


