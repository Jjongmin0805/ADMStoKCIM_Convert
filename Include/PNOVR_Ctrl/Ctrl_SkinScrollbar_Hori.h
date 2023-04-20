#pragma once


// CCtrl_SkinScrollbar_Hori

class CCtrl_SkinScrollbar_Hori : public CStatic
{
	DECLARE_DYNAMIC(CCtrl_SkinScrollbar_Hori)

public:
	CCtrl_SkinScrollbar_Hori();
	virtual ~CCtrl_SkinScrollbar_Hori();

public:
	void									SetParent( CWnd* pParent );
	void									ScrollLeft();
	void									ScrollRight();
	void									PageLeft();
	void									PageRight();
	void									LimitThumbPosition();
	void									UpdateThumbPosition();
	void									Draw();

protected:

protected:
	CWnd*									m_pParent;
	bool									m_bMDClick, m_bMDArrowLeft, m_bMDArrowRight, m_bDragging;
	int										m_nThumbLeft;
	double									m_dThumbRemainder, m_dThumbInterval;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


