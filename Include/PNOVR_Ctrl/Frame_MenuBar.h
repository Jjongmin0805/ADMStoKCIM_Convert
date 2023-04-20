#pragma once

// Frame_MenuBar 대화 상자입니다.
class CFrame_MenuBar_Ctrl;
class AFX_EXT_CLASS CFrame_MenuBar : public CDialog
{
	DECLARE_DYNAMIC(CFrame_MenuBar)

public:
	CFrame_MenuBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFrame_MenuBar();

	void									CreateMenuBarCtrl( UINT nMainResourceID, UINT nSubClassResourceID );
	void									CreateMenuBarCtrl( UINT nResourceID );
	void									DrawMenuDlg();
	CRect									GetWinRect(){ return m_creatRect; };
	CRect									GetControlItemTrect();
	void									SetMoveThisWndReSize();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FRAME_MENUBAR };

protected:
	CFrame_MenuBar_Ctrl*					m_wndMenuBarCtrl;
	CRect									m_creatRect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
};
