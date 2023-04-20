#pragma once


// CCtrl_SkinHeader

class CCtrl_SkinHeader : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CCtrl_SkinHeader)

public:
	CCtrl_SkinHeader();
	virtual ~CCtrl_SkinHeader();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void							OnPaint();
	afx_msg BOOL							OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT							OnLayout(WPARAM wparam, LPARAM lparam);
};
