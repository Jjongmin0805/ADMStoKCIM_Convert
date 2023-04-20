#pragma once

struct TOOLBARICONS
{
	CRect							m_pSideRect;
	int								m_nSourcePos;
	int								m_nStatus;
	int								m_nSelect;
};

class AFX_EXT_CLASS CFrame_MenuBar_ToolbarEx : public CMFCToolBar
{
	DECLARE_DYNAMIC(CFrame_MenuBar_ToolbarEx)

public:
	CFrame_MenuBar_ToolbarEx();
	virtual ~CFrame_MenuBar_ToolbarEx();

	void							SetStartPoint( CPoint pPt ){ m_StartPt = pPt; };
	void							SetBackGround();
	void							SetToolBarICONs();
	void							SetReDraw( BOOL bDraw = TRUE ){ m_bReDraw = bDraw; };
	void							SetMenuDrawRect( CRect _rect );
	void							SetChildFrmSysICONsIdx( int nIdx ){ m_nChildFrmSysICONsIdx = nIdx; };
	int								GetChildFrmSysICONsIdx(){ return m_nChildFrmSysICONsIdx; };
	void							DrawToolBar();
	void							SetIconSelect( int nSelectIndex );
	void							SetIconUnselect( int nSelectIndex );

protected:
	void							DrawBackGround( CDC* pDC, CRect DrawBKRect = CRect( 0, 0, 0, 0 ) );

	void							DrawToolBarAllICONs( CDC* pDC );
	void							DrawToolBarNormalICONs( TOOLBARICONS* pIconImageInfor, CDC* pDC );
	void							DrawToolBarDisableICONs( TOOLBARICONS* pIconImageInfor, CDC* pDC );
	void							DrawToolBarOverICONs( TOOLBARICONS* pIconImageInfor, CDC* pDC );
	void							DrawToolBarDownICONs( TOOLBARICONS* pIconImageInfor, CDC* pDC );
	void							DrawToolBarSelectCheckICONs( TOOLBARICONS* pIconImageInfor, CDC* pDC );
	TOOLBARICONS*					GetPositionToIconIdx( CPoint point );
	void							SetToolbarIndexOverDownPoint( CPoint point, int nODIndex );
	void							DrawSelectCheckBox( CDC* pDC, CRect rectIconRound );

protected:
	CRect							m_MenuDrawRect;
	CPoint							m_StartPt;
	HBITMAP							m_bkImage,			m_bkLeftImage,			m_bkRightImage;
	BITMAP							m_bkImageInfor,		m_bkLeftImageInfor,		m_bkRightImageInfor;

	HBITMAP							m_TBIsNormal,		m_TBIsDisable,			m_TBIsOver,				m_TBIsDown,			m_TBIsCheckBox;
	BITMAP							m_TBIsNormalInfor,	m_TBIsDisableInfor,		m_TBIsOverInfor,		m_TBIsDownInfor,	m_TBIsCheckBoxInfor;

	HBITMAP							m_TBSysMin[3],		m_TBSysMax[3],			m_TBSysExit[3];
	SIZE							m_TBSysMinSize[3],	m_TBSysMaxSize[3],		m_TBSysExitSize[3];

	vector<TOOLBARICONS*>			m_vecToolIcons;
	TOOLBARICONS*					m_pMoveBeforeData, *m_pClickData;
	BOOL							m_bReDraw;
	int								m_nChildFrmSysICONsIdx;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


