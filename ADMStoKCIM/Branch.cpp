#include "stdafx.h"
#include "Branch.h"

CBranch::CBranch(void)
{
	m_nLT = 0;
	m_nKind = 0;
	m_bCheck = FALSE;
	m_dLength = 0;
	m_nCeq_Type = 0;
	m_bStatus = FALSE;
	m_nKCIM_Type= 0 ;

	m_nDLID = 0 ;
	m_nKCIM_FNDID= 0 ;
	m_nKCIM_TNDID= 0 ;
	m_nCheck = 0;
	m_nCheck_Imsangju = 0;
	m_nMULTICIR_NUMBER = 0 ;
}


CBranch::~CBranch(void)
{
}
