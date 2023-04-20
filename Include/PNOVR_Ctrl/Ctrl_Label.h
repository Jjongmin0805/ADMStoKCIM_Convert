#pragma once


// CCtrl_Label

class AFX_EXT_CLASS CCtrl_Label : public CWnd
{
	DECLARE_DYNAMIC(CCtrl_Label)

public:
	CCtrl_Label( int nIndex );
	virtual ~CCtrl_Label();

	BOOL																					Create( const RECT& rect, CWnd* pParentWnd, _tstring szTitle );
	void																					SetSubDraw( int nSubType, CRect drawRect, COLORREF drawColor );
	void																					SetDrawImage( int nIndex, _tstring szFilePath, CRect drawRect, bool bIsDraw = false, bool bIsInverse = false );
	void																					SetDrawimageInverse( int nIndex, bool bIsInverse );
	void																					SetDrawImageRect( int nIndex, CRect drawRect );
	CRect																					GetDrawImageRect( int nIndex );
	CSize																					GetDrawImageSize( int nIndex );
	void																					SetDrawImageShow( int nIndex, bool bIsShow );
	void																					SetDrawImageMSActive( int nIndex, bool bIsActive );
	CRect																					GetRect(){ return m_Rect; };

protected:
	int								m_nIndex;
	_tstring						m_szTitle;
	CRect							m_Rect;
	COLORREF						m_crBkg;

	int								m_nSubType;						// 1=사각형 , 2=원
	CRect							m_SubRect;
	COLORREF						m_crSubBkg;

	Bitmap*							m_pImage_Status[ CTRLLABEL_OBJECTS_COUNT ];
	CRect							m_ImageRect[ CTRLLABEL_OBJECTS_COUNT ];
	bool							m_bImageDraw[ CTRLLABEL_OBJECTS_COUNT ];
	bool							m_bImageInverse[ CTRLLABEL_OBJECTS_COUNT ];
	bool							m_bImageAction[ CTRLLABEL_OBJECTS_COUNT ];

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


