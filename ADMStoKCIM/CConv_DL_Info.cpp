#include "stdafx.h"
#include "CConv_DL_Info.h"

CConv_DL_Info::CConv_DL_Info(void)
{

}


CConv_DL_Info::~CConv_DL_Info(void)
{
}


BOOL CConv_DL_Info::Check2EQC_MRFK(CString strEQC_MRFK)
{
	CString strEQC;
	for (int i = 0; i < m_pEQC_MRFK_Arry.GetSize(); i++)
	{
		strEQC = m_pEQC_MRFK_Arry.GetAt(i);

		if (strEQC == strEQC_MRFK)
			return TRUE;
	}

	return FALSE;
}


