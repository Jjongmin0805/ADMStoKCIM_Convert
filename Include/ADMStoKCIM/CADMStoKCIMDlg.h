#pragma once
#include <atldbcli.h>
#include <atldbsch.h>
#include "Node.h"
#include "Branch.h"
#include "ADMS.h"
#include "stdafx.h"
#include "afxdb.h"

#include "CEQ_MESH_NO2.h"

// CADMStoKCIMDlg 대화 상자
typedef CArray<CENTER_STA, CENTER_STA&> CCENTER_STA;
typedef CArray<BOF_STA, BOF_STA&> CBOF_STA;
typedef CArray<SS_STA, SS_STA&> CSS_STA;
typedef CArray<MTR_STA, MTR_STA&> CMTR_STA;
typedef CArray<DL_STA, DL_STA&> CDL_STA;
typedef CArray<SNV_STA, SNV_STA&> CSNV_STA;
typedef CArray<ND_STA, ND_STA&> CND_STA;
typedef CArray<CBSW_STA, CBSW_STA&> CCBSW_STA;
typedef CArray<GEN_STA, GEN_STA&> CGEN_STA;
typedef CArray<SVC_STA, SVC_STA&> CSVC_STA;
typedef CArray<SHUNTEQ_STA, SHUNTEQ_STA&> CSHUNTEQ_STA;
typedef CArray<LD_STA, LD_STA&> CLD_STA;
typedef CArray<LNSEC_STA, LNSEC_STA&> CLNSEC_STA;
typedef CArray<TR_STA, TR_STA&> CTR_STA;
typedef CArray<IJ_STA, IJ_STA&> CIJ_STA;
typedef CArray<BR_STA, BR_STA&> CBR_STA;
typedef CArray<ESS_STA, ESS_STA&> CESS_STA;
typedef CArray<EQUTY_STA, EQUTY_STA&> CEQUTY_STA;
typedef CArray<INNERSEC_STA, INNERSEC_STA&> CINNERSEC_STA;
typedef CArray<INNERPOLE_STA, INNERPOLE_STA&> CINNERPOLE_STA;
typedef CArray<POLE_STA, POLE_STA&> CPOLE_STA;
typedef CArray<DIAINFO_STA, DIAINFO_STA&> CDIAINFO_STA;
typedef CArray<GND_STA, GND_STA&> CGND_STA;
typedef CArray<GBR_STA, GBR_STA&> CGBR_STA;
typedef CArray<TERMINAL, TERMINAL&> CTERMINAL;
typedef CArray<BI_VALUE, BI_VALUE&> CBI_VALUE;
typedef CArray<NAME_TYPE, NAME_TYPE&> CNAME_TYPE;
typedef CArray<IDENTIFIEDOBJECT, IDENTIFIEDOBJECT&> CIDENTIFIEDOBJECT;
typedef CArray<conductingequipment, conductingequipment&> Cconductingequipment;
typedef CArray<LINESEGMENT_TYPE, LINESEGMENT_TYPE&> CLINESEGMENT_TYPE;
typedef CArray<LINESYSID, LINESYSID&> CLINESYSID;
typedef CArray<GLINESEGMENT, GLINESEGMENT&> CGLINESEGMENT;
typedef CArray<HVCUS_STA, HVCUS_STA&> CHVCUS_STA;

typedef CArray<INNERSEC_STA, INNERSEC_STA&> CINNERSEC_STA;
typedef CArray<INNERPOLE_STA, INNERPOLE_STA&> CINNERPOLE_STA;
typedef CArray<POLE_STA, POLE_STA&> CPOLE_STA;
typedef CArray<LINESEGMENT_DETAIL, LINESEGMENT_DETAIL&> CLINESEGMENT_DETAIL;
typedef CArray<GLINESEGMENT, GLINESEGMENT&> CGLINESEGMENT;
typedef CArray<LINESYSID, LINESYSID&> CLINESYSID;
typedef CArray<PRDE_STA, PRDE_STA&> CPRDE_STA;
//20210907
typedef CArray<GENUNIT_STA, GENUNIT_STA&> CGENUNIT_STA;


class AFX_EXT_CLASS CADMStoKCIMDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CADMStoKCIMDlg)

public:
	CADMStoKCIMDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CADMStoKCIMDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADMSTOKCIM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


protected:
	HICON m_hIcon;

	CDatabase		m_ADMSDB;
	CDatabase		m_ADMSCDDB;

	CNodeArray		m_pNodeArr;
	CBranchArray	m_pBranchArr;

	CProgressCtrl	m_progress;
	CString			m_szTime;
	CString			m_szDataName_Data;


