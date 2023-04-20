#pragma once


// CImage_AniPNG_Viewer
class CImage_AniPNG_Object;
typedef								vector<CImage_AniPNG_Object*>								VECANIOBJ;
class AFX_EXT_CLASS CImage_AniPNG_Viewer : public CWnd
{
	DECLARE_DYNAMIC(CImage_AniPNG_Viewer)

public:
	CImage_AniPNG_Viewer();
	virtual ~CImage_AniPNG_Viewer();
	void																						RemoveData();

public:
	void																						SetEditMode( bool bStatus );
	bool																						GetEditMode();
	void																						SetAllDrawAniOBJ( bool bDraw );
	bool																						GetAllDrawAniOBJ();
	void																						SetBGtransparent( bool bStatus );
	bool																						GetBGtransparent();

public:
	BOOL																						Create( const RECT& rect, CWnd* pParentWnd );
	CPoint																						GetViewCenterPoint(){ return m_ptViewCenter; };
	void																						Stop_AniImage( BOOL bThisAniObjSelected = TRUE );

	BOOL																						Layer_Add();
	BOOL																						Layer_Del();
	CImage_AniPNG_Object*																		Layer_Get( TCHAR* szIndex );
	void																						Layer_SetIndex( TCHAR* szIndex );
	void																						Layer_GetIndexs( vector<_tstring>& vecIDs );
	TCHAR*																						Layer_GetSelectIndex();
	TCHAR*																						Layer_GetSelectedOBJindex();
	
	BOOL																						IsSelectedAniOBJ();
	BOOL																						IsAniOBJimage();
	void																						Layer_SetIndexShow( BOOL bShow );
	void																						Layer_SetIndexShow( TCHAR* szObjIndex, BOOL bShow );
	BOOL																						Layer_GetIndexShow();
	BOOL																						Layer_GetIndexShow( TCHAR* szObjIndex );
	void																						Layer_SetIndexColor( COLORREF clrIndex );
	void																						Layer_SetIndexColor( TCHAR* szObjIndex, COLORREF clrIndex );
	COLORREF																					Layer_GetIndexColor();
	COLORREF																					Layer_GetIndexColor( TCHAR* szObjIndex );

	CRect																						Layer_GetImageRect();

	BOOL																						Layer_IsAniOBJInputText();
	void																						Layer_SelectAniOBJInputText( int nInputTxtIndex );
	void																						Layer_AddAniOBJInputText( TCHAR* szInputText );
	void																						Layer_DelAniOBJInputText( int nInputTxtIndex );
	void																						Layer_SetInputTextData( int nIndex, int nTextData );
	void																						Layer_SetInputTextData( int nIndex, TCHAR* szTextData );
	void																						Layer_SetInputTextPosition( int nIndex, int nPosition );
	void																						Layer_SetInputTextOffset( int nIndex, CPoint ptOffset );
	void																						Layer_SetInputTextColor( int nIndex, Color clrText );
	void																						Layer_SetAllInputTextColor( int nIndex, Color clrText );
	void																						Layer_SetInputTextSize( int nIndex, int nSize );		// 0번 부터 시작함
	void																						Layer_SetAllInputTextSize( int nIndex, int nSize );
	void																						Layer_SetInputTextFontType( int nIndex, TCHAR* szFontType );
	void																						Layer_SetAllInputTextFontType( int nIndex, TCHAR* szFontType );
	int																							Layer_GetInputTextCount();
	void																						Layer_GetInputTextInfor( int nIndex, int& nTxtPos, CPoint& ptOffset );
	void																						Layer_ResetSelect();

	int																							Layer_IMAGEOBJ_Add();
	BOOL																						Layer_IMAGEOBJ_Del();
	int																							Layer_IMAGEOBJ_GetCount();
	BOOL																						Layer_IMAGEOBJ_SetSelectDataIndex( TCHAR* szDataIndex );
	void																						Layer_IMAGEOBJ_GetDataIndexs( vector<_tstring>& szIndexs );
	TCHAR*																						Layer_IMAGEOBJ_GetSelectDataIndex();
	int																							Layer_IMAGEOBJ_GetSelectDataPos();

	bool																						IsRun_AniImage();
	void																						Run_AniImage();
	void																						Run_AniImage( UINT unCycle );
	void																						SetAniThreadStatus( bool bStatus );
	bool																						GetAniThreadStatus();
	void																						SetAniImage_BackGroundImage( CDC* pDC );
	void																						DrawAniImage( CDC* pDC );
	void																						SetAniCycleValue( UINT unCycle );
	UINT																						GetAniCycleValue();
	void																						LoadBGimage();
	BOOL																						SetSelectAniObj( CString szAniObjIndex );
	void																						SetUnSelectAniObj();
	BOOL																						CopyOBJ();

	int																							LoadFile();
	int																							LoadFile( TCHAR* szFileName );
	void																						SaveFile();
	void																						SaveAsFile();

	CRect																						GetBKRect();
	void																						SetBGColor( COLORREF clrBG );
	void																						SetBGColor( Color clrBG );
	COLORREF																					GetBGColor();
	void																						SetViewCenterOffSet( CPoint ptOffSet );
	void																						SetParentBkImage( CDC* pDC, CRect rectthisWnd );


protected:
	void																						Reset();
	void																						DrawBKGrid( Graphics* graphics, CRect cdDrawRect, BOOL bEnableMode );
	void																						DrawBGImage( Graphics* graphics );
	void																						DrawMoveSelectObjLine( CDC* pDC, CRect rectMvSelect, BOOL bErase = FALSE );
	void																						DrawAniPNGObj( CDC* pDC, Graphics* graphics, int nDrawMode );
	void																						DrawParentBGImage( Graphics* graphics, CRect rectDrawImg );
	int																							OpenFileData( TCHAR* szFilePath );
	void																						SaveFileData( TCHAR* szFilePath );

	static UINT WINAPI																			AniImage_Process( LPVOID pParam );

protected:
	CPoint							m_ptViewCenter;				// no save
	CPoint							m_ptViewCenterOffset;		// no save
	CImage_AniPNG_Object*			m_pThisAniOBJ;				// no save
	VECANIOBJ						m_vecAniOBJ;				// save
	int								m_unAniCycleValue;			// save

	RectF							m_rectBKImage;				// no save
	BitmapData						m_csBGImageInfor;			// save
	BYTE*							m_pBGImageBuff;				// save
	COLORREF						m_clrBGcolor;				// no save

	HANDLE							m_hAniThread;				// no save
	bool							m_bAniThreadRun;			// no save

	CPoint							m_ptSelectMoveStart;		// no save
	CRect							m_rectStartMove;			// no save
	bool							m_bIsSelectMove;			// no save
	bool							m_bAllDrawAniOBJ;			// save
	bool							m_bEditMode;				// no save
	TCHAR							m_szFilePath[ MAX_PATH ];	// no save
	bool							m_bIsBGtransparent;

	BitmapData						m_csParentBGImageInfor;			// no save
	BYTE*							m_pParentBGImageBuff;				// no save

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


