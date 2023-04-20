#pragma once


// CCtrl_RadioButton

class AFX_EXT_CLASS CCtrl_RadioButton : public CButton
{
	DECLARE_DYNAMIC(CCtrl_RadioButton)

public:
	CCtrl_RadioButton( CString szName, COLORREF _text, COLORREF _bk, int nCheck = BST_UNCHECKED );
	virtual ~CCtrl_RadioButton();

	BOOL																					Create( CWnd* pParentWnd, UINT unID, CRect rect = CRect( 0, 0, 0, 0 ) );
	void																					SetColor( COLORREF _text, COLORREF _bk );

protected:
	int								m_nChecked;
	COLORREF						m_pColor, m_pBKColor;
	CString							m_szName;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


