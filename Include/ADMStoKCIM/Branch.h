#pragma once
#include "afx.h"
class AFX_EXT_CLASS CBranch :
	public CObject
{
public:
	CBranch(void);
	~CBranch(void);

public:
	CString	m_strMRID;
	CString m_strName;
	CString	m_strNum;

	CString	m_strFwdID;
	CString	m_strBwdID;

	int		m_nKind;			//ADMS NAME_TYPE_FK
	int		m_nLT;				//ADMS LINESEGMENT_TYPE_FK
	int		m_nCeq_Type;
	BOOL	m_bStatus;
	double	m_dLength;

	BOOL	m_bCheck;
	CString	m_strDL;

	int		m_nKCIM_Type ;

	int		m_nDLID;
	int		m_nKCIM_FNDID;
	int		m_nKCIM_TNDID;
	int		m_nCheck;
	int		m_nCheck_Imsangju;

	//멀티 스위치
	CString m_szMULTISW;
	int		m_nMULTICIR_NUMBER;

	//
	//CCBranch_opArray			m_Array_Brach_op;
};

typedef CTypedPtrArray<CObArray, CBranch*> CBranchArray;