public:
	void			ADMStoKCIM_Offic_Config( CString szDate );
	void			ADMStoKCIM_Offic_Code( CString szDate );
	void			ADMStoKCIM_CSV_Route( CString szDate );
	void			ADMStoKCIM_Mappig_Config(CString szDate);

	CString			m_szADMSDB_Office, m_szADMSDB_Code, m_szADMSDB_Mappig;
	CString			m_szCSV_Route;
	CString			m_szADMS_Code;

public:
	virtual BOOL OnInitDialog();

	void OnTimer(UINT_PTR nIDEvent);

	CString MyPath();
	void RemoveAllData();
	void ADMStoKCIM_Config();
	void ADMStoKCIM_Read_CSV();
	void ADMStoKCIM_Read();
	void ADMStoKCIM_Convert();
	void ADMStoKCIM_Insert();
	void ADMStoKCIM_RemoveAll();
protected:
	CDataSource m_KCIMConnect;
	CSession    m_KCIMSession;

protected:
	CBranch* GetBranch(CString strMRID);
	CNode* GetNode(CString strMRID);
	CNode* GetNode_New(CString strMRID);
	CNode* GetNodeTER(CString strMRID);
	CNode* GetNodeTER_NEW(CBranch* pbranch);
	int GetMasterCD(int nCeqTp, int nCircuit);
	int GetMasterCode(int nMst, int nCeqTp);

public:
	int GetBOF_II_CENTER(CString stDate);
	int	GetDL_II_BOF(int nDate);
	int GetSNV_II_SS(CString stDate);
	int GetMTR_II_SS(CString stDate);
	int GetDL_II_MTR(CString stDate);
	int	GetND_II_SNV(CString stDate);
	int	GetND_II_SNV_OCB(CString stDate);
	int GetND_II_SNV_BR(CString stDate);
	int	GetND_II_SNV_2(int nDate);
	int GetTR_II_SS(CString stDate);
	int GetTR_II_FND(CString stDate);
	int GetTR_II_TND(CString stDate, int nDate);
	int GetIJ_II_DL( int nDate);
	int GetDLID(CString stDate);
	int GetDLID_ORIGINAL(CString stDate);
	int GetDLID_OCB(CString stDate);
	int GetSSID(CString stDate);

	void GET_GENUNIT_STA(CString stDate, int nGENID);
	//
	int Get_OVERHEAD_CABLE(int nDate);
	float Get_POSR(float fDate, int nDate);
	float Get_POSX(float fDate, int nDate);
	float Get_ZERR(float fDate, int nDate);
	float Get_ZERX(float fDate, int nDate);

public:
	//ACMDB
	CCENTER_STA	 			 m_arrCENTER;
	CBOF_STA				 m_arrBOF;
	CSS_STA					 m_arrSS;
	CMTR_STA				 m_arrMTR;
	CDL_STA					 m_arrDL;
	CSNV_STA				 m_arrSNV;
	CND_STA					 m_arrND;
	CCBSW_STA				 m_arrCBSW;
	CGEN_STA				 m_arrGEN;
	CSVC_STA				 m_arrSVC;
	CSHUNTEQ_STA			 m_arrSHUNTEQ;
	CLD_STA					 m_arrLD;
	CLNSEC_STA				 m_arrLNSEC;
	CTR_STA					 m_arrTR;
	CIJ_STA					 m_arrIJ;
	CBR_STA					 m_arrBR;
	CESS_STA				 m_arrESS;
	CEQUTY_STA				 m_arrEQUTY;

	CINNERSEC_STA			 m_arrINNERSEC;
	CINNERPOLE_STA			 m_arrINNERPOLE;
	CPOLE_STA				 m_arrPOLE;
	CDIAINFO_STA			 m_arrDIAINFO;

	CHVCUS_STA				 m_arrHVCUS;

	//G				 
	CGND_STA				 m_arrGND;
	CGBR_STA				 m_arrGBR;
	//ADMS			 
	CTERMINAL				 m_arrTER;
	CBI_VALUE				 m_arrBI;
	CNAME_TYPE				 m_arrNAME;
	CIDENTIFIEDOBJECT		 m_arrIDTER;
	Cconductingequipment	 m_arrCEQ;
	CLINESEGMENT_TYPE		 m_arrrLINESEGMENT_TYPE;
	CPRDE_STA				 m_arrPRDE_STA;

	CGENUNIT_STA			 m_arrGENUNIT;


	//////////////
	int Get_CNFK_Check2(CString stDate);
	int SET_MTRND(int  nDLID);
	int SET_SSID(CString stDate);
	int SET_br_ii_fnd( CString stDate);
	int SET_br_ii_tnd( CString stDate);

	void PumpMessages();
	void IDC_LIST_DATA_HISTORY(CString  strData_Name);

	int m_nIJ_STA_EQU4;
	int m_nIJ_STA_EQU6;
	int m_nIJ_STA_EQU8;

	CString LIST_Current_Time();
	CString LIST_Current_Time_Kasim();
	CProgressCtrl m_ctrProgressADMStoKCIM;
	CListBox m_ListCtrl_Data;


	int		m_nSTMODE_MEM_OFFICE_ID;
	CString m_szSTMODE_MEM_OFFICE_NM;
