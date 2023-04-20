#pragma once

class AniPNGData_InputText
{
public:
	AniPNGData_InputText(void) : m_nInputTextPosition( 4 ), m_ptInputTextOffset( 0, 0 ), m_clrInputText( Color::Black ), m_nInputTextSize( 12 )
	{
		memset( m_szInputText, 0x00, sizeof( m_szInputText ) );
		memset( m_szInputTextFontType, 0x00, sizeof( m_szInputTextFontType ) );
		swprintf( m_szInputTextFontType, _T("Verdana") );
	};
	~AniPNGData_InputText(void)
	{
		//
	};

	void SetTxt( TCHAR* szText )
	{
		memset( m_szInputText, 0x00, sizeof( m_szInputText ) );
		swprintf( m_szInputText, _T("%s"), szText );
	};
	void SetFontType( TCHAR* szFontType )
	{
		memset( m_szInputTextFontType, 0x00, sizeof( m_szInputTextFontType ) );
		swprintf( m_szInputTextFontType, _T("%s"), szFontType );
	};

	void Load( CFile* pFileStream )
	{
		if( pFileStream == NULL )
		{
			return ;
		}

		pFileStream->Read( m_szInputText, sizeof( m_szInputText ) );
		pFileStream->Read( &m_nInputTextPosition, sizeof( m_nInputTextPosition ) );
		pFileStream->Read( &m_ptInputTextOffset, sizeof( m_ptInputTextOffset ) );
		pFileStream->Read( &m_clrInputText, sizeof( m_clrInputText ) );
		pFileStream->Read( &m_nInputTextSize, sizeof( m_nInputTextSize ) );
		pFileStream->Read( m_szInputTextFontType, sizeof( m_szInputTextFontType ) );
	}

	void Save( CFile* pFileStream )
	{
		if( pFileStream == NULL )
		{
			return ;
		}

		pFileStream->Write( m_szInputText, sizeof( m_szInputText ) );
		pFileStream->Write( &m_nInputTextPosition, sizeof( m_nInputTextPosition ) );
		pFileStream->Write( &m_ptInputTextOffset, sizeof( m_ptInputTextOffset ) );
		pFileStream->Write( &m_clrInputText, sizeof( m_clrInputText ) );
		pFileStream->Write( &m_nInputTextSize, sizeof( m_nInputTextSize ) );
		pFileStream->Write( m_szInputTextFontType, sizeof( m_szInputTextFontType ) );
	}


	TCHAR							m_szInputText[ 512 ];						// save
	int								m_nInputTextPosition;						// save
	CPoint							m_ptInputTextOffset;						// save
	Color							m_clrInputText;								// save
	int								m_nInputTextSize;							// save
	TCHAR							m_szInputTextFontType[ 128 ];				// save
};
typedef vector<AniPNGData_InputText*>															VECIAData_INPUTTXT;

class CImage_AniPNG_Data;
typedef vector<CImage_AniPNG_Data*>																VECIAData;

class CImage_AniPNG_Object
{
public:
	CImage_AniPNG_Object(void);
	CImage_AniPNG_Object( CWnd* pParent, TCHAR* szIndex );
	~CImage_AniPNG_Object(void);


	void																						SetParentWnd( CWnd* pParentWnd );
	CWnd*																						GetParentWnd();
	void																						SetIndex( TCHAR* szIndex );
	TCHAR*																						GetIndex();
	void																						SetIndexColor( COLORREF clrIndex );
	COLORREF																					GetIndexColor();

	void																						AddImage( CImage_AniPNG_Data* pAniPNGObjData );
	int																							AddImage( TCHAR* szImgDataIndex, CPoint ptCenter );
	int																							AddImage_Folder( CPoint ptCenter );
	BOOL																						DelImage();
	CImage_AniPNG_Data*																			GetImage( int nIndex );
	CImage_AniPNG_Data*																			GetImage( TCHAR* szIndex );
	void																						GetImageDataIndexs( vector<_tstring>& szIndexs );
	TCHAR*																						GetImageDataIndex();
//	void																						UpdateStartPoint( CDC* pDC, CPoint ptStart );
	void																						UpdateStartPoint( CPoint ptCenter, CPoint ptStart );
	void																						UpdateStartPoint( CPoint ptCenter );

