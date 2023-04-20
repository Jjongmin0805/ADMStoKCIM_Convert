#pragma once


// CImage_AniPNG_LayerAddDLG 대화 상자입니다.

class CImage_AniPNG_LayerAddDLG : public CDialog
{
	DECLARE_DYNAMIC(CImage_AniPNG_LayerAddDLG)

public:
	CImage_AniPNG_LayerAddDLG(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CImage_AniPNG_LayerAddDLG();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IAE_LAYERADD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_szLayerIndex;
	virtual void OnOK();
};
