#pragma once


// CFrame_SkinCtrl_SDIEx 프레임입니다.

class AFX_EXT_CLASS CFrame_SkinCtrl_SDIEx : public CFrameWndEx
{
	DECLARE_DYNCREATE(CFrame_SkinCtrl_SDIEx)
protected:
	CFrame_SkinCtrl_SDIEx();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFrame_SkinCtrl_SDIEx();

public:
	enum{									CLICKED_BOX_NONE = 0,
											CLICKED_BOX_MINIMIZE,
											CLICKED_BOX_MAXIMIZE,
											CLICKED_BOX_RESTORE,
											CLICKED_BOX_EXIT
	};

protected:
	CSize									m_pOriWVMFSize, m_pWVMFSize;		//Windows Ver 7 Main Frame Size;
	BOOL									m_bCreated;
	DWORD									m_dwOrgStyle;		// 기존 윈도우 스타일
	CRgn									m_rgnWnd;			// 윈도우 영역
	CBrush									m_CtlColorBrush;
	int										m_nClickedBox;		// 현재 클릭된 박스 정보
	BOOL									m_bTracking;		// 현재 마우스 박스위에 있는지 아닌지 상태 플래그
	BOOL									m_bDraging;			// 윈도우가 마우스 드래그로 이동 중인지 아닌지의 상태 플래그
	BOOL									m_bSizeCursor;		// 커서모양이 사이즈조절 모양인지 아닌지의 상태 플래그
	BOOL									m_bActive;
	COLORREF								m_cfText;			// 텍스트 색
	COLORREF								m_cfMask;			// 마스크 색

	// 비트맵 사이즈
	SIZE									m_sizeWindowCaption[ IDX_SYSTEMICON_MAX ];
	// 비트맵 핸들
	HBITMAP									m_hBmpWindowCaption[ IDX_SYSTEMICON_MAX ];

	HBITMAP									m_hCMin, m_hCMax, m_hCExit;
	CRect									m_rectMin, m_rectMax, m_rectExit;
	UINT									m_unNcMouseClick;

	// 아이콘 핸들
	HICON									m_hIcon;
	DWORD									m_dwMinWidth;
	DWORD									m_dwMinHeight;

protected:
	void																					SetskinIMGsize();
	void																					DrawFrame();
	void																					GetMonitorInforRect( CRect *pMonitorRect, UINT flags );
	void																					SetCheckLBtnUp( CPoint point );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int																				OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL																			PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void																			OnDestroy();
	afx_msg void																			OnSize(UINT nType, int cx, int cy);
	afx_msg void																			OnPaint();
	afx_msg void																			OnNcPaint();
	afx_msg BOOL																			OnNcActivate(BOOL bActive);
	afx_msg void																			OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void																			OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void																			OnEnterSizeMove();
	afx_msg void																			OnExitSizeMove();
	afx_msg void																			OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void																			OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void																			OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void																			OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void																			OnNcMouseLeave();
	afx_msg void																			OnLButtonUp(UINT nFlags, CPoint point);
};


