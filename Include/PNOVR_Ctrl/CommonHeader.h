#pragma once

//////////////////////////////////////////////////////////////////////////
// Ctrl Infor
//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#define			_tstring								wstring
#else
#define			_tstring								string
#endif
typedef			vector< CString >						VECCSTR;

#define			PNO_PI									3.1415926535897932384626433832

#define			START_USERMSG							150
#define			MAINFRAME_SENDMSG						( WM_USER + START_USERMSG )
#define			MAINFRAME_SENDDLGMSG					( WM_USER + START_USERMSG + 1 )
#define			MAINFRAME_SENDMENUMSG					( WM_USER + START_USERMSG + 2 )
#define			MAINFRAME_SENDfPRGINFOR					( WM_USER + START_USERMSG + 3 )
#define			PNOVR_CTRL_LABEL_SENDMSG				( WM_USER + START_USERMSG + 11 )
#define			PNOVR_LISTEDITRETURN					( WM_USER + START_USERMSG + 12 )
#define			PNOVR_LISTDBCLICK						( WM_USER + START_USERMSG + 21 )
#define			PNOVR_LISTBOXLCLICK						( WM_USER + START_USERMSG + 22 )
#define			PNOVR_LISTRBTNCLICK						( WM_USER + START_USERMSG + 23 )
#define			PNOVR_LISTCTRLITEMDATACHANGE			( WM_USER + START_USERMSG + 24 )
#define			PNOVR_LISTBOX_COLOR_LCLICK				( WM_USER + START_USERMSG + 31 )
#define			GRIDCTRL_SENDMSGDATA					( WM_USER + START_USERMSG + 41 )
#define			GRIDCTRL_FINDDLG_SENDMSGDATA			( WM_USER + START_USERMSG + 42 )
#define			IMAGEANI_MOUSE_SENDMSG					( WM_USER + START_USERMSG + 51 )
#define			EVENTMSG_ALRAMWND_2_MAINFRAME			( WM_USER + START_USERMSG + 63 )

#define			MAINFRAMECOLOR							RGB( 70, 80, 100 )
#define			MAINFRAMESELECTCOLOR					RGB( 196, 196, 196 )
#define			MAINFRAMESELECTROUNTCOLOR				RGB( 151, 151, 151 )
#define			MAINFRAMETEXTCOLOR						RGB( 255, 255, 255 )
#define			COLOR_WHITE								RGB( 255, 255, 255 )
#define			COLOR_BLACK								RGB( 0, 0, 0 )

#define			MAINFRAMETOOLBAR_IMAGECOUNT				30
#define			MAINFRAMETOOLBAR_ICONMAKECOLOR			RGB( 0, 0, 0 )
#define			MAINFRAMETOOLBAR_EDITWIDTH				75
#define			MAINFRAMETOOLBAR_EDITHEIGHT				20

#define			MAINFRAMETOOLBAR_FRAMEICONPOS_X			7
#define			MAINFRAMETOOLBAR_FRAMEICONPOS_Y			2

#define			MONITOR_CENTER							0x0001
#define			MONITOR_PART_MAX						0x0002
#define			MONITOR_CLIP							0x0000
#define			MONITOR_WORKAREA						0x0010
#define			MONITOR_AREA							0x0000

#define			MARGIN_BOX								10
#define			MARGIN_MAXIMIZED						2

#define			CTRLLABEL_OBJECTS_COUNT					4

static const	RGBTRIPLE	MAINFFRAMETOOLBAR_BGCOLOR	= {192, 192, 192};

struct PNOVR_EraseObjData
{
	template <typename _container> void operator()( _container* pObjdata )
	{
		if( pObjdata != NULL )
		{
			delete pObjdata;
			pObjdata = NULL;
		}
	}
};

static enum FILEACCESSMODE {
	FILEACCESS_EXISTENCE_ONLY		= 0,
	FILEACCESS_WRITE_ONLY			= 2,
	FILEACCESS_READ_ONLY			= 4,
	FILEACCESS_READ_AND_WRITE		= 6
};

static enum SKINLISTCTRLTYPE {
	SKINLISTCTRLTYPE_NORMAL			= 0,
	SKINLISTCTRLTYPE_CHECKBOX,
	SKINLISTCTRLTYPE_EDIT
};

