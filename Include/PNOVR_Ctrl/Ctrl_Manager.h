#pragma once

class AFX_EXT_CLASS CCtrl_Manager
{
public:
	static CCtrl_Manager*																	instance( void );

protected :
	static CCtrl_Manager*			m_pMngData;

//////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	struct TabColorInfor
	{
		TabColorInfor( void ) :		nIndex( 0 ), 
									clrText( RGB( 0, 0, 0 ) ), 
									clrBorder( RGB( 0, 0, 0 ) ), 
									clrOutline( RGB( 0, 0, 0 ) ), 
									clrTabBackLight( RGB( 0, 0, 0 ) ), 
									clrTabBackDark( RGB( 0, 0, 0 ) )
		{
			//
		};
		~TabColorInfor( void ){};

		int							nIndex;
		COLORREF					clrText;
		COLORREF					clrBorder;
		COLORREF					clrOutline;
		COLORREF					clrTabBackLight;
		COLORREF					clrTabBackDark;
	};

public:
	CCtrl_Manager(void);
	~CCtrl_Manager(void);

	void																					DeleteBitmaps();
	void																					LoadBitmaps();
	void																					LoadConfig();
	HBITMAP																					GethBitmap( UINT unBitmapID );
	PBITMAP																					GetBitmap( UINT unBitmapID );

//////////////////////////////////////////////////////////////////////////

	void																					SetColor_MainBKG( COLORREF crColor ){ m_crMain_bkg = crColor; };
	COLORREF																				GetColor_MainBKG(){ return m_crMain_bkg; };
	int																						GetHeader_Height(){ return m_Ctrl_nHeaderHeightSize; };
	int																						GetList_ItemHeight(){ return m_Ctrl_nItemSize; };
	TCHAR*																					GetList_ItemFontType(){ return (TCHAR*)m_Ctrl_szItemFont_Type.c_str(); };
	COLORREF																				GetColor_SubItem_Text(){ return m_crSubItem_txt; };
	COLORREF																				GetColor_SubItem_BackGround(){ return m_crSubItem_bkg; };
	COLORREF																				GetColor_UnderLine(){ return m_crUnderLine; };
	COLORREF																				GetColor_Tracking_Text(){ return m_crTracking_txt; };
	COLORREF																				GetColor_Tracking_BackGround(){ return m_crTracking_bkg; };
	COLORREF																				GetColor_HighLight_Text(){ return m_crHighLight_txt; };
	COLORREF																				GetColor_HighLight_BackGround(){ return m_crHighLight_bkg; };

//////////////////////////////////////////////////////////////////////////
	COLORREF																				GetColor_Label_Bk(){ return m_crLabelBk; };
	COLORREF																				GetColor_Label_GradientEndBk(){ return m_crLabelGradientEndBk; };
	COLORREF																				GetColor_LabelTitlePoint(){ return m_crLabelTitlePoint; };

//////////////////////////////////////////////////////////////////////////
	void																					SetTabCtrlColorIndex( int nIndex );
	BOOL																					IsTabCtrlColorData();
	COLORREF																				GetTabCtrlColor_Text();
	COLORREF																				GetTabCtrlColor_Border();
	COLORREF																				GetTabCtrlColor_OutLine();
	COLORREF																				GetTabCtrlColor_TabBackLight();
	COLORREF																				GetTabCtrlColor_TabBackDark();

protected :
	_tstring																				GetBitmapFileName( UINT unBitmapID );


protected :
	PNOVR_BITMAP_INFO				m_Ctrl_ListCtrlBMP[ IDX_CTRL_LISTCTRLBMP_MAX ];
	int								m_Ctrl_nHeaderHeightSize;										// List Ctrl Header ≥Ù¿Ã
	int								m_Ctrl_nItemSize;
	_tstring						m_Ctrl_szItemFont_Type;

	COLORREF						m_crMain_bkg;
	COLORREF						m_crSubItem_txt, m_crSubItem_bkg;
	COLORREF						m_crUnderLine;
	COLORREF						m_crTracking_txt, m_crTracking_bkg;
	COLORREF						m_crHighLight_txt, m_crHighLight_bkg;
	COLORREF						m_crLabelBk, m_crLabelGradientEndBk, m_crLabelTitlePoint;
	vector<TabColorInfor>			m_vecTabColor;
	int								m_nVecTabColor_Index;
};

#define								thePNO_Ctrlmng											CCtrl_Manager::instance()