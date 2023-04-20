#pragma once

#include "Ctrl_ColorMenuItem.h"

class AFX_EXT_CLASS CCtrl_ColorMenu :	public CMenu
{
public:
	CCtrl_ColorMenu(void);
	CCtrl_ColorMenu( CSize csSize );
	~CCtrl_ColorMenu(void);

	BOOL																							AppendMenu( UINT unID, TCHAR* szName, UINT unFlag, int nType = 1, int nDefault = 0, Bitmap* pIcon = NULL ); 
	void																							SetColor( Color clrNormalTxt, Color clrNormalBk, Color clrHighligntTxt, Color clrHighligntBk, Color clrHighligntLine );
	BOOL																							IsID( UINT unID );
	int																								GetValue( UINT unID );
	void																							SetValue( UINT unID, int nValue );
	void																							ResetValue();

protected:
	void																							DrawIconCheckBox( CDC* pDC, CCtrl_ColorMenuItem* pItemData, CPoint ptIconStart, COLORREF clrTxt, COLORREF clrBk );

protected:
	CSize							m_csSize;
	Color							m_clrNormal_Txt, m_clrNormal_Bk;
	Color							m_clrHighlight_Txt, m_clrHighlight_Bk, m_clrHighlight_Line;

	VECCOLORMENUITEM				m_vecMenuItems;

// 함수
protected:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

// 변수
protected:


};

