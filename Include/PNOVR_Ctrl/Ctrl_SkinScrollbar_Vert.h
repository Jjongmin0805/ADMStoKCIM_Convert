#pragma once


// CCtrl_SkinScrollbar_Vert

class CCtrl_SkinScrollbar_Vert : public CStatic
{
	DECLARE_DYNAMIC(CCtrl_SkinScrollbar_Vert)

public:
	CCtrl_SkinScrollbar_Vert();
	virtual ~CCtrl_SkinScrollbar_Vert();

public:
	void									SetParent( CWnd* pParent );
	void									ScrollUp();
	void									ScrollDown();
	void									PageUp();
	void									PageDown();
	void									LimitThumbPosition();
	void									UpdateThumbPosition();
	void									Draw();

protected:

protected:
	CWnd*									m_pParent;
	bool									m_bMDClick, m_bMDArrowUp, m_bMDArrowDown, m_bDragging;
	int										m_nThumbTop;
	double									m_dThumbInterval;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


