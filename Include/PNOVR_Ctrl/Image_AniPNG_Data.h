#pragma once
class CImage_AniPNG_Data
{
public:
	CImage_AniPNG_Data();
	CImage_AniPNG_Data( TCHAR* szIndex );
	~CImage_AniPNG_Data(void);

	TCHAR*																						GetIndex();
	bool																						OpenImage();
	void																						ResetFrame();
	void																						NextFrame();
	void																						PrevFrame();
	void																						SetStartPoint( CPoint csStartPt );
	void																						DrawImage( Graphics* graphics );
	void																						DrawAni( Graphics* graphics );
	CRect																						GetDrawRect();

	void																						SetBitmapData( BitmapData* pBitmapData );
	BitmapData*																					GetBitmapData();
	void																						SetBitmapBuff( BYTE* pBuff );
	BYTE*																						GetBitmapBuff( UINT unIndex );
	UINT																						GetBitmapBuffCount();
	CImage_AniPNG_Data*																			Copy();

	void																						Open( CFile* pFileStream );
	void																						Save( CFile* pFileStream );
	void																						SetImageLoading( TCHAR* szFilePath );

protected:
	void																						RemoveAll();

	UINT							m_unCurrentFrame;							// no Save
	TCHAR							m_szIndex[ 128 ];							// Save
	BitmapData						m_csBMPInfor;								// Save
	vector<BYTE*>					m_vecBmpBuff;								// Save
	Rect							m_DrawRect;									// no Save
};

