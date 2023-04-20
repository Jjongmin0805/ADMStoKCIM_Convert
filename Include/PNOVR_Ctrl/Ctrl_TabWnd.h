#pragma once

#include "Ctrl_TabWnd_Tools.h"
/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)

typedef struct _HTAB {} *HTAB;
// 
enum TAB_LAYOUT
{
	TAB_LAYOUT_TOP,
	TAB_LAYOUT_BOTTOM
};
 
enum TAB_BEHAVIOR
{
	TAB_BEHAVIOR_SCALE,
	TAB_BEHAVIOR_SCROLL
};

/////////////////////////////////////////////////////////////////////////////

class CCtrl_TabWnd;
struct TabCtrlDraw
{
	virtual bool IsStraightOrder(CCtrl_TabWnd * /*pCtrl*/) { return true; }	// true - paint tabs left to right, false - reverse order.
	virtual void DrawBegin(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/) {}
	virtual void DrawBorder(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/, CRect const * /*pRect*/) {}
	virtual void DrawCtrlAreaBack(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/, CRect const * /*pRect*/, int /*nDrawIndex*/ ) {}
	virtual void DrawTab(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/, HTAB /*hTab*/, CRgn * /*pRgn*/) {}
	virtual void DrawButtonClose(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/, CRect const * /*pRect*/, bool /*bHover*/, bool /*bPushed*/) {}
	virtual void DrawButtonMenu(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/, CRect const * /*pRect*/, bool /*bHover*/, bool /*bPushed*/, bool /*bPartialView*/) {}
	virtual void DrawButtonScrollLeft(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/, CRect const * /*pRect*/, bool /*bHover*/, bool /*bPushed*/, bool /*bPartialView*/) {}
	virtual void DrawButtonScrollRight(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/, CRect const * /*pRect*/, bool /*bHover*/, bool /*bPushed*/, bool /*bPartialView*/) {}
	virtual void DrawWndsAreaBack(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/, CRect const * /*pRect*/) {}
	virtual void DrawEnd(CCtrl_TabWnd * /*pCtrl*/, CDC * /*pDC*/) {}
};

interface ITabCtrlRecalc
{
	virtual int GetBorderWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;
	virtual CRect GetCtrlAreaPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;
	virtual CRect GetWndsAreaPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;
	virtual CRect GetTabHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;	// uses only CRect::left and CRect::right.
	virtual CRect GetTabPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;
	virtual int GetTabImageTextGap(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;	// IMAGE<- gap ->TEXT.
	virtual int GetTabExtraWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base, HTAB hTab) = 0;	// additional width of tab.
	virtual int GetTabMinWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;	// minimal width of tab.
	virtual CRect GetLeftScrollButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;	// uses only CRect::left and CRect::right.
	virtual CRect GetRightScrollButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;	// uses only CRect::left and CRect::right.
	virtual CRect GetMenuButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;	// uses only CRect::left and CRect::right.
	virtual CRect GetCloseButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base) = 0;	// uses only CRect::left and CRect::right.
};

interface ITabCtrlBehavior
{
	virtual HTAB HitTest(CCtrl_TabWnd *pCtrl, ITabCtrlBehavior *base, CPoint point) = 0;		// get tab in the given point.
	virtual bool SetCursor(CCtrl_TabWnd *pCtrl, ITabCtrlBehavior *base) = 0;	// return true if you set cursor.
};

interface ITabCtrlToolTip
{
	virtual CToolTipCtrl *CreateToolTip(CCtrl_TabWnd *pCtrl, ITabCtrlToolTip *base) = 0;
};

struct TabCtrlUserAbility
{
	virtual bool CanShowCloseButton(CCtrl_TabWnd * /*pCtrl*/) { return true; }
	virtual bool CanShowMenuButton(CCtrl_TabWnd * /*pCtrl*/) { return true; }
	virtual bool CanShowScrollButtons(CCtrl_TabWnd * /*pCtrl*/) { return true; }
};