public:

	////////////////////////
	void Error_KASIM();
///////////////////////GIS
	void GIS_CSV_OPEN();
	void GIS_ADMS_LOAD_2();
	void GIS_ADMS_CONVER();
	void GIS_ADMS_SI();
	void GIS_ADMS_CSV_INSERT();

	CString GIS_GET_POLE_NM(CString);

	int GETlnsec_hi_innerpole(int);
	int GETlnsec_hi_innersec(int);
	int	GET_innersec_ii_lnsec(CString stMRID);
	int GetSS_HI_SNV(int nDate);



	CDatabase				m_ADMSMapping_DB;

	CEQ_MESH_NO2Array		m_pMESH_NOArr;


	CLINESEGMENT_DETAIL		 m_arrrLINESEGMENT_DETAIL;
	CGLINESEGMENT			 m_arrrGLINESEGMENT;
	CLINESYSID			 	 m_arrLINESYSID;

	int	GET_PRDE_AO_VALUE(CString stMRID, int nCEQ_TYPE, int nCODE_FK);
	int	GET_PRDE_TCCSET(int nTCCSET);
	/////////////////////////////////////////

	int						m_Custmer;
	int						m_nType;
	//ST모드 입력
	void					ST_MODE();
	int						m_nSTMode;
	//
	void					ST_CODE(int nOfficeCode);

	//MAP만들기
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_NAME_MridName;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_Ter_CnCeq;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_Ter_CnOriCeq;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_Ter_CeqCn;
	CMap<CString, LPCTSTR, int, int>m_map_Ter_CnID;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_CEQ_MridCHCeq;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_CEQ_MridOriCeq;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_ND_CnCeq;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_ND_CeqCn;
	CMap<CString, LPCTSTR, int, int >m_map_ND_CnNDID;

	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_GEN_CnCeq;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_GEN_CeqCn;

	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_DL_DLMTR;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_DL_ECRMTR;
	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_DL_DLSUB;

	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_MTR_MTRVL;

	CMap<CString, LPCTSTR, int, int>m_map_SNV_MTRSNVID;
	CMap<CString, LPCTSTR, int, int>m_map_DL_CEQID;
	CMap<CString, LPCTSTR, int, int>m_map_DL_EQCID;
	CMap<CString, LPCTSTR, int, int>m_map_IDBranch;
	CMap<CString, LPCTSTR, int, int>m_map_IDNode;

	CMap<CString, LPCTSTR, int, int>m_map_IDNode_NEW;


	CMap<CString, LPCTSTR, int, int>m_map_TerCount;//

	//
	CMap<CString, LPCTSTR, int, int>m_map_IDBranchm_strFwdID;
	CMap<CString, LPCTSTR, int, int>m_map_IDBranchm_strTwdID;


	CMap<int, int, int, int>m_map_LINESEGMENT_TYPE_CABLE;
	CMap<int, int, float, float>m_map_LINESEGMENT_TYPE_POSR;
	CMap<int, int, float, float>m_map_LINESEGMENT_TYPE_POSX;
	CMap<int, int, float, float>m_map_LINESEGMENT_TYPE_ZERR;
	CMap<int, int, float, float>m_map_LINESEGMENT_TYPE_ZERX;

	CMap<CString, LPCTSTR,  int, int>m_map_PRDE_AO_VALUE;
	CMap<int, int, int, int>m_map_PRDE_TYPE_585960;
	CMap<int, int, int, int>m_map_PRDE_TYPE_65;
	CMap<int, int, int, int>m_map_PRDE_TYPE_68;
	CMap<int, int, int, int>m_map_PRDE_TYPE_75;
	CMap<int, int, int, int>m_map_PRDE_TYPE_302;

	CMap<CString, LPCTSTR, int, int>m_map_SW_comm_group;


	CMap<CString, LPCTSTR, CString, LPCTSTR >m_map_CBSW_PQMS;
	CMap<int, int, int, int>m_map_CEQTYPE_RTUCODE;



};