	void																						Draw( CDC* pDC, Graphics* graphics, BOOL bEditMode );
	int																							GetImageCount();
	CRect																						GetImageDrawRect();
	void																						ANIOBJ_DrawImage( CDC* pDC, Graphics* graphics, BOOL bEditMode );
	void																						ANIOBJ_DrawAni( CDC* pDC, Graphics* graphics, BOOL bEditMode );
	void																						ANIOBJ_NextFrame();
	void																						ANIOBJ_ResetFrame();

	void																						ANIOBJ_AllDrawImage( Graphics* graphics, BOOL bEditMode );
	void																						ANIOBJ_AllDrawAni( Graphics* graphics, BOOL bEditMode );
	void																						ANIOBJ_AllNextFrame();
	void																						ANIOBJ_AllResetFrame();
	int																							ANIOBJ_GetSelectIndexPos();
	AniPNGData_InputText*																		GetInputTxt( int nITindex );

	void																						SetSelect();
	void																						UnsetSelect();
	BOOL																						IsSelect();
	CRect																						GetSelectRect();
	BOOL																						IsPtInSelectRect( CPoint _ptSelect );
	BOOL																						SetSelectImageData( TCHAR* szIndex );
	CImage_AniPNG_Object*																		Copy( TCHAR* szIndex, CPoint ptStart );

	void																						SetIndexShow( BOOL bShow );
	BOOL																						GetIndexShow();
 	BOOL																						IsInputText();
	void																						SelectIndexInputText( int nITindex );
	void																						AddInputText( AniPNGData_InputText* pInputTxtData );
	void																						AddInputText( TCHAR* szTxt );
	void																						DelInputText( int nITindex );
	void																						SetInputTxt_ChangeText( int nITindex, int nIntValue );
	void																						SetInputTxt_ChangeText( int nITindex, TCHAR* szTxt );
	void																						SetInputTxt_PositionIndex( int nITindex, int nPosIndex );
	void																						SetInputTxt_OffSet( int nITindex, CPoint ptOffSet );
	void																						SetInputTxt_Color( int nITindex, Color clrText );
	void																						SetInputTxt_TextSize( int nITindex, int nTextSize );
	void																						SetInputTxt_FontType( int nITindex, TCHAR* szFontType );
	int																							GetInputTxt_Count();
	void																						GetInputTxt_Infor( int nITindex, int& nTxtPos, CPoint& ptOffset );
	void																						Load( CFile* pFileStream );
	void																						Save( CFile* pFileStream );
	int																							GetSelectInputTxtIndex();

protected:
	void																						RemoveData();
	void																						RemoveData_InputTxt();
	void																						SetSelectRect();
 	void																						DrawSelect( Graphics* graphics );
	void																						DrawIndex( Graphics* graphics );
	void																						DrawInputText( Graphics* graphics, BOOL bShowSelectLine, BOOL bEditMode );
	void																						DrawNEditSelectDraw( CDC* pDC );

protected:
	CWnd*							m_pParentWnd;								// no save
	 
	TCHAR							m_szIndex[ 128 ];							// save
	BOOL							m_bIndexShow;
	Color							m_clrIndex;
	VECIAData						m_vecImageData;								// save
	CImage_AniPNG_Data*				m_pThisImageData;							// no save

	CPoint							m_ptStart;									// save
	CRect							m_rectSelect;								// no save
	BOOL							m_bIsSelect;								// no save

	int								m_nSelectInputTxtIndex;						// no save
	VECIAData_INPUTTXT				m_vecID_InputTxt;							// save
};