struct TabCtrlNotify
{
	virtual void OnTabPreCreate(CCtrl_TabWnd * /*pCtrl*/, HWND /*hWnd*/, TCHAR const * /*text*/, int /*image*/) {}
	virtual void OnTabPostCreate(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/) {}
	virtual void OnTabPreDestroy(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/) {}
	virtual void OnCloseButtonClicked(CCtrl_TabWnd * /*pCtrl*/, CRect const * /*pRect*/, CPoint /*ptScr*/) {}	// ptScr - in screen space.
	virtual void OnMenuButtonClicked(CCtrl_TabWnd * /*pCtrl*/, CRect const * /*pRect*/, CPoint /*ptScr*/) {}	// ptScr - in screen space.
	virtual void OnTabSelected(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/) {}
	virtual void OnLButtonDown(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/, CPoint /*ptScr*/) {}	// ptScr - in screen space.
	virtual void OnLButtonDblClk(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/, CPoint /*ptScr*/) {}	// ptScr - in screen space.
	virtual void OnRButtonDown(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/, CPoint /*ptScr*/) {}		// ptScr - in screen space, hTab can be NULL.
	virtual void OnRButtonUp(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/, CPoint /*ptScr*/) {}	// ptScr - in screen space, hTab can be NULL.
	virtual void OnStartDrag(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/, CPoint /*ptScr*/) {}	// ptScr - in screen space.
	virtual void OnDrag(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/, CPoint /*ptScr*/, bool /*outside*/) {}	// ptScr - in screen space, outside==true - dragging out of tabs area.
	virtual void OnFinishDrag(CCtrl_TabWnd * /*pCtrl*/, HTAB /*hTab*/, bool /*cancel*/) {}	// cancel==false - dragging was finished using left button up.
};

/////////////////////////////////////////////////////////////////////////////

