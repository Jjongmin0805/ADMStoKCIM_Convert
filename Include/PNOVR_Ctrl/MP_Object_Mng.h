#pragma once

#include "MP_Object.h"

class AFX_EXT_CLASS CMP_Object_Mng
{
public:
	static CMP_Object_Mng*														InitInstance( void );

protected:
	static void																	ExitInstance( void ) ;
	static CMP_Object_Mng*			m_pMngData;

//----------------------------------------------------------------------------------------------

public:
	CMP_Object_Mng(void);
	~CMP_Object_Mng(void);

	void																		RemoveData();
	void																		SetDisplayWnd( CWnd* pWnd );
	void																		Add( CPoint ptBegin, CPoint ptEnd, int nDisplayWidth );
	void																		Draw( CDC* pDC );

	void																		MP_Stop();
	void																		MP_Run();
	void																		SetThreadRun_Status( BOOL bStatus );
	BOOL																		GetThreadRun_Status();
	CWnd*																		GetViewWnd();
	void																		SetInit();
	void																		SetException_DrawRect( CRect rectException );

	void																		SetColorIndicate( COLORREF clrIndi );
	COLORREF																	GetColorIndicate();
	void																		SetColorBGK( COLORREF clrIndi );
	COLORREF																	GetColorBGK();
	void																		SetColorRound( COLORREF clrIndi );
	COLORREF																	GetColorRound();
	void																		SetDrawType( int nType );
	void																		SetAniDrawMTime( int nTime );
	int																			GetAniDrawMTime();

protected:
	static UINT WINAPI															Thread_DrawProcess( LPVOID pParam );

	void																		CloseThread();

protected:
	CWnd*							m_pViewWnd;
	VECMPOBJs						m_vecMPOBJdata;

	HANDLE							m_hThreadProcess;
	BOOL							m_bThreadProcess;

	COLORREF						m_clrMPOB_Indi_Color;
	COLORREF						m_clrMPOB_BGK_Color;
	COLORREF						m_clrMPOB_Round_Color;
	int								m_nAniDrawTime;
};

#define								theMPOBJmng									CMP_Object_Mng::InitInstance()