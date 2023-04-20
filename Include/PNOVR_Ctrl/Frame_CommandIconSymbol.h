#pragma once
class AFX_EXT_CLASS CFrame_CommandIconSymbol
{
public:
	CFrame_CommandIconSymbol(void);
	~CFrame_CommandIconSymbol(void);

	void																						SetIcon_Deactive( Bitmap* pIcon, UINT unIndex );
	void																						SetIcon_Normal( Bitmap* pIcon, UINT unIndex );
	void																						SetIcon_Over( Bitmap* pIcon, UINT unIndex );
	void																						SetIcon_Down( Bitmap* pIcon, UINT unIndex );
	void																						SetIcon_Select_Normal( Bitmap* pIcon, UINT unIndex );
	void																						SetIcon_Select_Over( Bitmap* pIcon, UINT unIndex );
	void																						SetIcon_Select_Down( Bitmap* pIcon, UINT unIndex );
	void																						DrawCMDIcon( Graphics &graphics );

public:
	Bitmap*							m_pIcon_Deactive;
	Bitmap*							m_pIcon_Normal;
	Bitmap*							m_pIcon_Over;
	Bitmap*							m_pIcon_Down;
	Bitmap*							m_pIcon_Select_Normal;
	Bitmap*							m_pIcon_Select_Over;
	Bitmap*							m_pIcon_Select_Down;
};

typedef								vector< CFrame_CommandIconSymbol* >							VECCMDICONSYMBOL;