interface ITabCtrlStyle
{	
	virtual TabCtrlDraw *GetDrawManager() = 0;
	virtual ITabCtrlRecalc *GetRecalcManager() = 0;
	virtual ITabCtrlBehavior *GetBehaviorManager() = 0;
	virtual ITabCtrlToolTip *GetToolTipManager() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// CCtrl_TabWnd

class AFX_EXT_CLASS CCtrl_TabWnd : public CWnd, private TabCtrlUserAbility, private ActivityHook::INotify, private KeyboardHook::Notify
{
public:
	CCtrl_TabWnd();
	~CCtrl_TabWnd();

public:
	bool Create(CWnd *parent, DWORD dwStyle, RECT const &rect, UINT nID);
	HTAB Add(HWND wnd, TCHAR const *text, int image) throw(std::bad_alloc);
	HTAB Insert(HTAB hBefore, HWND wnd, TCHAR const *text, int image) throw(std::bad_alloc);
	void RemoveBefore(HTAB hBefore, HTAB hSrc) throw(std::bad_alloc);
	void RemoveAfter(HTAB hAfter, HTAB hSrc) throw(std::bad_alloc);
	void Delete(HTAB hTab);
	void DeleteAll();
	void Update(bool redraw = true);
	void InstallStyle(ITabCtrlStyle *style);
	void SetDrawManager(TabCtrlDraw *p);
	TabCtrlDraw *GetDrawManager() const;
	void SetRecalcManager(ITabCtrlRecalc *p);
	ITabCtrlRecalc *GetRecalcManager() const;
	void SetBehaviorManager(ITabCtrlBehavior *p);
	ITabCtrlBehavior *GetBehaviorManager() const;
	void SetToolTipManager(ITabCtrlToolTip *p);
	ITabCtrlToolTip *GetToolTipManager() const;
	void SetAbilityManager(TabCtrlUserAbility *p);
	TabCtrlUserAbility *GetAbilityManager() const;
	void SetNotifyManager(TabCtrlNotify *p);
	TabCtrlNotify *GetNotifyManager() const;
	void SetLayout(TAB_LAYOUT layout);
	TAB_LAYOUT GetLayout() const;
	void SetBehavior(TAB_BEHAVIOR behavior);
	TAB_BEHAVIOR GetBehavior() const;
	bool SetImageLists(CImageList *pImageList, CImageList *pImageListDisabled);
	void SetImageListsRef(CImageList *pImageList, CImageList *pImageListDisabled);
	void GetImageLists(CImageList **pImageList/*out*/, CImageList **pImageListDisabled/*out*/);
	void GetImageSizes(CSize *pszImage/*out*/, CSize *pszImageDisabled/*out*/) const;
	bool SetSystemImageList(CImageList *pImageList);
	void SetSystemImageListRef(CImageList *pImageList);
	CImageList *GetSystemImageList() const;
	CSize GetSystemImageSize() const;
	bool SetCursor(UINT uId);
	bool SetCursor(HMODULE hModule, UINT uId);
	bool SetCursor(HCURSOR hCursor);
	void SetCursorRef(HCURSOR *phCursor);
	HCURSOR GetCursor() const;
	bool SetFont(CFont *pFont);
	void SetFontRef(CFont *pFont);
	bool SetFont(LOGFONT const *pLf);
	CFont *GetFont();
	bool SetFontSelect(CFont *pFont);
	void SetFontSelectRef(CFont *pFont);
	bool SetFontSelect(LOGFONT const *pLf);
	CFont *GetFontSelect();
	void ToolTipEnable(bool enable);
	bool IsToolTipEnable() const;
	void SetTabTooltipText(HTAB hTab, TCHAR const *text) throw(std::bad_alloc);
	CString GetTabTooltipText(HTAB hTab) const;
	void SetCloseButToolTipText(TCHAR const *text) throw(std::bad_alloc);
	CString GetCloseButToolTipText() const;
	void SetMenuButToolTipText(TCHAR const *text) throw(std::bad_alloc);
	CString GetMenuButToolTipText() const;
	void SetLeftScrollButToolTipText(TCHAR const *text) throw(std::bad_alloc);
	CString GetLeftScrollButToolTipText() const;
	void SetRightScrollButToolTipText(TCHAR const *text) throw(std::bad_alloc);
	CString GetRightScrollButToolTipText() const;
	void SetTabText(HTAB hTab, TCHAR const *text) throw(std::bad_alloc);
	CString GetTabText(HTAB hTab) const;
	void SetTabImage(HTAB hTab, int image);
	int GetTabImage(HTAB hTab) const;
	void SetTabWnd(HTAB hTab, HWND hWnd);
	HWND GetTabWnd(HTAB hTab) const;
	void SetTabData(HTAB hTab, __int64 data);
	__int64 GetTabData(HTAB hTab) const;
	void CopyTab(HTAB hDst, CCtrl_TabWnd const *pTabCtrlSrc, HTAB hSrc) throw(std::bad_alloc);
	int GetCount() const;
	HTAB GetTab(int idx) const;	// idx - index of tab (>=0).
	HTAB GetFirstEnableTab() const;
	HTAB GetPrevEnableTab(HTAB hTab) const;
	HTAB GetNextEnableTab(HTAB hTab) const;
	void SetSel(HTAB hTab);
	HTAB GetSel() const;
	HTAB GetHover() const;
	HTAB GetPushed() const;
	void Disable(HTAB hTab, bool disable) const;
	bool IsDisable(HTAB hTab) const;
	HTAB HitTest(CPoint point) const;
	int GetIndex(HTAB hTab) const;
	bool IsExist(HTAB hTab) const;
	HTAB GetTabWithWindowID(int id) const;
	int CmpTabs(HTAB hTab1, HTAB hTab2) const;
	void GetTabRect(HTAB hTab, RECT *rc/*out*/) const;
	bool IsTabVisible(HTAB hTab, bool *partially/*out*/) const;
	void EnsureVisible(HTAB hTab);
	void ScrollToBegin();
	void ScrollToEnd();
	void SetScrollingStep(int step);
	int GetScrollingStep() const;
	void ShowBorder(bool show);
	bool IsShowBorder() const;
	void EqualTabsSize(bool equal);
	bool IsEqualTabsSize() const;
	void RemoveTabEnable(bool enable);
	bool IsRemoveTabEnable() const;
	void HideSingleTab(bool hide);
	bool IsHideSingleTab() const;
	void ShowCloseButton(bool show);
	bool IsShowCloseButton() const;
	void ShowMenuButton(bool show);
	bool IsShowMenuButton() const;
	void ShowScrollButtons(bool show);
	bool IsShowScrollButtons() const;
	bool IsDragging() const;
	void CancelDragging();
	void WatchActivityCtrl(bool watch);
	bool IsWatchActivityCtrl() const;
	bool IsActive() const;
	RECT GetCtrlArea() const;
	RECT GetTabsArea() const;
	RECT GetWindowsArea() const;
	int CalcCtrlAreaHeight();
	RECT GetCloseButtonRect() const;
	RECT GetMenuButtonRect() const;
	RECT GetScrollLeftRect() const;
	RECT GetScrollRightRect() const;
	bool LoadState(CWinApp *app, TCHAR const *section, TCHAR const *entry);
	bool SaveState(CWinApp *app, TCHAR const *section, TCHAR const *entry) const;
	bool LoadState(CArchive *ar);
	bool SaveState(CArchive *ar) const;
	void																						SetTabObj_DrawIndex( void* pTabObj, int nDrawIndex );
	void																						SetTabObj_ImageIndex( void* pTabObj, int nImageIndex );

public:
	int GetBorderWidth();
	CRect GetCtrlAreaPadding();
	CRect GetWndsAreaPadding();
	CRect GetTabHorzMargin();
	CRect GetTabPadding();
	int GetTabImageTextGap();
	int GetTabExtraWidth(HTAB hTab);
	int GetTabMinWidth();
	CRect GetCloseButtonHorzMargin();
	CRect GetMenuButtonHorzMargin();
	CRect GetLeftScrollButtonHorzMargin();
	CRect GetRightScrollButtonHorzMargin();

public:
	CToolTipCtrl *GetToolTip() const;

public:
	enum IMAGE
	{	IMAGE_CLOSE,
		IMAGE_MENU, 
		IMAGE_MENU_PARTIAL,
		IMAGE_ARROW_LEFT, 
		IMAGE_ARROW_LEFT_DISABLE,
		IMAGE_ARROW_RIGHT, 
		IMAGE_ARROW_RIGHT_DISABLE
	};

private:
	TAB_LAYOUT m_Layout;
	TAB_BEHAVIOR m_Behavior;
	CImageList m_ImageList,*m_pImageListRef, m_ImageListDisabled,*m_pImageListDisabledRef;
	CSize m_szImage, m_szImageDisabled;
	CImageList m_SysImageList, *m_pSysImageListRef;
	CSize m_szSysImage;
	CFont m_Font,*m_pFontRef, m_FontSelect,*m_pFontSelectRef;
	HCURSOR m_hCursor, *m_phCursorRef;
	CString m_sToolTip;
	CString m_sToolTipButClose, m_sToolTipButMenu, m_sToolTipButLeftScroll,m_sToolTipButRightScroll;
	// 
	bool m_bShowBorder;
	bool m_bEqualTabsSize;
	bool m_bRemoveTabEnable;
	bool m_bHideSingleTab;
	bool m_bShowCloseButton;
	bool m_bShowMenuButton;
	bool m_bShowScrollButtons;
	bool m_bToolTipEnable;
	int m_iScrollingStep;
	TabCtrlDraw *m_pDrawManager;
	ITabCtrlRecalc *m_pRecalcManager;
	ITabCtrlBehavior *m_pBehaviorManager;
	ITabCtrlToolTip *m_pToolTipManager;
	TabCtrlUserAbility *m_pAbilityManager;
	TabCtrlNotify *m_pNotifyManager;
	const HTAB m_hButClose, m_hButMenu, m_hButLeftScroll,m_hButRightScroll;

private:
	struct Tab : public _HTAB
	{	HWND hWnd;
		int image;
		CString text, tooltipText;
		bool disable;
		__int64 data;
		CRect rc;
		int width;
		int nDrawIndex;
	};
	HTAB m_hCurTab;
	HTAB m_hHoverArea, m_hPushedArea;
	int m_iTabsOffset, m_iMaxTabsOffset;
	bool m_bPartialView, m_bScrollLeftAllow,m_bScrollRightAllow;
	bool m_bTabDrag;
	CPoint m_ptStartTabDrag;
	bool m_bWatchActivityCtrl;
	bool m_bActive;
	CRect m_rcCtrlArea, m_rcTabs, m_rcWindows;
	CRect m_rcCloseButton, m_rcMenuButton, m_rcScrollLeft,m_rcScrollRight;
	CToolTipCtrl *m_pToolTip;
	std::vector<Tab *> m_vtrtabs;
	typedef std::vector<Tab *>::iterator m_i_vtrtabs;
	typedef std::vector<Tab *>::reverse_iterator m_ri_vtrtabs;
	typedef std::vector<Tab *>::const_iterator m_ci_vtrtabs;
	ActivityHook m_ActivityHook;
	KeyboardHook m_KeyboardHook;
	std::vector<Tab *> m_vtrtabs_dragstore;
	bool *m_pLifeStatus;

private:
	struct Service : public ITabCtrlRecalc, public ITabCtrlBehavior, public ITabCtrlToolTip
	{
		virtual int GetBorderWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual CRect GetCtrlAreaPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual CRect GetWndsAreaPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual CRect GetTabHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual CRect GetTabPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual int GetTabImageTextGap(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual int GetTabExtraWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base, HTAB hTab);
		virtual int GetTabMinWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual CRect GetLeftScrollButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual CRect GetRightScrollButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual CRect GetMenuButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual CRect GetCloseButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
		virtual HTAB HitTest(CCtrl_TabWnd *pCtrl, ITabCtrlBehavior *base, CPoint point);
		virtual bool SetCursor(CCtrl_TabWnd *pCtrl, ITabCtrlBehavior *base);
		virtual CToolTipCtrl *CreateToolTip(CCtrl_TabWnd *pCtrl, ITabCtrlToolTip *base);
		CToolTipCtrl m_ToolTipCtrl;
	} m_Service;

private:
	virtual void OnActive(bool active, HWND wnd);
	virtual void OnKeyDown(UINT keyCode, UINT msgFlag);

private:
	HTAB Insert(m_i_vtrtabs before, HWND hWnd, TCHAR const *text, int image) throw(std::bad_alloc);
	void Recalc(bool redraw);
	int GetFullTabsWidth();
	void RecalcScale(int iVisibleTabsWidth, int iFullTabsWidth);
	void RecalcScroll();
	void CalcTabsWidth();
	int CalcTabWidth(HTAB hTab);
	HTAB GetTabWithWindowID(int id, HTAB exceptTab) const;
	bool GetTabAndIndex(int id, HTAB *tab/*out*/, int *idx/*out*/) const;
	void LButtonDown(CPoint point);
	bool IsSystemButton(HTAB hTab) const;
	void StepLeft();
	void StepRight();
	void StopScrolling();
	void StartDragging(CPoint point);
	void StopDragging(bool cancel);
	void AssignHoverArea(CPoint point);
	void SetFocusInChildWnd();
	bool LoadStateInner(CArchive *ar);
	void SaveStateInner(CArchive *ar) const;
	void TtnNeedText(NMTTDISPINFO *pTT);

private:
	DECLARE_DYNCREATE(CCtrl_TabWnd)

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

/////////////////////////////////////////////////////////////////////////////
struct AFX_EXT_CLASS TabCtrlRecalcStub : public ITabCtrlRecalc
{	
	virtual int GetBorderWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetCtrlAreaPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetWndsAreaPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetTabHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetTabPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual int GetTabImageTextGap(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);	// IMAGE<- gap ->TEXT.
	virtual int GetTabExtraWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base, HTAB hTab);
	virtual int GetTabMinWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetLeftScrollButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetRightScrollButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetMenuButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetCloseButtonHorzMargin(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
};

/////////////////////////////////////////////////////////////////////////////
struct AFX_EXT_CLASS TabCtrlBehaviorStub : public ITabCtrlBehavior
{	
	virtual HTAB HitTest(CCtrl_TabWnd *pCtrl, ITabCtrlBehavior *base, CPoint point);		// get tab in the given point.
	virtual bool SetCursor(CCtrl_TabWnd *pCtrl, ITabCtrlBehavior *base);
};

/////////////////////////////////////////////////////////////////////////////
struct AFX_EXT_CLASS TabCtrlStyle_base : public ITabCtrlStyle, public TabCtrlDraw
{
	virtual TabCtrlDraw *GetDrawManager();
	virtual ITabCtrlRecalc *GetRecalcManager();
	virtual ITabCtrlBehavior *GetBehaviorManager();
	virtual ITabCtrlToolTip *GetToolTipManager();

protected:
	virtual void DrawBorder(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect);
//	virtual void DrawCtrlAreaBack(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect);
	virtual void DrawTab(CCtrl_TabWnd *pCtrl, CDC *pDC, HTAB hTab, CRgn *pRgn);
	virtual void DrawButtonClose(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect, bool bHover, bool bPushed);
	virtual void DrawButtonMenu(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect, bool bHover, bool bPushed, bool bPartialView);
	virtual void DrawButtonScrollLeft(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect, bool bHover, bool bPushed, bool bPartialView);
	virtual void DrawButtonScrollRight(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect, bool bHover, bool bPushed, bool bPartialView);
	virtual void DrawWndsAreaBack(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect);

protected:
	virtual COLORREF GetBorderColor(CCtrl_TabWnd *pCtrl);
	virtual COLORREF GetTabBorderColor(CCtrl_TabWnd *pCtrl);
	virtual COLORREF GetCtrlAreaBackColor(CCtrl_TabWnd *pCtrl);
	virtual COLORREF GetWndsAreaBackColor(CCtrl_TabWnd *pCtrl);
	virtual COLORREF GetTabSelectedBackColor(CCtrl_TabWnd *pCtrl);
	virtual COLORREF GetTabTextColor(CCtrl_TabWnd *pCtrl, HTAB hTab);
	virtual COLORREF GetButtonMarkerColor(CCtrl_TabWnd *pCtrl, bool bHover, bool bPushed);
	virtual COLORREF GetChildWndBackColor(CCtrl_TabWnd *pCtrl);
	virtual COLORREF GetEmptyWndsAreaBackColor(CCtrl_TabWnd *pCtrl);

protected:
	virtual void DrawTabBack(CCtrl_TabWnd *pCtrl, CDC *pDC, HTAB hTab, CRect const *pRect, CRgn *pRgn);	// draw background of tab.
	virtual void DrawTabContext(CCtrl_TabWnd *pCtrl, CDC *pDC, HTAB hTab, CRect const *pRect, CRgn *pRgn);	// draw image and text.
	virtual void DrawTabImage(CCtrl_TabWnd *pCtrl, CDC *pDC, HTAB hTab, CRect const *pRect, CRgn *pRgn);
	virtual void DrawTabText(CCtrl_TabWnd *pCtrl, CDC *pDC, HTAB hTab, CRect const *pRect, CRgn *pRgn);
	virtual void DrawButtonFrame(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect, bool bHover, bool bPushed);	// draw close, menu or scroll button without image.
};

/////////////////////////////////////////////////////////////////////////////
struct AFX_EXT_CLASS TabCtrlStyle_VS2008_client_base : public TabCtrlStyle_base, public TabCtrlRecalcStub, public TabCtrlBehaviorStub
{
	virtual ITabCtrlRecalc *GetRecalcManager();
	virtual ITabCtrlBehavior *GetBehaviorManager();

protected:
	virtual int GetBorderWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetCtrlAreaPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetWndsAreaPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual CRect GetTabPadding(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);
	virtual int GetTabExtraWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base, HTAB hTab);
	virtual int GetTabMinWidth(CCtrl_TabWnd *pCtrl, ITabCtrlRecalc *base);

protected:
	virtual HTAB HitTest(CCtrl_TabWnd *pCtrl, ITabCtrlBehavior *base, CPoint point);		// get tab in the given point.

protected:
	virtual bool IsStraightOrder(CCtrl_TabWnd *pCtrl);
	virtual void DrawCtrlAreaBack(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect, int nDrawIndex);
	virtual void DrawWndsAreaBack(CCtrl_TabWnd *pCtrl, CDC *pDC, CRect const *pRect);

protected:
	virtual void DrawTabBack(CCtrl_TabWnd *pCtrl, CDC *pDC, HTAB hTab, CRect const *pRect, CRgn *pRgn);
	virtual void DrawTabContext(CCtrl_TabWnd *pCtrl, CDC *pDC, HTAB hTab, CRect const *pRect, CRgn *pRgn);	// draw image and text.

protected:
	virtual COLORREF GetTabBorderColor(CCtrl_TabWnd *pCtrl, bool active, bool disable) = 0;
	virtual COLORREF GetTabOutlineColor(CCtrl_TabWnd *pCtrl, bool active, bool hover, bool disable, bool left) = 0;
	virtual COLORREF GetTabGradientLightColor(CCtrl_TabWnd *pCtrl, bool active, bool hover, bool disable) = 0;
	virtual COLORREF GetTabGradientDarkColor(CCtrl_TabWnd *pCtrl, bool active, bool hover, bool disable) = 0;

protected:
	void GetTabOutline(CCtrl_TabWnd *pCtrl, HTAB hTab, CRect const *rect, bool top, POINT pts[8]/*out*/, RECT *rcFill/*out*/) const;
	bool HitTest(CCtrl_TabWnd *pCtrl, HTAB hTab, bool top, CPoint point) const;
	int GetSlantWidth(CCtrl_TabWnd *pCtrl) const;
};

/////////////////////////////////////////////////////////////////////////////
struct AFX_EXT_CLASS TabCtrlStyle_VS2008_client_classic : public TabCtrlStyle_VS2008_client_base
{
protected:
	virtual COLORREF GetTabBorderColor(CCtrl_TabWnd *pCtrl, bool active, bool disable);
	virtual COLORREF GetTabOutlineColor(CCtrl_TabWnd *pCtrl, bool active, bool hover, bool disable, bool left);
	virtual COLORREF GetTabGradientLightColor(CCtrl_TabWnd *pCtrl, bool active, bool hover, bool disable);
	virtual COLORREF GetTabGradientDarkColor(CCtrl_TabWnd *pCtrl, bool active, bool hover, bool disable);
};

/////////////////////////////////////////////////////////////////////////////
#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////
