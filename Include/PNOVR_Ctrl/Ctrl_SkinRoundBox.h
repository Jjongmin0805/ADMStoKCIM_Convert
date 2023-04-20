#pragma once


// CCtrl_SkinRoundBox

class AFX_EXT_CLASS CCtrl_SkinRoundBox : public CWnd
{
	DECLARE_DYNAMIC(CCtrl_SkinRoundBox)

public:
	CCtrl_SkinRoundBox();
	virtual ~CCtrl_SkinRoundBox();

	BOOL																					Create( const RECT& rect, CWnd* pParentWnd = NULL );
	void																					Load_Image();
	void																					Draw_Round( CDC* pDC, CRect _roundRect );
	void																					SetText( _tstring szStr ){ m_szText = szStr; };
protected:
	Color																					GetListBkColor();

protected:
	Bitmap*							m_round_image[ 8 ];
	BOOL							m_bIsImageInitial;

	_tstring						m_szText;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


