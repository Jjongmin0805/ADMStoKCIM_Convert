#ifndef __DBEXCEPT_H__
#define __DBEXCEPT_H__

///////////////////////////////////////////////////////////////////////////
// class COLEDBException

class AFX_EXT_CLASS COLEDBException : public CException
{
	DECLARE_DYNAMIC(COLEDBException)

public:
	COLEDBException();
	COLEDBException(LPUNKNOWN lpUnk, const IID& iid);
	virtual ~COLEDBException();

// Attributes
public:
	LPUNKNOWN m_lpUnk;
	IID m_iid;
};
void AFXAPI AfxThrowOLEDBException(LPUNKNOWN lpUnk, const IID& iid);

#endif
