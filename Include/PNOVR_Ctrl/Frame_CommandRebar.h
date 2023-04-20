#pragma once


// CFrame_CommandRebar

class AFX_EXT_CLASS CFrame_CommandRebar : public CMFCReBar
{
	DECLARE_DYNAMIC(CFrame_CommandRebar)

public:
	CFrame_CommandRebar( CWnd* pParent );
	virtual ~CFrame_CommandRebar();


	BOOL																						Create();

protected:
	CWnd*							m_pParent;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