//////////////////////////////////////////////////////////////////////////
// Ctrl End
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// MainFrm Skin
//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------
#define IDX_SYSTEMICON_CAPTION_LEFT					0
#define IDX_SYSTEMICON_CAPTION_CENTER				1
#define IDX_SYSTEMICON_CAPTION_RIGHT				2
#define IDX_SYSTEMICON_FRAME_LEFT					3
#define IDX_SYSTEMICON_FRAME_LEFT_BOTTOM			4
#define IDX_SYSTEMICON_FRAME_BOTTOM					5
#define IDX_SYSTEMICON_FRAME_RIGHT					6
#define IDX_SYSTEMICON_FRAME_RIGHT_BOTTOM			7
#define IDX_CAPTION_MINIMIZE_ON						8
#define IDX_CAPTION_MINIMIZE_OFF					9
#define IDX_CAPTION_MINIMIZE_DOWN					10
#define IDX_CAPTION_MINIMIZE_DISABLE				11
#define IDX_CAPTION_MAXIMIZE_ON						12
#define IDX_CAPTION_MAXIMIZE_OFF					13
#define IDX_CAPTION_MAXIMIZE_DOWN					14
#define IDX_CAPTION_MAXIMIZE_DISABLE				15
#define IDX_CAPTION_EXIT_ON							16
#define IDX_CAPTION_EXIT_OFF						17
#define IDX_CAPTION_EXIT_DOWN						18
#define IDX_CAPTION_EXIT_DISABLE					19
#define IDX_CAPTION_RESTORE_ON						20
#define IDX_CAPTION_RESTORE_OFF						21
#define IDX_CAPTION_RESTORE_DOWN					22
#define IDX_CAPTION_RESTORE_DISABLE					23

// #define IDX_CHILDICON_CAPTION_LEFT					24
// #define IDX_CHILDICON_CAPTION_CENTER				25
// #define IDX_CHILDICON_CAPTION_RIGHT					26
// #define IDX_CHILDICON_FRAME_LEFT					27
// #define IDX_CHILDICON_FRAME_LEFT_BOTTOM				28
// #define IDX_CHILDICON_FRAME_BOTTOM					29
// #define IDX_CHILDICON_FRAME_RIGHT					30
// #define IDX_CHILDICON_FRAME_RIGHT_BOTTOM			31
// #define IDX_CHILDCAPTION_MINIMIZE_ON				32
// #define IDX_CHILDCAPTION_MINIMIZE_OFF				33
// #define IDX_CHILDCAPTION_MINIMIZE_DOWN				34
// #define IDX_CHILDCAPTION_EXIT_ON					35
// #define IDX_CHILDCAPTION_EXIT_OFF					36
// #define IDX_CHILDCAPTION_EXIT_DOWN					37
// #define IDX_CHILDCAPTION_RESTORE_ON					38
// #define IDX_CHILDCAPTION_RESTORE_OFF				39
// #define IDX_CHILDCAPTION_RESTORE_DOWN				40
// 
// #define IDX_DLG_SYSTEMICON_CAPTION_LEFT				41
// #define IDX_DLG_SYSTEMICON_CAPTION_CENTER			42
// #define IDX_DLG_SYSTEMICON_CAPTION_RIGHT			43
// #define IDX_DLG_SYSTEMICON_FRAME_LEFT				44
// #define IDX_DLG_SYSTEMICON_FRAME_LEFT_BOTTOM		45
// #define IDX_DLG_SYSTEMICON_FRAME_BOTTOM				46
// #define IDX_DLG_SYSTEMICON_FRAME_RIGHT				47
// #define IDX_DLG_SYSTEMICON_FRAME_RIGHT_BOTTOM		48
// #define IDX_DLG_CAPTION_SYSTEMICON_ON				49
// #define IDX_DLG_CAPTION_SYSTEMICON_OFF				50
// #define IDX_DLG_CAPTION_SYSTEMICON_DOWN				51
// #define IDX_DLG_CAPTION_MINIMIZE_ON					52
// #define IDX_DLG_CAPTION_MINIMIZE_OFF				53
// #define IDX_DLG_CAPTION_MINIMIZE_DOWN				54
// #define IDX_DLG_CAPTION_MAXIMIZE_ON					55
// #define IDX_DLG_CAPTION_MAXIMIZE_OFF				56
// #define IDX_DLG_CAPTION_MAXIMIZE_DOWN				57
// #define IDX_DLG_CAPTION_EXIT_ON						58
// #define IDX_DLG_CAPTION_EXIT_OFF					59
// #define IDX_DLG_CAPTION_EXIT_DOWN					60
// #define IDX_DLG_CAPTION_RESTORE_ON					61
// #define IDX_DLG_CAPTION_RESTORE_OFF					62
// #define IDX_DLG_CAPTION_RESTORE_DOWN				63

