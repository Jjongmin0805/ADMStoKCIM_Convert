#pragma once
#include "afx.h"

class CLnsec_Ter :
	public CObject
{
public:
	CLnsec_Ter(void);
	~CLnsec_Ter(void);

public:
	CString m_szLnsec_MRID;
	CString	m_szLnsec_CNID;
	CString m_szLnsec_Type;
	CString m_szLnsec_ChageEQC;
};

typedef CTypedPtrArray<CObArray, CLnsec_Ter*> CLnsec_TerArray;
