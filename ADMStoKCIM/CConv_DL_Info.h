
#pragma once
#include "afx.h"
class AFX_EXT_CLASS CConv_DL_Info :
	public CObject
{
public:
	CConv_DL_Info(void);
	~CConv_DL_Info(void);

public:

	CString m_szDL_CEQ_MRFK;
	CString m_szDL_EQC_MRFK;
	int m_nDL_Index;

	CStringArray m_pEQC_MRFK_Arry;

	BOOL	Check2EQC_MRFK(CString strEQC_MRFK);
};

typedef CTypedPtrArray<CObArray, CConv_DL_Info*> CConv_DL_InfoArray;
