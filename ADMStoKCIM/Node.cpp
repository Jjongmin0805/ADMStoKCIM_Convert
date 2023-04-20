#include "stdafx.h"
#include "Node.h"


CNode::CNode(void)
{
	m_nSS = 0;
	m_nDL = 0;
	m_nKind = 0;
	m_nConn = 0;
	m_nCheck= 0;

	m_nSwid= 0 ;
	m_nCeq_Type= 0 ;
	m_nName_Type = 0;

	m_genType = 0;
	m_fDG_CAPACITY = 0;

}


CNode::~CNode(void)
{
}
