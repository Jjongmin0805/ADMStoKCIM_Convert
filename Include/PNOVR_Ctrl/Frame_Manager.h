#pragma once

#include "Frame_CommandIconSymbol.h"

class AFX_EXT_CLASS CFrame_Manager
{
public:
	CFrame_Manager(void);
	~CFrame_Manager(void);

	static CFrame_Manager*																	instance( void );

protected :
	static CFrame_Manager*			m_pMngData;

//////////////////////////////////////////////////////////////////////////////////////////////////////

// Attributes
public:
	CString							m_szSkinFolderName;

	COLORREF						m_crTitle;
	CFont							*m_pFont[ IDX_PNOVR_FONT_MAX ];
	PNOVR_BITMAP_INFO				m_infoBitmap[IDX_SYSTEMICON_MAX];
	HICON							m_hIcon[ IDX_ICON_MAX ][ 3 ];
	COLORREF						m_cf[ IDX_PNOVR_RGB_MAX ];
	LPFounction						SetLayeredWindowAttributes;
	CRect							m_RectToolbarChildSysMin, m_RectToolbarChildSysMax, m_RectToolbarChildSysExit;
	COLORREF						m_crMenuConfig[ 12 ];
	CString							m_szToolbarFolderName;

	CString							m_szTitle_FontName;					// 윈도우 타이틀 폰트 타입
	int								m_nTitle_FontSize;					// 윈도우 타이틀 폰트 크기
	Color							m_clrTitle_FontColor, m_clrTitle_BGColor;		// 타이틀 폰트 색상, 폰트외 바탕 색상

	Color							m_clrCMDTool_IconBGColor;			// Command Tool Icon 바탕 색상
	Color							m_clrCMDTool_BGColor;				// Command Tool Bar 바탕 색상
	Bitmap*							m_btmCMDTool_Logo, *m_btmCMDTool_Logo_R;					// Command Tool Bar의 이미지, _R right
	CString							m_szCMDTool_Icon_FontName;			// Command Tool Bar Icon 타이틀 폰트 타입
	int								m_nCMDTool_Icon_FontSize;			// Command Tool Bar Icon 타이틀 폰트 크기
	Color							m_clrCMDTool_Icon_FontColor;		// Command Tool Bar Icon 타이틀 폰트 색상
	Color							m_clrCMDTool_Menu_Text, m_clrCMDTool_Menu_BKnormal, m_clrCMDTool_Menu_TextOver, m_clrCMDTool_Menu_BKOver;

	CSize							m_sizeCMDTool_ICON;					// Command Tool Icon 사이즈
	VECCMDICONSYMBOL				m_vecCMDIconSymbols;

	BOOL							m_bActivationFrontLogo;
	BOOL							m_bActivationWeatherInfor;
	Bitmap							*m_btmCMDWeather_Icons[ 20 ];

protected:

// Operations
public:
	void																					SetSkinFolderName( CString szFolderName );
	CString																					GetSkinFolderName();

	COLORREF																				GetTitleColor(){ return m_crTitle; };
	void																					DeleteBitmaps();	
	void																					LoadBitmaps();	
	HBITMAP																					GetBitmap(UINT nBitmapID);
	SIZE																					GetBitmapSize(UINT nBitmapID);
	CString																					GetBitmapFileName(UINT nBitmapID);
	void																					DeleteIcons();
	void																					DeleteCMDIcons();
	void																					DeleteCMDWeatherIcons();

	void																					LoadIcons();
	void																					LoadCMDIcons();
	CString																					GetIconFileName(UINT nIconID);
	HICON																					GetIcon(UINT nIconID, UINT nType);
	void																					SetRGB(UINT nRGBID, COLORREF cf);
	COLORREF																				GetRGB(UINT nRGBID);
	void																					DeleteFonts();
	CFont*																					SetFont(CWnd* pWnd, UINT nFontID, CString sFaceName, UINT nFontSize, BOOL bBold = FALSE);
	CFont*																					GetFont(UINT nFontID);
	void																					LoadConfig();
	COLORREF																				GetMenuCtrlColor( int nIndex );
	
	void																					SetToolbarPathName( CString szToolbarName );
	CString																					GetToolbarPathName();

	Bitmap*																					GetCMDIcon( int nIconIndex, int nStatus, BOOL bSelect );

	void																					SetTitle_FontName( CString szFontName );
	CString																					GetTitle_FontName();
	void																					SetTitle_FontSize( int nFontSize );
	int																						GetTitle_FontSize();
	void																					SetTitle_FontColor( Color clrColor );
	Color																					GetTitle_FontColor();
	void																					SetTitle_BGColor( Color clrColor );
	Color																					GetTitle_BGColor();

	void																					SetCMDTool_IconBGColor( Color clrColor );
	Color																					GetCMDTool_IconBGColor();
	void																					SetCMDTool_BGColor( Color clrColor );
	Color																					GetCMDTool_BGColor();
	void																					SetCMDTool_Logo();
	Bitmap*																					GetCMDTool_Logo();
	BOOL																					IsCMDTool_Logo();
	Bitmap*																					GetCMDTool_Logo_R();
	BOOL																					IsCMDTool_Logo_R();
	void																					SetSize_CMDTool_ICON( int nCX, int nCY );
	void																					SetSize_CMDTool_ICON( CSize sizeIcon );
	CSize																					GetSize_CMDTool_ICON();

	void																					SetCMDTool_Icon_FontName( CString szFontName );
	CString																					GetCMDTool_Icon_FontName();
	void																					SetCMDTool_Icon_FontSize( int nFontSize );
	int																						GetCMDTool_Icon_FontSize();
	void																					SetCMDTool_Icon_FontColor( Color clrColor );
	Color																					GetCMDTool_Icon_FontColor();

	void																					SetCMDTool_Menu_TextColor( Color clrColor );
	Color																					GetCMDTool_Menu_TextColor();
	void																					SetCMDTool_Menu_BKnormalColor( Color clrColor );
	Color																					GetCMDTool_Menu_BKnormalColor();
	void																					SetCMDTool_Menu_TextOverColor( Color clrColor );
	Color																					GetCMDTool_Menu_TextOverColor();
	void																					SetCMDTool_Menu_BKOverColor( Color clrColor );
	Color																					GetCMDTool_Menu_BKOverColor();

	void																					SetActivationFrontLogo( BOOL bActivation );
	BOOL																					GetActivationFrontLogo();
	void																					SetActivationWeatherInfor( BOOL bActivation );
	BOOL																					GetActivationWeatherInfor();
	void																					SetCMDWeatherIcon_Loading();
	Bitmap*																					GetCMDWeatherIcon( UINT unIcon );

};

#define								thePNO_FRMmng											CFrame_Manager::instance()