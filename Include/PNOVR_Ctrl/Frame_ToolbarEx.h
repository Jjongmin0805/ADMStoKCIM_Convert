#pragma once


// CFrame_ToolbarEx
class CFrame_MenuBar_Toolbar;
class AFX_EXT_CLASS CFrame_ToolbarEx : public CWnd
{
	DECLARE_DYNAMIC(CFrame_ToolbarEx)

public:
	CFrame_ToolbarEx();
	virtual ~CFrame_ToolbarEx();


	BOOL																							Create( CWnd* pParentWnd, CRect rectWnd, UINT unID );

protected:
	CFrameWndEx*					m_pParentFrm;
	CMFCReBar*						m_wndCTrebar;
	CFrame_MenuBar_Toolbar*			m_pToolbars;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


