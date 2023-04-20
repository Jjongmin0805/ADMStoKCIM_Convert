#pragma once

// CPNOCtrl_ResizingCtrlbar
#define SCBS_EDGELEFT       0x00000001
#define SCBS_EDGERIGHT      0x00000002
#define SCBS_EDGETOP        0x00000004
#define SCBS_EDGEBOTTOM     0x00000008
#define SCBS_EDGEALL        0x0000000F
#define SCBS_SHOWEDGES      0x00000010
#define SCBS_GRIPPER        0x00000020

class CSCBDockBar : public CDockBar
{
	friend class CFrame_ResizeCtrlBar;
};

// CFrame_ResizeCtrlBar
class AFX_EXT_CLASS CFrame_ResizeCtrlBar : public CControlBar
{
	DECLARE_DYNAMIC(CFrame_ResizeCtrlBar)

public:
	CFrame_ResizeCtrlBar();
	virtual ~CFrame_ResizeCtrlBar();

enum{		CLICKED_BOX_NONE = 0,
			CLICKED_BOX_SYSTEM,
			CLICKED_BOX_MINIMIZE,
			CLICKED_BOX_MAXIMIZE,
			CLICKED_BOX_RESTORE,
			CLICKED_BOX_EXIT
	};

public:
	CSize							m_szHorz;
	CSize							m_szVert;
	CSize							m_szFloat;

protected:
	CWnd*							m_InforWnd;
	DWORD							m_dwSCBSstyle;
	UINT							m_unDocEdge;
	CSize							m_szMin, m_szMinTemp, m_szMaxTemp;
	CSize							m_szHorzVertOld;
	CPoint							m_ptEdgeOld;
	BOOL							m_bTracking;
	BOOL							m_bKeepSize;
	BOOL							m_bParentSizing;
	BOOL							m_bDragContent;
	UINT							m_nDockBarID;
	int								m_nEdgeXsize;
	BOOL							m_bResizingActive;

public:
	void							SetInInforWnd( CWnd* pInforWnd );
	CWnd*							GetInInforWnd();
	const BOOL						IsFloating() const;
	const BOOL						IsHorzDocked() const;
	const BOOL						IsVertDocked() const;
	const BOOL						IsSideTracking() const;

	virtual BOOL					Create( LPCTSTR lpszWindowName, CWnd* pParentWnd, CSize sizeDefault, UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP );
	virtual void					OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize					CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize					CalcDynamicLayout(int nLength, DWORD dwMode);
	virtual BOOL					DestroyWindow();

	void							SetResizingActive(BOOL bActive);
	BOOL							GetResizingActive();

protected:
	UINT							GetEdgeCode(int nEdge);
	BOOL							GetEdgeRect(CRect rcWnd, UINT nHitTest, CRect& rcEdge);
	BOOL							NegoSpace(int nLengthAvail, BOOL bHorz);

	virtual void					StartTracking(UINT nHitTest);
	virtual void					StopTracking();
	virtual void					OnTrackUpdateSize(CPoint& point);
	virtual void					OnTrackInvertTracker();
	virtual void					RecalcLayoutControlBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcActivate(BOOL bActive);
};


