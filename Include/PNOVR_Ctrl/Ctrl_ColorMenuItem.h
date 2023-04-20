#pragma once
class CCtrl_ColorMenuItem
{
public:
	CCtrl_ColorMenuItem( UINT unID, TCHAR* szName, CSize szSize, int nType, int nDefault, Bitmap* pIcon );
	~CCtrl_ColorMenuItem(void);

public:
	HANDLE							m_hParentHandle;

	UINT							m_unID;
	TCHAR							m_szName[ 128 ];
	int								m_nType;
	int								m_nValue;
	Bitmap*							m_pIcon;
	CSize							m_csSize;
};

typedef								vector<CCtrl_ColorMenuItem*>									VECCOLORMENUITEM;