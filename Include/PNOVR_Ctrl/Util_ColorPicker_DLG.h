#pragma once

class CColorPicker_Obj
{
public:
	CColorPicker_Obj() : m_rectColorBox( 0, 0, 0, 0 ), m_csColor( RGB( 255, 255, 255 ) ), m_bSelect( FALSE )
	{
		//
	};
	~CColorPicker_Obj()
	{
		//
	};

	void DrawObj( CDC* pDC )
	{
		CPen						Pen;
		if( !m_bSelect )
		{
			Pen.CreatePen( PS_SOLID, 1, RGB( 225, 225, 225 ) );
		} else {
			Pen.CreatePen( PS_SOLID, 2, RGB( 255, 64, 16 ) );
		}
		CPen*						pOldPen													= pDC->SelectObject( &Pen );
		CBrush						_brush( m_csColor );
		CBrush*						pOldBrush												= pDC->SelectObject( &_brush );

//		pDC->RoundRect( m_rectColorBox, CPoint( 2, 2 ) );
		pDC->Rectangle( m_rectColorBox );

		Pen.DeleteObject();
		pDC->SelectObject( pOldBrush );
		_brush.DeleteObject();
		pDC->SelectObject( pOldPen );
	};


	CRect							m_rectColorBox;
	COLORREF						m_csColor;
	BOOL							m_bSelect;
};

#define								COLORINDEXCOUNT												270


// CUtil_ColorPicker_DLG 대화 상자입니다.
class AFX_EXT_CLASS CUtil_ColorPicker_DLG : public CDialog
{
	DECLARE_DYNAMIC(CUtil_ColorPicker_DLG)

public:
	CUtil_ColorPicker_DLG(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUtil_ColorPicker_DLG();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COLORPICKER_DLG };

public:
	void																						SetInitColor();
	void																						SetInitRect();
	CRect																						GetViewRect();
	void																						SetViewRect( CRect rectView, int nMode = 1 );
	COLORREF																					GetRtColor();

protected:
	COLORREF																					SetCalcuInterColor( COLORREF clrSt, COLORREF clrEn, double dCalcuIn, double dCalcuOut );
	void																						SetCalcuColor( int nCalIndex , COLORREF clrSt, COLORREF clrEn );

protected:
	CColorPicker_Obj				m_csColorCtrl[ COLORINDEXCOUNT ];
	CRect							m_rectView, m_rectMvWnd;
	int								m_nOldSelect;
	CRect							m_rectExit;

	COLORREF						m_clrReturnValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