#define IDX_SYSTEMICON_MAX							24

//--------------------------------------------------------

#define CAPTION_SYSTEM								0x01
#define CAPTION_MIN									0x02
#define CAPTION_MAX									0x04
#define CAPTION_EXIT								0x08

//--------------------------------------------------------
#define IDX_ICON_OK									0
#define IDX_ICON_MAX								1
//--------------------------------------------------------
#define ICON16										0
#define ICON24										1
#define ICON32										2
//--------------------------------------------------------
#define IDX_PNOVR_FONT_SMALL						0
#define IDX_PNOVR_FONT_MEDIUM						1
#define IDX_PNOVR_FONT_LARGE						2
#define IDX_PNOVR_FONT_MAX							3
//--------------------------------------------------------
#define IDX_PNOVR_RGB_MASK							0
#define IDX_PNOVR_RGB_BACKGROUND					1
#define IDX_PNOVR_RGB_FONT							2
#define IDX_PNOVR_RGB_FONT_CAPTION					3
#define IDX_PNOVR_RGB_FONT_CAPTION_INACTIVE			4
#define IDX_PNOVR_RGB_CONTROL_BORDER				5
#define IDX_PNOVR_RGB_MAX							6

#define GETSYSTEMCHECKYV							2000
#define GETSYSTEMCHECKYD							_T("1017/0506")			// 2와 1을 더한다
#define GETSYSTEMCHECKYW							0

typedef BOOL ( WINAPI *LPFounction )( HWND hWnd, COLORREF _Color, BYTE bAlpha, DWORD dwFlags );

typedef struct tagPNOVR_BITMAP_INFO
{
	HBITMAP hBitmap;
	BITMAP	bitmap;
} PNOVR_BITMAP_INFO;


//////////////////////////////////////////////////////////////////////////
// Skin End
//////////////////////////////////////////////////////////////////////////


#define	IDX_CTRL_LISTCTRLBMP_MAX					13
#define	FRAME_CTRLBAR								15001

enum PNOUTIL_LOCK {
	PNOLOCK_CRITICALSECTION								= 1,
	PNOLOCK_MUTEX
};

//////////////////////////////////////////////////////////////////////////

//typedef struct CallbackWndData
//{
//	DWORD	m_PID;
//	HWND	m_hWnd;
//} CallbackWndData;

#define								ANIINDILINEDISP_SIZE										20
#define								FRMCOMMANDICON_MENU_START									32001

static enum FRMCOMMANDICONTYPE {
	FRMCOMMANDICONTYPE_ICON																		= 0,
	FRMCOMMANDICONTYPE_SEPARATOR,
	FRMCOMMANDICONTYPE_MENU
};

static enum FRMCOMMANDICONSTATUS {
	FRMCOMMANDICONSTATUS_NULL																	= 0,
	FRMCOMMANDICONSTATUS_DEACTIVE,
	FRMCOMMANDICONSTATUS_NORMAL,
	FRMCOMMANDICONSTATUS_OVER,
	FRMCOMMANDICONSTATUS_DOWN,
};

static enum FRMCOMMANDICONSTATUSSELECT {
	FRMCOMMANDICONSTATUSSELECT_NULL																	= 0,
	FRMCOMMANDICONSTATUSSELECT_DEACTIVE,
	FRMCOMMANDICONSTATUSSELECT_SELECTNORMAL,
	FRMCOMMANDICONSTATUSSELECT_SELECTOVER,
	FRMCOMMANDICONSTATUSSELECT_SELECTDOWN
};

static enum FRMCOMMANDMENUTYPE {
	FRMCOMMANDMENUTYPE_NORMAL																		= 0,
	FRMCOMMANDMENUTYPE_POPUP, 
	FRMCOMMANDMENUTYPE_RADIO,
	FRMCOMMANDMENUTYPE_CHECKBOX
};

static enum EventMsg_AlramDoc2MainFrame
{
	EveMSG_A2MFrame_ReFresh		= 0, 
	EveMSG_A2MFrame_ShowHide
};

static enum DOCKINGPANOUTLISTBOXTYPE
{
	DockingPan_OutputListBox_Single		= 1, 
	DockingPan_OutputListBox_Multi
};