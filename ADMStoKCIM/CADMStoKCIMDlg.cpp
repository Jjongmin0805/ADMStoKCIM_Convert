// CADMStoKCIMDlg.cpp: 구현 파일
//
#define MY_TIMER		100
#define LC_ALL			0

#include "stdafx.h"
#include "CADMStoKCIMDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "XMLMgr.h"
#include <math.h>
#include "TcrMathCoord.h"
//
// CADMStoKCIMDlg 대화 상자

IMPLEMENT_DYNAMIC(CADMStoKCIMDlg, CDialogEx)

CADMStoKCIMDlg::CADMStoKCIMDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMSTOKCIM, pParent)
{
	m_szADMSDB_Office.Format(_T(""));
	m_szADMSDB_Code.Format(_T(""));
	m_szDataName_Data.Format(_T(""));
	m_szCSV_Route.Format(_T(""));
	m_szADMS_Code.Format(_T(""));
	m_nIJ_STA_EQU4 = 1;
	m_nIJ_STA_EQU6 = 1;
	m_nIJ_STA_EQU8 = 1;
	m_nType = 0;			// 충북 20201110 변환데이터
	m_nSTMode = 0;			// ST 모드를 만들면 1을 입력한다.
	m_nSuccess = 99;		// 0 이면 
	m_Custmer = 0;
	//공용선로
	m_nDL_TEST = 99;
	for (int i = 0; i < 500; i++)
	{
		m_nMstCD[i][0] = 0;
		m_nMstCD[i][1] = 0;
		m_nMstCD[i][2] = 0;
	}
	//20231017 
	m_szTime.Format(_T(""));
	m_nSTMODE_KASIM_Office_ID = 0;
	m_szSTMODE_KASIM_Office_Ver.Format(_T("1.10.5"));
	m_szSTMODE_KASIM_Office_Name.Format(_T(""));
}

CADMStoKCIMDlg::~CADMStoKCIMDlg()
{
	ADMStoKCIM_RemoveAll();
}

void CADMStoKCIMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_ADMStoKCIM, m_ctrProgressADMStoKCIM);
	DDX_Control(pDX, IDC_LIST_ADMStoKCIM, m_ListCtrl_Data);
}

BEGIN_MESSAGE_MAP(CADMStoKCIMDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CADMStoKCIMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int nProgress;
	nProgress = 96;
	m_ctrProgressADMStoKCIM.SetRange(0, nProgress);	 //범위 0~200
	m_ctrProgressADMStoKCIM.SetPos(0);				 //0으로 초기화
	m_ctrProgressADMStoKCIM.SetStep(1);				 //1씩 증가

#ifdef _TESTDEV
	if (!theDBContainer->ConnectDB())
	{
		TRACE(L"Err : DB Connect Fail.\n");
	}
#endif	 

	SetTimer(MY_TIMER, 1000, NULL);
	_wsetlocale(LC_ALL, _T("kor"));
	return TRUE;  // return TRUE unless you set the focus to a control				  
}

BOOL CADMStoKCIMDlg::DestroyWindow()
{

#ifdef _TESTDEV 
	theDBContainer->Close();
#endif
	return CDialogEx::DestroyWindow();
}

int CADMStoKCIMDlg::GetSuccess()
{
	return m_nSuccess;
}

void CADMStoKCIMDlg::RemoveAllData()
{
	ADMStoKCIM_RemoveAll();			// 배열 삭제
}

//프로그램 시작 
void CADMStoKCIMDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case MY_TIMER:
	{
		CWaitCursor wait;
		KillTimer(MY_TIMER);
		if (m_nSuccess == 1)
		{
			CDialogEx::OnOK();
			KillTimer(MY_TIMER);
			break;
		}
	
		m_szTime = LIST_Current_Time();
		m_szDataName_Data.Format(_T("%s[1/7]========== ADMStoKASIM Start (%s) =========="), m_szTime , m_szSTMODE_KASIM_Office_Ver);
		IDC_LIST_DATA_HISTORY(m_szDataName_Data);
		m_ctrProgressADMStoKCIM.StepIt();  

		m_szTime = LIST_Current_Time();
		m_szDataName_Data.Format(_T("%s[2/7]========== ADMStoKASIM DataBase 연결 (%s) =========="), m_szTime, m_szSTMODE_KASIM_Office_Ver);
		IDC_LIST_DATA_HISTORY(m_szDataName_Data);
		m_ctrProgressADMStoKCIM.StepIt();  

		ADMStoKASIM_Config();			//config 파일 가져오기
		MakeDirectory();				//폴도 생성 부분 !!! 파일 삭제 부분 
		ADMStoKCIM_Read_CSV();			//CSV 파일 읽어서 처리 !
		ADMStoKASIM_MYSQL();			//db 연결 하는부분 

		m_szTime = LIST_Current_Time();
		m_szDataName_Data.Format(_T("%s[3/7]========== ADMStoKASIM SVG 파일 생성 (%s) =========="), m_szTime, m_szSTMODE_KASIM_Office_Ver);
		IDC_LIST_DATA_HISTORY(m_szDataName_Data);
		m_ctrProgressADMStoKCIM.StepIt();  

		if (m_nSTMode > 0)
		{
			if (m_nType != 0)
			{
				MakeDiagram_LoasID_KASIM(m_nType);
			}
			else
			{
				MakeDiagram_Member_KASIM(); 
			}
		}

		m_szTime = LIST_Current_Time();
		m_szDataName_Data.Format(_T("%s[4/7]========== ADMStoKASIM 데이터 읽기 (%s) =========="), m_szTime, m_szSTMODE_KASIM_Office_Ver);
		IDC_LIST_DATA_HISTORY(m_szDataName_Data);
		m_ctrProgressADMStoKCIM.StepIt();

		ADMStoKASIM_Read();			    //ADMS DB 불러오기

		m_szTime = LIST_Current_Time();
		m_szDataName_Data.Format(_T("%s[5/7]========== ADMStoKASIM 데이터 변환 (%s) =========="), m_szTime, m_szSTMODE_KASIM_Office_Ver);
		IDC_LIST_DATA_HISTORY(m_szDataName_Data);
		m_ctrProgressADMStoKCIM.StepIt();

		ADMStoKASIM_Conver();			//ADMS DB 변환하기 

		m_szTime = LIST_Current_Time();
		m_szDataName_Data.Format(_T("%s[6/7]========== ADMStoKASIM 데이터 입력 (%s) =========="), m_szTime, m_szSTMODE_KASIM_Office_Ver);
		IDC_LIST_DATA_HISTORY(m_szDataName_Data);
		m_ctrProgressADMStoKCIM.StepIt();

		ADMStoKASIM_Insert();			//ADMS DB 저장하기

		m_szTime = LIST_Current_Time();
		m_szDataName_Data.Format(_T("%s[7/7]========== ADMStoKASIM END (%s) =========="), m_szTime, m_szSTMODE_KASIM_Office_Ver);
		IDC_LIST_DATA_HISTORY(m_szDataName_Data);
		m_ctrProgressADMStoKCIM.StepIt();
						
		wait.Restore();
		m_nSuccess = 0; //부장님한테 0으로 보내면 만들어진 DB 이동!

		CDialogEx::OnOK();
	}
	KillTimer(MY_TIMER);
	break;
	}
}

////////////////////////////////////////////////////////////////////////// 

//경로!
CString CADMStoKCIMDlg::MyPath()			
{
	CString slmpath;
	WCHAR szDirve[256], szDir[256];
	WCHAR programpath[2048];
	GetModuleFileName(0, programpath, 1024);
	_wsplitpath(programpath, szDirve, szDir, NULL, NULL);
	slmpath.Format(_T("%s%s"), szDirve, szDir);
	return slmpath.Left(slmpath.GetLength() - 1);
}
//시간!
CString CADMStoKCIMDlg::LIST_Current_Time()
{
	CString strTime;
	CTime cTime = CTime::GetCurrentTime();
	strTime.Format(_T("[%04d %02d.%02d %02d:%02d:%02d ]"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond()); // 현재 초 반환
	return strTime;
}
//시간-2
CString CADMStoKCIMDlg::LIST_Current_Time_Kasim()
{
	CString strTime;
	CTime cTime = CTime::GetCurrentTime();
	strTime.Format(_T("%04d%02d%02d"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay()); // 현재 초 반환
	return strTime;
}

//메모리 삭제!
void CADMStoKCIMDlg::ADMStoKCIM_RemoveAll()
{
	for (int i = 0; i < m_pNodeArr.GetSize(); i++)
		delete m_pNodeArr.GetAt(i);
	m_pNodeArr.RemoveAll();

	for (int i = 0; i < m_pBranchArr.GetSize(); i++)
		delete m_pBranchArr.GetAt(i);
	m_pBranchArr.RemoveAll();

	m_arrCENTER.RemoveAll();
	m_arrBOF.RemoveAll();
	m_arrSS.RemoveAll();
	m_arrMTR.RemoveAll();
	m_arrDL.RemoveAll();
	m_arrSNV.RemoveAll();
	m_arrND.RemoveAll();
	m_arrCBSW.RemoveAll();
	m_arrGEN.RemoveAll();
	m_arrSVC.RemoveAll();
	m_arrSHUNTEQ.RemoveAll();
	m_arrLD.RemoveAll();
	m_arrLNSEC.RemoveAll();
	m_arrTR.RemoveAll();
	m_arrIJ.RemoveAll();
	m_arrBR.RemoveAll();
	m_arrESS.RemoveAll();
	m_arrEQUTY.RemoveAll();
	m_arrDIAINFO.RemoveAll();
	m_arrGND.RemoveAll();
	m_arrGBR.RemoveAll();
	m_arrBI.RemoveAll();
	m_arrIDTER.RemoveAll();
	m_arrCEQ.RemoveAll();

	for (int i = 0; i < m_pConv_DL_InfoArr.GetSize(); i++)
		delete m_pConv_DL_InfoArr.GetAt(i);

	m_pConv_DL_InfoArr.RemoveAll();
}

void CADMStoKCIMDlg::IDC_LIST_DATA_HISTORY(CString  strData_Name)
{
	CListBox *ListBox = (CListBox*)GetDlgItem(IDC_LIST_ADMStoKCIM);
	ListBox->InsertString(ListBox->GetCount(), strData_Name);
	PumpMessages();
	m_ListCtrl_Data.SetCurSel(m_ListCtrl_Data.GetCount() - 1);

	//
	CString szTime;
	szTime = LIST_Current_Time_Kasim();
	CString szFilePath;
	FILE* stream;
	//개수 확인 
	szFilePath.Format(_T("\\log\\%sADMStoKASIM.log"), szTime);
	stream = _wfopen(MyPath() + szFilePath, L"a+");
	if (stream != NULL)
	{
		fwprintf(stream, L"%s\n", strData_Name);
		fclose(stream);
	}
}

void CADMStoKCIMDlg::IDC_LIST_DATA_HISTORY_ERROR(CString  strData_Name)
{
	CListBox *ListBox = (CListBox*)GetDlgItem(IDC_LIST_ADMStoKCIM);
	ListBox->InsertString(ListBox->GetCount(), strData_Name);
	PumpMessages();
	m_ListCtrl_Data.SetCurSel(m_ListCtrl_Data.GetCount() - 1);

	//
	CString szTime;
	szTime = LIST_Current_Time_Kasim();
	CString szFilePath;
	FILE* stream;
	//개수 확인 
	szFilePath.Format(_T("\\log\\%sADMStoKASIM_Error.log"), szTime);
	stream = _wfopen(MyPath() + szFilePath, L"a+");
	if (stream != NULL)
	{
		fwprintf(stream, L"%s\n", strData_Name);
		fclose(stream);
	}
}

void CADMStoKCIMDlg::ConvertWCtoC(CString str, char* ch)
{
	int nSize = 0;
	memset(ch, 0x00, 255);
	//입력받은 wchar_t 변수의 길이를 구함
	nSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	//형 변환 
	WideCharToMultiByte(CP_ACP, 0, str, -1, ch, nSize, 0, 0);
}

void CADMStoKCIMDlg::PumpMessages() //변환된 데이터를 변경을 하기 위한 메세지 함수
{
	ASSERT(m_hWnd != NULL);

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

int	CADMStoKCIMDlg::GetInt_Row(char *chDate)
{
	CString szDate;
	int nDate = 0;
	if (chDate == NULL)
	{
		return 0;
	}
	szDate = (LPSTR)chDate;
	if (!(szDate.IsEmpty()))
	{
		nDate = _ttoi(szDate);
		return nDate;
	}
	return 0;
}

double	CADMStoKCIMDlg::GetDouble_Row(char* chDate)
{
	CString szDate;
	double dDate = 0;
	if (chDate == NULL)
	{
		return 0;
	}
	szDate = (LPSTR)chDate;
	if (!(szDate.IsEmpty()))
	{
		dDate = _ttof(szDate);
		return dDate;
	}
	return 0;
}

CString	CADMStoKCIMDlg::GetCString_Row(char* chDate)
{
	CString szDate;
	if (chDate == NULL)
	{
		return _T("");
	}
	szDate = (LPSTR)chDate;
	if (!(szDate.IsEmpty()))
	{
		return szDate;
	}
	return _T("");
}



/////////////////////////////////////////////////////////////////////////
//DB연결 하는 부분 - 받아오는 부분 3

void CADMStoKCIMDlg::ADMStoKCIM_CSV_Route(CString szDate)
{
	m_szCSV_Route.Format(_T("%s"), szDate);
}
//ST모드 값  0 DUAL 만 / 1 전체만 - 받아오는 부분 4
void CADMStoKCIMDlg::ST_MODE(int nMode)
{
	m_nSTMode = nMode;
}
//코드 입력 부분 사업소 코드 예-4520 4510 - 받아오는 부분 5
void CADMStoKCIMDlg::ST_CODE(int nOfficeCode)
{
	m_nType = nOfficeCode;
}
/////////////////////////////////////////////////////////////////////////

void CADMStoKCIMDlg::MakeDirectory()
{
	CString szRoute, stream;
	szRoute.Format(_T("%sDUAL"), m_szCSV_Route);
	stream = (MyPath() + szRoute);
	DeleteAllFiles(stream);
	if (m_nSTMode > 0)
	{
		szRoute.Format(_T("%sST"), m_szCSV_Route);
		stream = (MyPath() + szRoute);
		DeleteAllFiles(stream);
	}
}
//
void CADMStoKCIMDlg::ADMStoKCIM_Read_CSV()
{
	char		chBuffer[4096] = { 0x00, };
	char		*token = NULL;

	FILE*		stream = NULL;
	FILE*		stream_STA = NULL;
	FILE*		stream_UIN = NULL;

	CString		szFilePath;
	CString		strTmp;
	CString		szRoute;

	CString		strName;
	CString		strData;
	double		typefloat1;
	double		typefloat2;
	double		typefloat3;
	double		typefloat4;
	double		typefloat5;

	int			count = 0;
	int			type1 = 0;
	int			type2 = 0;
	int			type3 = 0;
	int			type4 = 0;

	//TCCSET
	szFilePath.Format(_T("\\config\\VVM.xml"));
	stream = _wfopen(MyPath() + szFilePath, L"r+");
	if (stream != NULL)
	{
		//VVM_STA
		szRoute.Format(_T("%sDUAL\\VVM_STA.csv"), m_szCSV_Route);
		stream_STA = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream_STA, L"VVM_NM\n");
		//VVM_DYN_UIN
		szRoute.Format(_T("%sDUAL\\VVM_DYN_UIN.csv"), m_szCSV_Route);
		stream_UIN = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream_UIN, L"VVM_MORFLAG,VVM_LMHI,VVM_LMLO,VVM_TOLHI,VVM_TOLLO,VVM_DBAND\n");


		fgets(chBuffer, sizeof(chBuffer), stream);
		while (fgets(chBuffer, sizeof(chBuffer), stream))
		{
			count = 0;
			token = strtok(chBuffer, ",");
			while (token != NULL)
			{
				if (count == 0)			type1 = atoi(token);
				else if (count == 1)	type2 = atoi(token);
				else if (count == 2)	typefloat1 = atof(token);
				else if (count == 3)	typefloat2 = atof(token);
				else if (count == 4)	typefloat3 = atof(token);
				else if (count == 5)	typefloat4 = atof(token);
				else if (count == 6)	typefloat5 = atof(token);


				token = strtok(NULL, ",");
				count++;
			}
			fwprintf(stream_STA, L"%d\n", type1);
			fwprintf(stream_UIN, L"%d,%.4f,%.4f,%.4f,%.4f,%.4f\n", type2, typefloat1, typefloat2, typefloat3, typefloat4, typefloat5);
		}
		fclose(stream);
		fclose(stream_STA);
		fclose(stream_UIN);
	}
	//20230214
	if (m_nSTMode > 0)
	{
		//TCCSET
		szFilePath.Format(_T("\\config\\TCCSET.xml"));
		stream = _wfopen(MyPath() + szFilePath, L"r+");
		if (stream != NULL)
		{
			//TCCSET_STA
			szRoute.Format(_T("%sST\\TCCSET_STA.csv"), m_szCSV_Route);
			stream_STA = _wfopen(MyPath() + szRoute, L"w+");
			fwprintf(stream_STA, L"TCCSET_NM\n");
			//TCCSET_DYN_UIN
			szRoute.Format(_T("%sST\\TCCSET_DYN_UIN.csv"), m_szCSV_Route);
			stream_UIN = _wfopen(MyPath() + szRoute, L"w+");
			fwprintf(stream_UIN, L"TCCSET_A,TCCSET_B,TCCSET_C,TCCSET_D\n");

			fgets(chBuffer, sizeof(chBuffer), stream);
			while (fgets(chBuffer, sizeof(chBuffer), stream))
			{
				count = 0;
				token = strtok(chBuffer, ",");
				while (token != NULL)
				{
					if (count == 0)	type1 = atoi(token);
					else if (count == 1)	strName = CA2W((token)).m_psz;
					else if (count == 2)	typefloat1 = atof(token);
					else if (count == 3)	typefloat2 = atof(token);
					else if (count == 4)	typefloat3 = atof(token);
					else if (count == 5)	typefloat4 = atof(token);


					token = strtok(NULL, ",");
					count++;
				}
				fwprintf(stream_STA, L"%s\n", strName);
				fwprintf(stream_UIN, L"%f,%f,%f,%f\n", typefloat1, typefloat2, typefloat3, typefloat4);
			}
			fclose(stream);
			fclose(stream_STA);
			fclose(stream_UIN);
		}

		//MACHINE_DYN_UIN.csv
		szFilePath.Format(_T("\\config\\MACHINE_DYN_UIN.csv"));
		stream = _wfopen(MyPath() + szFilePath, L"r+");


		if (stream != NULL)
		{
			//MACHINE_DYN_UIN
			szRoute.Format(_T("%sST\\MACHINE_DYN_UIN.csv"), m_szCSV_Route);
			stream_UIN = _wfopen(MyPath() + szRoute, L"w+");
			fwprintf(stream_UIN, L"MACHINE_NM,MACHINE_MANUF_NM,MACHINE_II_TCCSET\n");

			fgets(chBuffer, sizeof(chBuffer), stream);
			while (fgets(chBuffer, sizeof(chBuffer), stream))
			{
				count = 0;
				token = strtok(chBuffer, ",");
				while (token != NULL)
				{
					if (count == 0)			type1 = atoi(token);
					else if (count == 1)	strName = CA2W((token)).m_psz;
					else if (count == 2)	strData = CA2W((token)).m_psz;
					else if (count == 3)	type2 = atoi(token);


					token = strtok(NULL, ",");
					count++;
				}
				fwprintf(stream_UIN, L"%s,%s,%d\n", strName, strData, type2);
			}
			fclose(stream);
			fclose(stream_UIN);
		}

		//RELAY_TYPE_DYN_UIN.csv
		szFilePath.Format(_T("\\config\\RELAY_TYPE_DYN_UIN.csv"));
		stream = _wfopen(MyPath() + szFilePath, L"r+");

		if (stream != NULL)
		{
			//RELAY_TYPE_DYN_UIN
			szRoute.Format(_T("%sST\\RELAY_TYPE_DYN_UIN.csv"), m_szCSV_Route);
			stream_UIN = _wfopen(MyPath() + szRoute, L"w+");
			fwprintf(stream_UIN, L"RELAY_II_MACHINE,RELAY_AD_SEP,RELAY_OG_SEP,RELAY_FD_SEP\n");

			fgets(chBuffer, sizeof(chBuffer), stream);
			while (fgets(chBuffer, sizeof(chBuffer), stream))
			{
				count = 0;
				token = strtok(chBuffer, ",");
				while (token != NULL)
				{
					if (count == 0)			type1 = atoi(token);
					else if (count == 1)	type2 = atoi(token);
					else if (count == 2)	type3 = atoi(token);
					else if (count == 3)	type4 = atoi(token);


					token = strtok(NULL, ",");
					count++;
				}
				fwprintf(stream_UIN, L"%d,%d,%d,%d\n", type1, type2, type3, type4);
			}
			fclose(stream);
			fclose(stream_UIN);
		}
		//RELAY_A_TAP_DYN_UIN.csv
		szFilePath.Format(_T("\\config\\RELAY_A_TAP_DYN_UIN.csv"));
		stream = _wfopen(MyPath() + szFilePath, L"r+");

		if (stream != NULL)
		{
			//RELAY_A_TAP_DYN_UIN
			szRoute.Format(_T("%sST\\RELAY_A_TAP_DYN_UIN.csv"), m_szCSV_Route);
			stream_UIN = _wfopen(MyPath() + szRoute, L"w+");
			fwprintf(stream_UIN, L"RELAY_II_MACHINE,RELAY_OG_SEP,RELAY_FD_SEP,RELAY_TAP_VALUE\n");

			fgets(chBuffer, sizeof(chBuffer), stream);
			while (fgets(chBuffer, sizeof(chBuffer), stream))
			{
				count = 0;
				token = strtok(chBuffer, ",");
				while (token != NULL)
				{
					if (count == 0)			type1 = atoi(token);
					else if (count == 1)	type2 = atoi(token);
					else if (count == 2)	type3 = atoi(token);
					else if (count == 3)	typefloat1 = atof(token);


					token = strtok(NULL, ",");
					count++;
				}
				fwprintf(stream_UIN, L"%d,%d,%d,%.4f\n", type1, type2, type3, typefloat1);
			}
			fclose(stream);
			fclose(stream_UIN);
		}
		//RELAY_D_TAP_DYN_UIN.csv
		szFilePath.Format(_T("\\config\\RELAY_D_TAP_DYN_UIN.csv"));
		stream = _wfopen(MyPath() + szFilePath, L"r+");

		if (stream != NULL)
		{
			//RELAY_D_TAP_DYN_UIN
			szRoute.Format(_T("%sST\\RELAY_D_TAP_DYN_UIN.csv"), m_szCSV_Route);
			stream_UIN = _wfopen(MyPath() + szRoute, L"w+");
			fwprintf(stream_UIN, L"RELAY_II_MACHINE,RELAY_OG_SEP,RELAY_FD_SEP,RELAY_TAP_START,RELAY_TAP_END,RELAY_TAP_STEP\n");

			fgets(chBuffer, sizeof(chBuffer), stream);
			while (fgets(chBuffer, sizeof(chBuffer), stream))
			{
				count = 0;
				token = strtok(chBuffer, ",");
				while (token != NULL)
				{
					if (count == 0)			type1 = atoi(token);
					else if (count == 1)	type2 = atoi(token);
					else if (count == 2)	type3 = atoi(token);
					else if (count == 3)	typefloat1 = atof(token);
					else if (count == 4)	typefloat2 = atof(token);
					else if (count == 5)	typefloat3 = atof(token);


					token = strtok(NULL, ",");
					count++;
				}
				fwprintf(stream_UIN, L"%d,%d,%d,%.4f,%.4f,%.4f\n", type1, type2, type3, typefloat1, typefloat2, typefloat3);
			}
			fclose(stream);
			fclose(stream_UIN);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//DB 연결 하는 부분
//DB Cinfig 연결 하는 부분 
bool CADMStoKCIMDlg::ADMStoKASIM_Config()
{
	int								nDBIndex;
	if (m_nSTMode == 2)
	{
		nDBIndex = 3;
		m_szMYSQL_Dbname = theDBContainer->GetConfigDBName(nDBIndex);
		m_szMYSQL_Userid = theDBContainer->GetConfigUserID(nDBIndex);
		m_szMYSQL_Password = theDBContainer->GetConfigPassWord(nDBIndex);
		m_szMYSQL_ServerIP = theDBContainer->GetConfigServerIP(nDBIndex);
		m_nMYSQL_Port = theDBContainer->GetConfigServerPort(nDBIndex);
		if (m_szMYSQL_Dbname.IsEmpty() || m_szMYSQL_Userid.IsEmpty() || m_szMYSQL_Password.IsEmpty() || m_szMYSQL_ServerIP.IsEmpty())
		{
			LOGOUT("* Err: Get XML file fail.( DB_Config : %d )", nDBIndex);
			return false;
		}
	}
	else
	{
		nDBIndex = 1;
		m_szMYSQL_Dbname = theDBContainer->GetConfigDBName(nDBIndex);
		m_szMYSQL_Userid = theDBContainer->GetConfigUserID(nDBIndex);
		m_szMYSQL_Password = theDBContainer->GetConfigPassWord(nDBIndex);
		m_szMYSQL_ServerIP = theDBContainer->GetConfigServerIP(nDBIndex);
		m_nMYSQL_Port = theDBContainer->GetConfigServerPort(nDBIndex);
		/*m_szMYSQL_ServerIP.Format(_T(""));*/
		if (m_szMYSQL_Dbname.IsEmpty() || m_szMYSQL_Userid.IsEmpty() || m_szMYSQL_Password.IsEmpty() || m_szMYSQL_ServerIP.IsEmpty())
		{
			LOGOUT("* Err: Get XML file fail.( DB_Config : %d )", nDBIndex);
			return false;
		}
	}


	nDBIndex = 2;
	m_szADMS_Code = theDBContainer->GetConfigDBName(nDBIndex);
	m_szMYSQL_Userid = theDBContainer->GetConfigUserID(nDBIndex);
	m_szMYSQL_Password = theDBContainer->GetConfigPassWord(nDBIndex);
	m_szMYSQL_ServerIP = theDBContainer->GetConfigServerIP(nDBIndex);
	m_nMYSQL_Port = theDBContainer->GetConfigServerPort(nDBIndex);
	if (m_szMYSQL_Dbname.IsEmpty() || m_szMYSQL_Userid.IsEmpty() || m_szMYSQL_Password.IsEmpty() || m_szMYSQL_ServerIP.IsEmpty())
	{
		LOGOUT("* Err: Get XML file fail.( DB_Config : %d )", nDBIndex);
		return false;
	}

// 
// 	CString											szXMLPath(MYPATH(_T("config"), _T("dbconfig.xml")).c_str());
// 	CXMLMgr											classLoadDataXML;
// 	HRESULT											nInit = classLoadDataXML.Initlize();
// 	bool											bload(classLoadDataXML.Load(szXMLPath));
// 
// 	MSXML2::IXMLDOMElement*							pRoot = classLoadDataXML.GetRoot();
// 	if (!pRoot)
// 	{
// 		return false;
// 	}
// 	CString											szTableName(_T(""));
// 	CString											szLoadMainName(_T("")), szLoadSubName(_T(""));
// 	CString											szValue_Provider(_T("")), szValue_Datasource(_T("")), szValue_Dbname(_T("")), szValue_Userid(_T("")), szValue_Password(_T("")), szFailover(_T(""));
// 	wchar_t											strDasConnect[256] = { 0x00, };
// 	MSXML2::IXMLDOMNode*							pMainNode = classLoadDataXML.GetFirstChild(), *pSubNode = NULL;
// 
// 	int nIndex_Check = 0;		//두번째 office 받기위해 내가 설정함
// 	if (pMainNode)
// 	{
// 		while (pMainNode)
// 		{
// 			classLoadDataXML.GetNodeName(pMainNode, szLoadMainName);
// 			if (szLoadMainName.Find(_T("dbconfigure")) != -1)
// 			{
// 				pSubNode = classLoadDataXML.GetFirstChild(pMainNode);
// 				if (pMainNode)
// 				{
// 					while (pSubNode)
// 					{
// 						classLoadDataXML.GetNodeName(pSubNode, szLoadSubName);
// 						if (szLoadSubName.Find(_T("provider")) != -1)
// 						{
// 							classLoadDataXML.GetItemText(pSubNode, szValue_Provider);
// 						}
// 						else if (szLoadSubName.Find(_T("datasource")) != -1)
// 						{
// 							classLoadDataXML.GetItemText(pSubNode, szValue_Datasource);
// 						}
// 						else if (szLoadSubName.Find(_T("dbname")) != -1)
// 						{
// 							classLoadDataXML.GetItemText(pSubNode, szValue_Dbname);
// 						}
// 						else if (szLoadSubName.Find(_T("userid")) != -1)
// 						{
// 							classLoadDataXML.GetItemText(pSubNode, szValue_Userid);
// 						}
// 						else if (szLoadSubName.Find(_T("userpassword")) != -1)
// 						{
// 							classLoadDataXML.GetItemText(pSubNode, szValue_Password);
// 						}
// 						else if (szLoadSubName.Find(_T("server")) != -1)
// 						{
// 							classLoadDataXML.GetItemText(pSubNode, szFailover);
// 						}
// 
// 						pSubNode = classLoadDataXML.GetNextSibling(pSubNode);
// 					}
// 				}
// 			}
// 			pMainNode = classLoadDataXML.GetNextSibling(pMainNode);
// 
// 			memset(strDasConnect, 0x00, sizeof(strDasConnect));
// 			swprintf_s(strDasConnect, _T("Provider=%s;Data Source=%s;Initial Catalog=%s;user ID=%s;password=%s;failoverpartner=%s;"),
// 				szValue_Provider.GetBuffer(),
// 				szValue_Datasource.GetBuffer(),
// 				szValue_Dbname.GetBuffer(),
// 				szValue_Userid.GetBuffer(),
// 				szValue_Password.GetBuffer(),
// 				szFailover.GetBuffer());
// 
// 			if (nIndex_Check == 1)		//office db연결 정보 
// 			{
// 				//szValue_Provider.GetBuffer()						 //<provider>MSDASQL.1;Driver={MariaDB ODBC 3.1 Driver};Persist Security Info=False</provider>
// 				//szValue_Datasource.GetBuffer(),					 //<datasource>ADMS_OFFICE</datasource>
// 				m_szMYSQL_Dbname = szValue_Dbname.GetBuffer();		 //<dbname>adms_office_16</dbname>
// 				m_szMYSQL_Userid = szValue_Userid.GetBuffer();		 //<userid>root</userid>
// 				m_szMYSQL_Password = szValue_Password.GetBuffer();	 //<userpassword>pnoinserver</userpassword>
// 				m_szMYSQL_Failover = szFailover.GetBuffer();		 //<server>pnoinserver</server>
// 			}
// 			if (nIndex_Check == 2)		//code 이름 정보 
// 			{
// 				m_szADMS_Code = szValue_Dbname.GetBuffer();			 //ADMS CODE 이름 받아오기!
// 			}
// 			nIndex_Check++;
// 		}
// 	}
	return true;
}

void CADMStoKCIMDlg::ADMStoKASIM_MYSQL()
{

	MYSQL *conn_result;
	unsigned int timeout_sec = 1;
	char m_cMYSQL_Dbname[256] = { 0x00, };
	char m_cMYSQL_Userid[256] = { 0x00, };
	char m_cMYSQL_Password[256] = { 0x00, };
	char m_cMYSQL_ServerIP[256] = { 0x00, };

	//_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	ConvertWCtoC(m_szMYSQL_Dbname, m_cMYSQL_Dbname);										//형변환을 해야함 근데 이부분...흠.
	ConvertWCtoC(m_szMYSQL_Userid, m_cMYSQL_Userid);
	ConvertWCtoC(m_szMYSQL_Password, m_cMYSQL_Password);
	ConvertWCtoC(m_szMYSQL_ServerIP, m_cMYSQL_ServerIP);

	//DB 접속 하는 부분 
	MYSQL_DB = mysql_init(NULL);
	mysql_options(MYSQL_DB, MYSQL_OPT_CONNECT_TIMEOUT, &timeout_sec);
	conn_result = mysql_real_connect(MYSQL_DB, m_cMYSQL_ServerIP, m_cMYSQL_Userid, m_cMYSQL_Password, m_cMYSQL_Dbname, m_nMYSQL_Port, NULL, 0);
	if (NULL == conn_result)
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("ADMStoKASIM_MYSQL-데이터베이스 연결 실패"));
	}
}

void CADMStoKCIMDlg::ADMStoKASIM_Read()
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	//
	CNode* pNode;
	CBranch* pBranch;
	pBranch = NULL;
	pNode = NULL;

	//변수 선언
	CString stStringNULL;
	stStringNULL.Format(_T(""));

	CString stEQC_MRFK, strData, stNAME, stALIAS_NAME;
	CString stTERID, strMRID, stO_EQC_MRFK, stC_EQC_MRFK;
	CString stCEQMRID, stORIGINAL_EQC_MRFK, stCHANGE_EQC_MRFK;
	CString stTerCNID;
	CString strNM, strCEQ;
	CString szADMS_Code;
	int	nNAME_TYPE_FK;
	int nType, nIdx, nMST_CD;
	int nn1 = 0;

	//LINESEGMENT_TYPE
	CString stPHASE_LINE_NAME, stNEUTRAL_LINE_NAME;
	CString stPHASE_LINE_NAME_CODE, stNEUTRAL_LINE_NAME_CODE;
	int		nLINESEGMENT_TYPE_ID, nOVERHEAD_CABLE;
	float	fPHASE_SECTIONAREA, fNEUTRAL_SECTIONAREA;
	float   fPOSITIVE_R, fPOSITIVE_X, fZERO_R, fZERO_X, fR_KM, fX_KM;
	int		nLNSECCODE_INDEX = 1;

	//HDOF_STA
	CString szHDOF_NM;
	int nHDOF_CODE = 0;
	//CENTER_STA
	CString stCENTER_OFFICE_ID;
	int nHEAD_OFFICE_FK = 0;
	//BOF_STA
	CString stHEAD_OFFICE_ID;
	CString stCENTER_OFFICE_FK;
	int  nAREA_BASE_CODE;
	int  nBOF_OFFICE_TYPE;
	//SS_STA
	CString stAREA_BASE_CODE;
	//ND_STA
	int nCount_i = 0;
	int nNAMEFK;
	CString strCNID;
	//GEN_STA
	float fDG_CAPACITY = 0;
	float fGEN_ITR_CAP = 0, fGEN_ITR_R = 0, fGEN_ITR_X = 6, fGEN_ITR_NGR_R = 0, fGEN_ITR_NGR_X = 0;
	int nGEN_ITR_WDC = 1, nGEN_MACH_TYPE = 1;
	int nGEN_TYPE = 0;
	CString szGEN_DG_TR_MRID;
	int nTR_GRIDSIDE, nTR_GENSIDE, nGEN_ITR_CAP;
	nTR_GRIDSIDE = nTR_GENSIDE = nGEN_ITR_CAP = 0;
	CString szGENUNIT_CEQ, szGENUNIT_CUS_NM, szGENUNIT_CAP_KW, szGENUNIT_CUS_NO, szGENUNIT_LOCA_NO, szGENUNIT_LOCA_NM, szGENUNIT_II_EQU_ID;
	float fGENUNIT_CAP_KW;
	int nGENUNIT_TYPE;
	int nGEN_STA_ID = 0;
	int nCheckGEN = 0;
	int nCheck = 0;
	//MTR_STA/TR_STA
	float fHV, fLV;
	int nMTR_BANK = 0;
	CString stHV_LIMIT, stLV_LIMIT, stPRIMARY_VOLTAGELEVEL, stSECONDARY_VOLTAGELEVEL;
	//DL_STA
	int nDLID = 0;
	CString stMTR_MRFK, stPRIMARY_VOLTAGELEVEL_FK, stSECONDARY_VOLTAGELEVEL_FK;
	int nMember_office_fk = 0;
	CString strData2;
	//CBSW_STA
	CString szMULTISW;
	int nMULTNUMBER;
	//SNV_STA
	int nBASEVOLTAGE_FK;
	float fHV_LIMIT;
	CString stSUBS_MRFK;
	CString stVLNM;
	//LNSEC_STA
	int nlineType = 0;
	float flength = 0;
	float fPROMISE_LOAD = 0;
	float fTHERMAL_LIMIT = 0; //20231129 

	//
	CString stMaster_codeID, stMasterCEQ_TYPE, stMaster_Circuit;
	int nMaster_codeID, nMasterCEQ_TYPE, nMaster_Circuit;
	int nMaster_Index = 0;
	int nMaster_code = 0;
	double dAO_Value = 0;
	CString stAO_ceqid;
	CString stAO_key;
	int nCOMM_CODE = 0;
	int nCeqTypeID, nRTU_Code_G1;
	int nCeq_type = 0;
	int nCode = 0;
	int nCN_Count = 0;
	int nCOUNT = 0;
	CString stCNDate;

	int nHead_OFFICE_ID = 0;
	int nCENTER_OFFICE_ID = 0;
	int nMEMBER_OFFICE_ID = 0;


	CString strADMS_SQL;
	CString stDLEQC_MRFK;


	char buffer[4096] = { 0x00, };
	char query[4096] = { 0x00, };
	//char chAdms_code[128] = { 0x00, };

	TCHAR tcBuffer[4096] = { 0x00, };
	TCHAR tcAdms_code[128] = { 0x00, };
	int len = 4096;
	int fields = 0;


	//1. 사업소 선택하는 부분!!
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][1/33]========== ADMStoKASIM 데이터 읽기 [사업소 변환] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		strADMS_SQL.Format(_T("select MEMBER_OFFICE_ID, NAME from `%s`.`member_office` where MEMBER_OFFICE_ID = %d;"), m_szADMS_Code, m_nType);

		_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
		WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환

		mysql_query(MYSQL_DB, "set Names euckr");
		if (mysql_query(MYSQL_DB, query))
		{
			IDC_LIST_DATA_HISTORY_ERROR(_T("[사업소 변환] 데이터베이스 연결 실패"));
			return;
		}
		result = mysql_store_result(MYSQL_DB);
		fields = mysql_num_fields(result);			 // 필드 갯수 구함
		while (row = mysql_fetch_row(result))		  // 모든 레코드 탐색
		{
			m_nSTMODE_KASIM_Office_ID = GetInt_Row(row[0]);
			m_szSTMODE_KASIM_Office_Name = GetCString_Row(row[1]);;

		}
		mysql_free_result(result);
	}
	else
	{
		strADMS_SQL.Format(_T("select HEAD_OFFICE_ID, NAME from  `%s`.`head_office` where head_office_id in \
			( select head_office_fk from `%s`.`center_office` where center_office_id in  \
			( select center_office_fk from `%s`.`member_office` where member_office_id in \
		    ( select distinct member_office_fk from dl)))"), m_szADMS_Code, m_szADMS_Code, m_szADMS_Code);

		_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
		WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환

		mysql_query(MYSQL_DB, "set Names euckr");
		if (mysql_query(MYSQL_DB, query))
		{
			IDC_LIST_DATA_HISTORY_ERROR(_T("[사업소 변환] 데이터베이스 연결 실패"));
			return;
		}
		result = mysql_store_result(MYSQL_DB);
		fields = mysql_num_fields(result);			 // 필드 갯수 구함
		while (row = mysql_fetch_row(result))		  // 모든 레코드 탐색
		{
			m_nSTMODE_KASIM_Office_ID = GetInt_Row(row[0]);
			m_szSTMODE_KASIM_Office_Name = GetCString_Row(row[1]);
		}
		mysql_free_result(result);
	}

	//공용선로 DL관리
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][2/33]========== ADMStoKASIM 데이터 읽기 [공용선로 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	strADMS_SQL.Format(_T("select eqc_mrfk , ceq_mrfk, dl_index from dl  WHERE CEQ_MRFK IS NOT NULL order by ceq_mrfk , dl_index "));
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환

	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[공용선로 정보] 데이터베이스 연결 실패"));
		return;
	}
	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);			 // 필드 갯수 구함
	while (row = mysql_fetch_row(result))		  // 모든 레코드 탐색
	{
		stEQC_MRFK = GetCString_Row(row[0]);
		strMRID = GetCString_Row(row[1]);;

		Conv_DL_Info_Array(strMRID, stEQC_MRFK); 
	}
	mysql_free_result(result);


	//20231208

	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][2/33]========== ADMStoKASIM 데이터 읽기 [DL_VL정보입력!!!!!1] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	strADMS_SQL.Format(_T("SELECT D.EQC_MRFK, PV.EQC_MRFK , D.CEQ_MRFK FROM dl D INNER JOIN (SELECT P.CEQ_MRFK, V.EQC_MRFK FROM powertransformer P INNER JOIN voltagelevel V ON P.SECONDARY_VOLTAGELEVEL_FK  = V.EQC_MRFK) PV ON D.MTR_MRFK = PV.CEQ_MRFK;"));
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환

	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[공용선로 정보] 데이터베이스 연결 실패"));
		return;
	}
	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);			 // 필드 갯수 구함
	while (row = mysql_fetch_row(result))		  // 모든 레코드 탐색
	{
		stDLEQC_MRFK = GetCString_Row(row[0]);
		stEQC_MRFK = GetCString_Row(row[1]);
		strMRID = GetCString_Row(row[1]);

		if (m_nDL_TEST != 0) //공용선로만 해주는 작업입니다.
		{
			stDLEQC_MRFK = GetEQC_MRFK(stDLEQC_MRFK); //13번을 바꾸는 작업입니다.
		}
		m_map_DL_EQC_VL_EQC.SetAt(stDLEQC_MRFK, stEQC_MRFK);
		m_map_VL_EQC_DL_EQC.SetAt(stEQC_MRFK, stDLEQC_MRFK);
		m_map_DL_CEQ_DL_EQC.SetAt(strMRID, stDLEQC_MRFK);
	}
	mysql_free_result(result);

	//////
	
	//3. TERMINAL
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][3/33]========== ADMStoKASIM 데이터 읽기 [TERMINAL 정보 - 1] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();
	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로
		{
			strADMS_SQL.Format(_T("SELECT  A.CEQ_MRFK, A.CONNECTIVITYNODE_FK,  B.CHANGE_EQC_MRFK,  B.NAME_TYPE_FK FROM terminal A, (SELECT A.MRID, A.CEQ_TYPE_FK, A.ORIGINAL_EQC_MRFK, A.CHANGE_EQC_MRFK, B.NAME, B.NAME_TYPE_FK FROM conductingequipment A, identifiedobject B WHERE A.MRID = B.MRID) B WHERE A.CEQ_MRFK = B.MRID and B.MRID in (select MRID from conductingequipment where CHANGE_EQC_MRFK in (select EQC_MRFK from dl where member_office_fk = %d) OR MRID IN (select CEQ_MRFK from dl where member_office_fk = %d )) and b.NAME_TYPE_FK != 15 and b.NAME_TYPE_FK != 16;"), m_nType, m_nType); //임시!
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("SELECT  A.CEQ_MRFK, A.CONNECTIVITYNODE_FK,  B.CHANGE_EQC_MRFK,  B.NAME_TYPE_FK FROM terminal A, (SELECT A.MRID, A.CEQ_TYPE_FK, A.ORIGINAL_EQC_MRFK, A.CHANGE_EQC_MRFK, B.NAME, B.NAME_TYPE_FK FROM conductingequipment A, identifiedobject B WHERE A.MRID = B.MRID) B WHERE A.CEQ_MRFK = B.MRID and B.MRID in (select MRID from conductingequipment where CHANGE_EQC_MRFK in (  select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d) ) OR MRID IN (select CEQ_MRFK from dl where member_office_fk = %d )) and b.NAME_TYPE_FK != 15 and b.NAME_TYPE_FK != 16;"), m_nType, m_nType); //임시!
		}
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT  A.CEQ_MRFK, A.CONNECTIVITYNODE_FK,  B.CHANGE_EQC_MRFK,  B.NAME_TYPE_FK FROM  terminal A, \
				(SELECT A.MRID, A.CEQ_TYPE_FK, A.ORIGINAL_EQC_MRFK, A.CHANGE_EQC_MRFK, B.NAME, B.NAME_TYPE_FK FROM conductingequipment A, identifiedobject B \
				WHERE A.MRID = B.MRID and b.NAME_TYPE_FK != 11 and b.NAME_TYPE_FK != 12 and b.NAME_TYPE_FK != 13 and b.NAME_TYPE_FK != 15 and b.NAME_TYPE_FK != 16 and A.CEQ_TYPE_FK != 207) B \
				WHERE A.CEQ_MRFK = B.MRID; "));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[TERMINAL 정보 - 1] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		stTerCNID = GetCString_Row(row[1]);
		stC_EQC_MRFK = GetCString_Row(row[2]);
		nNAME_TYPE_FK = GetInt_Row(row[3]);;


		if (stTerCNID.IsEmpty())
		{
			if (nNAME_TYPE_FK == 21 || nNAME_TYPE_FK == 22 || nNAME_TYPE_FK == 25 || nNAME_TYPE_FK == 27)
			{
				stTerCNID.Format(_T("%s88"), strMRID);
			}
		}
		if (!(stTerCNID.IsEmpty()))
		{
			if (nNAME_TYPE_FK == 14)
			{
				m_map_VL_EQC_DL_EQC.Lookup(stC_EQC_MRFK, stO_EQC_MRFK);
				stC_EQC_MRFK.Format(_T("%s"), stO_EQC_MRFK);
			}
			if (m_nDL_TEST != 0) //공용선로만 해주는 작업입니다.
			{
				stC_EQC_MRFK = GetEQC_MRFK(stC_EQC_MRFK); //13번을 바꾸는 작업입니다.
			}

			//MAP정보
			m_map_Ter_CnID.SetAt(stTerCNID, stC_EQC_MRFK);
			m_map_Ter_CeqCn.SetAt(strMRID, stTerCNID);
			m_map_Common_CB_Check.SetAt(strMRID, stTerCNID); //20230504 	
			if (nNAME_TYPE_FK == 51) //20230830
			{
				m_map_Ter_CNID_NAMETYPEFK.SetAt(stTerCNID, nNAME_TYPE_FK);
			}
		}
	}
	mysql_free_result(result);

	
	//4터미널 정보
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][4/33]========== ADMStoKASIM 데이터 읽기 [TERMINAL 정보 - 2] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	strADMS_SQL.Format(_T("SELECT A.NAME_TYPE_FK, B.CONNECTIVITYNODE_FK FROM identifiedobject A, terminal B WHERE A.MRID = B.CEQ_MRFK AND A.NAME_TYPE_FK = 15"));
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환

	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[TERMINAL 정보 - 2] 데이터베이스 연결 실패"));
		return;
	}
	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);			 // 필드 갯수 구함
	while (row = mysql_fetch_row(result))		  // 모든 레코드 탐색
	{
		nNAME_TYPE_FK = GetInt_Row(row[0]);
		stTerCNID = GetCString_Row(row[1]);
		m_map_Ter_CNID_NAMETYPEFK.SetAt(stTerCNID, nNAME_TYPE_FK);
	}
	mysql_free_result(result);

	//5. Conductingequipment
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][5/33]========== ADMStoKASIM 데이터 읽기 [Conductingequipment 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select mrid, ORIGINAL_EQC_MRFK, CHANGE_EQC_MRFK , ceq_type_fk from conductingequipment where CHANGE_EQC_MRFK in \
						(select EQC_MRFK from dl where member_office_fk = %d) OR MRID IN (select CEQ_MRFK from dl where member_office_fk = %d)"), m_nType, m_nType); //임시!
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select mrid, ORIGINAL_EQC_MRFK, CHANGE_EQC_MRFK, ceq_type_fk from conductingequipment where CHANGE_EQC_MRFK in \
				( select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d)) OR MRID IN (select CEQ_MRFK from dl where member_office_fk = %d)"), m_nType, m_nType); //임시!
		}
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT mrid, ORIGINAL_EQC_MRFK, CHANGE_EQC_MRFK, ceq_type_fk FROM conductingequipment"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[Conductingequipment 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		stCEQMRID = GetCString_Row(row[0]);
		stORIGINAL_EQC_MRFK = GetCString_Row(row[1]);
		stCHANGE_EQC_MRFK = GetCString_Row(row[2]);
		nCeqTypeID = GetInt_Row(row[3]);;

		if (nCeqTypeID == 71 || nCeqTypeID == 207)
		{
			m_map_VL_EQC_DL_EQC.Lookup(stCHANGE_EQC_MRFK, stO_EQC_MRFK);
			stCHANGE_EQC_MRFK.Format(_T("%s"), stO_EQC_MRFK);
		}
		if (m_nDL_TEST != 0) //공용선로만 해주는 작업입니다.
		{
			stCHANGE_EQC_MRFK = GetEQC_MRFK(stCHANGE_EQC_MRFK);
		}
		m_map_CEQ_MridCHCeq.SetAt(stCEQMRID, stCHANGE_EQC_MRFK); //12번 찾기 위한 작엄
	}
	mysql_free_result(result);


	//6. LINESEGMENT_TYPE
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][6/33]========== ADMStoKASIM 데이터 읽기 [LINESEGMENT_TYPE 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();
	
	strADMS_SQL.Format(_T("SELECT LST.LINESEGMENT_SET_TYPE_ID, BB.NAME AS PHASE_NAME , BB.SECTIONAREA AS PHASE_SECTIONAREA , CC.NAME AS NEUTAL_NAME, CC.SECTIONAREA AS NEUTAL_SECTIONAREA, \
					 LST.POSITIVE_R, LST.POSITIVE_X, LST.ZERO_R, LST.ZERO_X , LST.R_KM, LST.X_KM, LST.OVERHEAD_CABLE, LST.THERMAL_LIMIT FROM `%s`.`linesegment_set_type` LST, \
					(SELECT A.LINESEGMENT_SET_TYPE_ID, B.NAME, C.SECTIONAREA FROM `%s`.`linesegment_set_type` A, `%s`.`linesegment_type_code` B, `%s`.`linesegment_sectionarea_type` C \
					WHERE A.PHASE_TYPE_CODE_FK = B.LINESEGMENT_TYPE_CODE_ID AND A.PHASE_SECTIONAREA_TYPE_FK = C.LINESEGMENT_SECTIONAREA_TYPE_ID) BB, \
					 (SELECT A.LINESEGMENT_SET_TYPE_ID, B.NAME, C.SECTIONAREA FROM `%s`.`linesegment_set_type` A, `%s`.`linesegment_type_code` B, `%s`.`linesegment_sectionarea_type` C \
					 WHERE A.NEUTRAL_TYPE_CODE_FK = B.LINESEGMENT_TYPE_CODE_ID AND A.NEUTRAL_SECTIONAREA_TYPE_FK = C.LINESEGMENT_SECTIONAREA_TYPE_ID) CC \
					 WHERE LST.LINESEGMENT_SET_TYPE_ID = BB.LINESEGMENT_SET_TYPE_ID AND LST.LINESEGMENT_SET_TYPE_ID = CC.LINESEGMENT_SET_TYPE_ID  ORDER BY LST.LINESEGMENT_SET_TYPE_ID;")
		, m_szADMS_Code, m_szADMS_Code, m_szADMS_Code, m_szADMS_Code, m_szADMS_Code, m_szADMS_Code, m_szADMS_Code);

	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[LINESEGMENT_TYPE 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		nLINESEGMENT_TYPE_ID = GetInt_Row(row[0]);
		stPHASE_LINE_NAME = GetCString_Row(row[1]);
		fPHASE_SECTIONAREA = GetDouble_Row(row[2]);
		stNEUTRAL_LINE_NAME = GetCString_Row(row[3]);
		fNEUTRAL_SECTIONAREA = GetDouble_Row(row[4]);

		fPOSITIVE_R = GetDouble_Row(row[5]);
		fPOSITIVE_X = GetDouble_Row(row[6]);
		fZERO_R = GetDouble_Row(row[7]);
		fZERO_X = GetDouble_Row(row[8]);
		fR_KM = GetDouble_Row(row[9]);
		fX_KM = GetDouble_Row(row[10]);
		nOVERHEAD_CABLE = GetInt_Row(row[11]);
		fTHERMAL_LIMIT = GetDouble_Row(row[12]);

		//20220824 변경 요청사항!
		stPHASE_LINE_NAME_CODE.Format(_T("%s(%g)"), stPHASE_LINE_NAME, fPHASE_SECTIONAREA);
		stNEUTRAL_LINE_NAME_CODE.Format(_T("%s(%g)"), stNEUTRAL_LINE_NAME, fNEUTRAL_SECTIONAREA);

		for (int i = 0; i < 100; i++)
		{
			if (nLNSECCODE_INDEX == nLINESEGMENT_TYPE_ID)
			{
				LINESEGMENT_TYPE stLINESEGMENT_TYPE;
				memset(&stLINESEGMENT_TYPE, 0, sizeof(LINESEGMENT_TYPE));
				stLINESEGMENT_TYPE.LINESEGMENT_TYPE_ID = nLINESEGMENT_TYPE_ID;
				stLINESEGMENT_TYPE.OVERHEAD_CABLE = nOVERHEAD_CABLE;
				_stprintf_s(stLINESEGMENT_TYPE.PHASE_LINE_TYPE, L"%s", stPHASE_LINE_NAME_CODE);
				_stprintf_s(stLINESEGMENT_TYPE.NEUTRAL_LINE_TYPE, L"%s", stNEUTRAL_LINE_NAME_CODE);
				stLINESEGMENT_TYPE.POSITIVE_R = fPOSITIVE_R;
				stLINESEGMENT_TYPE.POSITIVE_X = fPOSITIVE_X;
				stLINESEGMENT_TYPE.ZERO_R = fZERO_R;
				stLINESEGMENT_TYPE.ZERO_X = fZERO_X;
				stLINESEGMENT_TYPE.R_KM = fR_KM;
				stLINESEGMENT_TYPE.X_KM = fX_KM;
				stLINESEGMENT_TYPE.THERMAL_LIMIT = fTHERMAL_LIMIT;
				m_arrrLINESEGMENT_TYPE.Add(stLINESEGMENT_TYPE);

				m_map_LINESEGMENT_TYPE_CABLE.SetAt(nLINESEGMENT_TYPE_ID, nOVERHEAD_CABLE);
				m_map_LINESEGMENT_TYPE_POSR.SetAt(nLINESEGMENT_TYPE_ID, fPOSITIVE_R);
				m_map_LINESEGMENT_TYPE_POSX.SetAt(nLINESEGMENT_TYPE_ID, fPOSITIVE_X);
				m_map_LINESEGMENT_TYPE_ZERR.SetAt(nLINESEGMENT_TYPE_ID, fZERO_R);
				m_map_LINESEGMENT_TYPE_ZERX.SetAt(nLINESEGMENT_TYPE_ID, fZERO_X);
				nLNSECCODE_INDEX++;
				i = 1000;
			}
			else
			{
				LINESEGMENT_TYPE stLINESEGMENT_TYPE;
				memset(&stLINESEGMENT_TYPE, 0, sizeof(LINESEGMENT_TYPE));
				stLINESEGMENT_TYPE.LINESEGMENT_TYPE_ID = nLINESEGMENT_TYPE_ID;
				stLINESEGMENT_TYPE.OVERHEAD_CABLE = 0;
				_stprintf_s(stLINESEGMENT_TYPE.PHASE_LINE_TYPE, L"0-공란(0)");
				_stprintf_s(stLINESEGMENT_TYPE.NEUTRAL_LINE_TYPE, L"0-공란(0)");
				stLINESEGMENT_TYPE.POSITIVE_R = 0;
				stLINESEGMENT_TYPE.POSITIVE_X = 0;
				stLINESEGMENT_TYPE.ZERO_R = 0;
				stLINESEGMENT_TYPE.ZERO_X = 0;
				stLINESEGMENT_TYPE.R_KM = 0;
				stLINESEGMENT_TYPE.X_KM = 0;
				m_arrrLINESEGMENT_TYPE.Add(stLINESEGMENT_TYPE);
				nLNSECCODE_INDEX++;
			}
		}
	}
	mysql_free_result(result);

	//7. HEAD_OFFICE
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][7/33]========== ADMStoKASIM 데이터 읽기 [HEAD_OFFICE 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		strADMS_SQL.Format(_T("SELECT NAME,HEAD_OFFICE_ID FROM `%s`.`head_office`	where HEAD_OFFICE_ID in( \
			SELECT HEAD_OFFICE_FK FROM `%s`.`center_office` \
			WHERE CENTER_OFFICE_ID IN(SELECT CENTER_OFFICE_FK FROM `%s`.`member_office` WHERE MEMBER_OFFICE_ID = %d))"), m_szADMS_Code, m_szADMS_Code, m_szADMS_Code, m_nType); //임시!
	}
	else
	{
		if (m_nSTMODE_KASIM_Office_ID != 0)
		{
			strADMS_SQL.Format(_T("SELECT NAME,HEAD_OFFICE_ID FROM `%s`.`head_office` where  HEAD_OFFICE_ID = %d"), m_szADMS_Code , m_nSTMODE_KASIM_Office_ID);
		}
		else
		{
			strADMS_SQL.Format(_T("SELECT NAME,HEAD_OFFICE_ID FROM `%s`.`head_office`"), m_szADMS_Code);
		}
	}

	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("HEAD_OFFICE 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);		// 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		szHDOF_NM = GetCString_Row(row[0]);
		nHDOF_CODE = GetInt_Row(row[1]);

		nn1 = 0;
		nn1 = szHDOF_NM.Find(',');
		if (nn1 > -1)
		{
			szHDOF_NM.Replace(_T(","), _T("_"));
		}
		HDOF_STA stHDOF_STA;
		memset(&stHDOF_STA, 0, sizeof(HDOF_STA));
		_stprintf_s(stHDOF_STA.HDOF_NM, L"%s", szHDOF_NM);
		stHDOF_STA.HDOF_CODE = nHDOF_CODE;
		m_arrHDOF.Add(stHDOF_STA);
	}
	mysql_free_result(result);


	//8. CENTER_OFFICE
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][8/33]========== ADMStoKASIM 데이터 읽기 [CENTER_OFFICE 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		strADMS_SQL.Format(_T("SELECT CENTER_OFFICE_ID, NAME, HEAD_OFFICE_FK FROM `%s`.`center_office` \
		 WHERE CENTER_OFFICE_ID IN  ( SELECT CENTER_OFFICE_FK FROM `%s`.`member_office` WHERE MEMBER_OFFICE_ID = %d)"), m_szADMS_Code, m_szADMS_Code, m_nType); //임시!
	}
	else
	{
		if (m_nSTMODE_KASIM_Office_ID != 0)
		{
			strADMS_SQL.Format(_T("SELECT CENTER_OFFICE_ID, NAME, HEAD_OFFICE_FK FROM `%s`.`center_office` where HEAD_OFFICE_FK = %d"), m_szADMS_Code, m_nSTMODE_KASIM_Office_ID);
		}
		else
		{
			strADMS_SQL.Format(_T("SELECT CENTER_OFFICE_ID, NAME, HEAD_OFFICE_FK FROM `%s`.`center_office`"), m_szADMS_Code);
		}
	}

	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[CENTER_OFFICE 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);		// 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		stCENTER_OFFICE_ID = GetCString_Row(row[0]);
		stNAME = GetCString_Row(row[1]);
		nHEAD_OFFICE_FK = GetInt_Row(row[2]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		CENTER_STA stCENTER;
		memset(&stCENTER, 0, sizeof(CENTER_STA));
		_stprintf_s(stCENTER.center_nm, L"%s", stNAME);
		_stprintf_s(stCENTER.center_officeid, L"%s", stCENTER_OFFICE_ID);
		stCENTER.nCENTER_HEAD_OFFICE_FK = nHEAD_OFFICE_FK;						//HI작업을 하기 위한 필드값 입력
		stCENTER.nCENTER_II_HDOF = GetCENTER_II_HDOF(nHEAD_OFFICE_FK);
		m_arrCENTER.Add(stCENTER);
	}
	mysql_free_result(result);

	//9. BOF_STA
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][9/33]========== ADMStoKASIM 데이터 읽기 [MEMBER_OFFICE 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		strADMS_SQL.Format(_T("SELECT MEMBER_OFFICE_ID, NAME, OFFICE_TYPE, CENTER_OFFICE_FK  from `%s`.`member_office` WHERE MEMBER_OFFICE_ID = %d"), m_szADMS_Code, m_nType); //임시!
	}
	else
	{
		if (m_nSTMODE_KASIM_Office_ID != 0)
		{
			strADMS_SQL.Format(_T("SELECT MEMBER_OFFICE_ID, NAME, OFFICE_TYPE, CENTER_OFFICE_FK  from `%s`.`member_office` \
						where CENTER_OFFICE_FK in ( SELECT CENTER_OFFICE_ID FROM `%s`.`center_office` where HEAD_OFFICE_FK = %d)")
						, m_szADMS_Code, m_szADMS_Code, m_nSTMODE_KASIM_Office_ID);
		}
		else
		{
			strADMS_SQL.Format(_T("SELECT MEMBER_OFFICE_ID, NAME, OFFICE_TYPE, CENTER_OFFICE_FK  from `%s`.`member_office`"), m_szADMS_Code);
		}
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[MEMBER_OFFICE 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		stEQC_MRFK = GetCString_Row(row[0]);
		stNAME = GetCString_Row(row[1]);
		nBOF_OFFICE_TYPE = GetInt_Row(row[2]);
		stCENTER_OFFICE_FK = GetCString_Row(row[3]);
		nAREA_BASE_CODE = GetInt_Row(row[0]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		BOF_STA stBOF;
		memset(&stBOF, 0, sizeof(BOF_STA));
		_stprintf_s(stBOF.bof_nm, L"%s", stNAME);
		_stprintf_s(stBOF.bof_officeid, L"%s", stEQC_MRFK);
		stBOF.bof_ii_center = GetBOF_II_CENTER(stCENTER_OFFICE_FK);
		stBOF.bof_AREA_BASE_CODE = nAREA_BASE_CODE; //MEMBER_OFFICE 코드입니다.
		m_arrBOF.Add(stBOF);
	}
	mysql_free_result(result);

	//10. SS_STA
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][10/33]========== ADMStoKASIM 데이터 읽기 [변전소 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		strADMS_SQL.Format(_T("SELECT A.EQC_MRFK, B.NAME ,A.SUBSTATION_CODE, A.HEAD_OFFICE_FK FROM substation A, identifiedobject B WHERE A.EQC_MRFK = B.MRID AND A.EQC_MRFK IN \
		 ( select distinct subs_mrfk from voltagelevel where eqc_mrfk in ( select distinct primary_voltagelevel_fk from powertransformer where ceq_mrfk in \
		 ( select distinct mtr_mrfk   from dl where member_office_fk = %d)));"), m_nType);
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT A.EQC_MRFK, B.NAME ,A.SUBSTATION_CODE, A.HEAD_OFFICE_FK FROM substation A, identifiedobject B WHERE A.EQC_MRFK = B.MRID"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[변전소 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		stEQC_MRFK = GetCString_Row(row[0]);
		stNAME = GetCString_Row(row[1]);
		stALIAS_NAME = GetCString_Row(row[2]);
		nHEAD_OFFICE_FK = GetInt_Row(row[3]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		if (stALIAS_NAME.IsEmpty())
		{
			stALIAS_NAME.Format(_T("NULL"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		//코드 찾기 문자열 자르기 !
		stAREA_BASE_CODE = stEQC_MRFK.Left(4);
		nAREA_BASE_CODE = _wtoi(stAREA_BASE_CODE);

		SS_STA stSS;
		memset(&stSS, 0, sizeof(SS_STA));
		_stprintf_s(stSS.ss_nm, L"%s", stNAME);
		_stprintf_s(stSS.ss_substationid, L"%s", stEQC_MRFK);
		_stprintf_s(stSS.ss_code, L"%s", stALIAS_NAME);
		m_arrSS.Add(stSS);
	}
	mysql_free_result(result);

	//11. SNV_STA
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][11/33]========== ADMStoKASIM 데이터 읽기 [VLOTAGELEVEL 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		strADMS_SQL.Format(_T("SELECT A.EQC_MRFK, A.SUBS_MRFK, A.BASEVOLTAGE_FK, A.HV_LIMIT, B.NAME FROM voltagelevel A, identifiedobject B WHERE A.EQC_MRFK = B.MRID AND A.SUBS_MRFK IN \
					( select distinct subs_mrfk from voltagelevel where eqc_mrfk in ( select distinct primary_voltagelevel_fk from powertransformer where ceq_mrfk in \
					( select distinct mtr_mrfk   from dl where member_office_fk = %d)));"), m_nType);
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT A.EQC_MRFK, A.SUBS_MRFK, A.BASEVOLTAGE_FK, A.HV_LIMIT, B.NAME FROM voltagelevel A, identifiedobject B WHERE A.EQC_MRFK = B.MRID"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[VLOTAGELEVEL 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		stEQC_MRFK = GetCString_Row(row[0]);
		stSUBS_MRFK = GetCString_Row(row[1]);
		nBASEVOLTAGE_FK = GetInt_Row(row[2]);
		fHV_LIMIT = GetDouble_Row(row[3]);
		stVLNM = GetCString_Row(row[4]);

		SNV_STA stSnv;
		memset(&stSnv, 0, sizeof(SNV_STA));
		_stprintf_s(stSnv.snv_nm, stVLNM);
		stSnv.snv_norkv = fHV_LIMIT / 1000;
		stSnv.snv_ii_ss = GetSNV_II_SS(stSUBS_MRFK);
		m_arrSNV.Add(stSnv);
		//MAP 정보
		m_map_SNV_MTRSNVID.SetAt(stEQC_MRFK, m_arrSNV.GetSize());
	}
	mysql_free_result(result);

	//12. 노드 -1
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][12/33]========== ADMStoKASIM 데이터 읽기 [노드 정보 - 1] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("SELECT A.CONNECTIVITYNODE_ID, A.CEQ_MRFK, B.NAME, B.NAME_TYPE_FK FROM (SELECT A.CONNECTIVITYNODE_ID, B.CEQ_MRFK FROM connectivitynode A INNER JOIN \
				(SELECT TER.* FROM TERMINAL TER INNER JOIN identifiedobject Ide ON TER.CEQ_MRFK = Ide.MRID and Ide.NAME_TYPE_FK != 51) B ON A.CONNECTIVITYNODE_ID = B.CONNECTIVITYNODE_FK  ) A, \
				identifiedobject B WHERE A.CEQ_MRFK = B.MRID AND B.MRID IN (select MRID from conductingequipment where CHANGE_EQC_MRFK in (select EQC_MRFK from dl where member_office_fk = %d) OR MRID IN \
				(select CEQ_MRFK from dl where member_office_fk = %d))AND B.NAME_TYPE_FK != 15 GROUP BY A.CONNECTIVITYNODE_ID;"), m_nType, m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("SELECT A.CONNECTIVITYNODE_ID, A.CEQ_MRFK, B.NAME, B.NAME_TYPE_FK FROM (SELECT A.CONNECTIVITYNODE_ID, B.CEQ_MRFK FROM connectivitynode A INNER JOIN \
				 (SELECT TER.* FROM TERMINAL TER INNER JOIN identifiedobject Ide ON TER.CEQ_MRFK = Ide.MRID and Ide.NAME_TYPE_FK != 51) B ON A.CONNECTIVITYNODE_ID = B.CONNECTIVITYNODE_FK  ) A, \
				 identifiedobject B WHERE A.CEQ_MRFK = B.MRID AND B.MRID IN (select MRID from conductingequipment where CHANGE_EQC_MRFK in ( select EQC_MRFK from dl where CEQ_MRFK IN \
				 (select CEQ_MRFK from dl where member_office_fk = %d))OR MRID IN (select CEQ_MRFK from dl where member_office_fk = %d))AND B.NAME_TYPE_FK != 15 GROUP BY A.CONNECTIVITYNODE_ID;")
				, m_nType, m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT A.CONNECTIVITYNODE_ID, A.CEQ_MRFK, B.NAME, B.NAME_TYPE_FK FROM (SELECT A.CONNECTIVITYNODE_ID, B.CEQ_MRFK FROM connectivitynode A INNER JOIN (SELECT TER.* FROM TERMINAL TER INNER JOIN identifiedobject Ide ON TER.CEQ_MRFK = Ide.MRID and Ide.NAME_TYPE_FK != 51) B ON A.CONNECTIVITYNODE_ID = B.CONNECTIVITYNODE_FK GROUP BY A.CONNECTIVITYNODE_ID) A, identifiedobject B WHERE A.CEQ_MRFK = B.MRID AND B.NAME_TYPE_FK != 15;"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[노드 정보 - 1] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strCNID = GetCString_Row(row[0]);
		strCEQ = GetCString_Row(row[1]);
		stNAME = GetCString_Row(row[2]);
		nNAMEFK = GetInt_Row(row[3]);

		if (nNAMEFK == 14)
		{
			stDLEQC_MRFK.Format(_T(""));
			m_map_DL_CEQ_DL_EQC.Lookup(strCEQ, stDLEQC_MRFK);
			if (stDLEQC_MRFK.IsEmpty())
			{
				continue;
			}
		}

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		pNode = new CNode();
		pNode->m_nKind = 0;
		pNode->m_strMRID.Format(_T("%s"), strCNID);
		pNode->m_strName.Format(_T("%s"), stNAME);
		pNode->m_strCEQID.Format(_T("%s"), strCEQ);
		pNode->m_nName_Type = nNAMEFK;
		m_pNodeArr.Add(pNode);
		//230504
		m_map_Common_ND_CnCeq.SetAt(strCNID, strCEQ);
		//m_map_Common_MUSW_CN.SetAt()

		m_map_IDNode.SetAt(strCNID, m_pNodeArr.GetSize());
	}
	
	//13. 노드 -2
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][13/33]========== ADMStoKASIM 데이터 읽기 [노드 정보 - 2] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select A.MRID, A.NAME_TYPE_FK, A.NAME, A.ALIAS_NAME, B.CONNECTIVITYNODE_FK from identifiedobject A, (select * from terminal where ceq_mrfk in \
			( select ceq_mrfk from junction )) B WHERE A.MRID = B.CEQ_MRFK AND A.MRID IN (select MRID from conductingequipment where CHANGE_EQC_MRFK in \
			(select EQC_MRFK from dl where member_office_fk = %d));"), m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select A.MRID, A.NAME_TYPE_FK, A.NAME, A.ALIAS_NAME, B.CONNECTIVITYNODE_FK from identifiedobject A, (select * from terminal where ceq_mrfk in \
			 ( select ceq_mrfk from junction )) B WHERE A.MRID = B.CEQ_MRFK AND A.MRID IN (select MRID from conductingequipment where CHANGE_EQC_MRFK in \
			 ( select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d)));"), m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("select A.MRID, A.NAME_TYPE_FK, A.NAME, A.ALIAS_NAME, B.CONNECTIVITYNODE_FK from identifiedobject A, (select * from terminal where ceq_mrfk in \
							  (select ceq_mrfk from junction )) B WHERE A.MRID = B.CEQ_MRFK"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[노드 정보 - 2] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nType = GetInt_Row(row[1]);
		stNAME = GetCString_Row(row[2]);
		strData = GetCString_Row(row[3]);
		strCNID = GetCString_Row(row[4]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		if (m_map_IDNode.Lookup(strCNID, nCount_i))
		{
			pNode = m_pNodeArr.GetAt(nCount_i - 1);
			pNode->m_strName = stNAME;
			pNode->m_strNum = strData;
			pNode->m_nName_Type = nType;
			pNode->m_nKind = nType;
			pNode->m_strCEQID.Format(_T("%s"), strMRID);
			pNode->m_strJun_CEQID.Format(_T("%s"), strMRID);
			//MAP정보

			m_map_ND_CnCeq.SetAt(strCNID, strCEQ);
			m_map_ND_CeqCn.SetAt(strCEQ, strCNID);
		}
	}
	mysql_free_result(result);

	//14. 분산형전원 - 1
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][14/33]========== ADMStoKASIM 데이터 읽기 [분산형전원 정보 - 1] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	strADMS_SQL.Format(_T("SELECT DISTINCT CONNECT_CEQ_MRFK,ENERGYSOURCE_TYPE_FK FROM energysource where CONTRACT_STATUS = 1"));
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[분산형전원 정보 - 1] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nType = GetInt_Row(row[1]);
		m_map_ENG_EQC_TYPE.SetAt(strMRID, nType);
	}

	//15. 분산형전원 - 2
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][15/33]========== ADMStoKASIM 데이터 읽기 [분산형전원 정보 - 2] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select AEE.CEQ_MRFK, AEE.RESISTOR, AEE.REACTANCE, AIT.NAME_TYPE_FK, AIT.NAME, AIT.CONNECTIVITYNODE_FK, AEE.CONNECT_SWITCH_CEQ_MRFK , AEE.TRANSFORMER_CAPACITY, \
								 AEE.TR_GRIDSIDE_WINDINGCONNECTION_FK, AEE.TR_GENSIDE_WINDINGCONNECTION_FK  from energysource_equipment AEE, (SELECT AI.MRID, AI.NAME_TYPE_FK, AI.NAME, \
								 BT.CONNECTIVITYNODE_FK FROM identifiedobject AI , terminal BT WHERE AI.MRID = BT.CEQ_MRFK AND CEQ_MRFK IN (select CEQ_MRFK from energysource_equipment) \
								 AND CEQ_MRFK IN (select MRID from conductingequipment where CHANGE_EQC_MRFK in (select EQC_MRFK from dl where member_office_fk = %d))) AIT  \
									WHERE AEE.CEQ_MRFK = AIT.MRID and AIT.CONNECTIVITYNODE_FK is not null ;"), m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select AEE.CEQ_MRFK, AEE.RESISTOR, AEE.REACTANCE, AIT.NAME_TYPE_FK, AIT.NAME, AIT.CONNECTIVITYNODE_FK, AEE.CONNECT_SWITCH_CEQ_MRFK , AEE.TRANSFORMER_CAPACITY,\
								 AEE.TR_GRIDSIDE_WINDINGCONNECTION_FK, AEE.TR_GENSIDE_WINDINGCONNECTION_FK  from energysource_equipment AEE, (SELECT AI.MRID, AI.NAME_TYPE_FK, AI.NAME, \
								 BT.CONNECTIVITYNODE_FK FROM identifiedobject AI , terminal BT WHERE AI.MRID = BT.CEQ_MRFK AND CEQ_MRFK IN (select CEQ_MRFK from energysource_equipment) \
								 AND CEQ_MRFK IN (select MRID from conductingequipment where CHANGE_EQC_MRFK in ( select EQC_MRFK from dl where CEQ_MRFK \
								IN ( select CEQ_MRFK from dl where member_office_fk = %d)))) AIT  WHERE AEE.CEQ_MRFK = AIT.MRID and AIT.CONNECTIVITYNODE_FK is not null ;"), m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("select AEE.CEQ_MRFK, AEE.RESISTOR, AEE.REACTANCE, AIT.NAME_TYPE_FK, AIT.NAME, AIT.CONNECTIVITYNODE_FK, AEE.CONNECT_SWITCH_CEQ_MRFK , AEE.TRANSFORMER_CAPACITY, \
								AEE.TR_GRIDSIDE_WINDINGCONNECTION_FK, AEE.TR_GENSIDE_WINDINGCONNECTION_FK  from energysource_equipment AEE, \
								(SELECT AI.MRID, AI.NAME_TYPE_FK, AI.NAME, BT.CONNECTIVITYNODE_FK FROM identifiedobject AI , terminal BT WHERE AI.MRID = BT.CEQ_MRFK AND CEQ_MRFK \
								IN (select CEQ_MRFK from energysource_equipment)) AIT  WHERE AEE.CEQ_MRFK = AIT.MRID and AIT.CONNECTIVITYNODE_FK is not null;"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[분산형전원 정보 - 2] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		fGEN_ITR_R = GetDouble_Row(row[1]);
		fGEN_ITR_X = GetDouble_Row(row[2]);
		nType = GetInt_Row(row[3]);
		stNAME = GetCString_Row(row[4]);
		strCNID = GetCString_Row(row[5]);
		strData = GetCString_Row(row[6]);
		fGEN_ITR_CAP = GetDouble_Row(row[7]);
		nTR_GRIDSIDE = GetInt_Row(row[8]);
		nTR_GENSIDE = GetInt_Row(row[9]);

		int nCheck = 0;
		if (strCNID.IsEmpty())
		{
			m_map_NEW_GEN_NDisnull.SetAt(strMRID, 9999);
			continue;
		}
		//20221011 조성민 차장님 요청 사항 
		if (nTR_GRIDSIDE == 1 && nTR_GENSIDE == 3)
		{
			nGEN_ITR_CAP = 2;
		}
		else
		{
			nGEN_ITR_CAP = 1;
		}

		//TYPE 찾기 
		m_map_ENG_EQC_TYPE.Lookup(strMRID, nGEN_TYPE);

		//2021 0728 심재성 차장이 메일로 보내줌 
		if (nGEN_TYPE == 1) { nGEN_TYPE = 5; }
		else  if (nGEN_TYPE == 2) { nGEN_TYPE = 4; }
		else  if (nGEN_TYPE == 3) { nGEN_TYPE = 6; }
		else  if (nGEN_TYPE == 4) { nGEN_TYPE = 7; }
		else  if (nGEN_TYPE == 5) { nGEN_TYPE = 8; }
		else  if (nGEN_TYPE == 6) { nGEN_TYPE = 3; }
		else  if (nGEN_TYPE == 7) { nGEN_TYPE = 9; }
		else  if (nGEN_TYPE == 100) { nGEN_TYPE = 10; }
		else { nGEN_TYPE = 5; }

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		if (m_map_IDNode.Lookup(strCNID, nCount_i))
		{
			pNode = m_pNodeArr.GetAt(nCount_i - 1);
			pNode->m_strGENName = stNAME;
			pNode->m_nName_Type = nType;
			pNode->m_nKind = nType;
			pNode->m_genType = nGEN_TYPE;
			pNode->m_strCEQID.Format(_T("%s"), strMRID);
			pNode->m_GEN_UIN_fGEN_ITR_CAP = fGEN_ITR_CAP;
			//GEN 속성 정보
			pNode->m_GEN_UIN_fGEN_ITR_R = fGEN_ITR_R;
			pNode->m_GEN_UIN_fGEN_ITR_X = fGEN_ITR_X;
			pNode->szGEN_DG_TR_MRID = strMRID;
			pNode->m_GEN_UIN_nGEN_ITR_WDC = nGEN_ITR_CAP;
			//MAP 정보
			m_map_ND_CnCeq.SetAt(strCNID, strMRID);
			m_map_ND_CeqCn.SetAt(strMRID, strCNID);
			m_map_GEN_STA_NULL.SetAt(strMRID, 9999);
		}
	}
	//GEN -3
	int nCONNECTION_TR_TYPE = 0;
	int nPOWERCONTROL_FLAG_VALUES = 0;


	//16. 분산형전원 - 3
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][16/33]========== ADMStoKASIM 데이터 읽기 [분산형전원 정보 - 3] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select AA.ENERGYSOURCE_ID, AA.CUSTOMER_NAME, AA.CONTRACT_CAPACITY, AA.LOCATION_NO, AA.LOCATION_NAME,AA.CUSTOMER_NO,AA.ENERGYSOURCE_TYPE_FK,AA.CONNECT_CEQ_MRFK , \
								 BB.NAME_TYPE_FK , AA.CONNECTION_TR_TYPE  ,AA.POWERCONTROL_FLAG_VALUES from energysource AA INNER JOIN identifiedobject BB ON AA.CONNECT_CEQ_MRFK = BB.MRID \
								WHERE AA.CONTRACT_STATUS = 1 and AA.CONNECT_CEQ_MRFK in (select MRID from conductingequipment where CHANGE_EQC_MRFK in(select EQC_MRFK from dl where member_office_fk = %d)) \
								ORDER BY AA.CONNECT_CEQ_MRFK ;"), m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select AA.ENERGYSOURCE_ID, AA.CUSTOMER_NAME, AA.CONTRACT_CAPACITY, AA.LOCATION_NO, AA.LOCATION_NAME,AA.CUSTOMER_NO,AA.ENERGYSOURCE_TYPE_FK,AA.CONNECT_CEQ_MRFK , \
								BB.NAME_TYPE_FK , AA.CONNECTION_TR_TYPE ,AA.POWERCONTROL_FLAG_VALUES from energysource AA INNER JOIN identifiedobject BB ON AA.CONNECT_CEQ_MRFK = BB.MRID \
								WHERE AA.CONTRACT_STATUS = 1 and AA.CONNECT_CEQ_MRFK in (select MRID from conductingequipment where CHANGE_EQC_MRFK in( select EQC_MRFK from dl where CEQ_MRFK \
								IN( select CEQ_MRFK from dl where member_office_fk = %d))) ORDER BY AA.CONNECT_CEQ_MRFK ;"), m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("select AA.ENERGYSOURCE_ID, AA.CUSTOMER_NAME, AA.CONTRACT_CAPACITY, AA.LOCATION_NO, AA.LOCATION_NAME,AA.CUSTOMER_NO,AA.ENERGYSOURCE_TYPE_FK,AA.CONNECT_CEQ_MRFK , \
						BB.NAME_TYPE_FK , AA.CONNECTION_TR_TYPE  ,AA.POWERCONTROL_FLAG_VALUES from energysource AA INNER JOIN identifiedobject BB ON AA.CONNECT_CEQ_MRFK = BB.MRID \
						WHERE AA.CONTRACT_STATUS = 1  ORDER BY AA.CONNECT_CEQ_MRFK ;"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[분산형전원 정보 - 3] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		szGENUNIT_CEQ = GetCString_Row(row[0]);
		szGENUNIT_CUS_NM = GetCString_Row(row[1]);
		fGENUNIT_CAP_KW = GetDouble_Row(row[2]);
		szGENUNIT_LOCA_NO = GetCString_Row(row[3]);
		szGENUNIT_LOCA_NM = GetCString_Row(row[4]);
		szGENUNIT_CUS_NO = GetCString_Row(row[5]);
		nGENUNIT_TYPE = GetInt_Row(row[6]);
		szGENUNIT_II_EQU_ID = GetCString_Row(row[7]);
		nNAME_TYPE_FK = GetInt_Row(row[8]);
		nCONNECTION_TR_TYPE = GetInt_Row(row[9]);
		nPOWERCONTROL_FLAG_VALUES = GetInt_Row(row[10]);


		if (nNAME_TYPE_FK == 51)
		{
			int a = 0;
		}
		nCheck = 0;
		m_map_NEW_GEN_NDisnull.Lookup(szGENUNIT_II_EQU_ID, nCheck);
		if (nCheck == 9999)
		{
			continue;
		}

		nn1 = 0;
		nn1 = szGENUNIT_CUS_NM.Find(',');
		if (nn1 > -1)
		{
			szGENUNIT_CUS_NM.Replace(_T(","), _T("_"));
		}
		nn1 = 0;
		nn1 = szGENUNIT_LOCA_NM.Find(',');
		if (nn1 > -1)
		{
			szGENUNIT_LOCA_NM.Replace(_T(","), _T("_"));
		}
		GENUNIT_STA stGENUNIT;
		memset(&stGENUNIT, 0, sizeof(GENUNIT_STA));
		_stprintf_s(stGENUNIT.GENUNIT_CEQID, L"%s", szGENUNIT_CEQ);
		_stprintf_s(stGENUNIT.GENUNIT_NM, L"%s", szGENUNIT_CUS_NM);
		_stprintf_s(stGENUNIT.GENUNIT_CUSTOMER_NO, L"%s", szGENUNIT_CUS_NO);
		_stprintf_s(stGENUNIT.GENUNIT_LOCATION_NM, L"%s", szGENUNIT_LOCA_NM);
		_stprintf_s(stGENUNIT.GENUNIT_LOCATION_NO, L"%s", szGENUNIT_LOCA_NO);
		_stprintf_s(stGENUNIT.GENUNIT_II_EQU_ID, L"%s", szGENUNIT_II_EQU_ID);
		stGENUNIT.GENUNIT_CAP_KW = fGENUNIT_CAP_KW;
		stGENUNIT.GENUNIT_TYPE = nGENUNIT_TYPE;
		stGENUNIT.GENUNIT_II_GEN = 0;
		stGENUNIT.GENUNIT_II_NAME_TYPE_FK = nNAME_TYPE_FK;
		stGENUNIT.GENUNIT_CONNECTION_TR_TYPE = nCONNECTION_TR_TYPE;
		//비트 연산 하는 부분 
		stGENUNIT.GENUNIT_LVRTXGROUP_FLAG = Get_POWERCONTROL_FLAG_VALUES(1, nPOWERCONTROL_FLAG_VALUES);
		stGENUNIT.GENUNIT_PUBXGROUP_FLAG = Get_POWERCONTROL_FLAG_VALUES(2, nPOWERCONTROL_FLAG_VALUES);
		stGENUNIT.GENUNIT_PUBOGROUP_FLAG = Get_POWERCONTROL_FLAG_VALUES(3, nPOWERCONTROL_FLAG_VALUES);
		stGENUNIT.GENUNIT_CACGROUP_FLAG = Get_POWERCONTROL_FLAG_VALUES(4, nPOWERCONTROL_FLAG_VALUES);
		stGENUNIT.GENUNIT_MANOGROUP_FLAG = Get_POWERCONTROL_FLAG_VALUES(5, nPOWERCONTROL_FLAG_VALUES);
		stGENUNIT.GENUNIT_MANXGROUP_FLAG = Get_POWERCONTROL_FLAG_VALUES(6, nPOWERCONTROL_FLAG_VALUES);

		m_arrGENUNIT.Add(stGENUNIT);
	}
	mysql_free_result(result);

	//17. MTR_STA
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][17/33]========== ADMStoKASIM 데이터 읽기 [변압기 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		strADMS_SQL.Format(_T("SELECT A.CEQ_MRFK, A.NAME, B.SUBS_MRFK, B.HV_LIMIT, (SELECT HV_LIMIT FROM  voltagelevel WHERE A.SECONDARY_VOLTAGELEVEL_FK = EQC_MRFK) \
				 AS HV_LIMIT, A.NAME_TYPE_FK, A.PRIMARY_VOLTAGELEVEL_FK, A.SECONDARY_VOLTAGELEVEL_FK , A.BANK_NO FROM (SELECT A.CEQ_MRFK, A.PRIMARY_VOLTAGELEVEL_FK, A.SECONDARY_VOLTAGELEVEL_FK, \
				 B.NAME_TYPE_FK, B.NAME , A.BANK_NO FROM powertransformer A LEFT JOIN identifiedobject B ON A.CEQ_MRFK = B.MRID) A , (SELECT A.CEQ_MRFK, B.EQC_MRFK, B.SUBS_MRFK, B.HV_LIMIT, \
				 B.LV_LIMIT FROM powertransformer A LEFT JOIN voltagelevel B ON A.PRIMARY_VOLTAGELEVEL_FK = B.EQC_MRFK) B where A.CEQ_MRFK = B.CEQ_MRFK and A.NAME_TYPE_FK != 26 AND B.SUBS_MRFK IN \
				 (select distinct subs_mrfk from voltagelevel where eqc_mrfk in ( select distinct primary_voltagelevel_fk from powertransformer where ceq_mrfk in \
				 (select distinct mtr_mrfk   from dl where member_office_fk = %d)))"), m_nType);
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT A.CEQ_MRFK, A.NAME, B.SUBS_MRFK, B.HV_LIMIT, (SELECT HV_LIMIT FROM  voltagelevel WHERE A.SECONDARY_VOLTAGELEVEL_FK = EQC_MRFK) \
				 AS HV_LIMIT , A.NAME_TYPE_FK, A.PRIMARY_VOLTAGELEVEL_FK, A.SECONDARY_VOLTAGELEVEL_FK , A.BANK_NO FROM (SELECT A.CEQ_MRFK, A.PRIMARY_VOLTAGELEVEL_FK, A.SECONDARY_VOLTAGELEVEL_FK, \
				 B.NAME_TYPE_FK, B.NAME, A.BANK_NO FROM powertransformer A LEFT JOIN identifiedobject B ON A.CEQ_MRFK = B.MRID) A , (SELECT A.CEQ_MRFK, B.EQC_MRFK, B.SUBS_MRFK, B.HV_LIMIT,\
				 B.LV_LIMIT FROM powertransformer A LEFT JOIN voltagelevel B ON A.PRIMARY_VOLTAGELEVEL_FK = B.EQC_MRFK) B where A.CEQ_MRFK = B.CEQ_MRFK and A.NAME_TYPE_FK != 26;"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[변압기 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		stNAME = GetCString_Row(row[1]);
		stSUBS_MRFK = GetCString_Row(row[2]);
		fHV = GetDouble_Row(row[3]);
		fLV = GetDouble_Row(row[4]);
		nNAME_TYPE_FK = GetInt_Row(row[5]);
		stPRIMARY_VOLTAGELEVEL = GetCString_Row(row[6]);
		stSECONDARY_VOLTAGELEVEL = GetCString_Row(row[7]);
		nMTR_BANK = GetInt_Row(row[8]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		MTR_STA stMTR;
		memset(&stMTR, 0, sizeof(MTR_STA));
		_stprintf_s(stMTR.mtr_nm, L"%s", stNAME);
		_stprintf_s(stMTR.mtr_maintrid, L"%s", strMRID);
		_stprintf_s(stMTR.stMTR_PRIMARY_VOLTAGELEVEL, L"%s", stPRIMARY_VOLTAGELEVEL);
		_stprintf_s(stMTR.stMTR_SECONDARY_VOLTAGELEVEL, L"%s", stSECONDARY_VOLTAGELEVEL);

		stMTR.mtr_hi_tr = m_arrTR.GetSize() + 1;
		stMTR.mtr_bank = nMTR_BANK;
		stMTR.mtr_ii_ss = GetMTR_II_SS(stSUBS_MRFK);
		m_arrMTR.Add(stMTR);
		//MAP정보
		m_map_MTR_MTRVL.SetAt(strMRID, stSECONDARY_VOLTAGELEVEL);

		//TR
		TR_STA stTR;
		memset(&stTR, 0, sizeof(TR_STA));
		_stprintf_s(stTR.tr_nm, L"%s", stNAME);
		_stprintf_s(stTR.tr_ceqid, L"%s", strMRID);
		stTR.tr_type = 1;											 //MTR 은1번입니다.
		stTR.tr_ii_ss = stMTR.mtr_ii_ss;
		stTR.tr_ii_mtr = m_arrMTR.GetSize();
		if (nNAME_TYPE_FK == 17) //MTR
		{
			stTR.tr_fnorkv = fHV / 1000;
			stTR.tr_posx = stTR.tr_zerx = 30;								//실제값으로 입력해야함.
			stTR.tr_patapmx = stTR.tr_pbtapmx = stTR.tr_pctapmx = 21;
			stTR.tr_patapmn = stTR.tr_pbtapmn = stTR.tr_pctapmn = 1;
			stTR.tr_patapnor = stTR.tr_pbtapnor = stTR.tr_pctapnor = 11;
		}
		else						//SVR
		{
			stTR.tr_fnorkv = fLV / 1000;
			stTR.tr_posx = stTR.tr_zerx = 3.65;							//실제값으로 입력해야함.
			stTR.tr_patapmx = stTR.tr_pbtapmx = stTR.tr_pctapmx = 32;
			stTR.tr_patapmn = stTR.tr_pbtapmn = stTR.tr_pctapmn = 1;
			stTR.tr_patapnor = stTR.tr_pbtapnor = stTR.tr_pctapnor = 15;
		}
		stTR.tr_patapstep = stTR.tr_pbtapstep = stTR.tr_pctapstep = -0.0125;
		stTR.tr_tnorkv = fLV / 1000;
		stTR.tr_conty = 2;
		stTR.tr_ii_br = (int)m_arrBR.GetSize() + 1;
		stTR.TR_LDCTYPE = 0; //LDC 타입 (1: 유성 OLTC, 2 : MR OLTC,	3 : SVR
		stTR.TR_DVMMXV = 112;
		stTR.TR_DVMMNV = 109;
		m_arrTR.Add(stTR);

		//BR
		BR_STA stBr;
		memset(&stBr, 0, sizeof(BR_STA));
		_stprintf_s(stBr.br_nm, L"%s", stNAME);
		_stprintf_s(stBr.br_ceq, L"%s", strMRID);
		stBr.br_posr = 0;
		stBr.br_posx = 30;
		stBr.br_zerr = 0;
		stBr.br_zerx = 0;
		stBr.br_norlm = 100;
		stBr.br_ii_equ = (int)m_arrTR.GetSize();
		stBr.br_ii_gbr = (int)m_arrGBR.GetSize() + 1;
		//stBr.br_ii_fnd = GetTR_II_FND(strMRID);						 //20231207 삭제예정2023
		//stBr.br_ii_tnd = GetTR_II_TND(strMRID, stBr.br_ii_fnd);		 //20231207 삭제예정2023	
		stBr.br_ii_equty = 2;
		m_arrBR.Add(stBr);
		//m_map_BR_MTRIDTND.SetAt(stBr.br_ii_equ, stBr.br_ii_tnd);
		//GBR 생성
		GBR_STA stGbr;
		memset(&stGbr, 0, sizeof(GBR_STA));
		_stprintf_s(stGbr.gbr_nm, L"%s", stNAME);
		stGbr.gbr_ii_equ = (int)m_arrTR.GetSize();
		stGbr.gbr_ii_fgnd = stBr.br_ii_fnd;
		stGbr.gbr_ii_tgnd = stBr.br_ii_tnd;
		stGbr.gbr_posr = 0;
		stGbr.gbr_posx = 30;
		stGbr.gbr_zerr = 0;
		stGbr.gbr_zerx = 0;
		stGbr.gbr_hi_br = (int)m_arrBR.GetSize();
		stGbr.gbr_ii_equty = 2;
		m_arrGBR.Add(stGbr);
	}
	mysql_free_result(result);

	//18. DL
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][18/33]========== ADMStoKASIM 데이터 읽기 [DL 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();
	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("SELECT A.EQC_MRFK, A.MTR_MRFK, A.CEQ_MRFK, A.NAME, B.PRIMARY_VOLTAGELEVEL_FK, B.SECONDARY_VOLTAGELEVEL_FK, B.SUBS_MRFK , A.NAME_TYPE_FK ,A.member_office_fk FROM \
								(SELECT A.EQC_MRFK, A.MTR_MRFK, A.CEQ_MRFK, A.MEMBER_OFFICE_FK, B.NAME, B.NAME_TYPE_FK FROM DL A, identifiedobject B WHERE A.EQC_MRFK = B.MRID  ) A, \
								(SELECT A.CEQ_MRFK, A.PRIMARY_VOLTAGELEVEL_FK, A.SECONDARY_VOLTAGELEVEL_FK, B.SUBS_MRFK  FROM powertransformer A, voltagelevel B WHERE A.PRIMARY_VOLTAGELEVEL_FK = B.EQC_MRFK) \
								 B WHERE A.MTR_MRFK = B.CEQ_MRFK AND A.member_office_fk = %d;"), m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("SELECT A.EQC_MRFK, A.MTR_MRFK, A.CEQ_MRFK, A.NAME, B.PRIMARY_VOLTAGELEVEL_FK, B.SECONDARY_VOLTAGELEVEL_FK, B.SUBS_MRFK , A.NAME_TYPE_FK ,A.member_office_fk FROM \
								(SELECT A.EQC_MRFK, A.MTR_MRFK, A.CEQ_MRFK, A.MEMBER_OFFICE_FK, B.NAME, B.NAME_TYPE_FK FROM DL A, identifiedobject B WHERE A.EQC_MRFK = B.MRID  ) A, \
								(SELECT A.CEQ_MRFK, A.PRIMARY_VOLTAGELEVEL_FK, A.SECONDARY_VOLTAGELEVEL_FK, B.SUBS_MRFK  FROM powertransformer A, voltagelevel B WHERE A.PRIMARY_VOLTAGELEVEL_FK = B.EQC_MRFK) \
								 B WHERE A.MTR_MRFK = B.CEQ_MRFK AND A.EQC_MRFK in (  select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d));"), m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT A.EQC_MRFK, A.MTR_MRFK, A.CEQ_MRFK, A.NAME, B.PRIMARY_VOLTAGELEVEL_FK, B.SECONDARY_VOLTAGELEVEL_FK, B.SUBS_MRFK , A.NAME_TYPE_FK ,A.member_office_fk \
									 FROM (SELECT A.EQC_MRFK, A.MTR_MRFK, A.CEQ_MRFK, A.MEMBER_OFFICE_FK, B.NAME, B.NAME_TYPE_FK FROM DL A, identifiedobject B WHERE A.EQC_MRFK = B.MRID  ) A, \
									(SELECT A.CEQ_MRFK, A.PRIMARY_VOLTAGELEVEL_FK, A.SECONDARY_VOLTAGELEVEL_FK, B.SUBS_MRFK  FROM powertransformer A, voltagelevel B WHERE A.PRIMARY_VOLTAGELEVEL_FK = B.EQC_MRFK) B \
									 WHERE A.MTR_MRFK = B.CEQ_MRFK;"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[DL 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		stEQC_MRFK = GetCString_Row(row[0]);
		stMTR_MRFK = GetCString_Row(row[1]);
		strMRID = GetCString_Row(row[2]);
		stNAME = GetCString_Row(row[3]);
		stPRIMARY_VOLTAGELEVEL_FK = GetCString_Row(row[4]);
		stSECONDARY_VOLTAGELEVEL_FK = GetCString_Row(row[5]);
		stSUBS_MRFK = GetCString_Row(row[6]);
		nType = GetInt_Row(row[7]);
		nMember_office_fk = GetInt_Row(row[8]);


		//20230414
		if (m_nDL_TEST != 0) //공용선로만 해주는 작업입니다.
		{
			strData2 = GetEQC_MRFK(stEQC_MRFK);
		}
		if (stEQC_MRFK != strData2)
		{
			continue;
		}
		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		pBranch = new CBranch();
		pBranch->m_strMRID.Format(_T("%s"), strMRID);
		pBranch->m_nKind = nType;
		pBranch->m_strName.Format(_T("%s"), stNAME);
		pBranch->m_strNum.Format(_T("%s"), strData);
		pBranch->m_nKCIM_Type = 4; // 1.SW 2.ACL 3.입상주 4.DL
		m_pBranchArr.Add(pBranch);

		DL_STA stDl;
		memset(&stDl, 0, sizeof(DL_STA));
		_stprintf_s(stDl.dl_nm, L"%s", stNAME);
		_stprintf_s(stDl.dl_distributionlineid, L"%s", stEQC_MRFK);
		stDl.dl_ii_olm = 1;
		stDl.dl_ii_mtr = GetDL_II_MTR(stMTR_MRFK);
		_stprintf_s(stDl.stDL_MTRFK, L"%s", stMTR_MRFK);
		_stprintf_s(stDl.stDL_CEQFK, L"%s", strMRID);
		_stprintf_s(stDl.stDL_PVLFK, L"%s", stPRIMARY_VOLTAGELEVEL_FK);
		_stprintf_s(stDl.stDL_SVLFK, L"%s", stSECONDARY_VOLTAGELEVEL_FK);
		_stprintf_s(stDl.stDL_SUBFK, L"%s", stSUBS_MRFK);
		stDl.dl_ii_bof = GetDL_II_BOF(nMember_office_fk);
		if (nMember_office_fk != m_nType && m_nType != 0) // 이부분은 공용선로 작업할때 bof 를 못찾는 경우가 있어서 작업 하는 부분입니다.! member 로 변환할때는 bof 는 무조건 1이기때문에 1로 입력!
		{
			stDl.dl_ii_bof = 1;
		}
		m_arrDL.Add(stDl);
		nDLID++;
		//MAP 정보

		m_map_DL_DLMTR.SetAt(strMRID, stMTR_MRFK);
		m_map_DL_ECRMTR.SetAt(stEQC_MRFK, stMTR_MRFK);
		m_map_DL_DLSUB.SetAt(strMRID, stSUBS_MRFK);
		m_map_IDBranch.SetAt(strMRID, m_pBranchArr.GetSize());
		m_map_DL_CEQID.SetAt(strMRID, nDLID);
		m_map_DL_EQCID.SetAt(stEQC_MRFK, nDLID);
		m_map_DL_MTRID.SetAt(nDLID, stDl.dl_ii_mtr);
		m_map_DL_DLID_SUBS.SetAt(nDLID, stSUBS_MRFK);
		m_map_DL_DLID_MTR.SetAt(nDLID, stMTR_MRFK);
	}
	mysql_free_result(result);


	//19. Switch - 1
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][19/33]========== ADMStoKASIM 데이터 읽기 [Switch 정보 - 1] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select aa.*,cc.CEQ_TYPE_FK, bb.COMPOSITE_SWITCH_MRFK ,cc.seq from identifiedobject aa, switch bb, conductingequipment cc where aa.MRID = bb.CEQ_MRFK and \
							 bb.CEQ_MRFK=cc.MRID and aa.NAME_TYPE_FK != 15 and cc.CEQ_TYPE_FK != 73 AND CC.MRID IN ( select MRID from conductingequipment where CHANGE_EQC_MRFK \
							 in (select EQC_MRFK from dl where member_office_fk = %d) OR MRID IN (select CEQ_MRFK from dl where member_office_fk = %d)); "), m_nType, m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select aa.*,cc.CEQ_TYPE_FK, bb.COMPOSITE_SWITCH_MRFK ,cc.seq from identifiedobject aa, switch bb, conductingequipment cc where aa.MRID = bb.CEQ_MRFK and \
							bb.CEQ_MRFK=cc.MRID and aa.NAME_TYPE_FK != 15 and cc.CEQ_TYPE_FK != 73 AND CC.MRID IN ( select MRID from conductingequipment where CHANGE_EQC_MRFK\
							 in ( select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d)) OR MRID \
							 IN (select CEQ_MRFK from dl where member_office_fk = %d)); "), m_nType, m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("select aa.*,cc.CEQ_TYPE_FK, bb.COMPOSITE_SWITCH_MRFK ,cc.seq from identifiedobject aa, switch bb, conductingequipment cc where \
								aa.MRID = bb.CEQ_MRFK and bb.CEQ_MRFK=cc.MRID and aa.NAME_TYPE_FK != 15 and cc.CEQ_TYPE_FK != 73;"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[Switch 정보 - 1] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nType = GetInt_Row(row[1]);
		stNAME = GetCString_Row(row[2]);
		strData = GetCString_Row(row[3]);
		strCEQ = GetCString_Row(row[4]);
		szMULTISW = GetCString_Row(row[5]);
		nMULTNUMBER = GetInt_Row(row[6]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭



		pBranch = new CBranch();
		pBranch->m_strMRID.Format(_T("%s"), strMRID);
		pBranch->m_nKind = nType;
		pBranch->m_strName.Format(_T("%s"), stNAME);
		pBranch->m_strNum.Format(_T("%s"), strData);
		pBranch->m_nCeq_Type = _wtoi(strCEQ);
		pBranch->m_nKCIM_Type = 1; // 1.SW 2.ACL 3.입상주 4.DL
		if (szMULTISW.IsEmpty())
		{
			szMULTISW.Format(_T("0"));
			pBranch->m_szMULTISW.Format(_T("%s"), szMULTISW);
			pBranch->m_nMULTICIR_NUMBER = 0;
		}
		else
		{
			pBranch->m_szMULTISW.Format(_T("%s"), szMULTISW);
			pBranch->m_nMULTICIR_NUMBER = nMULTNUMBER;
		}
		m_pBranchArr.Add(pBranch);
		//MAP 정보
		m_map_IDBranch.SetAt(strMRID, m_pBranchArr.GetSize());
	}
	mysql_free_result(result);


	//20. Switch - 2
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][20/33]========== ADMStoKASIM 데이터 읽기 [Switch 정보 - 2] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	//20230504 
	CString szMRID_old;
	szMRID_old.Format(_T(""));

	if (m_nType != 0 && m_nDL_TEST != 0)
	{
		strADMS_SQL.Format(_T("SELECT  A.CEQ_MRFK, A.CONNECTIVITYNODE_FK FROM terminal A, (SELECT A.MRID, A.CEQ_TYPE_FK, A.ORIGINAL_EQC_MRFK, A.CHANGE_EQC_MRFK, B.NAME, B.NAME_TYPE_FK \
					FROM conductingequipment A, identifiedobject B WHERE A.MRID = B.MRID) B WHERE A.CEQ_MRFK = B.MRID and b.NAME_TYPE_FK != 14 and b.NAME_TYPE_FK != 15 and b.NAME_TYPE_FK != 16 \
					and b.NAME_TYPE_FK != 17 and b.NAME_TYPE_FK != 28 and b.NAME_TYPE_FK != 29 and b.NAME_TYPE_FK != 37 and b.NAME_TYPE_FK != 51 order by A.CEQ_MRFK; "));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[Switch 정보 - 2] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		stTerCNID = GetCString_Row(row[1]);

		if (szMRID_old != strMRID)
		{
			m_map_Common_CB_FND.SetAt(strMRID, stTerCNID);
			szMRID_old.Format(_T("%s"), strMRID);
		}
		else
		{
			m_map_Common_CB_TND.SetAt(strMRID, stTerCNID);
		}
	}
	mysql_free_result(result);



	//21. Switch - 3
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][21/33]========== ADMStoKASIM 데이터 읽기 [Switch 정보 - 3] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	CString szCommon_CFTND;
	CString szCommon_CNID;
	CString szCommon_FCEQID, szCommon_TCEQID;
	CString szCommon_CFND, szCommon_CTND;
	CString szALIAS_NAME;
	if (m_nType != 0 && m_nDL_TEST != 0)
	{
		strADMS_SQL.Format(_T("select aa.mrid, aa.NAME_TYPE_FK, aa.NAME ,aa.ALIAS_NAME  ,cc.CEQ_TYPE_FK, bb.COMPOSITE_SWITCH_MRFK ,cc.seq from identifiedobject aa, switch bb, conductingequipment cc \
								where aa.MRID = bb.CEQ_MRFK and bb.CEQ_MRFK=cc.MRID and aa.NAME_TYPE_FK != 14 AND aa.NAME_TYPE_FK != 15 and cc.CEQ_TYPE_FK != 73;"));

	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[Switch 정보 - 3] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nType = GetInt_Row(row[1]);
		stNAME = GetCString_Row(row[2]);
		szALIAS_NAME = GetCString_Row(row[3]);
		strCEQ = GetCString_Row(row[4]);
		szMULTISW = GetCString_Row(row[5]);
		nMULTNUMBER = GetInt_Row(row[6]);

		szCommon_CNID.Format(_T(""));
		szCommon_CFND.Format(_T(""));
		szCommon_CTND.Format(_T(""));
		szCommon_FCEQID.Format(_T(""));
		szCommon_TCEQID.Format(_T(""));

		if ("11212200005095" == strMRID)
		{
			int a = 0;
		}

		m_map_Common_CB_Check.Lookup(strMRID, szCommon_CNID); //20230504 
		if (!(szCommon_CNID.IsEmpty())) //설비가 있으면 넘어간다?
		{
			continue;
		}

		m_map_Common_CB_FND.Lookup(strMRID, szCommon_CFND);
		m_map_Common_CB_TND.Lookup(strMRID, szCommon_CTND);

		m_map_Common_ND_CnCeq.Lookup(szCommon_CFND, szCommon_FCEQID);
		m_map_Common_ND_CnCeq.Lookup(szCommon_CTND, szCommon_TCEQID);

		if (szCommon_FCEQID.IsEmpty() && !(szCommon_TCEQID.IsEmpty()))
		{


			nn1 = 0;
			nn1 = stNAME.Find(',');
			if (nn1 > -1)
			{
				stNAME.Replace(_T(","), _T("_"));
			}
			stNAME.Remove('\r');	//개행 엔터
			stNAME.Remove('\n');	//개행 엔터	
			stNAME.Remove('\t');	//탭

			pNode = new CNode();
			pNode->m_nKind = 0;
			pNode->m_strMRID.Format(_T("%s"), szCommon_CFND);
			pNode->m_strName.Format(_T("%s"), stNAME);
			pNode->m_strCEQID.Format(_T("%s"), strMRID);
			pNode->m_nName_Type = nType;
			m_pNodeArr.Add(pNode);
			//MAP정보
			m_map_Common_ND_CnCeq.SetAt(szCommon_CFND, strMRID);
			m_map_IDNode.SetAt(szCommon_CFND, m_pNodeArr.GetSize());

			pBranch = new CBranch();
			pBranch->m_strMRID.Format(_T("%s"), strMRID);
			pBranch->m_nKind = nType;
			pBranch->m_strName.Format(_T("%s"), stNAME);
			pBranch->m_strNum.Format(_T("%s"), szALIAS_NAME);
			pBranch->m_nCeq_Type = _wtoi(strCEQ);
			pBranch->m_nKCIM_Type = 1; // 1.SW 2.ACL 3.입상주 4.DL
			if (szMULTISW.IsEmpty())
			{
				szMULTISW.Format(_T("0"));
				pBranch->m_szMULTISW.Format(_T("%s"), szMULTISW);
				pBranch->m_nMULTICIR_NUMBER = 0;
			}
			else
			{
				pBranch->m_szMULTISW.Format(_T("%s"), szMULTISW);
				pBranch->m_nMULTICIR_NUMBER = nMULTNUMBER;
			}
			m_pBranchArr.Add(pBranch);
			m_map_Common_CBSW_MUSW.SetAt(strMRID, m_pBranchArr.GetSize());
			//MAP 정보
			m_map_IDBranch.SetAt(strMRID, m_pBranchArr.GetSize());
			if (!szMULTISW.IsEmpty() && nMULTNUMBER > 1)
			{
				GET_MULTISW_Check_KASIM(strMRID, nType, stNAME, szALIAS_NAME, strCEQ, szMULTISW, nMULTNUMBER);
			}
		}
		if (!(szCommon_FCEQID.IsEmpty()) && szCommon_TCEQID.IsEmpty())
		{
			nn1 = 0;
			nn1 = stNAME.Find(',');
			if (nn1 > -1)
			{
				stNAME.Replace(_T(","), _T("_"));
			}
			stNAME.Remove('\r');	//개행 엔터
			stNAME.Remove('\n');	//개행 엔터	
			stNAME.Remove('\t');	//탭

			pNode = new CNode();
			pNode->m_nKind = 0;
			pNode->m_strMRID.Format(_T("%s"), szCommon_CTND);
			pNode->m_strName.Format(_T("%s"), stNAME);
			pNode->m_strCEQID.Format(_T("%s"), strMRID);
			pNode->m_nName_Type = nType;
			m_pNodeArr.Add(pNode);
			//MAP정보
			m_map_Common_ND_CnCeq.SetAt(szCommon_CTND, strMRID);
			m_map_IDNode.SetAt(szCommon_CTND, m_pNodeArr.GetSize());

			pBranch = new CBranch();
			pBranch->m_strMRID.Format(_T("%s"), strMRID);
			pBranch->m_nKind = nType;
			pBranch->m_strName.Format(_T("%s"), stNAME);
			pBranch->m_strNum.Format(_T("%s"), szALIAS_NAME);
			pBranch->m_nCeq_Type = _wtoi(strCEQ);
			pBranch->m_nKCIM_Type = 1; // 1.SW 2.ACL 3.입상주 4.DL
			if (szMULTISW.IsEmpty())
			{
				szMULTISW.Format(_T("0"));
				pBranch->m_szMULTISW.Format(_T("%s"), szMULTISW);
				pBranch->m_nMULTICIR_NUMBER = 0;
			}
			else
			{
				pBranch->m_szMULTISW.Format(_T("%s"), szMULTISW);
				pBranch->m_nMULTICIR_NUMBER = nMULTNUMBER;
			}
			m_pBranchArr.Add(pBranch);
			m_map_Common_CBSW_MUSW.SetAt(strMRID, m_pBranchArr.GetSize());
			//MAP 정보
			m_map_IDBranch.SetAt(strMRID, m_pBranchArr.GetSize());
			if (!szMULTISW.IsEmpty() && nMULTNUMBER > 1)
			{
				GET_MULTISW_Check_KASIM(strMRID, nType, stNAME, szALIAS_NAME, strCEQ, szMULTISW, nMULTNUMBER);
			}
		}
	}
	mysql_free_result(result);


	//22. SVR 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][22/33]========== ADMStoKASIM 데이터 읽기 [SVR 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select aa.*,cc.CEQ_TYPE_FK, cc.seq from identifiedobject aa, conductingequipment cc where aa.MRID = cc.MRID and cc.CEQ_TYPE_FK = 81 AND CC.MRID \
								IN (  select MRID from conductingequipment where CHANGE_EQC_MRFK in (select EQC_MRFK from dl where member_office_fk = %d) OR MRID \
								IN (select CEQ_MRFK from dl where member_office_fk = %d));"), m_nType, m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select aa.*,cc.CEQ_TYPE_FK, cc.seq from identifiedobject aa, conductingequipment cc where aa.MRID = cc.MRID and cc.CEQ_TYPE_FK = 81 AND CC.MRID \
								IN (  select MRID from conductingequipment where CHANGE_EQC_MRFK in ( select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d)) \
								OR MRID IN (select CEQ_MRFK from dl where member_office_fk = %d));"), m_nType, m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("select aa.*,cc.CEQ_TYPE_FK, cc.seq from identifiedobject aa, conductingequipment cc where aa.MRID = cc.MRID and cc.CEQ_TYPE_FK = 81;"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[SVR 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nType = GetInt_Row(row[1]);
		stNAME = GetCString_Row(row[2]);
		strData = GetCString_Row(row[3]);
		strCEQ = GetCString_Row(row[4]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		pBranch = new CBranch();
		pBranch->m_strMRID.Format(_T("%s"), strMRID);
		pBranch->m_nKind = nType;
		pBranch->m_strName.Format(_T("%s"), stNAME);
		pBranch->m_strNum.Format(_T("%s"), strData);
		pBranch->m_nCeq_Type = _wtoi(strCEQ);
		pBranch->m_nKCIM_Type = 1; // 1.SW 2.ACL 3.입상주 4.DL
		pBranch->m_szMULTISW.Format(_T("0"));
		pBranch->m_nMULTICIR_NUMBER = 0;
		m_pBranchArr.Add(pBranch);
		//MAP 정보
		m_map_IDBranch.SetAt(strMRID, m_pBranchArr.GetSize());
	}
	mysql_free_result(result);

	//23. SVR 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][23/33]========== ADMStoKASIM 데이터 읽기 [identifiedobject 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select * from identifiedobject where name_type_fk = 30 AND MRID IN ( select MRID from conductingequipment where CHANGE_EQC_MRFK \
								 in (select EQC_MRFK from dl where member_office_fk = %d))"), m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select * from identifiedobject where name_type_fk = 30 AND MRID IN ( select MRID from conductingequipment where CHANGE_EQC_MRFK \
								in ( select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d)))"), m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("select * from identifiedobject where name_type_fk = 30"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[identifiedobject 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nType = GetInt_Row(row[1]);
		stNAME = GetCString_Row(row[2]);
		strData = GetCString_Row(row[3]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		pBranch = new CBranch();
		pBranch->m_strMRID.Format(_T("%s"), strMRID);
		pBranch->m_nKind = nType;
		pBranch->m_strName.Format(_T("%s"), stNAME);
		pBranch->m_strNum.Format(_T("%s"), strData);
		pBranch->m_nKCIM_Type = 3; // 1.SW 2.ACL 3.입상주 4.DL
		m_pBranchArr.Add(pBranch);
		//MAP정보
		m_map_IDBranch.SetAt(strMRID, m_pBranchArr.GetSize());
	}
	mysql_free_result(result);

	//24 고객정보 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][24/33]========== ADMStoKASIM 데이터 읽기 [고객 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("SELECT AA.CEQ_MRFK,AA.PROMISE_LOAD, AA.NAME, AA.NAME_TYPE_FK, BB.connectivitynode_fk FROM ( select A.CEQ_MRFK,A.PROMISE_LOAD, B.NAME, B.NAME_TYPE_FK from energyconsumer A,\
							 identifiedobject B WHERE A.CEQ_MRFK = B.MRID) AA, (SELECT CEQ_MRFK, connectivitynode_fk FROM TERMINAL WHERE CEQ_MRFK IN ( select CEQ_MRFK from energyconsumer) AND CEQ_MRFK \
							 IN ( select MRID from conductingequipment where CHANGE_EQC_MRFK in (select EQC_MRFK from dl where member_office_fk = %d)) AND connectivitynode_fk is not null) BB \
							 WHERE AA.CEQ_MRFK = BB.CEQ_MRFK;"), m_nType); //임시!
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("SELECT AA.CEQ_MRFK,AA.PROMISE_LOAD, AA.NAME, AA.NAME_TYPE_FK, BB.connectivitynode_fk FROM ( select A.CEQ_MRFK,A.PROMISE_LOAD, B.NAME, B.NAME_TYPE_FK from energyconsumer A,\
							identifiedobject B WHERE A.CEQ_MRFK = B.MRID) AA, (SELECT CEQ_MRFK, connectivitynode_fk FROM TERMINAL WHERE CEQ_MRFK IN ( select CEQ_MRFK from energyconsumer) AND CEQ_MRFK \
							IN ( select MRID from conductingequipment where CHANGE_EQC_MRFK in ( select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d))) \
							AND connectivitynode_fk is not null) BB WHERE AA.CEQ_MRFK = BB.CEQ_MRFK;"), m_nType); //임시!
		}
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT AA.CEQ_MRFK,AA.PROMISE_LOAD, AA.NAME, AA.NAME_TYPE_FK, BB.connectivitynode_fk FROM ( select A.CEQ_MRFK,A.PROMISE_LOAD, B.NAME, B.NAME_TYPE_FK from energyconsumer A, \
							identifiedobject B WHERE A.CEQ_MRFK = B.MRID) AA, (SELECT CEQ_MRFK, connectivitynode_fk FROM TERMINAL WHERE CEQ_MRFK IN ( select CEQ_MRFK from energyconsumer) \
							AND connectivitynode_fk is not null) BB WHERE AA.CEQ_MRFK = BB.CEQ_MRFK"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[고객 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		fPROMISE_LOAD = GetDouble_Row(row[1]);
		stNAME = GetCString_Row(row[2]);
		nType = GetInt_Row(row[3]);
		strCNID = GetCString_Row(row[4]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		if (m_map_IDNode.Lookup(strCNID, nCount_i))
		{
			pNode = m_pNodeArr.GetAt(nCount_i - 1);
			pNode->m_strGENName = stNAME;
			pNode->m_strName = stNAME;
			pNode->m_strNum = strData;
			pNode->m_nName_Type = nType;
			pNode->m_nKind = nType;
			pNode->m_genType = 88;
			pNode->m_HVCUS_fPROMISE_LOAD = fPROMISE_LOAD;
			pNode->m_strCEQID.Format(_T("%s"), strMRID);
			m_map_ND_CnCeq.SetAt(strCNID, strCEQ);
			m_map_ND_CeqCn.SetAt(strCEQ, strCNID);
		}
	}
	mysql_free_result(result);

	//25 선로 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][25/33]========== ADMStoKASIM 데이터 읽기 [선로 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select aa.MRID, aa.NAME_TYPE_FK, bb.LINESEGMENT_SET_TYPE_FK, aa.NAME, bb.length from identifiedobject aa, linesegment bb where aa.MRID = bb.CEQ_MRFK AND aa.MRID \
							IN(select MRID from conductingequipment where CHANGE_EQC_MRFK in (select EQC_MRFK from dl where member_office_fk = %d))"), m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select aa.MRID, aa.NAME_TYPE_FK, bb.LINESEGMENT_SET_TYPE_FK, aa.NAME, bb.length from identifiedobject aa, linesegment bb where aa.MRID = bb.CEQ_MRFK AND aa.MRID \
							IN(select MRID from conductingequipment where CHANGE_EQC_MRFK in ( select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d)))"), m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("select aa.MRID, aa.NAME_TYPE_FK, bb.LINESEGMENT_SET_TYPE_FK, aa.NAME, bb.length from identifiedobject aa, linesegment bb where aa.MRID = bb.CEQ_MRFK"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[선로 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nType = GetInt_Row(row[1]);
		nlineType = GetInt_Row(row[2]);
		stNAME = GetCString_Row(row[3]);
		flength = GetDouble_Row(row[4]);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		pBranch = new CBranch();
		pBranch->m_strMRID.Format(_T("%s"), strMRID);
		pBranch->m_nKind = nType;
		pBranch->m_strName.Format(_T("%s"), stNAME);
		pBranch->m_nLT = nlineType;
		pBranch->m_dLength = flength;
		pBranch->m_nKCIM_Type = 2; // 1.SW 2.ACL 3.입상주 4.DL
		m_pBranchArr.Add(pBranch);
		m_map_IDBranch.SetAt(strMRID, m_pBranchArr.GetSize());
	}
	mysql_free_result(result);

	//26 연결관계 정보
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][26/33]========== ADMStoKASIM 데이터 읽기 [연결관계 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	strADMS_SQL.Format(_T("select ceq_mrfk ,CONNECTIVITYNODE_FK from terminal;"));

	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[연결관계 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		strData = GetCString_Row(row[1]);

		pBranch = GetBranch(strMRID);
		if (pBranch == NULL)
		{
			continue;
		}

		pNode = GetNode(strData);
		// 신규로 입력한 선로 때문에 입력 했습니다.
		if (pNode == NULL)
		{
			stCNDate.Format(_T("%s88"), strMRID);
			pNode = GetNode(stCNDate);
		}
		// 신규로 입력한 선로 때문에 입력 했습니다.
		if (pNode == NULL)
		{
			continue;
		}
		//////////////////////////////////
		if (pBranch->m_strFwdID.IsEmpty())
		{
			pBranch->m_strFwdID = pNode->m_strMRID;
		}
		else if (pBranch->m_strBwdID.IsEmpty())
		{
			if (pBranch->m_strFwdID != pNode->m_strMRID)
			{
				pBranch->m_strBwdID = pNode->m_strMRID;
			}
			else
			{
				continue;
			}
		}
		else
		{
			pBranch->m_strBwdID = pNode->m_strMRID;
		}
	}
	mysql_free_result(result);

	//27 Master Code OpenClose 정보
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][27/33]========== ADMStoKASIM 데이터 읽기 [Master Code OpenClose 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();



	strADMS_SQL.Format(_T("select master_code_id, ceq_type_fk, circuit from `%s`.`master_code` where   point_type_code_fk = 3 and code_fk = 1;"), m_szADMS_Code);
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[Master Code OpenClose 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		nMaster_codeID = GetInt_Row(row[0]);
		nMasterCEQ_TYPE = GetInt_Row(row[1]);
		nMaster_Circuit = GetInt_Row(row[2]);

		m_nMstCD[nMaster_Index][0] = nMaster_codeID;
		m_nMstCD[nMaster_Index][1] = nMasterCEQ_TYPE;
		m_nMstCD[nMaster_Index][2] = nMaster_Circuit;
		nMaster_Index++;
	}
	mysql_free_result(result);

	
	//28 Swith 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][28/33]========== ADMStoKASIM 데이터 읽기 [Swith 상태 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	strADMS_SQL.Format(_T(" SELECT bi.ceq_mrfk,value FROM \
		(select BI.ceq_mrfk, BI.master_code_fk, BI.value, CEQ.CEQ_TYPE_FK from bi_value BI INNER JOIN conductingequipment CEQ \
			ON BI.CEQ_MRFK = CEQ.MRID  AND   BI.VALUE != 0 AND CEQ.CEQ_TYPE_FK != 206) BI \
		INNER JOIN \
		(select master_code_id, ceq_type_fk, circuit from `%s`.`master_code` where   point_type_code_fk = 3 and code_fk = 1) MACODE \
		ON BI.MASTER_CODE_FK = MACODE.MASTER_CODE_ID \
		ORDER BY BI.CEQ_MRFK;"), m_szADMS_Code);
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[Swith 상태 정보] 데이터베이스 연결 실패"));
		return;
	}
	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		strData = GetCString_Row(row[1]);

		if (strMRID != strCEQ)
		{
			pBranch = GetBranch(strMRID);
			strCEQ = strMRID;
		}
		if (pBranch == NULL)
		{
			continue;
		}
		if (pBranch->m_nCeq_Type > 0)
		{
			pBranch->m_bStatus = _wtoi(strData);
		}
	}
	mysql_free_result(result);
	
	/*
	//28 Swith 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][28/33]========== ADMStoKASIM 데이터 읽기 [Swith 상태 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	strADMS_SQL.Format(_T("select ceq_mrfk, master_code_fk, value from bi_value where value != 0"));
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[Swith 상태 정보] 데이터베이스 연결 실패"));
		return;
	}
	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		strNM = GetCString_Row(row[1]);
		strData = GetCString_Row(row[2]);

		if (strMRID != strCEQ)
		{
			pBranch = GetBranch(strMRID);
			strCEQ = strMRID;
		}
		if (pBranch == NULL)
		{
			continue;
		}
		if (pBranch->m_nCeq_Type > 0)
		{
			if (pBranch->m_nKind == 21 || pBranch->m_nKind == 22 || pBranch->m_nKind == 23 || pBranch->m_nKind == 24 || pBranch->m_nKind == 25 || pBranch->m_nKind == 27)
			{
				nIdx = GetMasterCode_KASIM(_wtoi(strNM), pBranch->m_nCeq_Type);
				nMST_CD = GetMasterCD(pBranch->m_nCeq_Type, nIdx);
			}
			else
			{
				nMST_CD = GetMasterCD(pBranch->m_nCeq_Type, 1);
			}

			if (nMST_CD == _wtoi(strNM))
			{
				pBranch->m_bStatus = _wtoi(strData);
			}
		}
	}
	mysql_free_result(result);
	*/

	//29 AO_VALUE 상태 정보 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][29/33]========== ADMStoKASIM 데이터 읽기 [AO_VALUE 상태 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("SELECT ceq_mrfk, master_code_fk ,value FROM ao_value where ceq_mrfk in ( select MRID from conductingequipment where CHANGE_EQC_MRFK in (select EQC_MRFK from dl \
				 where member_office_fk = %d))  and value != 0 AND master_code_fk IN ( SELECT master_code_fk FROM ao_value where ceq_mrfk in ( SELECT mrid FROM conductingequipment \
				WHERE CEQ_TYPE_FK IN (58,59,60,65,68,75,302,317,318,319)))"), m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("SELECT ceq_mrfk, master_code_fk ,value FROM ao_value where ceq_mrfk in ( select MRID from conductingequipment where CHANGE_EQC_MRFK in ( select EQC_MRFK from dl \
			 where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d)))  and value != 0 AND master_code_fk IN ( SELECT master_code_fk FROM ao_value where ceq_mrfk in \
			 (SELECT mrid FROM conductingequipment WHERE CEQ_TYPE_FK IN (58,59,60,65,68,75,302,317,318,319)))"), m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("SELECT ceq_mrfk, master_code_fk ,value FROM ao_value  where value != 0 AND master_code_fk IN ( SELECT master_code_fk FROM ao_value where ceq_mrfk in \
						 ( SELECT mrid FROM conductingequipment WHERE CEQ_TYPE_FK IN (58,59,60,65,68,75,302,317,318,319)))"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[AO_VALUE 상태 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		stAO_ceqid = GetCString_Row(row[0]);
		nMaster_code = GetInt_Row(row[1]);
		dAO_Value = GetDouble_Row(row[2]);

		stAO_key.Format(_T("%s%d"), stAO_ceqid, nMaster_code);
		m_map_PRDE_AO_VALUE.SetAt(stAO_key, dAO_Value);
	}
	mysql_free_result(result);

	//30 AO_VALUE 상태 정보 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][30/33]========== ADMStoKASIM 데이터 읽기 [communication_link 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType != 0)
	{
		if (m_nDL_TEST == 0) // 20230503 수정전 공용선로 
		{
			strADMS_SQL.Format(_T("select A.CEQ_MRFK, B.COMM_CODE_FK from communication_link A, comm_group B WHERE A.COMM_GROUP_FK = B.COMM_GROUP_ID and a.ceq_mrfk in \
				( select MRID from conductingequipment where CHANGE_EQC_MRFK in (select EQC_MRFK from dl where member_office_fk = %d))"), m_nType);
		}
		else //공용선로 수정 후 !
		{
			strADMS_SQL.Format(_T("select A.CEQ_MRFK, B.COMM_CODE_FK from communication_link A, comm_group B WHERE A.COMM_GROUP_FK = B.COMM_GROUP_ID and a.ceq_mrfk in \
				( select MRID from conductingequipment where CHANGE_EQC_MRFK in ( select EQC_MRFK from dl where CEQ_MRFK IN( select CEQ_MRFK from dl where member_office_fk = %d)))"), m_nType);
		}
	}
	else
	{
		strADMS_SQL.Format(_T("select A.CEQ_MRFK, B.COMM_CODE_FK from communication_link A, comm_group B WHERE A.COMM_GROUP_FK = B.COMM_GROUP_ID"));
	}
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[communication_link 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nCOMM_CODE = GetInt_Row(row[1]);

		m_map_SW_comm_group.SetAt(strMRID, nCOMM_CODE);
		nCOMM_CODE = 0;
	}
	mysql_free_result(result);

	//31 Switch 설비 정보
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][31/33]========== ADMStoKASIM 데이터 읽기 [Switch 설비 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();


	strADMS_SQL.Format(_T("SELECT CEQ_TYPE_ID,RTU_CODE_G1  FROM `%s`.`ceq_type`"), m_szADMS_Code);
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[Switch 설비 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		nCeqTypeID = GetInt_Row(row[0]);
		nRTU_Code_G1 = GetInt_Row(row[1]);

		m_map_CEQTYPE_RTUCODE.SetAt(nCeqTypeID, nRTU_Code_G1);
	}
	mysql_free_result(result);

	//32 PQMS 정보 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][32/33]========== ADMStoKASIM 데이터 읽기 [PQMS 설비 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nType == 0)
	{
		strADMS_SQL.Format(_T("SELECT ev.CEQ_MRFK, ev.VALUE  FROM equip_attr_value ev JOIN conductingequipment ce ON ev.CEQ_MRFK = ce.MRID WHERE ev.MASTER_CODE_FK = 32221 and  ev.VALUE  != 0"));

		_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
		WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
		mysql_query(MYSQL_DB, "set Names euckr");
		if (mysql_query(MYSQL_DB, query))
		{
			IDC_LIST_DATA_HISTORY_ERROR(_T("[PQMS 설비 정보] 데이터베이스 연결 실패"));
			return;
		}

		result = mysql_store_result(MYSQL_DB);
		fields = mysql_num_fields(result);    // 필드 갯수 구함
		while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
		{
			strMRID = GetCString_Row(row[0]);
			strData = GetCString_Row(row[1]);
			m_map_CBSW_PQMS.SetAt(strMRID, strData);
		}
		mysql_free_result(result);
	}

	//33 MASTER_CODE 정보 
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[4/7][33/33]========== ADMStoKASIM 데이터 읽기 [MASTER_CODE 정보] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	strADMS_SQL.Format(_T("SELECT master_code_id, ceq_type_fk, code_fk FROM `%s`.`master_code` where master_code_id in ( SELECT master_code_fk FROM ao_value where ceq_mrfk in \
					 ( SELECT mrid FROM conductingequipment WHERE CEQ_TYPE_FK IN (58,59,60,65,68,75,302,301,305,304,312,315,316,317,318,319)));"), m_szADMS_Code);
	_stprintf_s(tcBuffer, _countof(tcBuffer), strADMS_SQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("[MASTER_CODE 정보] 데이터베이스 연결 실패"));
		return;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);    // 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		nMaster_code = GetInt_Row(row[0]);
		nCeq_type = GetInt_Row(row[1]);
		nCode = GetInt_Row(row[2]);

		if (nCeq_type == 58)
		{
			m_map_PRDE_TYPE_58.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 59)
		{
			m_map_PRDE_TYPE_59.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 60)
		{
			m_map_PRDE_TYPE_60.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 65)
		{
			m_map_PRDE_TYPE_65.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 68)
		{
			m_map_PRDE_TYPE_68.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 75)
		{
			m_map_PRDE_TYPE_75.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 302)
		{
			m_map_PRDE_TYPE_302.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 301)
		{
			m_map_PRDE_TYPE_301.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 305)
		{
			m_map_PRDE_TYPE_305.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 304 || nCeq_type == 315 || nCeq_type == 316)
		{
			m_map_PRDE_TYPE_304315316.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 312)
		{
			m_map_PRDE_TYPE_312.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 317)
		{
			m_map_PRDE_TYPE_317.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 318)
		{
			m_map_PRDE_TYPE_318.SetAt(nCode, nMaster_code);
		}
		if (nCeq_type == 319)
		{
			m_map_PRDE_TYPE_319.SetAt(nCode, nMaster_code);
		}
	}
	mysql_free_result(result);
	mysql_close(MYSQL_DB);
}

void CADMStoKCIMDlg::ADMStoKASIM_Conver()
{
	//탐색할까?
	int i;
	int k = 0;
	int nSNVID = 0;
	int nType = 0; //sw타입
	CString stNAME;

	CBranch* pBranch;
	CNode* pNode;

	//계통 탐색
	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[5/7][1/8]========== ADMStoKASIM 데이터 변환 [계통 탐색 - CB] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	for (i = 0; i < m_pBranchArr.GetSize(); i++)
	{
		pBranch = m_pBranchArr.GetAt(i);
		if (pBranch->m_nKind != 14) continue;
		stNAME.Format(_T("%s"), pBranch->m_strName);
		stNAME.Replace(_T("OCB"), _T(""));
		pBranch->m_strDL.Format(_T("%s"), stNAME);
		if (pBranch->m_nDLID == 0)
		{
			pBranch->m_nDLID = GetDLID_OCB(pBranch->m_strMRID);
		}
	}

	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[5/7][2/8]========== ADMStoKASIM 데이터 변환 [계통 탐색 - Node] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();
	///////////////////////////////////////
	//ND를 입력하는 부분입니다!!!!!
	//int nCheck;
	int nSNV = 0;
	int nTYPE = m_nType;
	int nCHECK_DLID = 0;
	int nGEN_II_LNSEC = 0;
	int nGEN_TR_TYPE = 0;
	for (i = 0; i < m_pNodeArr.GetSize(); i++)
	{
		pNode = m_pNodeArr.GetAt(i);
		if (pNode->m_nName_Type == 14)
		{
			nSNV = Get_MRID_SNV(pNode->m_strCEQID);
			if (nSNV == 0)
			{
				nSNV = 0;
			}
			if (nSNV == 0)
			{
				delete pNode;
				m_pNodeArr.RemoveAt(i);
				i--;
				continue;
			}
		}
		if (pNode->m_strCEQID == "11212100002257"			)
		{
			int a = 0;
		}
		ND_STA stNd;
		memset(&stNd, 0, sizeof(ND_STA));
		_stprintf_s(stNd.nd_nm, L"%s", pNode->m_strName);
		_stprintf_s(stNd.nd_ceqid, L"%s", pNode->m_strCEQID);
		//		_stprintf_s(stNd.nd_ceqid, L"%s", pNode->m_strJun_CEQID);
		_stprintf_s(stNd.nd_connectivitynodeid, L"%s", pNode->m_strMRID);
		stNd.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
		stNd.nd_ii_snv = Get_MRID_SNV(pNode->m_strCEQID);
		////
		m_map_IDNode_NEW.SetAt(pNode->m_strMRID, i + 1); //20210405
		m_map_ND_CnNDID.SetAt(pNode->m_strMRID, m_arrND.GetSize() + 1); //20210405
		m_arrND.Add(stNd);

		GND_STA stGND;
		memset(&stGND, 0, sizeof(GND_STA));
		_stprintf_s(stGND.gnd_nm, L"%s", pNode->m_strName);
		stGND.gnd_hi_nd = (int)m_arrND.GetSize();
		//
		m_arrGND.Add(stGND);

		pNode->m_nNDID = (int)m_arrND.GetSize();
		pNode->m_nGNDID = (int)m_arrGND.GetSize();


		if (pNode->m_nKind == 37)
		{
			/////////////////		
			GEN_STA stGEN;
			memset(&stGEN, 0, sizeof(GEN_STA));
			_stprintf_s(stGEN.gen_nm, L"%s", pNode->m_strGENName);
			_stprintf_s(stGEN.gen_ceqid, L"%s", pNode->m_strCEQID);
			stGEN.gen_namkv = 22.9;
			stGEN.gen_mwlmmx = 0; //20220809 요청사항
			stGEN.gen_mwlmmn = 0;
			stGEN.gen_mvarlmmx = 0.03;
			stGEN.gen_mvarlmmn = 0;
			stGEN.gen_r = 0;
			stGEN.gen_stx = 0;
			stGEN.gen_ssx = 0;
			stGEN.gen_type = pNode->m_genType; //20190722
			stGEN.gen_vol_cls = 0;
			stGEN.gen_conndrep = 1;
			stGEN.gen_contype = 7;//3상
			stGEN.gen_pf = 0.95;
			stGEN.gen_eff = 0.85;
			stGEN.gen_noofp = 4;
			stGEN.gen_avr = 1;
			stGEN.gen_ii_nd = stGEN.gen_ii_gnd = stGEN.gen_ii_connd = pNode->m_nNDID;
			stGEN.gen_ii_ij = (int)m_arrIJ.GetSize() + 1;
			stGEN.GEN_II_PRDE = (int)m_arrPRDE_STA.GetSize() + 1;

			stGEN.fGEN_ITR_CAP = pNode->m_GEN_UIN_fGEN_ITR_CAP;  //20221011 요청사항
			stGEN.fGEN_ITR_R = pNode->m_GEN_UIN_fGEN_ITR_R;
			stGEN.fGEN_ITR_X = pNode->m_GEN_UIN_fGEN_ITR_X;
			if (stGEN.fGEN_ITR_X == 0)
			{
				stGEN.fGEN_ITR_X = 6;
			}
			stGEN.fGEN_ITR_NGR_R = 0;  //20220809 요청사항
			stGEN.fGEN_ITR_NGR_X = 0;  //20220809 요청사항
			stGEN.nGEN_ITR_WDC = pNode->m_GEN_UIN_nGEN_ITR_WDC;  //20220809 요청사항
			stGEN.nGEN_MACH_TYPE = 1;  //20220809 요청사항			
			stGEN.GEN_SI_LNSEC = 0;						   //20230629
			stGEN.GEN_II_LNSEC = 0; //20230629 - 20231031 삭제요청
			stGEN.GEN_TREXCL = GET_GENUNIT_STA_TR(pNode->szGEN_DG_TR_MRID, m_arrGEN.GetSize() + 1);
			m_arrGEN.Add(stGEN);

			IJ_STA stIj;
			memset(&stIj, 0, sizeof(IJ_STA));
			_stprintf_s(stIj.ij_nm, L"%s", pNode->m_strGENName);
			stIj.ij_ii_equty = 4;  //고객 8 선로 6 발전기4
			stIj.ij_ii_equ = m_nIJ_STA_EQU4++;
			stIj.ij_ii_gnd = stGEN.gen_ii_gnd;
			nCHECK_DLID = GetDLID(pNode->m_strCEQID); //DL 찾기
			if (nCHECK_DLID == 0)
			{
				nCHECK_DLID = GetDLID_ORIGINAL(pNode->m_strCEQID); //DL 찾기
			}
			stIj.ij_ii_dl = nCHECK_DLID;
			m_arrIJ.Add(stIj);
			/////////////////

			// 발전기
			PRDE_STA stPRDE;
			memset(&stPRDE, 0, sizeof(PRDE_STA));
			_stprintf_s(stPRDE.PRDE_STA_NM, L"%s", pNode->m_strGENName);
			stPRDE.PRDE_OCRF_II_TCCSET = 0;
			stPRDE.PRDE_OCRD_II_TCCSET = 0;
			stPRDE.PRDE_OCR_Pickup_C = 0;
			stPRDE.PRDE_OCR_IIC = 0;
			stPRDE.PRDE_OCGRF_II_TCCSET = 0;
			stPRDE.PRDE_OCGRD_II_TCCSET = 0;
			stPRDE.PRDE_OCGR_Pickup_C = 0;
			stPRDE.PRDE_OCGR_IIC = 0;
			stPRDE.PRDE_TYPE = 0;
			stPRDE.PRDE_SET_GTYPE = 0;
			stPRDE.PRDE_OCR_NOF = 0;
			stPRDE.PRDE_OCR_NOD = 0;
			stPRDE.PRDE_OCGR_NOF = 0;
			stPRDE.PRDE_OCGR_NOD = 0;
			stPRDE.PRDE_OCRF_TMS = 0;
			stPRDE.PRDE_OCRF_TAS = 0;
			stPRDE.PRDE_OCRF_MRT = 0;
			stPRDE.PRDE_OCRD_TMS = 0;
			stPRDE.PRDE_OCRD_TAS = 0;
			stPRDE.PRDE_OCRD_MRT = 0;
			stPRDE.PRDE_OCGRF_TMS = 0;
			stPRDE.PRDE_OCGRF_TAS = 0;
			stPRDE.PRDE_OCGRF_MRT = 0;
			stPRDE.PRDE_OCGRD_TMS = 0;
			stPRDE.PRDE_OCGRD_TAS = 0;
			stPRDE.PRDE_OCGRD_MRT = 0;
			stPRDE.PRDE_OCR_CTR = 120;
			stPRDE.PRDE_OCGR_CTR = 120;
			stPRDE.PRDE_MX_LD_C_PHA = 0;
			m_arrPRDE_STA.Add(stPRDE);
		}
		if (pNode->m_nKind == 28) //고객 작업입니다.
		{
			HVCUS_STA stHVCUS;
			memset(&stHVCUS, 0, sizeof(HVCUS_STA));
			stHVCUS.HVCUS_STA_ID = m_arrHVCUS.GetSize() + 1;
			_stprintf_s(stHVCUS.HVCUS_NM, L"%s", pNode->m_strGENName);
			_stprintf_s(stHVCUS.HVCUS_CEQID, L"%s", pNode->m_strCEQID);
			stHVCUS.HVCUS_II_GND = pNode->m_nNDID;
			stHVCUS.HVCUS_SI_GND = 0;
			stHVCUS.HVCUS_CON_KVA = pNode->m_HVCUS_fPROMISE_LOAD;
			stHVCUS.HVCUS_ITR_KVA = 0;
			stHVCUS.HVCUS_ITR_PZ = 0;
			stHVCUS.HVCUS_II_PRDE = m_arrPRDE_STA.GetSize() + 1;
			stHVCUS.HVCUS_II_IJ = m_arrIJ.GetSize() + 1;
			m_arrHVCUS.Add(stHVCUS);

			IJ_STA stIj;
			memset(&stIj, 0, sizeof(IJ_STA));
			_stprintf_s(stIj.ij_nm, L"%s", pNode->m_strGENName);
			stIj.ij_ii_equty = 8;  //고객 8 선로 6 발전기4
			stIj.ij_ii_equ = m_nIJ_STA_EQU8++;
			stIj.ij_ii_gnd = pNode->m_nNDID;

			nCHECK_DLID = GetDLID(pNode->m_strCEQID); //DL 찾기
			if (nCHECK_DLID == 0)
			{
				nCHECK_DLID = GetDLID_ORIGINAL(pNode->m_strCEQID); //DL 찾기
			}
			stIj.ij_ii_dl = nCHECK_DLID;
			m_arrIJ.Add(stIj);

			// 고객입력시 
			PRDE_STA stPRDE;
			memset(&stPRDE, 0, sizeof(PRDE_STA));
			_stprintf_s(stPRDE.PRDE_STA_NM, L"%s", pNode->m_strGENName);
			stPRDE.PRDE_OCRF_II_TCCSET = 0;
			stPRDE.PRDE_OCRD_II_TCCSET = 0;
			stPRDE.PRDE_OCR_Pickup_C = 0;
			stPRDE.PRDE_OCR_IIC = 0;
			stPRDE.PRDE_OCGRF_II_TCCSET = 0;
			stPRDE.PRDE_OCGRD_II_TCCSET = 0;
			stPRDE.PRDE_OCGR_Pickup_C = 0;
			stPRDE.PRDE_OCGR_IIC = 0;
			stPRDE.PRDE_TYPE = 0;
			stPRDE.PRDE_SET_GTYPE = 0;
			stPRDE.PRDE_OCR_NOF = 0;
			stPRDE.PRDE_OCR_NOD = 0;
			stPRDE.PRDE_OCGR_NOF = 0;
			stPRDE.PRDE_OCGR_NOD = 0;
			stPRDE.PRDE_OCRF_TMS = 0;
			stPRDE.PRDE_OCRF_TAS = 0;
			stPRDE.PRDE_OCRF_MRT = 0;
			stPRDE.PRDE_OCRD_TMS = 0;
			stPRDE.PRDE_OCRD_TAS = 0;
			stPRDE.PRDE_OCRD_MRT = 0;
			stPRDE.PRDE_OCGRF_TMS = 0;
			stPRDE.PRDE_OCGRF_TAS = 0;
			stPRDE.PRDE_OCGRF_MRT = 0;
			stPRDE.PRDE_OCGRD_TMS = 0;
			stPRDE.PRDE_OCGRD_TAS = 0;
			stPRDE.PRDE_OCGRD_MRT = 0;
			stPRDE.PRDE_OCR_CTR = 120;
			stPRDE.PRDE_OCGR_CTR = 120;
			stPRDE.PRDE_MX_LD_C_PHA = 0;
			m_arrPRDE_STA.Add(stPRDE);
		}
	}

	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[5/7][3/8]========== ADMStoKASIM 데이터 변환 [계통 탐색 - CBSW/LNSEC] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	CString stSUBSMRID;
	CNode* pNode1, *pNode2;
	int nMUTingID = 1;
	int nMUTingNember = 1;
	CString szMULTISW;
	szMULTISW.Format(_T(""));
	CString szMULTISW_O;
	szMULTISW_O.Format(_T("0"));
	//20200901 멀티 스위치 작업
	for (i = 0; i < m_pBranchArr.GetSize(); i++)
	{
		pBranch = m_pBranchArr.GetAt(i);

		if (pBranch->m_nKCIM_Type == 2) continue;//lnsec
		if (pBranch->m_nKCIM_Type == 3) continue;
		if (pBranch->m_nKCIM_Type == 4) continue;
		if (pBranch->m_szMULTISW.IsEmpty()) continue;

		if (szMULTISW.IsEmpty()) //처음꺼 입력
		{
			szMULTISW = pBranch->m_szMULTISW;
		}
	}//

	double dPRDE_OCRF_TMS, dPRDE_OCRF_TAS, dPRDE_OCRF_MRT, dPRDE_OCRD_TMS, dPRDE_OCRD_TAS, dPRDE_OCRD_MRT, dPRDE_OCGRF_TMS, dPRDE_OCGRF_TAS, dPRDE_OCGRF_MRT, dPRDE_OCGRD_TMS, dPRDE_OCGRD_TAS, dPRDE_OCGRD_MRT;

	//PRDE 

	 //변환
	int nNAME_TYPE_FK1 = 0, nNAME_TYPE_FK2 = 0;//20230830
	int nComposite_Number = 0;
	int nComType = 0;
	int nCbsw_RTUCODE = 0;
	for (i = 0; i < m_pBranchArr.GetSize(); i++)
	{
		pBranch = m_pBranchArr.GetAt(i);
		nCbsw_RTUCODE = 0;

		if (pBranch->m_nKCIM_Type == 3) continue;
		if (pBranch->m_nKCIM_Type == 4) continue;
		if (pBranch->m_nKind != 14)
		{
			pBranch->m_nDLID = GetDLID(pBranch->m_strMRID); //DL 찾기
		}
		if (pBranch->m_nDLID == 0 && pBranch->m_nKind != 14)
		{
			pBranch->m_nDLID = GetDLID_ORIGINAL(pBranch->m_strMRID); //DL 찾기
		}

		pNode1 = GetNode_New(pBranch->m_strFwdID);
		pNode2 = GetNode_New(pBranch->m_strBwdID);

		if (pNode1 == NULL && pNode2 == NULL)
		{
			//이건 넘기는게 맞고 !!
			continue;
		}

		//20230504
		if (pBranch->m_nDLID == 0 && pBranch->m_nKind != 14 && pNode1 != NULL)
		{
			pBranch->m_nDLID = GetDLID_ORIGINAL(pNode1->m_strCEQID); //DL 찾기
			if (pBranch->m_nDLID == 0 && pNode2 != NULL)
			{
				pBranch->m_nDLID = GetDLID_ORIGINAL(pNode2->m_strCEQID); //DL 찾기
			}
		}
		if (pBranch->m_nDLID == 0)
		{
			pBranch->m_nDLID = 0; //DL 찾기
		}
		//

		if (pBranch->m_nKind == 21 || pBranch->m_nKind == 22 || pBranch->m_nKind == 23 || pBranch->m_nKind == 24 || pBranch->m_nKind == 25 || pBranch->m_nKind == 27)
		{
			CString szMrid;
			if (pNode1 == NULL)
			{
				pNode1 = GetNodeTER_NEW(pBranch);
			}
			if (pNode2 == NULL)
			{
				pNode2 = GetNodeTER_NEW(pBranch);
			}
		}
		//CB쪽 작업하려고

		if (pBranch->m_nKCIM_Type == 1) //SW
		{
			//CB쪽 작업하려고20220526
			if (pBranch->m_nKind == 14)
			{
				if (pNode1 == NULL && pNode2 == NULL)
				{
					//이건 넘기는게 맞고 !!
					continue;
				}
				if (pNode1 == NULL)
				{
					pNode1 = GetNodeTER_NEW_CB(pBranch, pNode2);
				}
				if (pNode2 == NULL)
				{
					pNode2 = GetNodeTER_NEW_CB(pBranch, pNode1);
				}
			}

			if (pNode1 == NULL || pNode2 == NULL)
			{
				if (pBranch->m_nKind != 14)
				{
					continue;
				}
			}
			if (pBranch->m_nCeq_Type == 81)  //SVR
			{
				//SVR
				TR_STA stTR;
				memset(&stTR, 0, sizeof(TR_STA));
				_stprintf_s(stTR.tr_nm, L"%s", pBranch->m_strName);
				_stprintf_s(stTR.tr_ceqid, L"%s", pBranch->m_strMRID);
				stTR.tr_type = 3; //SVR 은1번입니다.
				stTR.tr_fnorkv = 10;
				stTR.tr_posx = 3.65;//실제값으로 입력해야함.
				stTR.tr_zerx = 3.65;//실제값으로 입력해야함.
				stTR.tr_patapmx = stTR.tr_pbtapmx = stTR.tr_pctapmx = 32;
				stTR.tr_patapmn = stTR.tr_pbtapmn = stTR.tr_pctapmn = 1;
				stTR.tr_patapnor = stTR.tr_pbtapnor = stTR.tr_pctapnor = 15;
				stTR.tr_pbtapstep = stTR.tr_pctapstep = -0.0125;
				stTR.tr_patapstep = 0.625;
				stTR.tr_tnorkv = 10;
				stTR.tr_conty = 2;
				stTR.tr_ii_ss = GetSSID(pBranch->m_nDLID);
				stTR.tr_ii_br = (int)m_arrBR.GetSize() + 1;
				stTR.tr_ii_mtr = GetMTRID(pBranch->m_nDLID);

				stTR.TR_LDCTYPE = 3; //LDC 타입 (1: 유성 OLTC, 2 : MR OLTC,	3 : SVR
				stTR.TR_DVMMXV = 120;
				stTR.TR_DVMMNV = 118;

				stTR.tr_II_FND = pNode1->m_nNDID;
				stTR.tr_II_TND = pNode2->m_nNDID;
				m_arrTR.Add(stTR);

				//BR
				BR_STA stBr;
				memset(&stBr, 0, sizeof(BR_STA));
				_stprintf_s(stBr.br_nm, L"%s", pBranch->m_strName);
				stBr.br_posr = 0.03;
				stBr.br_posx = 0.06;
				stBr.br_zerr = 0.01;
				stBr.br_zerx = 0.02;
				stBr.br_norlm = 100;
				stBr.br_ii_fnd = pNode1->m_nNDID;
				stBr.br_ii_tnd = pNode2->m_nNDID;
				stBr.br_ii_equ = (int)m_arrTR.GetSize();
				stBr.br_ii_gbr = (int)m_arrGBR.GetSize() + 1;
				stBr.br_ii_dl = pBranch->m_nDLID;
				m_arrBR.Add(stBr);
				//GBR 생성
				GBR_STA stGbr;
				memset(&stGbr, 0, sizeof(GBR_STA));
				_stprintf_s(stGbr.gbr_nm, L"%s", pBranch->m_strName);
				stGbr.gbr_ii_equ = (int)m_arrTR.GetSize();
				stGbr.gbr_ii_fgnd = stBr.br_ii_fnd;
				stGbr.gbr_ii_tgnd = stBr.br_ii_tnd;
				stGbr.gbr_posr = stBr.br_posr;
				stGbr.gbr_posx = stBr.br_posx;
				stGbr.gbr_zerr = stBr.br_zerr;
				stGbr.gbr_zerx = stBr.br_zerx;
				stGbr.gbr_hi_br = (int)m_arrBR.GetSize();
				stGbr.gbr_ii_equty = 2;
				stGbr.gbr_ii_dl = pBranch->m_nDLID;
				m_arrGBR.Add(stGbr);
			}
			else
			{
				if (pBranch->m_nCeq_Type == 71 || pBranch->m_nCeq_Type == 207)
				{	//1. 차단기
					nType = 1;
				}
				else if (pBranch->m_nCeq_Type == 58 || pBranch->m_nCeq_Type == 59 || pBranch->m_nCeq_Type == 60 || pBranch->m_nCeq_Type == 61 || pBranch->m_nCeq_Type == 62 ||
					pBranch->m_nCeq_Type == 63 || pBranch->m_nCeq_Type == 64 || pBranch->m_nCeq_Type == 65 || pBranch->m_nCeq_Type == 66 || pBranch->m_nCeq_Type == 67 ||
					pBranch->m_nCeq_Type == 110)
				{ // 2.리클로저
					nType = 2;
				}
				else if (pBranch->m_nCeq_Type == 1 || pBranch->m_nCeq_Type == 2 || pBranch->m_nCeq_Type == 4 || pBranch->m_nCeq_Type == 7 || pBranch->m_nCeq_Type == 9 ||
					pBranch->m_nCeq_Type == 10 || pBranch->m_nCeq_Type == 11 || pBranch->m_nCeq_Type == 12 || pBranch->m_nCeq_Type == 14 || pBranch->m_nCeq_Type == 15 ||
					pBranch->m_nCeq_Type == 16 || pBranch->m_nCeq_Type == 19 || pBranch->m_nCeq_Type == 20 || pBranch->m_nCeq_Type == 21 || pBranch->m_nCeq_Type == 24 ||
					pBranch->m_nCeq_Type == 25 || pBranch->m_nCeq_Type == 26 || pBranch->m_nCeq_Type == 28 || pBranch->m_nCeq_Type == 29 || pBranch->m_nCeq_Type == 31 ||
					pBranch->m_nCeq_Type == 33 || pBranch->m_nCeq_Type == 34 || pBranch->m_nCeq_Type == 35 || pBranch->m_nCeq_Type == 36 || pBranch->m_nCeq_Type == 37 ||
					pBranch->m_nCeq_Type == 38 || pBranch->m_nCeq_Type == 39 || pBranch->m_nCeq_Type == 48 || pBranch->m_nCeq_Type == 49 || pBranch->m_nCeq_Type == 50 ||
					pBranch->m_nCeq_Type == 51 || pBranch->m_nCeq_Type == 52 || pBranch->m_nCeq_Type == 72 || pBranch->m_nCeq_Type == 320 || pBranch->m_nCeq_Type == 321 ||
					pBranch->m_nCeq_Type == 322 || pBranch->m_nCeq_Type == 323 || pBranch->m_nCeq_Type == 324 || pBranch->m_nCeq_Type == 325 || pBranch->m_nCeq_Type == 326
					)
				{ // 3.자동 || pBranch->m_nCeq_Type == 73 || pBranch->m_nCeq_Type == 79
					nType = 3;
				}
				else if (pBranch->m_nCeq_Type == 3 || pBranch->m_nCeq_Type == 5 || pBranch->m_nCeq_Type == 6 || pBranch->m_nCeq_Type == 8 || pBranch->m_nCeq_Type == 13 ||
					pBranch->m_nCeq_Type == 23 || pBranch->m_nCeq_Type == 27 || pBranch->m_nCeq_Type == 30 || pBranch->m_nCeq_Type == 32 || pBranch->m_nCeq_Type == 40 ||
					pBranch->m_nCeq_Type == 41 || pBranch->m_nCeq_Type == 42 || pBranch->m_nCeq_Type == 43 || pBranch->m_nCeq_Type == 44 || pBranch->m_nCeq_Type == 55 ||
					pBranch->m_nCeq_Type == 56 || pBranch->m_nCeq_Type == 121 || pBranch->m_nCeq_Type == 206 || pBranch->m_nCeq_Type == 311)
				{ // 4.수동
					nType = 4;
				}

				else if (pBranch->m_nCeq_Type == 68 || pBranch->m_nCeq_Type == 70 || pBranch->m_nCeq_Type == 74 || pBranch->m_nCeq_Type == 76 || pBranch->m_nCeq_Type == 77 ||
					pBranch->m_nCeq_Type == 78) { // 5.퓨즈
					nType = 5;
				}
				else if (pBranch->m_nCeq_Type == 69) { // 
					nType = 6;
				}


				else if (pBranch->m_nCeq_Type == 18 || pBranch->m_nCeq_Type == 314 || pBranch->m_nCeq_Type == 327) { // 7. 저압
					nType = 7;
				}
				else if (pBranch->m_nCeq_Type == 17 || pBranch->m_nCeq_Type == 22 || pBranch->m_nCeq_Type == 45 || pBranch->m_nCeq_Type == 46 || pBranch->m_nCeq_Type == 47 ||
					pBranch->m_nCeq_Type == 313 || pBranch->m_nCeq_Type == 328 || pBranch->m_nCeq_Type == 329 || pBranch->m_nCeq_Type == 330) { // 8. 고압
					nType = 8;
				}
				else if (pBranch->m_nCeq_Type == 53 || pBranch->m_nCeq_Type == 54 || pBranch->m_nCeq_Type == 57 || pBranch->m_nCeq_Type == 82 || pBranch->m_nCeq_Type == 83 ||
					pBranch->m_nCeq_Type == 84 || pBranch->m_nCeq_Type == 85 || pBranch->m_nCeq_Type == 86 || pBranch->m_nCeq_Type == 87 || pBranch->m_nCeq_Type == 88 ||
					pBranch->m_nCeq_Type == 89 || pBranch->m_nCeq_Type == 114 || pBranch->m_nCeq_Type == 115) { // 8. 고압
					nType = 9;
				}
				else if (pBranch->m_nCeq_Type == 79 || pBranch->m_nCeq_Type == 80) { // 7. 저압
					nType = 10;
				}
				else if (pBranch->m_nCeq_Type == 75)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1 || pBranch->m_nMULTICIR_NUMBER == 4)
					{
						nType = 3;
					}
					if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3)
					{
						nType = 5;
					}
				}
				else if (pBranch->m_nCeq_Type == 301 || pBranch->m_nCeq_Type == 302 || pBranch->m_nCeq_Type == 303)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1)
					{
						nType = 3;
					}
					if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3 || pBranch->m_nMULTICIR_NUMBER == 4)
					{
						nType = 5;
					}
				}
				else if (pBranch->m_nCeq_Type == 312 || pBranch->m_nCeq_Type == 315 || pBranch->m_nCeq_Type == 316)///// 20220718
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1)
					{
						nType = 3;
					}
					if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3)
					{
						nType = 5;
					}
				}
				else if (pBranch->m_nCeq_Type == 304 || pBranch->m_nCeq_Type == 305)///// 20220718
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1 || pBranch->m_nMULTICIR_NUMBER == 4)
					{
						nType = 3;
					}
					if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3)
					{
						nType = 5;
					}
				}
				else if (pBranch->m_nCeq_Type == 317 || pBranch->m_nCeq_Type == 318 || pBranch->m_nCeq_Type == 319)///// 20220718
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1 || pBranch->m_nMULTICIR_NUMBER == 3)
					{
						nType = 3;
					}
					if (pBranch->m_nMULTICIR_NUMBER == 2)
					{
						nType = 5;
					}
				}
				else if (pBranch->m_nCeq_Type == 73) { // 20210719변경된  PQMS	연결정보가 없으므로 변환 불가		
					nType = 0;
				}
				else
				{
					nType = 3;
				}

				///////////////////이게 될까 20210706 작업내용
				if (pBranch->m_nCeq_Type == 53 || pBranch->m_nCeq_Type == 54 || pBranch->m_nCeq_Type == 57)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1 || pBranch->m_nMULTICIR_NUMBER == 2)
					{
						continue;
					}
				}
				if (pBranch->m_nCeq_Type == 83)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1 || pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3 || pBranch->m_nMULTICIR_NUMBER == 4)
					{
						continue;
					}
				}
				if (pBranch->m_nCeq_Type == 84)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1 || pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3 || pBranch->m_nMULTICIR_NUMBER == 4
						|| pBranch->m_nMULTICIR_NUMBER == 5 || pBranch->m_nMULTICIR_NUMBER == 6)
					{
						continue;
					}
				}
				if (pBranch->m_nCeq_Type == 85 || pBranch->m_nCeq_Type == 86 || pBranch->m_nCeq_Type == 87 || pBranch->m_nCeq_Type == 88 || pBranch->m_nCeq_Type == 89)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 1 || pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3 || pBranch->m_nMULTICIR_NUMBER == 4
						|| pBranch->m_nMULTICIR_NUMBER == 5)
					{
						continue;
					}
				}
				//아 함수로 빼야겠다!!!
				if (pBranch->m_nCeq_Type == 53 || pBranch->m_nCeq_Type == 54 || pBranch->m_nCeq_Type == 57)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 3) { pBranch->m_nMULTICIR_NUMBER = 1; }
					if (pBranch->m_nMULTICIR_NUMBER == 4) { pBranch->m_nMULTICIR_NUMBER = 2; }
				}
				if (pBranch->m_nCeq_Type == 83)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 5) { pBranch->m_nMULTICIR_NUMBER = 1; }
					if (pBranch->m_nMULTICIR_NUMBER == 6) { pBranch->m_nMULTICIR_NUMBER = 2; }
				}
				if (pBranch->m_nCeq_Type == 84)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 7) { pBranch->m_nMULTICIR_NUMBER = 1; }
					if (pBranch->m_nMULTICIR_NUMBER == 8) { pBranch->m_nMULTICIR_NUMBER = 2; }
				}
				if (pBranch->m_nCeq_Type == 85 || pBranch->m_nCeq_Type == 86 || pBranch->m_nCeq_Type == 87 || pBranch->m_nCeq_Type == 88 || pBranch->m_nCeq_Type == 89)
				{
					if (pBranch->m_nMULTICIR_NUMBER == 6) { pBranch->m_nMULTICIR_NUMBER = 1; }
					if (pBranch->m_nMULTICIR_NUMBER == 7) { pBranch->m_nMULTICIR_NUMBER = 2; }
				}

				///////////////////
				nComType = 0;
				if (pBranch->m_nKind != 14)
				{
					//CBSW
					CBSW_STA stCbsw;
					memset(&stCbsw, 0, sizeof(CBSW_STA));
					_stprintf_s(stCbsw.cbsw_nm, L"%s", pBranch->m_strName);
					_stprintf_s(stCbsw.cbsw_ceqid, L"%s", pBranch->m_strMRID);
					stCbsw.cbsw_type = nType;
					stCbsw.cbsw_norstat = pBranch->m_bStatus;
					stCbsw.cbsw_rtutype = pBranch->m_nCeq_Type;
					m_map_CEQTYPE_RTUCODE.Lookup(pBranch->m_nCeq_Type, nCbsw_RTUCODE);
					stCbsw.cbsw_RTUCODE = nCbsw_RTUCODE;
					m_map_SW_comm_group.Lookup(pBranch->m_strMRID, nComType);
					stCbsw.cbsw_comtype = nComType;
					//
					if (stCbsw.cbsw_norstat == 1)
					{
						stCbsw.cbsw_norstat = 0;
					}
					else
					{
						stCbsw.cbsw_norstat = 1;
					}
					stCbsw.cbsw_ii_fnd = stCbsw.cbsw_ii_fgnd = pNode1->m_nNDID;
					stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = pNode2->m_nNDID;
					stCbsw.cbsw_ii_dl = pBranch->m_nDLID;
					_stprintf_s(stCbsw.cbsw_multisw_id, L"%s", pBranch->m_szMULTISW);
					stCbsw.cbsw_multicir_number = pBranch->m_nMULTICIR_NUMBER;
					//20211119
					if (!(pBranch->m_szMULTISW == _T("0")))
					{
						m_map_SW_comm_group.Lookup(pBranch->m_szMULTISW, nComType);
						stCbsw.cbsw_comtype = nComType;
					}
					//
					stCbsw.cbsw_ii_prde = 0;
					//20220824
					if (stCbsw.cbsw_multicir_number == 1)
					{
						m_map_CBSW_ND.SetAt(pBranch->m_szMULTISW, stCbsw.cbsw_ii_fnd);
					}
					//작업해야함
					if (pBranch->m_nCeq_Type == 58 || pBranch->m_nCeq_Type == 59 || pBranch->m_nCeq_Type == 60 || pBranch->m_nCeq_Type == 61 || pBranch->m_nCeq_Type == 62 ||
						pBranch->m_nCeq_Type == 63 || pBranch->m_nCeq_Type == 64 || pBranch->m_nCeq_Type == 65 || pBranch->m_nCeq_Type == 66 || pBranch->m_nCeq_Type == 67 ||
						pBranch->m_nCeq_Type == 68 || pBranch->m_nCeq_Type == 69 || pBranch->m_nCeq_Type == 70 || pBranch->m_nCeq_Type == 74 || pBranch->m_nCeq_Type == 75 ||
						pBranch->m_nCeq_Type == 76 || pBranch->m_nCeq_Type == 77 || pBranch->m_nCeq_Type == 78 || pBranch->m_nCeq_Type == 110 || pBranch->m_nCeq_Type == 301 ||
						pBranch->m_nCeq_Type == 302 || pBranch->m_nCeq_Type == 303 || pBranch->m_nCeq_Type == 305
						|| pBranch->m_nCeq_Type == 304 || pBranch->m_nCeq_Type == 312 || pBranch->m_nCeq_Type == 315 || pBranch->m_nCeq_Type == 316
						|| pBranch->m_nCeq_Type == 317 || pBranch->m_nCeq_Type == 318 || pBranch->m_nCeq_Type == 319)
					{
						stCbsw.cbsw_ii_prde = m_arrPRDE_STA.GetSize() + 1;

						PRDE_STA stPRDE;
						memset(&stPRDE, 0, sizeof(PRDE_STA));
						_stprintf_s(stPRDE.PRDE_STA_NM, L"%s", pBranch->m_strName);
						_stprintf_s(stPRDE.PRDE_STA_NM, L"%s", pBranch->m_strName);
						if (pBranch->m_nCeq_Type == 58 || pBranch->m_nCeq_Type == 59 || pBranch->m_nCeq_Type == 60)
						{
							stPRDE.PRDE_OCRF_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 413));
							stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 414));
							stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
							stPRDE.PRDE_OCR_IIC = 0;
							stPRDE.PRDE_OCGRF_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 387));
							stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
							stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
							stPRDE.PRDE_OCGR_IIC = 0;
							stPRDE.PRDE_TYPE = 0;
							stPRDE.PRDE_SET_GTYPE = 0;
							stPRDE.PRDE_OCR_NOF = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 376);
							stPRDE.PRDE_OCR_NOD = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 374);//????????????? 375 376??
							if (stPRDE.PRDE_OCR_NOD == 0)
							{
								stPRDE.PRDE_OCR_NOD = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 376);//????????????? 375 376??
							}
							stPRDE.PRDE_OCGR_NOF = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 377);
							stPRDE.PRDE_OCGR_NOD = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 375);//????????????? 376 3767??
							if (stPRDE.PRDE_OCGR_NOD == 0)
							{
								stPRDE.PRDE_OCGR_NOD = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 377);//????????????? 375 376??
							}
							stPRDE.PRDE_OCRF_TMS = 0;
							stPRDE.PRDE_OCRF_TAS = 0;
							stPRDE.PRDE_OCRF_MRT = 0;
							stPRDE.PRDE_OCRD_TMS = 0;
							stPRDE.PRDE_OCRD_TAS = 0;
							stPRDE.PRDE_OCRD_MRT = 0;
							stPRDE.PRDE_OCGRF_TMS = 0;
							stPRDE.PRDE_OCGRF_TAS = 0;
							stPRDE.PRDE_OCGRF_MRT = 0;
							stPRDE.PRDE_OCGRD_TMS = 0;
							stPRDE.PRDE_OCGRD_TAS = 0;
							stPRDE.PRDE_OCGRD_MRT = 0;
							stPRDE.PRDE_OCR_CTR = 0;
							stPRDE.PRDE_OCGR_CTR = 0;
							stPRDE.PRDE_MX_LD_C_PHA = 0;
						}
						else if (pBranch->m_nCeq_Type == 65)
						{
							stPRDE.PRDE_OCRF_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 381));
							stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 297));
							stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
							stPRDE.PRDE_OCR_IIC = 0;
							stPRDE.PRDE_OCGRF_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 387));
							stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
							stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
							stPRDE.PRDE_OCGR_IIC = 0;
							stPRDE.PRDE_TYPE = 0;
							stPRDE.PRDE_SET_GTYPE = 0;
							stPRDE.PRDE_OCR_NOF = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 376);
							stPRDE.PRDE_OCR_NOD = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 374);//????????????? 375 376??
							if (stPRDE.PRDE_OCR_NOD == 0)
							{
								stPRDE.PRDE_OCR_NOD = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 376);//????????????? 375 376??
							}
							stPRDE.PRDE_OCGR_NOF = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 377);
							stPRDE.PRDE_OCGR_NOD = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 375);//????????????? 376 3767??
							if (stPRDE.PRDE_OCGR_NOD == 0)
							{
								stPRDE.PRDE_OCGR_NOD = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 377);//????????????? 375 376??
							}
							dPRDE_OCRF_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 382);
							dPRDE_OCRF_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 383);
							dPRDE_OCRF_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 384);
							dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 298);
							dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 385);
							dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 386);
							dPRDE_OCGRF_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 388);
							dPRDE_OCGRF_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 389);
							dPRDE_OCGRF_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 390);
							dPRDE_OCGRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 302);
							dPRDE_OCGRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 391);
							dPRDE_OCGRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 392);
							stPRDE.PRDE_OCRF_TMS = dPRDE_OCRF_TMS;
							stPRDE.PRDE_OCRF_TAS = dPRDE_OCRF_TAS;
							stPRDE.PRDE_OCRF_MRT = dPRDE_OCRF_MRT;
							stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
							stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
							stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
							stPRDE.PRDE_OCGRF_TMS = dPRDE_OCGRF_TMS;
							stPRDE.PRDE_OCGRF_TAS = dPRDE_OCGRF_TAS;
							stPRDE.PRDE_OCGRF_MRT = dPRDE_OCGRF_MRT;
							stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
							stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
							stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
							stPRDE.PRDE_OCR_CTR = 0;
							stPRDE.PRDE_OCGR_CTR = 0;
							stPRDE.PRDE_MX_LD_C_PHA = 0;
						}
						else if (pBranch->m_nCeq_Type == 68)
						{
							stPRDE.PRDE_OCRF_II_TCCSET = 0;
							stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 297));
							stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
							stPRDE.PRDE_OCR_IIC = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 305);
							stPRDE.PRDE_OCGRF_II_TCCSET = 0;
							stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
							stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
							stPRDE.PRDE_OCGR_IIC = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 307);
							stPRDE.PRDE_TYPE = 1;
							stPRDE.PRDE_SET_GTYPE = 1;
							stPRDE.PRDE_OCR_NOF = 0;
							stPRDE.PRDE_OCR_NOD = 0;
							stPRDE.PRDE_OCGR_NOF = 0;
							stPRDE.PRDE_OCGR_NOD = 0;
							stPRDE.PRDE_OCRF_TMS = 0;
							stPRDE.PRDE_OCRF_TAS = 0;
							stPRDE.PRDE_OCRF_MRT = 0;
							dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 298);
							dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 299);
							dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 300);
							stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
							stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
							stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
							stPRDE.PRDE_OCGRF_TMS = 0;
							stPRDE.PRDE_OCGRF_TAS = 0;
							stPRDE.PRDE_OCGRF_MRT = 0;
							dPRDE_OCGRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 302);
							dPRDE_OCGRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 303);
							dPRDE_OCGRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 304);
							stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
							stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
							stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
							stPRDE.PRDE_OCR_CTR = 120;
							stPRDE.PRDE_OCGR_CTR = 120;
							stPRDE.PRDE_MX_LD_C_PHA = 0;
						}
						else if (pBranch->m_nCeq_Type == 75)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 414));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 418);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 299);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 419);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 421));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 303));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 392));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 0;
								stPRDE.PRDE_OCGR_CTR = 0;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 301)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3 || pBranch->m_nMULTICIR_NUMBER == 4)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 496));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 500));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 497);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 498);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 499);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 501));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 502));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 503));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 120;
								stPRDE.PRDE_OCGR_CTR = 120;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 302)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3 || pBranch->m_nMULTICIR_NUMBER == 4)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 414));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 418);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 299);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 419);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 421));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 303));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 392));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 0;
								stPRDE.PRDE_OCGR_CTR = 0;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 303)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = 0;
								stPRDE.PRDE_OCR_Pickup_C = 0;
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = 0;
								stPRDE.PRDE_OCGR_Pickup_C = 0;
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								stPRDE.PRDE_OCRD_TMS = 0;
								stPRDE.PRDE_OCRD_TAS = 0;
								stPRDE.PRDE_OCRD_MRT = 0;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								stPRDE.PRDE_OCGRD_TMS = 0;
								stPRDE.PRDE_OCGRD_TAS = 0;
								stPRDE.PRDE_OCGRD_MRT = 0;
								stPRDE.PRDE_OCR_CTR = 0;
								stPRDE.PRDE_OCGR_CTR = 0;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
								stPRDE.PRDE_OCR_CTR = 120;
								stPRDE.PRDE_OCGR_CTR = 120;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 305)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 414));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 418);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 299);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 419);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 421));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 303));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 392));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 120;
								stPRDE.PRDE_OCGR_CTR = 120;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 304 || pBranch->m_nCeq_Type == 315 || pBranch->m_nCeq_Type == 316) //20221011
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 414));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 418);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 299);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 419);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 421));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 303));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 392));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 120;
								stPRDE.PRDE_OCGR_CTR = 120;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 312)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2 || pBranch->m_nMULTICIR_NUMBER == 3)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 496));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 500));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 497);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 498);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 499);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 501));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 502));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 503));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 120;
								stPRDE.PRDE_OCGR_CTR = 120;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 317)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 414));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 418);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 299);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 419);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 421));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 303));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 392));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 120;
								stPRDE.PRDE_OCGR_CTR = 120;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 318)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 414));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 301));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 418);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 299);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 419);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 421));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 303));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 392));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 120;
								stPRDE.PRDE_OCGR_CTR = 120;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else if (pBranch->m_nCeq_Type == 319)
						{
							if (pBranch->m_nMULTICIR_NUMBER == 2)
							{
								stPRDE.PRDE_OCRF_II_TCCSET = 0;
								stPRDE.PRDE_OCRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 496));
								stPRDE.PRDE_OCR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 12);
								stPRDE.PRDE_OCR_IIC = 0;
								stPRDE.PRDE_OCGRF_II_TCCSET = 0;
								stPRDE.PRDE_OCGRD_II_TCCSET = GET_PRDE_TCCSET(GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 500));
								stPRDE.PRDE_OCGR_Pickup_C = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 13);
								stPRDE.PRDE_OCGR_IIC = 0;
								stPRDE.PRDE_TYPE = 1;
								stPRDE.PRDE_SET_GTYPE = 1;
								stPRDE.PRDE_OCR_NOF = 0;
								stPRDE.PRDE_OCR_NOD = 0;
								stPRDE.PRDE_OCGR_NOF = 0;
								stPRDE.PRDE_OCGR_NOD = 0;
								stPRDE.PRDE_OCRF_TMS = 0;
								stPRDE.PRDE_OCRF_TAS = 0;
								stPRDE.PRDE_OCRF_MRT = 0;
								dPRDE_OCRD_TMS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 497);
								dPRDE_OCRD_TAS = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 498);
								dPRDE_OCRD_MRT = GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 499);
								stPRDE.PRDE_OCRD_TMS = dPRDE_OCRD_TMS;
								stPRDE.PRDE_OCRD_TAS = dPRDE_OCRD_TAS;
								stPRDE.PRDE_OCRD_MRT = dPRDE_OCRD_MRT;
								stPRDE.PRDE_OCGRF_TMS = 0;
								stPRDE.PRDE_OCGRF_TAS = 0;
								stPRDE.PRDE_OCGRF_MRT = 0;
								dPRDE_OCGRD_TMS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 501));
								dPRDE_OCGRD_TAS = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 502));
								dPRDE_OCGRD_MRT = (GET_PRDE_AO_VALUE(pBranch->m_strMRID, pBranch->m_nCeq_Type, 503));
								stPRDE.PRDE_OCGRD_TMS = dPRDE_OCGRD_TMS;
								stPRDE.PRDE_OCGRD_TAS = dPRDE_OCGRD_TAS;
								stPRDE.PRDE_OCGRD_MRT = dPRDE_OCGRD_MRT;
								stPRDE.PRDE_OCR_CTR = 120;
								stPRDE.PRDE_OCGR_CTR = 120;
								stPRDE.PRDE_MX_LD_C_PHA = 0;
							}
						}
						else
						{
							stPRDE.PRDE_OCRF_II_TCCSET = 0;
							stPRDE.PRDE_OCRD_II_TCCSET = 0;
							stPRDE.PRDE_OCR_Pickup_C = 0;
							stPRDE.PRDE_OCR_IIC = 0;
							stPRDE.PRDE_OCGRF_II_TCCSET = 0;
							stPRDE.PRDE_OCGRD_II_TCCSET = 0;
							stPRDE.PRDE_OCGR_Pickup_C = 0;
							stPRDE.PRDE_OCGR_IIC = 0;
							stPRDE.PRDE_TYPE = 1;
							stPRDE.PRDE_SET_GTYPE = 1;
							stPRDE.PRDE_OCR_NOF = 0;
							stPRDE.PRDE_OCR_NOD = 0;
							stPRDE.PRDE_OCGR_NOF = 0;
							stPRDE.PRDE_OCGR_NOD = 0;
							stPRDE.PRDE_OCRF_TMS = 0;
							stPRDE.PRDE_OCRF_TAS = 0;
							stPRDE.PRDE_OCRF_MRT = 0;
							stPRDE.PRDE_OCRD_TMS = 0;
							stPRDE.PRDE_OCRD_TAS = 0;
							stPRDE.PRDE_OCRD_MRT = 0;
							stPRDE.PRDE_OCGRF_TMS = 0;
							stPRDE.PRDE_OCGRF_TAS = 0;
							stPRDE.PRDE_OCGRF_MRT = 0;
							stPRDE.PRDE_OCGRD_TMS = 0;
							stPRDE.PRDE_OCGRD_TAS = 0;
							stPRDE.PRDE_OCGRD_MRT = 0;
							stPRDE.PRDE_OCR_CTR = 0;
							stPRDE.PRDE_OCGR_CTR = 0;
							stPRDE.PRDE_MX_LD_C_PHA = 0;
							stPRDE.PRDE_OCR_CTR = 120;
							stPRDE.PRDE_OCGR_CTR = 120;
							stPRDE.PRDE_MX_LD_C_PHA = 0;
						}
						m_arrPRDE_STA.Add(stPRDE);
					}
					/////////////////
					m_arrCBSW.Add(stCbsw);
				}
				else
				{
					CString stPQMS;
					stPQMS.Format(_T(""));
					//MRID 변경 하는부분입력해야함 !
					//CBSW
					CBSW_STA stCbsw;
					memset(&stCbsw, 0, sizeof(CBSW_STA));
					//PQ작업때문에
					_stprintf_s(stCbsw.cbsw_nm, L"%s", pBranch->m_strName);
					_stprintf_s(stCbsw.cbsw_ceqid, L"%s", pBranch->m_strMRID);
					stCbsw.cbsw_type = nType;
					stCbsw.cbsw_norstat = pBranch->m_bStatus;
					stCbsw.cbsw_rtutype = pBranch->m_nCeq_Type;
					m_map_CEQTYPE_RTUCODE.Lookup(pBranch->m_nCeq_Type, nCbsw_RTUCODE);
					stCbsw.cbsw_RTUCODE = nCbsw_RTUCODE;
					m_map_SW_comm_group.Lookup(pBranch->m_strMRID, nComType);
					stCbsw.cbsw_comtype = nComType;
					//

					if (stCbsw.cbsw_norstat == 1)
					{
						stCbsw.cbsw_norstat = 0;
					}
					else
					{
						stCbsw.cbsw_norstat = 1;
					}

					stCbsw.cbsw_ii_dl = 0;
					if (pNode1 == NULL) continue;
					//20230830  

					//stCbsw.cbsw_ii_fnd = stCbsw.cbsw_ii_fgnd = pNode1->m_nNDID;
					if (pNode2 == NULL)
					{
						stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = 0;
						stCbsw.cbsw_ii_dl = 0;
					}
					else
					{
						stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = pNode2->m_nNDID;
						stCbsw.cbsw_ii_dl = pBranch->m_nDLID;
					}
					//20230830
					nNAME_TYPE_FK1 = 0;
					nNAME_TYPE_FK2 = 0;
					m_map_Ter_CNID_NAMETYPEFK.Lookup(pNode1->m_strMRID, nNAME_TYPE_FK1);
					m_map_Ter_CNID_NAMETYPEFK.Lookup(pNode2->m_strMRID, nNAME_TYPE_FK2);
					if (nNAME_TYPE_FK1 == 51) //1번노드가 BR이면 TNO 에 입력
					{
						stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = pNode1->m_nNDID;
						stCbsw.cbsw_ii_fnd = stCbsw.cbsw_ii_fgnd = pNode2->m_nNDID;
					}
					if (nNAME_TYPE_FK2 == 51) // 2번 노드가 BR 이면 TND 에 입력
					{
						stCbsw.cbsw_ii_fnd = stCbsw.cbsw_ii_fgnd = pNode1->m_nNDID;
						stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = pNode2->m_nNDID;
					}
					if (nNAME_TYPE_FK1 != 51 && nNAME_TYPE_FK2 != 51) //BR이 없어서 ????
					{
						if (nNAME_TYPE_FK1 == 15 || nNAME_TYPE_FK1 == 0) //1번 노드가 DS 면 TNO 2번
						{
							stCbsw.cbsw_ii_fnd = stCbsw.cbsw_ii_fgnd = pNode1->m_nNDID;
							stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = pNode2->m_nNDID;
						}
						if (nNAME_TYPE_FK2 == 15 || nNAME_TYPE_FK2 == 0) // 2번 노드가 DB TND 는 1번
						{
							stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = pNode1->m_nNDID;
							stCbsw.cbsw_ii_fnd = stCbsw.cbsw_ii_fgnd = pNode2->m_nNDID;
						}
					}
					//


					_stprintf_s(stCbsw.cbsw_multisw_id, L"%s", pBranch->m_szMULTISW);
					stCbsw.cbsw_multicir_number = pBranch->m_nMULTICIR_NUMBER;
					//20211119
					if (!(pBranch->m_szMULTISW == _T("0")))
					{
						m_map_SW_comm_group.Lookup(pBranch->m_szMULTISW, nComType);
						stCbsw.cbsw_comtype = nComType;
					}

					//필요없는ocb를 제거 하기 위한 방편!!!
					if (stCbsw.cbsw_ii_tnd != 0)
					{
						stCbsw.cbsw_ii_prde = m_arrPRDE_STA.GetSize() + 1;
						//20220610 추가 PQMS
						m_map_CBSW_PQMS.Lookup(pBranch->m_strMRID, stPQMS);
						if (!(stPQMS.IsEmpty()))
						{
							if (pBranch->m_nCeq_Type == 71)
							{
								_stprintf_s(stCbsw.cbsw_nm, L"%s_PQMS", pBranch->m_strName);
								_stprintf_s(stCbsw.cbsw_multisw_id, L"%s", stPQMS);
								stCbsw.cbsw_multicir_number = 0;
							}
						}
						//
						m_arrCBSW.Add(stCbsw);


						//CB일때 생성
						PRDE_STA stPRDE;
						memset(&stPRDE, 0, sizeof(PRDE_STA));
						_stprintf_s(stPRDE.PRDE_STA_NM, L"%s", pBranch->m_strName);
						stPRDE.PRDE_OCRF_II_TCCSET = 0;
						stPRDE.PRDE_OCRD_II_TCCSET = 0;
						stPRDE.PRDE_OCR_Pickup_C = 0;
						stPRDE.PRDE_OCR_IIC = 0;
						stPRDE.PRDE_OCGRF_II_TCCSET = 0;
						stPRDE.PRDE_OCGRD_II_TCCSET = 0;
						stPRDE.PRDE_OCGR_Pickup_C = 0;
						stPRDE.PRDE_OCGR_IIC = 0;
						stPRDE.PRDE_TYPE = 0;
						stPRDE.PRDE_SET_GTYPE = 0;
						stPRDE.PRDE_OCR_NOF = 0;
						stPRDE.PRDE_OCR_NOD = 0;
						stPRDE.PRDE_OCGR_NOF = 0;
						stPRDE.PRDE_OCGR_NOD = 0;
						stPRDE.PRDE_OCRF_TMS = 0;
						stPRDE.PRDE_OCRF_TAS = 0;
						stPRDE.PRDE_OCRF_MRT = 0;
						stPRDE.PRDE_OCRD_TMS = 0;
						stPRDE.PRDE_OCRD_TAS = 0;
						stPRDE.PRDE_OCRD_MRT = 0;
						stPRDE.PRDE_OCGRF_TMS = 0;
						stPRDE.PRDE_OCGRF_TAS = 0;
						stPRDE.PRDE_OCGRF_MRT = 0;
						stPRDE.PRDE_OCGRD_TMS = 0;
						stPRDE.PRDE_OCGRD_TAS = 0;
						stPRDE.PRDE_OCGRD_MRT = 0;
						stPRDE.PRDE_OCR_CTR = 120;
						stPRDE.PRDE_OCGR_CTR = 120;
						stPRDE.PRDE_MX_LD_C_PHA = 0;
						m_arrPRDE_STA.Add(stPRDE);
					}
				}
			}
		}
		else if (pBranch->m_nKCIM_Type == 2) //lnsec
		{
			if (pNode1 == NULL && pNode2 == NULL)
			{
				continue;
			}
			if (pNode1 == NULL)
			{
				pNode1 = GetNodeTER_NEW_LNSEC(pBranch);
			}
			if (pNode2 == NULL)
			{
				pNode2 = GetNodeTER_NEW_LNSEC(pBranch);
			}


			if (pBranch->m_strName.IsEmpty())
			{
				pBranch->m_strName.Format(_T("%s"), pBranch->m_strMRID);
			}
			//BR
			BR_STA stBr;
			memset(&stBr, 0, sizeof(BR_STA));
			_stprintf_s(stBr.br_nm, L"%s", pBranch->m_strName);
			stBr.br_posr = Get_POSR((float)pBranch->m_dLength, pBranch->m_nLT);
			stBr.br_posx = Get_POSX((float)pBranch->m_dLength, pBranch->m_nLT);
			stBr.br_zerr = Get_ZERR((float)pBranch->m_dLength, pBranch->m_nLT);
			stBr.br_zerx = Get_ZERX((float)pBranch->m_dLength, pBranch->m_nLT);
			stBr.br_norlm = 100;
			stBr.br_ii_fnd = pNode1->m_nNDID;
			stBr.br_ii_tnd = pNode2->m_nNDID;
			stBr.br_ii_equ = (int)m_arrLNSEC.GetSize() + 1;
			stBr.br_ii_gbr = (int)m_arrGBR.GetSize() + 1;
			stBr.br_ii_dl = pBranch->m_nDLID;
			stBr.br_ii_equty = 1;
			m_arrBR.Add(stBr);
			//GBR 생성
			GBR_STA stGbr;
			memset(&stGbr, 0, sizeof(GBR_STA));
			_stprintf_s(stGbr.gbr_nm, L"%s", pBranch->m_strName);
			stGbr.gbr_ii_equ = (int)m_arrLNSEC.GetSize() + 1;
			stGbr.gbr_ii_fgnd = pNode1->m_nNDID;
			stGbr.gbr_ii_tgnd = pNode2->m_nNDID;
			stGbr.gbr_posr = stBr.br_posr;
			stGbr.gbr_posx = stBr.br_posx;
			stGbr.gbr_zerr = stBr.br_zerr;
			stGbr.gbr_zerx = stBr.br_zerx;
			stGbr.gbr_hi_br = (int)m_arrBR.GetSize();
			stGbr.gbr_ii_equty = 1;
			stGbr.gbr_ii_dl = pBranch->m_nDLID;
			m_arrGBR.Add(stGbr);
			//LD
			if (!(pNode2->m_nKind == 37 || pNode2->m_nKind == 28) && !(pNode1->m_nKind == 37 || pNode1->m_nKind == 28))
			{
				LD_STA stLd;
				memset(&stLd, 0, sizeof(LD_STA));
				_stprintf_s(stLd.ld_nm, L"%s", pBranch->m_strName);
				stLd.ld_ii_gnd = pNode2->m_nNDID;
				stLd.ld_ii_ij = (int)m_arrIJ.GetSize() + 1;
				m_arrLD.Add(stLd);
			}

			//LD
			//IJ
			IJ_STA stIj;
			memset(&stIj, 0, sizeof(IJ_STA));
			_stprintf_s(stIj.ij_nm, L"%s", pBranch->m_strName);
			stIj.ij_ii_equty = 6;
			stIj.ij_ii_equ = m_nIJ_STA_EQU6++;
			stIj.ij_ii_dl = pBranch->m_nDLID;
			stIj.ij_ii_gnd = pNode2->m_nNDID;
			m_arrIJ.Add(stIj);
			//IJ

			/////////////////////////////////////////////////////////////////////////
			//BR
			//LNSEC
			LNSEC_STA stLnsec;
			memset(&stLnsec, 0, sizeof(LNSEC_STA));
			_stprintf_s(stLnsec.lnsec_nm, L"%s", pBranch->m_strName);
			_stprintf_s(stLnsec.lnsec_ceqid, L"%s", pBranch->m_strMRID);
			stLnsec.lnsec_ii_br = (int)m_arrBR.GetSize();
			stLnsec.lnsec_length = pBranch->m_dLength;
			stLnsec.lnsec_type_id = pBranch->m_nLT;

			stLnsec.lnsec_II_FND = pNode1->m_nNDID;
			stLnsec.lnsec_II_TND = pNode2->m_nNDID;
			stLnsec.lnsec_constype = Get_OVERHEAD_CABLE(pBranch->m_nLT);

			//m_map_LNSEC_ND
			m_map_LNSEC_ND.SetAt(pBranch->m_strMRID, pNode2->m_nNDID);
			m_map_LNSEC_GEN.SetAt(pBranch->m_strMRID, m_arrLNSEC.GetSize() + 1);

			m_arrLNSEC.Add(stLnsec);
			/////
		}
	}

	

	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[5/7][4/8]========== ADMStoKASIM 데이터 변환 [계통 탐색 - Subs Index] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	for (i = 0; i < m_arrSS.GetSize(); i++)
	{
		m_arrSS[i].ss_hi_mtr = 0;
		m_arrSS[i].ss_hi_snv = GetSS_HI_SNV(i + 1);
	}

	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[5/7][5/8]========== ADMStoKASIM 데이터 변환 [계통 탐색 - GenUnit] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();
	int nLNSEC_ND = 0;
	CString szCEQ_Check;
	CString sdsdsdsd;
	int nGEN_Size = 0;
	int nLNSEC_GEN_INDEX = 0;
	szCEQ_Check.Format(_T(""));
	for (int i = 0; i < m_arrGENUNIT.GetSize(); i++)
	{
		if (m_arrGENUNIT[i].GENUNIT_II_GEN != 0) continue;
		if (m_arrGENUNIT[i].GENUNIT_II_NAME_TYPE_FK == 51)
		{
			if (m_arrGENUNIT[i].GENUNIT_II_EQU_ID == szCEQ_Check)
			{
				m_arrGENUNIT[i].GENUNIT_II_GEN = nGEN_Size;
				m_arrGEN[nGEN_Size - 1].fGEN_ITR_CAP = m_arrGEN[nGEN_Size - 1].fGEN_ITR_CAP + m_arrGENUNIT[i].GENUNIT_CAP_KW;
				continue;
			}
			else
			{
				szCEQ_Check.Format(_T("%s"), m_arrGENUNIT[i].GENUNIT_II_EQU_ID);
			}
			m_map_LNSEC_ND.Lookup(m_arrGENUNIT[i].GENUNIT_II_EQU_ID, nLNSEC_ND);
			if (nLNSEC_ND == 0)
			{
				//연결된 노드가 없을때 쓰려고  10895100001604
				//delete m_arrGENUNIT;
				m_arrGENUNIT.RemoveAt(i);
				i--;
				continue;
			}
			m_map_LNSEC_GEN.Lookup(m_arrGENUNIT[i].GENUNIT_II_EQU_ID, nLNSEC_GEN_INDEX); //20230630

			//snv찾기
			int nSNV = 0;			nSNV = m_arrND[nLNSEC_ND - 1].nd_ii_snv;
			//DL찾기
			nCHECK_DLID = GetDLID(m_arrGENUNIT[i].GENUNIT_II_EQU_ID); //DL 찾기
			if (nCHECK_DLID == 0)
			{
				nCHECK_DLID = GetDLID_ORIGINAL(m_arrGENUNIT[i].GENUNIT_II_EQU_ID); //DL 찾기
			}
			//GEN만들기!!!
			CString stNM, stCEQ;

			stNM.Format(_T("%s"), m_arrGENUNIT[i].GENUNIT_NM);
			stCEQ.Format(_T("999951%08d"), m_arrLNSEC.GetSize() + 1);
			ND_STA stNd1;
			memset(&stNd1, 0, sizeof(ND_STA));
			_stprintf_s(stNd1.nd_nm, L"%s", stNM);
			_stprintf_s(stNd1.nd_ceqid, L"%s", stCEQ);
			_stprintf_s(stNd1.nd_connectivitynodeid, L"%s99", stCEQ);
			stNd1.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
			stNd1.nd_ii_snv = nSNV;
			m_arrND.Add(stNd1);

			GND_STA stGND1;
			memset(&stGND1, 0, sizeof(GND_STA));
			_stprintf_s(stGND1.gnd_nm, L"%s", stNM);
			stGND1.gnd_hi_nd = (int)m_arrND.GetSize();
			stGND1.gnd_hi_fgbr = (int)m_arrBR.GetSize() + 1;
			m_arrGND.Add(stGND1);

			BR_STA stBr;
			memset(&stBr, 0, sizeof(BR_STA));
			_stprintf_s(stBr.br_nm, L"%s", stNM);
			stBr.br_posr = 0.001;
			stBr.br_posx = 0.001;
			stBr.br_zerr = 0.001;
			stBr.br_zerx = 0.001;
			stBr.br_norlm = 100;
			stBr.br_ii_fnd = nLNSEC_ND;
			stBr.br_ii_tnd = m_arrND.GetSize();
			stBr.br_ii_equ = (int)m_arrLNSEC.GetSize() + 1;
			stBr.br_ii_gbr = (int)m_arrGBR.GetSize() + 1;
			stBr.br_ii_dl = nCHECK_DLID;
			stBr.br_ii_equty = 1;
			m_arrBR.Add(stBr);
			//GBR 생성
			GBR_STA stGbr;
			memset(&stGbr, 0, sizeof(GBR_STA));
			_stprintf_s(stGbr.gbr_nm, L"%s", stNM);
			stGbr.gbr_ii_equ = (int)m_arrLNSEC.GetSize() + 1;
			stGbr.gbr_ii_fgnd = nLNSEC_ND;
			stGbr.gbr_ii_tgnd = m_arrND.GetSize();
			stGbr.gbr_posr = stBr.br_posr;
			stGbr.gbr_posx = stBr.br_posx;
			stGbr.gbr_zerr = stBr.br_zerr;
			stGbr.gbr_zerx = stBr.br_zerx;
			stGbr.gbr_hi_br = (int)m_arrBR.GetSize();
			stGbr.gbr_ii_equty = 1;
			stGbr.gbr_ii_dl = nCHECK_DLID;
			m_arrGBR.Add(stGbr);
			//LNSEC
			LNSEC_STA stLnsec;
			memset(&stLnsec, 0, sizeof(LNSEC_STA));
			_stprintf_s(stLnsec.lnsec_nm, L"%s", stNM);
			_stprintf_s(stLnsec.lnsec_ceqid, L"%s", stCEQ);
			stLnsec.lnsec_ii_br = (int)m_arrBR.GetSize();
			stLnsec.lnsec_length = 0.001;
			stLnsec.lnsec_type_id = 1;
			stLnsec.lnsec_constype = 0;
			m_arrLNSEC.Add(stLnsec);
			//가상-
			GEN_STA stGEN;
			memset(&stGEN, 0, sizeof(GEN_STA));
			_stprintf_s(stGEN.gen_nm, L"%s", stNM);
			_stprintf_s(stGEN.gen_ceqid, L"%s", m_arrGENUNIT[i].GENUNIT_II_EQU_ID);
			stGEN.gen_namkv = 22.9;
			stGEN.gen_mvarlmmx = 0.03;
			stGEN.gen_type = 11; //210728	//
			//stGEN.GEN_TREXCL = 4; //가상 GEN 일때 만드는 부분 
			stGEN.GEN_TREXCL = m_arrGENUNIT[i].GENUNIT_CONNECTION_TR_TYPE; //20230704
			stGEN.gen_vol_cls = 0;
			stGEN.gen_conndrep = 1;
			stGEN.gen_contype = 7;//3상
			stGEN.gen_pf = 0.95;
			stGEN.gen_eff = 0.85;
			stGEN.gen_noofp = 4;
			stGEN.gen_avr = 1;
			stGEN.gen_ii_nd = stGEN.gen_ii_gnd = stGEN.gen_ii_connd = m_arrND.GetSize();
			stGEN.gen_ii_ij = (int)m_arrIJ.GetSize() + 1;
			stGEN.GEN_II_PRDE = (int)m_arrPRDE_STA.GetSize() + 1;
			stGEN.fGEN_ITR_R = 0;
			stGEN.fGEN_ITR_X = 6;
			stGEN.nGEN_ITR_WDC = 1; //20231114 기본값?
			stGEN.fGEN_ITR_CAP = m_arrGENUNIT[i].GENUNIT_CAP_KW;

			stGEN.GEN_SI_LNSEC = 0;	//20230629
			stGEN.GEN_II_LNSEC = nLNSEC_GEN_INDEX; //20230629
			m_arrGEN.Add(stGEN);

			//20220823
			m_arrGENUNIT[i].GENUNIT_II_GEN = m_arrGEN.GetSize();
			nGEN_Size = m_arrGEN.GetSize();
			//
			IJ_STA stIj;
			memset(&stIj, 0, sizeof(IJ_STA));
			_stprintf_s(stIj.ij_nm, L"%s", m_arrGENUNIT[i].GENUNIT_NM);
			stIj.ij_ii_equty = 4;  //고객 8 선로 6 발전기4
			stIj.ij_ii_equ = m_nIJ_STA_EQU4++;
			stIj.ij_ii_gnd = stGEN.gen_ii_gnd;
			stIj.ij_ii_dl = nCHECK_DLID;
			m_arrIJ.Add(stIj);

			// 발전기
			PRDE_STA stPRDE;
			memset(&stPRDE, 0, sizeof(PRDE_STA));
			_stprintf_s(stPRDE.PRDE_STA_NM, L"%s", m_arrGENUNIT[i].GENUNIT_NM);
			stPRDE.PRDE_OCR_CTR = 120;
			stPRDE.PRDE_OCGR_CTR = 120;
			m_arrPRDE_STA.Add(stPRDE);
		}
	}

	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[5/7][6/8]========== ADMStoKASIM 데이터 변환 [계통 탐색 - Subs] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	int a = 0;
	int nFND, nTND;
	CString szSSNM;
	CString szConnectivitynode;
	CString szDate;
	for (int i = 0; i < m_arrSS.GetSize(); i++)
	{
		m_arrSS[i].ss_nd = (int)m_arrND.GetSize() + 1;
		nFND = (int)m_arrND.GetSize() + 1;
		szSSNM.Format(_T("%s"), m_arrSS[i].ss_nm);

		ND_STA stNd;
		memset(&stNd, 0, sizeof(ND_STA));

		szConnectivitynode.Format(_T("99996100%08d"), i + 1);
		_stprintf_s(stNd.nd_nm, L"%s", m_arrSS[i].ss_nm);
		_stprintf_s(stNd.nd_ceqid, L"999961%08d", i + 1);
		_stprintf_s(stNd.nd_connectivitynodeid, L"999961%08d00", i + 1);
		stNd.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
		stNd.nd_ii_snv = m_arrSS[i].ss_hi_snv;

		stNd.nd_hi_fcbsw = 0;
		stNd.nd_hi_tcbsw = 0;
		stNd.nd_hi_fbr = (int)m_arrBR.GetSize() + 1;
		stNd.nd_hi_tbr = 0;
		stNd.nd_si_snv = 0;
		stNd.nd_hi_svc = 0;
		stNd.nd_hi_gen = (int)m_arrGEN.GetSize() + 1;
		stNd.nd_si_gnd = 0;

		m_arrND.Add(stNd);

		GND_STA stGND;
		memset(&stGND, 0, sizeof(GND_STA));
		_stprintf_s(stGND.gnd_nm, L"%s", m_arrSS[i].ss_nm);
		stGND.gnd_hi_nd = (int)m_arrND.GetSize();
		stGND.gnd_hi_fgbr = (int)m_arrBR.GetSize() + 1;
		m_arrGND.Add(stGND);

		GEN_STA stGEN;
		memset(&stGEN, 0, sizeof(GEN_STA));

		szDate.Format(_T("999961%08d"), i + 1);
		_stprintf_s(stGEN.gen_nm, L"%s", m_arrSS[i].ss_nm);
		_stprintf_s(stGEN.gen_ceqid, L"999961%08d", i + 1);
		stGEN.gen_namkv = 154;
		stGEN.gen_mwlmmx = 0.01;
		stGEN.gen_mwlmmn = 0;
		stGEN.gen_mvarlmmx = 0.03;
		stGEN.gen_mvarlmmn = 0;
		stGEN.gen_r = 0;
		stGEN.gen_stx = 0;
		stGEN.gen_ssx = 0;
		stGEN.gen_type = 1; //20221011
		stGEN.gen_vol_cls = 0;
		stGEN.gen_conndrep = 1;
		stGEN.gen_contype = 7;//3상
		stGEN.gen_pf = 0.95;
		stGEN.gen_eff = 0.85;
		stGEN.gen_noofp = 4;
		stGEN.gen_avr = 1;
		stGEN.GEN_TREXCL = 2;
		stGEN.GEN_SI_LNSEC = 0;	//20230629
		stGEN.GEN_II_LNSEC = 0; //20230629 - 20231031 삭제요청
		stGEN.fGEN_ITR_R = 0;
		stGEN.fGEN_ITR_X = 6;
		stGEN.nGEN_ITR_WDC = 1;

		stGEN.gen_ii_nd = stGEN.gen_ii_gnd = stGEN.gen_ii_connd = (int)m_arrND.GetSize();
		stGEN.gen_ii_ij = (int)m_arrIJ.GetSize() + 1;
		stGEN.GEN_II_PRDE = (int)m_arrPRDE_STA.GetSize() + 1;
		m_arrGEN.Add(stGEN);

		//20220530 추가
		m_arrSS[i].ss_ii_vgen = m_arrGEN.GetSize();


		IJ_STA stIj;
		memset(&stIj, 0, sizeof(IJ_STA));
		_stprintf_s(stIj.ij_nm, L"%s", m_arrSS[i].ss_nm);
		stIj.ij_ii_equty = 4;
		stIj.ij_ii_equ = m_nIJ_STA_EQU4++;
		stIj.ij_ii_gnd = stGEN.gen_ii_gnd;
		m_arrIJ.Add(stIj);

		// 고객입력시 
		PRDE_STA stPRDE;
		memset(&stPRDE, 0, sizeof(PRDE_STA));
		_stprintf_s(stPRDE.PRDE_STA_NM, L"%s", m_arrSS[i].ss_nm);
		stPRDE.PRDE_OCRF_II_TCCSET = 0;
		stPRDE.PRDE_OCRD_II_TCCSET = 0;
		stPRDE.PRDE_OCR_Pickup_C = 0;
		stPRDE.PRDE_OCR_IIC = 0;
		stPRDE.PRDE_OCGRF_II_TCCSET = 0;
		stPRDE.PRDE_OCGRD_II_TCCSET = 0;
		stPRDE.PRDE_OCGR_Pickup_C = 0;
		stPRDE.PRDE_OCGR_IIC = 0;
		stPRDE.PRDE_TYPE = 0;
		stPRDE.PRDE_SET_GTYPE = 0;
		stPRDE.PRDE_OCR_NOF = 0;
		stPRDE.PRDE_OCR_NOD = 0;
		stPRDE.PRDE_OCGR_NOF = 0;
		stPRDE.PRDE_OCGR_NOD = 0;
		stPRDE.PRDE_OCRF_TMS = 0;
		stPRDE.PRDE_OCRF_TAS = 0;
		stPRDE.PRDE_OCRF_MRT = 0;
		stPRDE.PRDE_OCRD_TMS = 0;
		stPRDE.PRDE_OCRD_TAS = 0;
		stPRDE.PRDE_OCRD_MRT = 0;
		stPRDE.PRDE_OCGRF_TMS = 0;
		stPRDE.PRDE_OCGRF_TAS = 0;
		stPRDE.PRDE_OCGRF_MRT = 0;
		stPRDE.PRDE_OCGRD_TMS = 0;
		stPRDE.PRDE_OCGRD_TAS = 0;
		stPRDE.PRDE_OCGRD_MRT = 0;
		stPRDE.PRDE_OCR_CTR = 120;
		stPRDE.PRDE_OCGR_CTR = 120;
		stPRDE.PRDE_MX_LD_C_PHA = 0;
		m_arrPRDE_STA.Add(stPRDE);
		//////////////////////////////////////////////////////////////////////////////
		//선로
		nTND = (int)m_arrND.GetSize() + 1;

		ND_STA stNd1;
		memset(&stNd1, 0, sizeof(ND_STA));

		szSSNM.Format(_T("%s-BR"), m_arrSS[i].ss_nm);
		szDate.Format(_T("99995100%08d"), i + 1);
		_stprintf_s(stNd1.nd_nm, L"%s-BR", m_arrSS[i].ss_nm);
		_stprintf_s(stNd1.nd_ceqid, L"999951%08d", i + 1);
		_stprintf_s(stNd1.nd_connectivitynodeid, L"999951%08d00", i + 1);
		stNd1.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
		stNd1.nd_ii_snv = m_arrSS[i].ss_hi_snv;

		stNd1.nd_hi_fcbsw = 0;
		stNd1.nd_hi_tcbsw = 0;
		stNd1.nd_hi_fbr = 0;
		stNd1.nd_hi_tbr = 0;
		stNd1.nd_si_snv = 0;
		stNd1.nd_hi_svc = 0;
		stNd1.nd_hi_gen = 0;
		stNd1.nd_si_gnd = 0;
		stNd1.nd_hi_shunteq = 0;

		m_arrND.Add(stNd1);

		GND_STA stGND1;
		memset(&stGND1, 0, sizeof(GND_STA));
		_stprintf_s(stGND1.gnd_nm, L"%s-BR", m_arrSS[i].ss_nm);
		stGND1.gnd_hi_nd = (int)m_arrND.GetSize();
		stGND1.gnd_hi_fgbr = (int)m_arrBR.GetSize() + 1;
		m_arrGND.Add(stGND1);

		BR_STA stBr;
		memset(&stBr, 0, sizeof(BR_STA));
		_stprintf_s(stBr.br_nm, L"%s-BR", m_arrSS[i].ss_nm);
		stBr.br_posr = 0.001;
		stBr.br_posx = 0.001;
		stBr.br_zerr = 0.001;
		stBr.br_zerx = 0.001;
		stBr.br_norlm = 100;
		stBr.br_ii_fnd = nFND;
		stBr.br_ii_tnd = nTND;
		stBr.br_ii_equ = (int)m_arrLNSEC.GetSize() + 1;
		stBr.br_ii_gbr = (int)m_arrGBR.GetSize() + 1;
		stBr.br_ii_dl = 0;
		stBr.br_ii_equty = 1;
		m_arrBR.Add(stBr);
		//GBR 생성
		GBR_STA stGbr;
		memset(&stGbr, 0, sizeof(GBR_STA));
		_stprintf_s(stGbr.gbr_nm, L"%s", m_arrSS[i].ss_nm);
		stGbr.gbr_ii_equ = (int)m_arrLNSEC.GetSize() + 1;
		stGbr.gbr_ii_fgnd = nFND;
		stGbr.gbr_ii_tgnd = nTND;
		stGbr.gbr_posr = stBr.br_posr;
		stGbr.gbr_posx = stBr.br_posx;
		stGbr.gbr_zerr = stBr.br_zerr;
		stGbr.gbr_zerx = stBr.br_zerx;
		stGbr.gbr_hi_br = (int)m_arrBR.GetSize();
		stGbr.gbr_ii_equty = 1;
		stGbr.gbr_ii_dl = 0;
		m_arrGBR.Add(stGbr);
		//LNSEC
		LNSEC_STA stLnsec;
		memset(&stLnsec, 0, sizeof(LNSEC_STA));
		szSSNM.Format(_T("%s"), m_arrSS[i].ss_nm);
		szDate.Format(_T("999951%08d"), i + 1);
		_stprintf_s(stLnsec.lnsec_nm, L"%s", m_arrSS[i].ss_nm);
		_stprintf_s(stLnsec.lnsec_ceqid, L"999951%08d", i + 1);
		stLnsec.lnsec_ii_br = (int)m_arrBR.GetSize();
		stLnsec.lnsec_length = 0.001;
		stLnsec.lnsec_type_id = 1;
		stLnsec.lnsec_II_FND = nFND;
		stLnsec.lnsec_II_TND = nTND;
		stLnsec.lnsec_constype = 0;
		m_arrLNSEC.Add(stLnsec);
	}


	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[5/7][7/8]========== ADMStoKASIM 데이터 변환 [계통 탐색 - BR] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	int nSSID;
	int nDS1, nDS2, nDS3, nDS5;
	int nCB1, nCB2, nCB3;
	for (int i = 0; i < m_arrBR.GetSize(); i++)
	{
		if (m_arrBR[i].br_ii_equty == 2 && m_arrBR[i].br_ii_dl == 0)
		{
			nTND = (int)m_arrND.GetSize() + 1;

			CString szCEQ;
			szCEQ.Format(_T("%s"), m_arrBR[i].br_ceq);

			m_arrBR[i].br_ii_fnd = SET_br_ii_fnd(szCEQ);
			m_arrBR[i].br_ii_tnd = nTND;

			m_arrGBR[i].gbr_ii_fgnd = m_arrBR[i].br_ii_fnd;
			m_arrGBR[i].gbr_ii_tgnd = m_arrBR[i].br_ii_tnd;

			////////////////////////////////////////////////////////
			nSSID = SET_SSID(szCEQ);
			//CBSW 생성 DS-1
			ND_STA stNd;
			memset(&stNd, 0, sizeof(ND_STA));

			szSSNM.Format(_T("%sDS-1"), m_arrBR[i].br_nm);
			szDate.Format(_T("99991201%08d"), i + 1);
			_stprintf_s(stNd.nd_nm, L"%sDS-1", m_arrBR[i].br_nm);
			_stprintf_s(stNd.nd_ceqid, L"99991201%06d", i + 1);
			_stprintf_s(stNd.nd_connectivitynodeid, L"99991201%06d00", i + 1);
			stNd.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
			stNd.nd_ii_snv = nSSID;

			stNd.nd_hi_fcbsw = (int)m_arrCBSW.GetSize() + 1;
			stNd.nd_hi_tcbsw = 0;
			stNd.nd_hi_fbr = 0;
			stNd.nd_hi_tbr = i + 1;
			stNd.nd_si_snv = 0;
			stNd.nd_hi_svc = 0;
			stNd.nd_hi_gen = 0;
			stNd.nd_si_gnd = 0;

			m_arrND.Add(stNd);
			nDS1 = (int)m_arrND.GetSize();

			GND_STA stGND;
			memset(&stGND, 0, sizeof(GND_STA));
			_stprintf_s(stGND.gnd_nm, L"%sDS-1", m_arrBR[i].br_nm);
			stGND.gnd_hi_nd = (int)m_arrND.GetSize();


			m_arrGND.Add(stGND);

			CBSW_STA stCbsw;
			memset(&stCbsw, 0, sizeof(CBSW_STA));
			szDate.Format(_T("99991201%06d"), i + 1);
			_stprintf_s(stCbsw.cbsw_nm, L"%sDS-1", m_arrBR[i].br_nm);
			_stprintf_s(stCbsw.cbsw_ceqid, L"99991201%06d", i + 1);
			stCbsw.cbsw_type = 4;
			stCbsw.cbsw_norstat = 1;
			stCbsw.cbsw_ii_fnd = stCbsw.cbsw_ii_fgnd = nTND;
			stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = (int)m_arrND.GetSize() + 1;
			stCbsw.cbsw_ii_dl = 0;
			_stprintf_s(stCbsw.cbsw_multisw_id, L"0");
			m_arrCBSW.Add(stCbsw);
			nCB1 = (int)m_arrCBSW.GetSize();
			//CBSW 생성 DS-2
			ND_STA stNd1;
			memset(&stNd1, 0, sizeof(ND_STA));
			szSSNM.Format(_T("%sDS-2"), m_arrBR[i].br_nm);
			szDate.Format(_T("99991202%08d"), i + 1);
			_stprintf_s(stNd1.nd_nm, L"%sDS-2", m_arrBR[i].br_nm);
			_stprintf_s(stNd1.nd_ceqid, L"99991202%06d", i + 1);
			_stprintf_s(stNd1.nd_connectivitynodeid, L"99991202%06d00", i + 1);
			stNd1.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
			stNd1.nd_ii_snv = nSSID;
			stNd1.nd_si_snv = 0;
			m_arrND.Add(stNd1);

			nDS2 = (int)m_arrND.GetSize();

			GND_STA stGND1;
			memset(&stGND1, 0, sizeof(GND_STA));
			_stprintf_s(stGND1.gnd_nm, L"%sDS-2", m_arrBR[i].br_nm);
			stGND1.gnd_hi_nd = (int)m_arrND.GetSize();
			m_arrGND.Add(stGND1);

			CBSW_STA stCbsw1;
			memset(&stCbsw1, 0, sizeof(CBSW_STA));
			szDate.Format(_T("99991202%06d"), i + 1);
			_stprintf_s(stCbsw1.cbsw_nm, L"%sDS-2", m_arrBR[i].br_nm);
			_stprintf_s(stCbsw1.cbsw_ceqid, L"99991202%06d", i + 1);
			stCbsw1.cbsw_type = 4;
			stCbsw1.cbsw_norstat = 1;
			stCbsw1.cbsw_ii_fnd = stCbsw1.cbsw_ii_fgnd = nDS2;
			stCbsw1.cbsw_ii_tnd = stCbsw1.cbsw_ii_tgnd = (int)m_arrND.GetSize() + 1;
			stCbsw1.cbsw_ii_dl = 0;
			_stprintf_s(stCbsw1.cbsw_multisw_id, L"0");
			m_arrCBSW.Add(stCbsw1);

			nCB2 = (int)m_arrCBSW.GetSize();
			//CBSW 생성 DS-3
			ND_STA stNd2;
			memset(&stNd2, 0, sizeof(ND_STA));
			szSSNM.Format(_T("%sDS-3"), m_arrBR[i].br_nm);
			szDate.Format(_T("99991203%08d"), i + 1);
			_stprintf_s(stNd2.nd_nm, L"%sDS-3", m_arrBR[i].br_nm);
			_stprintf_s(stNd2.nd_ceqid, L"99991203%06d", i + 1);
			_stprintf_s(stNd2.nd_connectivitynodeid, L"99991203%06d00", i + 1);
			stNd2.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
			stNd2.nd_ii_snv = nSSID;
			stNd2.nd_si_snv = 0;
			m_arrND.Add(stNd2);
			nDS3 = (int)m_arrND.GetSize();

			GND_STA stGND2;
			memset(&stGND2, 0, sizeof(GND_STA));
			_stprintf_s(stGND2.gnd_nm, L"%sDS-3", m_arrBR[i].br_nm);
			stGND2.gnd_hi_nd = (int)m_arrND.GetSize();
			m_arrGND.Add(stGND2);

			CBSW_STA stCbsw2;
			memset(&stCbsw2, 0, sizeof(CBSW_STA));
			szDate.Format(_T("99991203%06d"), i + 1);
			_stprintf_s(stCbsw2.cbsw_nm, L"%sDS-3", m_arrBR[i].br_nm);
			_stprintf_s(stCbsw2.cbsw_ceqid, L"99991203%06d", i + 1);
			stCbsw2.cbsw_type = 4;
			stCbsw2.cbsw_norstat = 0;
			stCbsw2.cbsw_ii_fnd = stCbsw2.cbsw_ii_fgnd = nDS2;
			stCbsw2.cbsw_ii_tnd = stCbsw2.cbsw_ii_tgnd = (int)m_arrND.GetSize() + 1;
			stCbsw2.cbsw_ii_dl = 0;
			_stprintf_s(stCbsw2.cbsw_multisw_id, L"0");
			m_arrCBSW.Add(stCbsw2);
			nCB3 = (int)m_arrCBSW.GetSize();
			//CBSW 생성 DS-3
			ND_STA stNd3;
			memset(&stNd3, 0, sizeof(ND_STA));
			szSSNM.Format(_T("%sDS-4"), m_arrBR[i].br_nm);
			szDate.Format(_T("99991204%08d"), i + 1);
			_stprintf_s(stNd3.nd_nm, L"%sDS-4", m_arrBR[i].br_nm);
			_stprintf_s(stNd3.nd_ceqid, L"99991204%06d", i + 1);
			_stprintf_s(stNd3.nd_connectivitynodeid, L"99991204%06d00", i + 1);
			stNd3.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
			stNd3.nd_ii_snv = nSSID;
			stNd3.nd_si_snv = 0;
			m_arrND.Add(stNd3);
			nDS3 = (int)m_arrND.GetSize();

			GND_STA stGND3;
			memset(&stGND3, 0, sizeof(GND_STA));
			_stprintf_s(stGND3.gnd_nm, L"%sDS-4", m_arrBR[i].br_nm);
			stGND3.gnd_hi_nd = (int)m_arrND.GetSize();
			m_arrGND.Add(stGND3);
		}
	}

	m_szTime = LIST_Current_Time();
	m_szDataName_Data.Format(_T("%s[5/7][8/8]========== ADMStoKASIM 데이터 변환 [계통 탐색 - CBSW] =========="), m_szTime);
	IDC_LIST_DATA_HISTORY(m_szDataName_Data);
	m_ctrProgressADMStoKCIM.StepIt();

	int m;
	int nOCBND, nOCBND1, nOCBND2;
	for (int i = 0; i < m_arrCBSW.GetSize(); i++)
	{
		if (m_arrCBSW[i].cbsw_type == 1)
		{
			nOCBND = SET_MTRND(m_arrCBSW[i].cbsw_ii_dl);
			nOCBND1 = nOCBND + 2; //순차적으로 입력해서 2입니다. 고정픽입니다. 현재까지는!!!20200603
			nOCBND2 = nOCBND + 3; //순차적으로 입력해서 3입니다. 고정픽입니다. 현재까지는!!!20200603

			for (m = 0; m < m_arrND.GetSize(); m++)
			{
				if (m + 1 == m_arrCBSW[i].cbsw_ii_fnd)
				{
					m_arrND[m].nd_ii_snv = nSSID + 1;
					break;
				}
			}
			nDS5 = m_arrCBSW[i].cbsw_ii_fnd;

			pNode = m_pNodeArr.GetAt(nDS5 - 1);
			if (pNode->m_nName_Type != 14)
			{
				nDS5 = m_arrCBSW[i].cbsw_ii_tnd;
				m_arrCBSW[i].cbsw_ii_tnd = m_arrCBSW[i].cbsw_ii_tgnd = m_arrCBSW[i].cbsw_ii_fnd;
				pNode = m_pNodeArr.GetAt(nDS5 - 1);
				if (pNode->m_nName_Type != 14)
				{
					int a = 0;
				}
			}

			//	여기서 만들어야 겠다 cb랑 dl 관계 20220530 
			for (int k = 0; k < m_arrDL.GetSize(); k++)
			{
				CString szdl, szcb;
				szdl.Format(_T("%s"), m_arrDL[k].stDL_CEQFK);
				szcb.Format(_T("%s"), m_arrCBSW[i].cbsw_ceqid);
				if (m_arrDL[k].stDL_CEQFK == m_arrCBSW[i].cbsw_ceqid) //이건 왜 안될까요?
				{
					m_arrDL[k].dl_ii_cb = i + 1;
				}
				if (szdl == szcb)
				{
					m_arrDL[k].dl_ii_cb = i + 1;
				}
			}

			//임시작업 예전에 임시 확인함
			m_arrCBSW[i].cbsw_ii_fnd = m_arrCBSW[i].cbsw_ii_fgnd = nDS5;

			CBSW_STA stCbsw;
			memset(&stCbsw, 0, sizeof(CBSW_STA));
			szSSNM.Format(_T("%sCB-1"), m_arrCBSW[i].cbsw_nm);
			szDate.Format(_T("99991204%06d"), i + 1);
			_stprintf_s(stCbsw.cbsw_nm, L"%sCB-1", m_arrCBSW[i].cbsw_nm);
			_stprintf_s(stCbsw.cbsw_ceqid, L"99991204%06d", i + 1);
			stCbsw.cbsw_type = 4;
			stCbsw.cbsw_norstat = 1;
			stCbsw.cbsw_ii_fnd = stCbsw.cbsw_ii_fgnd = nOCBND1;
			stCbsw.cbsw_ii_tnd = stCbsw.cbsw_ii_tgnd = nDS5;
			stCbsw.cbsw_ii_dl = 0;
			_stprintf_s(stCbsw.cbsw_multisw_id, L"0");
			m_arrCBSW.Add(stCbsw);
			//CBSW 생성 DS-5

			CBSW_STA stCbsw1;
			memset(&stCbsw1, 0, sizeof(CBSW_STA));
			szSSNM.Format(_T("%sCB-2"), m_arrCBSW[i].cbsw_nm);
			szDate.Format(_T("99991205%06d"), i + 1);
			_stprintf_s(stCbsw1.cbsw_nm, L"%sCB-2", m_arrCBSW[i].cbsw_nm);
			_stprintf_s(stCbsw1.cbsw_ceqid, L"99991205%06d", i + 1);
			stCbsw1.cbsw_type = 4;
			stCbsw1.cbsw_norstat = 0;
			stCbsw1.cbsw_ii_fnd = stCbsw1.cbsw_ii_fgnd = nOCBND2;
			stCbsw1.cbsw_ii_tnd = stCbsw1.cbsw_ii_tgnd = nDS5;
			stCbsw1.cbsw_ii_dl = 0;
			_stprintf_s(stCbsw1.cbsw_multisw_id, L"0");
			m_arrCBSW.Add(stCbsw1);
		}
	}
}

void CADMStoKCIMDlg::ADMStoKASIM_Insert()
{
	int i;
	int j;
	CString strSQL;
	FILE* stream;
	CString szRoute;

	//SYSINFO_DYN_UIN
	szRoute.Format(_T("%sDUAL\\SYSINFO_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"SYSINFO_NM,SYSINFO_TYPE,SYSINFO_VALUE\n");
	for (i = 0; i < 1; i++)
	{
		fwprintf(stream, _T("DER-Schedule,2,0\n"));
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//HDOF_STA	
	szRoute.Format(_T("%sDUAL\\HDOF_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"HDOF_NM,HDOF_CODE,HDOF_HI_CENTER,CENTER_HI_BOF\n");
	for (i = 0; i < m_arrHDOF.GetSize(); i++)
	{
		fwprintf(stream, _T("%s,%d,%d,%d\n")
			, m_arrHDOF[i].HDOF_NM, m_arrHDOF[i].HDOF_CODE, m_arrHDOF[i].HDOF_HI_CENTER, m_arrHDOF[i].CENTER_HI_BOF);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//CENTER_STA	
	szRoute.Format(_T("%sDUAL\\CENTER_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"CENTER_NM,CENTER_OFFICEID,CENTER_HI_BOF,CENTER_II_HDOF,CENTER_SI_HDOF\n");
	for (i = 0; i < m_arrCENTER.GetSize(); i++)
	{
		fwprintf(stream, _T("%s,%s,%d,%d,%d\n")
			, m_arrCENTER[i].center_nm, m_arrCENTER[i].center_officeid, m_arrCENTER[i].center_hi_bof, m_arrCENTER[i].nCENTER_II_HDOF, m_arrCENTER[i].nCENTER_SI_HDOF);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();

	//BOF_STA
	szRoute.Format(_T("%sDUAL\\BOF_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"BOF_NM,BOF_OFFICEID,BOF_II_CENTER,BOF_SI_CENTER,BOF_HI_DL\n");
	for (i = 0; i < m_arrBOF.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%d\n"
			, m_arrBOF[i].bof_nm, m_arrBOF[i].bof_officeid, m_arrBOF[i].bof_ii_center, m_arrBOF[i].bof_si_center, m_arrBOF[i].bof_hi_dl);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//SS_STA 
	szRoute.Format(_T("%sDUAL\\SS_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"SS_NM,SS_SUBSTATIONID,SS_HI_MTR,SS_HI_SNV,SS_CODE,SS_II_VGEN\n");
	for (i = 0; i < m_arrSS.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%s,%d\n"
			, m_arrSS[i].ss_nm, m_arrSS[i].ss_substationid
			, m_arrSS[i].ss_hi_mtr, m_arrSS[i].ss_hi_snv, m_arrSS[i].ss_code, m_arrSS[i].ss_ii_vgen);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//SS_DYN_UIN
	szRoute.Format(_T("%sDUAL\\SS_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"SS_DNRFLAG,SS_TIEOUTKW,SS_TIEINKW\n");
	for (i = 0; i < m_arrSS.GetSize(); i++)
	{
		fwprintf(stream, L"1,10,10\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//MTR_STA
	szRoute.Format(_T("%sDUAL\\MTR_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"MTR_NM,MTR_MAINTRID,MTR_II_SS,MTR_SI_SS,MTR_HI_DL,MTR_HI_TR,MTR_BANK\n");
	for (i = 0; i < m_arrMTR.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%d,%d,%d\n"
			, m_arrMTR[i].mtr_nm, m_arrMTR[i].mtr_maintrid, m_arrMTR[i].mtr_ii_ss
			, m_arrMTR[i].mtr_si_ss, m_arrMTR[i].mtr_hi_dl, m_arrMTR[i].mtr_hi_tr, m_arrMTR[i].mtr_bank);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//DL_STA
	szRoute.Format(_T("%sDUAL\\DL_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"DL_NM,DL_DistributionLineID,DL_II_OLM,DL_II_MTR,DL_SI_MTR,DL_II_BOF,DL_SI_BOF,DL_HI_IJ,DL_HI_CBSW,DL_HI_BR,DL_II_CB\n");
	for (i = 0; i < m_arrDL.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d\n"
			, m_arrDL[i].dl_nm, m_arrDL[i].dl_distributionlineid, m_arrDL[i].dl_ii_olm, m_arrDL[i].dl_ii_mtr
			, m_arrDL[i].dl_si_mtr, m_arrDL[i].dl_ii_bof, m_arrDL[i].dl_si_bof, m_arrDL[i].dl_hi_ij, m_arrDL[i].dl_hi_cbsw, m_arrDL[i].dl_hi_br, m_arrDL[i].dl_ii_cb);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//DL_DYN_UIN 
	szRoute.Format(_T("%sDUAL\\DL_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"DL_PROTECSEQ,DL_CVRFACTOR,DL_PEAKLIM,DL_OPRCAPACITY,DL_SCA_ALLFLAG,DL_SOP_FLAG,DL_DGCT_FLAG\n");
	for (i = 0; i < m_arrDL.GetSize(); i++)
	{
		fwprintf(stream, L"0,0.1,10,1,0,0,0\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//SNV_STA
	szRoute.Format(_T("%sDUAL\\SNV_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"SNV_NM,SNV_NORKV,SNV_SI_SS,SNV_II_SS,SNV_HI_ND\n");
	for (i = 0; i < m_arrSNV.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%.4f,%d,%d,%d\n"
			, m_arrSNV[i].snv_nm, m_arrSNV[i].snv_norkv, m_arrSNV[i].snv_si_ss, m_arrSNV[i].snv_ii_ss
			, m_arrSNV[i].snv_hi_nd);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//ND_STA
	szRoute.Format(_T("%sDUAL\\ND_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"ND_NM,ND_CEQID,ND_ConnectivityNodeID,ND_SI_SNV,ND_II_SNV,ND_HI_FCBSW,ND_HI_TCBSW,ND_HI_FBR,ND_HI_TBR,ND_HI_GEN,ND_HI_SHUNTEQ,ND_HI_SVC,ND_II_GND,ND_SI_GND\n");
	for (i = 0; i < m_arrND.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n"
			, m_arrND[i].nd_nm, m_arrND[i].nd_ceqid, m_arrND[i].nd_connectivitynodeid, m_arrND[i].nd_si_snv, m_arrND[i].nd_ii_snv
			, m_arrND[i].nd_hi_fcbsw, m_arrND[i].nd_hi_tcbsw, m_arrND[i].nd_hi_fbr, m_arrND[i].nd_hi_tbr, m_arrND[i].nd_hi_gen
			, m_arrND[i].nd_hi_shunteq, m_arrND[i].nd_hi_svc, m_arrND[i].nd_ii_gnd, m_arrND[i].nd_si_gnd);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//ND_DYN_UIN
	szRoute.Format(_T("%sDUAL\\ND_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"ND_MORFLAG,ND_II_VVM,ND_CONN_LN,ND_CONN_ND\n");
	for (i = 0; i < m_arrND.GetSize(); i++)
	{
		fwprintf(stream, L"1,1,0,0\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//GND_STA
	szRoute.Format(_T("%sDUAL\\GND_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"GND_NM,GND_HI_ND,GND_HI_FCBSW,GND_HI_TCBSW,GND_HI_FGBR,GND_HI_TGBR,GND_HI_GEN,GND_HI_SHUNTEQ,GND_HI_SVC,GND_HI_LD,GND_HI_IJ,GND_HI_HVCUS\n");
	for (i = 0; i < m_arrGND.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n"
			, m_arrGND[i].gnd_nm, m_arrGND[i].gnd_hi_nd, m_arrGND[i].gnd_hi_fcbsw, m_arrGND[i].gnd_hi_tcbsw
			, m_arrGND[i].gnd_hi_fgbr, m_arrGND[i].gnd_hi_tgbr, m_arrGND[i].gnd_hi_gen, m_arrGND[i].gnd_hi_shunteq, m_arrGND[i].gnd_hi_svc
			, m_arrGND[i].gnd_hi_ld, m_arrGND[i].gnd_hi_ij, m_arrGND[i].gnd_hi_hvcus);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//CBSW_STA
	szRoute.Format(_T("%sDUAL\\CBSW_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"CBSW_NM,CBSW_CEQID,CBSW_TYPE,CBSW_RTUTYPE,CBSW_RTUCODE,CBSW_COMTYPE,CBSW_NORSTAT,CBSW_II_PRDE,CBSW_SI_FND,CBSW_II_FND,CBSW_SI_TND,CBSW_II_TND,CBSW_SI_FGND,CBSW_II_FGND,CBSW_SI_TGND,CBSW_II_TGND,CBSW_II_DL,CBSW_SI_DL,CBSW_MULTISW_ID,CBSW_MULTICIR_NUMBER\n");
	for (i = 0; i < m_arrCBSW.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d\n"
			, m_arrCBSW[i].cbsw_nm, m_arrCBSW[i].cbsw_ceqid, m_arrCBSW[i].cbsw_type, m_arrCBSW[i].cbsw_rtutype, m_arrCBSW[i].cbsw_RTUCODE, m_arrCBSW[i].cbsw_comtype
			, m_arrCBSW[i].cbsw_norstat, m_arrCBSW[i].cbsw_ii_prde, m_arrCBSW[i].cbsw_si_fnd, m_arrCBSW[i].cbsw_ii_fnd
			, m_arrCBSW[i].cbsw_si_tnd, m_arrCBSW[i].cbsw_ii_tnd, m_arrCBSW[i].cbsw_si_fgnd, m_arrCBSW[i].cbsw_ii_fgnd, m_arrCBSW[i].cbsw_si_tgnd
			, m_arrCBSW[i].cbsw_ii_tgnd, m_arrCBSW[i].cbsw_ii_dl, m_arrCBSW[i].cbsw_si_dl, m_arrCBSW[i].cbsw_multisw_id, m_arrCBSW[i].cbsw_multicir_number);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//CBSW_DYN_UIN
	szRoute.Format(_T("%sDUAL\\CBSW_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"CBSW_MEAUSE,CBSW_MANF,CBSW_MANSTAT,CBSW_DNRIMPO,CBSW_SRCDIRECTION,CBSW_PFSIGN,CBSW_DIRREF,CBSW_BASE_STATE,CBSW_ISCONTROLED\n");
	for (i = 0; i < m_arrCBSW.GetSize(); i++)
	{
		fwprintf(stream, L"1,0,1,1,1,0,0,0,0\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//CBSW_DYN_MEA
	szRoute.Format(_T("%sDUAL\\CBSW_DYN_MEA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"CBSW_ODSTAT,CBSW_NWSTAT,CBSW_PAMEAKV,CBSW_PBMEAKV,CBSW_PCMEAKV,CBSW_PAMEAAMP,CBSW_PBMEAAMP,CBSW_PCMEAAMP,CBSW_PAMEAAMP_AVG,CBSW_PBMEAAMP_AVG,CBSW_PCMEAAMP_AVG, CBSW_PAMEAKV_ANG, CBSW_PBMEAKV_ANG, CBSW_PCMEAKV_ANG, CBSW_PAMEAAMP_ANG, CBSW_PBMEAAMP_ANG, CBSW_PCMEAAMP_ANG, CBSW_PAMEAADIFF, CBSW_PBMEAADIFF, CBSW_PCMEAADIFF, CBSW_COMSTATSER, CBSW_CONTSTAT, CBSW_DIRSET,CBSW_DERSETPOINT\n");
	for (i = 0; i < m_arrCBSW.GetSize(); i++)
	{
		fwprintf(stream, L"%d,%d,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n", m_arrCBSW[i].cbsw_norstat, m_arrCBSW[i].cbsw_norstat);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//GEN_STA
	szRoute.Format(_T("%sDUAL\\GEN_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"GEN_NM,GEN_CEQID,GEN_SI_ND,GEN_II_ND,GEN_SI_GND,GEN_II_GND,GEN_II_IJ,GEN_SI_LNSEC,GEN_II_LNSEC, GEN_NAMKV, GEN_MWLMMX, GEN_MWLMMN, GEN_MVARLMMX, GEN_MVARLMMN, GEN_R, GEN_STX, GEN_SSX, GEN_TYPE, GEN_VOL_CLS, GEN_II_CONND, GEN_CONNDREP, GEN_CONTYPE, GEN_NOOFP, GEN_PF, GEN_EFF, GEN_PFMINLAG, GEN_PFMINLEAD, GEN_II_ESS, GEN_MEA, GEN_TREXCL, GEN_II_PRDE,GEN_II_CBSW, GEN_II_CTRLSENSOR, GEN_HI_GENUNIT\n");
	for (i = 0; i < m_arrGEN.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%d,%d,%d,%d,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d,%d,%d,%d,%d,%d,%.4f,%.4f,%.4f,%.4f,%d,%d,%d,%d,0,0,0\n"
			, m_arrGEN[i].gen_nm, m_arrGEN[i].gen_ceqid, m_arrGEN[i].gen_si_nd, m_arrGEN[i].gen_ii_nd
			, m_arrGEN[i].gen_si_gnd, m_arrGEN[i].gen_ii_gnd, m_arrGEN[i].gen_ii_ij, m_arrGEN[i].GEN_SI_LNSEC, m_arrGEN[i].GEN_II_LNSEC, m_arrGEN[i].gen_namkv, m_arrGEN[i].gen_mwlmmx
			, m_arrGEN[i].gen_mwlmmn, m_arrGEN[i].gen_mvarlmmx, m_arrGEN[i].gen_mvarlmmn, m_arrGEN[i].gen_r, m_arrGEN[i].gen_stx
			, m_arrGEN[i].gen_ssx, m_arrGEN[i].gen_type, m_arrGEN[i].gen_vol_cls, m_arrGEN[i].gen_ii_connd, m_arrGEN[i].gen_conndrep, m_arrGEN[i].gen_contype
			, m_arrGEN[i].gen_noofp, m_arrGEN[i].gen_pf, m_arrGEN[i].gen_eff, m_arrGEN[i].gen_pfminlag
			, m_arrGEN[i].gen_pfminlead, m_arrGEN[i].gen_ii_ess, m_arrGEN[i].gen_mea, m_arrGEN[i].GEN_TREXCL, m_arrGEN[i].GEN_II_PRDE);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();

	//GEN_DYN_UIN
	szRoute.Format(_T("%sDUAL\\GEN_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"GEN_AVR,GEN_TBSVL,GEN_DEV,GEN_PF,GEN_POWERCTRL_FLAG	,GEN_POWERCTRL_VALUE,GEN_VVOFLAG,GEN_MVARCONMODE,GEN_OUTQ,GEN_PFVVOMINLAG,GEN_PFVVOMINLEAD,GEN_QVMINS,GEN_DERAVMVUBREF,GEN_DERAVMVLBREF,GEN_DERAVMQVRATIO,GEN_QVCURVEQMAX,GEN_QVCURVEQMIN,GEN_QVCURVEV1,GEN_QVCURVEV2,GEN_QVCURVEV3,GEN_QVCURVEV4,GEN_ITR_CAP,GEN_ITR_R,GEN_ITR_X,GEN_ITR_NGR_R,GEN_ITR_NGR_X,GEN_ITR_WDC,GEN_MACH_TYPE,GEN_SCA_EXCEPTION,GEN_PDVRFLAG,GEN_EST_PM1,GEN_EST_PM2,GEN_EST_PM3,GEN_EST_PM4,GEN_EST_PM5,GEN_EST_PM6,GEN_MDL_TIME\n");
	for (i = 0; i < m_arrGEN.GetSize(); i++)
	{
		fwprintf(stream, L"%d,1,0.02,0.95,0,100,0,4,0,0,0,0,0,0,0,43,0,0,0,0,0,%.4f,%.4f,%.4f,%.4f,%.4f,%d,1,0,0,0,0,0,0,0,0,0\n"
			, m_arrGEN[i].gen_avr
			, m_arrGEN[i].fGEN_ITR_CAP
			, m_arrGEN[i].fGEN_ITR_R
			, m_arrGEN[i].fGEN_ITR_X
			, m_arrGEN[i].fGEN_ITR_NGR_R
			, m_arrGEN[i].fGEN_ITR_NGR_X
			, m_arrGEN[i].nGEN_ITR_WDC

		);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//ESS_STA
	szRoute.Format(_T("%sDUAL\\ESS_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"ESS_NM,ESS_CHGTM,ESS_DCHGTM,ESS_II_GEN,ESS_SI_GEN,ESS_TYPE,ESS_CHARMXMW,ESS_CHARMNMW,ESS_DICHMXMW,ESS_DICHMNMW,ESS_MXSOC,ESS_MNSOC,ESS_CHGEFF,ESS_C_RATIO,ESS_HI_PCS,ESS_NOOFPCS,ESS_CAPMWh\n");
	for (i = 0; i < m_arrESS.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%d,%d,%d,%d,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f%.4f,%.4f,%d,%d,%.4f\n"
			, m_arrESS[i].ess_nm, m_arrESS[i].ess_chgtm, m_arrESS[i].ess_dchgtm, m_arrESS[i].ess_ii_gen, m_arrESS[i].ess_si_gen
			, m_arrESS[i].ess_type, m_arrESS[i].ess_charmxmw, m_arrESS[i].ess_charmnmw, m_arrESS[i].ess_dichmxmw
			, m_arrESS[i].ess_dichmnmw, m_arrESS[i].ess_mxsoc, m_arrESS[i].ess_mnsoc, m_arrESS[i].ess_chgeff, m_arrESS[i].ess_c_ratio
			, m_arrESS[i].ess_hi_pcs, m_arrESS[i].ess_noofpcs, m_arrESS[i].ess_capmwh);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  
	
	//ESS_DYN_UIN 
	szRoute.Format(_T("%sDUAL\\ESS_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"ESS_SETMXSOC,ESS_SETMNSOC,ESS_EFIN,ESS_CLRMXSOC,ESS_CLRMNSOC,ESS_CNTMODE,ESS_SOCMXDBAND,ESS_SOCMNDBAND,ESS_SOC_NCOUNT_MAX,ESS_SOC_DEV_ABSOLUTE_MAX,ESS_MODE,");
	for (j = 0; j < 192; j++)
	{
		fprintf(stream, "ESS_GENSCHE%d, ", j + 1);
	}
	for (j = 0; j < 192; j++)
	{
		fprintf(stream, "ESS_ESSCHGMODE%d, ", j + 1);
	}
	fprintf(stream, " ESS_OPP,ESS_OUTAGE_TIME\n");
	for (i = 0; i < m_arrESS.GetSize(); i++)
	{
		fwprintf(stream, L"80,20,50,70,30,1,0.45,0.45,10,50,1,");
		for (j = 0; j < 192; j++)
		{
			fprintf(stream, "0,");
		}
		for (j = 0; j < 192; j++)
		{
			fprintf(stream, "0,");
		}
		fprintf(stream, "0,15\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//SVC_STA
	szRoute.Format(_T("%sDUAL\\SVC_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"SVC_NM,SVC_CEQID,SVC_SI_ND,SVC_II_ND,SVC_SI_GND,SVC_II_GND,SVC_II_IJ,SVC_SHMVAR,SVC_BANO,SVC_MVARLMMN,SVC_MVARLMMX,SVC_II_CONND,SVC_CONNDREP\n");
	for (i = 0; i < m_arrSVC.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%d,%d,%d,%.4f,%d,%.4f,%.4f,%d,%d\n"
			, m_arrSVC[i].svc_nm, m_arrSVC[i].svc_ceqid, m_arrSVC[i].svc_si_nd, m_arrSVC[i].svc_ii_nd
			, m_arrSVC[i].svc_si_gnd, m_arrSVC[i].svc_ii_gnd, m_arrSVC[i].svc_ii_ij, m_arrSVC[i].svc_shmvar, m_arrSVC[i].svc_bano
			, m_arrSVC[i].svc_mvarlmmn, m_arrSVC[i].svc_mvarlmmx, m_arrSVC[i].svc_ii_connd, m_arrSVC[i].svc_conndrep);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//SVC_DYN_UIN
	szRoute.Format(_T("%sDUAL\\SVC_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"SVC_TARVOL,SVC_SLOP,SVC_VVOFLAG\n");
	for (i = 0; i < m_arrSVC.GetSize(); i++)
	{
		fwprintf(stream, L"1,0.2,1\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//SHUNTEQ_STA
	szRoute.Format(_T("%sDUAL\\SHUNTEQ_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"SHUNTEQ_NM,SHUNTEQ_CEQID,SHUNTEQ_SI_ND,SHUNTEQ_II_ND,SHUNTEQ_SI_GND,SHUNTEQ_II_GND,SHUNTEQ_II_IJ,SHUNTEQ_TYPE,SHUNTEQ_MVAR,SHUNTEQ_II_CONND,SHUNTEQ_REPPHS\n");
	for (i = 0; i < m_arrSHUNTEQ.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%d,%d,%d,%d,%.4f,%d,%d\n"
			, m_arrSHUNTEQ[i].shunteq_nm, m_arrSHUNTEQ[i].shunteq_ceqid, m_arrSHUNTEQ[i].shunteq_si_nd, m_arrSHUNTEQ[i].shunteq_ii_nd
			, m_arrSHUNTEQ[i].shunteq_si_gnd, m_arrSHUNTEQ[i].shunteq_ii_gnd, m_arrSHUNTEQ[i].shunteq_ii_ij, m_arrSHUNTEQ[i].shunteq_type, m_arrSHUNTEQ[i].shunteq_mvar
			, m_arrSHUNTEQ[i].shunteq_ii_connd, m_arrSHUNTEQ[i].shunteq_repphs);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//SHUNTEQ_DYN_UIN
	szRoute.Format(_T("%sDUAL\\SHUNTEQ_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"SHUNTEQ_AVR,SHUNTEQ_TBSVL,SHUNTEQ_DEV,SHUNTEQ_VVOFLAG,SHUNTEQ_SHCOST,SHUNTEQ_OPMX,SHUNTEQ_CONMXDAY,SHUNTEQ_OPDAY\n");
	for (i = 0; i < m_arrSHUNTEQ.GetSize(); i++)
	{
		fwprintf(stream, L"0,1,0.02,0,1,1,1,0\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//LD_STA
	szRoute.Format(_T("%sDUAL\\LD_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"LD_NM,LD_SI_GND,LD_II_GND,LD_II_IJ\n");
	for (i = 0; i < m_arrLD.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%d,%d,%d\n"
			, m_arrLD[i].ld_nm, m_arrLD[i].ld_si_gnd, m_arrLD[i].ld_ii_gnd, m_arrLD[i].ld_ii_ij);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//LD_DYN_UIN
	szRoute.Format(_T("%sDUAL\\LD_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"LD_DIV\n");
	for (i = 0; i < m_arrLD.GetSize(); i++)
	{
		fwprintf(stream, L"1\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//IJ_STA
	szRoute.Format(_T("%sDUAL\\IJ_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"IJ_NM,IJ_SI_EQUTY,IJ_II_EQUTY,IJ_II_EQU,IJ_SI_GND,IJ_II_GND,IJ_II_DL,IJ_SI_DL\n");
	for (i = 0; i < m_arrIJ.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%d,%d,%d,%d,%d,%d,%d\n"
			, m_arrIJ[i].ij_nm, m_arrIJ[i].ij_si_equty, m_arrIJ[i].ij_ii_equty, m_arrIJ[i].ij_ii_equ
			, m_arrIJ[i].ij_si_gnd, m_arrIJ[i].ij_ii_gnd, m_arrIJ[i].ij_ii_dl, m_arrIJ[i].ij_si_dl);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//IJ_DYN_UIN
	szRoute.Format(_T("%sDUAL\\IJ_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"IJ_SOCMAXORDER,IJ_SOCMINORDER\n");
	for (i = 0; i < m_arrLD.GetSize(); i++)
	{
		fwprintf(stream, L"0,0\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//LNSEC_STA
	szRoute.Format(_T("%sDUAL\\LNSEC_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"LNSEC_NM,LNSEC_CEQID,LNSEC_II_BR,LNSEC_HI_INNERPOLE,LNSEC_HI_FPOLE,LNSEC_HI_TPOLE,LNSEC_HI_INNERSEC,LNSEC_HI_GEN,LNSEC_PRIVAT,LNSEC_TYPE_ID,LNSEC_LENGTH,LNSEC_CONSTYPE\n");
	for (i = 0; i < m_arrLNSEC.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%.4f,%d\n"
			, m_arrLNSEC[i].lnsec_nm, m_arrLNSEC[i].lnsec_ceqid
			, m_arrLNSEC[i].lnsec_ii_br, m_arrLNSEC[i].lnsec_hi_innerpole, m_arrLNSEC[i].lnsec_hi_fpole
			, m_arrLNSEC[i].lnsec_hi_tpole, m_arrLNSEC[i].lnsec_hi_innersec, m_arrLNSEC[i].LNSEC_HI_GEN, m_arrLNSEC[i].lnsec_privat, m_arrLNSEC[i].lnsec_type_id, m_arrLNSEC[i].lnsec_length
			, m_arrLNSEC[i].lnsec_constype);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//LNSEC_DYN_UIN
	szRoute.Format(_T("%sDUAL\\LNSEC_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"LNSEC_SW_FOR_CONST,LNSEC_HIS_NOTSRCH\n");
	for (i = 0; i < m_arrLNSEC.GetSize(); i++)
	{
		fwprintf(stream, L"%d,%d\n"
			, 1, 0);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//TR_STA
	szRoute.Format(_T("%sDUAL\\TR_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"TR_NM,TR_CEQID,TR_TYPE,TR_II_BR,TR_FNORKV,TR_TNORKV,TR_TRMVA,TR_CONTY,TR_ONLTC,TR_POSX,TR_ZERX,TR_PATAPMX,TR_PATAPMN,TR_PATAPNOR,TR_PATAPSTEP,TR_PBTAPMX,TR_PBTAPMN,TR_PBTAPNOR,TR_PBTAPSTEP,TR_PCTAPMX,TR_PCTAPMN,TR_PCTAPNOR,TR_PCTAPSTEP,TR_II_CONND,TR_PGR,TR_PGX,TR_SGR,TR_SGX,TR_II_SS,TR_SI_SS,TR_II_MTR,TR_SI_MTR,TR_LOCTAP\n");
	for (i = 0; i < m_arrTR.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%.4f,%.4f,%.4f,%d,%d,%.1f,%.1f,%d,%d,%d,%.4f,%d,%d,%d,%.4f,%d,%d,%d,%.4f,%d,%.4f,%.4f,%.4f,%.4f,%d,%d,%d,%d,%d\n"
			, m_arrTR[i].tr_nm, m_arrTR[i].tr_ceqid, m_arrTR[i].tr_type
			, m_arrTR[i].tr_ii_br, m_arrTR[i].tr_fnorkv, m_arrTR[i].tr_tnorkv, m_arrTR[i].tr_trmva
			, m_arrTR[i].tr_conty, m_arrTR[i].tr_onltc, m_arrTR[i].tr_posx, m_arrTR[i].tr_zerx, m_arrTR[i].tr_patapmx, m_arrTR[i].tr_patapmn
			, m_arrTR[i].tr_patapnor, m_arrTR[i].tr_patapstep, m_arrTR[i].tr_pbtapmx, m_arrTR[i].tr_pbtapmn, m_arrTR[i].tr_pbtapnor, m_arrTR[i].tr_pbtapstep
			, m_arrTR[i].tr_pctapmx, m_arrTR[i].tr_pctapmn, m_arrTR[i].tr_pctapnor, m_arrTR[i].tr_pctapstep, m_arrTR[i].tr_ii_connd, m_arrTR[i].tr_pgr
			, m_arrTR[i].tr_pgx, m_arrTR[i].tr_sgr, m_arrTR[i].tr_sgx, m_arrTR[i].tr_ii_ss, m_arrTR[i].tr_si_ss, m_arrTR[i].tr_ii_mtr, m_arrTR[i].tr_si_mtr, m_arrTR[i].tr_loctap);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//TR_DYN_UIN
	szRoute.Format(_T("%sDUAL\\TR_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"TR_AVR,TR_PATBSVL,TR_PADEV,TR_PBTBSVL,TR_PBDEV,TR_PCTBSVL,TR_PCDEV,TR_VVOFLAG,TR_CON3P,TR_II_PFVM,TR_PACOMPR,TR_PACOMPX,TR_PBCOMPR,TR_PBCOMPX,TR_PCCOMPR,TR_PCCOMPX,TR_3CONMEAP,TR_PATAPCOST,TR_PACONTAPMX,TR_PBTAPCOST,TR_PBCONTAPMX,TR_PCTAPCOST,TR_PCCONTAPMX,TR_PACONTAPMXDAY,TR_PATAPOPDAY,TR_PBCONTAPMXDAY,TR_PBTAPOPDAY,TR_PCCONTAPMXDAY,TR_PCTAPOPDAY,TR_VREFMX,TR_VREFMN,TR_VREFSS,TR_VBWMX,TR_VBWMN,TR_VBWSS,TR_RXMX,TR_RXMN,TR_RXSS,TR_PDVRFLAG,TR_PTRVH,TR_PTRVL,TR_CTRCH,TR_CTRCL,TR_LDCTYPE,TR_DVMMXV,TR_DVMMNV\n");
	for (i = 0; i < m_arrTR.GetSize(); i++)
	{
		fwprintf(stream, L"0,1,0.02,1,0.02,1,0.02,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,1,0,13200,85,0.5,9,0.5,0.1,25,-25,1,0,0,0,0,0,%d,%.4f,%.4f\n"
			, m_arrTR[i].TR_LDCTYPE, m_arrTR[i].TR_DVMMXV, m_arrTR[i].TR_DVMMNV);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//BR_STA
	szRoute.Format(_T("%sDUAL\\BR_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"BR_NM,BR_II_EQU,BR_SI_FND,BR_II_FND,BR_SI_TND,BR_II_TND,BR_POSR,BR_POSX,BR_POSG,BR_POSB,BR_ZERR,BR_ZERX,BR_ZERG,BR_ZERB,BR_NORLM,BR_EMRLM,BR_SI_GBR,BR_II_GBR,BR_II_DL,BR_SI_DL\n");
	for (i = 0; i < m_arrBR.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%d,%d,%d,%d,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d,%d,%d,%d\n"
			, m_arrBR[i].br_nm, m_arrBR[i].br_ii_equ, m_arrBR[i].br_si_fnd, m_arrBR[i].br_ii_fnd
			, m_arrBR[i].br_si_tnd, m_arrBR[i].br_ii_tnd, m_arrBR[i].br_posr, m_arrBR[i].br_posx, m_arrBR[i].br_posg
			, m_arrBR[i].br_posb, m_arrBR[i].br_zerr, m_arrBR[i].br_zerx, m_arrBR[i].br_zerg, m_arrBR[i].br_zerb
			, m_arrBR[i].br_norlm, m_arrBR[i].br_emrlm, m_arrBR[i].br_si_gbr, m_arrBR[i].br_ii_gbr, m_arrBR[i].br_ii_dl, m_arrBR[i].br_si_dl);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//GBR_STA
	szRoute.Format(_T("%sDUAL\\GBR_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"GBR_NM,GBR_II_EQU,GBR_SI_FGND,GBR_II_FGND,GBR_SI_TGND,GBR_II_TGND,GBR_POSR,GBR_POSX,GBR_POSG,GBR_POSB,GBR_ZERR,GBR_ZERX,GBR_ZERG,GBR_ZERB,GBR_HI_BR,GBR_SI_EQUTY,GBR_II_EQUTY\n");
	for (i = 0; i < m_arrGBR.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%d,%d,%d,%d,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d,%d,%d\n"
			, m_arrGBR[i].gbr_nm, m_arrGBR[i].gbr_ii_equ, m_arrGBR[i].gbr_si_fgnd, m_arrGBR[i].gbr_ii_fgnd
			, m_arrGBR[i].gbr_si_tgnd, m_arrGBR[i].gbr_ii_tgnd, m_arrGBR[i].gbr_posr, m_arrGBR[i].gbr_posx, m_arrGBR[i].gbr_posg
			, m_arrGBR[i].gbr_posb, m_arrGBR[i].gbr_zerr, m_arrGBR[i].gbr_zerx, m_arrGBR[i].gbr_zerg, m_arrGBR[i].gbr_zerb
			, m_arrGBR[i].gbr_hi_br, m_arrGBR[i].gbr_si_equty, m_arrGBR[i].gbr_ii_equty);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//GBR_DYN_UIN
	szRoute.Format(_T("%sDUAL\\GBR_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"GBR_MORFLAG,GBR_SCAFLAG\n");
	for (i = 0; i < m_arrGBR.GetSize(); i++)
	{
		fwprintf(stream, L"1,1\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//EQUTY_STA
	szRoute.Format(_T("%sDUAL\\EQUTY_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"EQUTY_NM,EQUTY_HI_GBR,EQUTY_HI_IJ\n");
	for (i = 0; i < m_arrEQUTY.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%d,%d\n"
			, m_arrEQUTY[i].equty_nm, m_arrEQUTY[i].equty_hi_gbr, m_arrEQUTY[i].equty_hi_ij);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//NCPOPT_DYN_UIN
	szRoute.Format(_T("%sDUAL\\NCPOPT_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"NCPOPT_ISLBS,NCPOPT_ISLGEN,NCPOPT_ISLLD,NCPOPT_INIT\n");
	for (i = 0; i < 1; i++)
	{
		fwprintf(stream, L"2,1,1,1\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//VVOOPT_DYN_UIN
	szRoute.Format(_T("%sDUAL\\VVOOPT_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"VVOOPT_TOLMX,VVOOPT_VEMERSTEP,VVOOPT_OLEMERSTEP,VVOOPT_PFEMERSTEP,VVOOPT_EMERLEVELMX,VVOOPT_ELITERMX,VVOOPT_SCMITERMX,VVOOPT_LMITERMX,VVOOPT_PFMSLM,VVOOPT_PFITERMX,VVOOPT_SCM,VVOOPT_LM,VVOOPT_OBJMN,VVOOPT_MINLOSS,VVOOPT_SCMOBJ,VVOOPT_MVARMSLM,VVOOPT_CVR\n");
	for (i = 0; i < 1; i++)
	{
		fwprintf(stream, L"0.05,10,10,10,5,50,50,100,1,100,1,1,0.0001,50,1,0.0001,1\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//RPFOPT_DYN_UIN
	szRoute.Format(_T("%sDUAL\\RPFOPT_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"RPFOPT_MODE,RPFOPT_FLAT,RPFOPT_MAX_ITERATIONS,RPFOPT_SVC_ADJ_LIMIT,RPFOPT_ISLAND_MAX_VOLTAGE_LIMIT,RPFOPT_ISLAND_MIN_VOLTAGE_LIMIT,RPFOPT_NEAR_CONVERGENCE_ANGLE_LIMIT,RPFOPT_NEAR_CONVERGENCE_VOLTAGE_LIMIT,RPFOPT_NEAR_CONVERGENCE_MW_LIMIT,RPFOPT_NEAR_CONVERGENCE_MVAR_LIMIT,RPFOPT_CONVERGENCE_ANGLE_LIMIT,RPFOPT_CONVERGENCE_VOLTAGE_LIMIT,RPFOPT_CONVERGENCE_MW_LIMIT,RPFOPT_CONVERGENCE_MVAR_LIMIT,RPFOPT_LARGEST_MISMATCH_LIMIT,RPFOPT_HIST_MODE,RPFOPT_HIST_DL,RPFOPT_HIST_SRTDAY,RPFOPT_HIST_ENDDAY,RPFOPT_HIST_SRTHR,RPFOPT_HIST_ENDHR\n");
	for (i = 0; i < 1; i++)
	{
		fwprintf(stream, L"1,1,20,2,2,0,0.01,0.01,0.01,0.01,0.001,0.001,0.001,0.001,0.001,0,0,20210701,20210730,0,23\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//OLM_STA
	szRoute.Format(_T("%sDUAL\\OLM_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"OLM_NM\n");
	for (i = 0; i < 10; i++)
	{
		fwprintf(stream, L"0\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();

	//OLM_DYN_UIN
	szRoute.Format(_T("%sDUAL\\OLM_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"OLM_MORFLAG,OLM_TOL,OLM_DBAND\n");
	for (i = 0; i < 10; i++)
	{
		fwprintf(stream, L"1,1.0,0.01\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//AVM_STA
	szRoute.Format(_T("%sDUAL\\AVM_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"AVM_NM\n");
	for (i = 0; i < 10; i++)
	{
		fwprintf(stream, L"0\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//AVM_DYN_UIN
	szRoute.Format(_T("%sDUAL\\AVM_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"AVM_MORFLAG,AVM_LM,AVM_TOL,AVM_DBAND\n");
	for (i = 0; i < 10; i++)
	{
		fwprintf(stream, L"1,30,1.0,0.01\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//PFVM_STA
	szRoute.Format(_T("%sDUAL\\PFVM_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"PFVM_NM\n");
	for (i = 0; i < 10; i++)
	{
		fwprintf(stream, L"0\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt(); 

	//PFVM_DYN_UIN
	szRoute.Format(_T("%sDUAL\\PFVM_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"PFVM_MORFLAG,PFVM_LM,PFVM_TOL,PFVM_DBAND\n");
	for (i = 0; i < 10; i++)
	{
		fwprintf(stream, L"1,0.9,1.0,0.01\n");
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();  

	//HVCUS_STA
	szRoute.Format(_T("%sDUAL\\HVCUS_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"HVCUS_NM,HVCUS_CEQID,HVCUS_II_GND,HVCUS_SI_GND,HVCUS_CON_KVA,HVCUS_ITR_KVA,HVCUS_ITR_PZ,HVCUS_II_PRDE,HVCUS_ITR_WDC,HVCUS_II_IJ\n");
	for (i = 0; i < m_arrHVCUS.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%d,%d,%.4f,%.4f,%.4f,%d,1,%d\n"
			, m_arrHVCUS[i].HVCUS_NM, m_arrHVCUS[i].HVCUS_CEQID, m_arrHVCUS[i].HVCUS_II_GND, m_arrHVCUS[i].HVCUS_SI_GND, m_arrHVCUS[i].HVCUS_CON_KVA
			, m_arrHVCUS[i].HVCUS_ITR_KVA, m_arrHVCUS[i].HVCUS_ITR_PZ, m_arrHVCUS[i].HVCUS_II_PRDE, m_arrHVCUS[i].HVCUS_II_IJ);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();

	//GENUNIT_STA
	szRoute.Format(_T("%sDUAL\\GENUNIT_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"GENUNIT_CEQID,GENUNIT_NM,GENUNIT_CONNECT_CEQID,GENUNIT_II_GEN,GENUNIT_SI_GEN,GENUNIT_CAP_KW,GENUNIT_LOCATION_NO,GENUNIT_LOCATION_NM,GENUNIT_CUSTOMER_NO,GENUNIT_TYPE,GENUNIT_CONNECT_TR_TYPE\n");
	for (i = 0; i < m_arrGENUNIT.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%s,%d,%d,%.4f,%s,%s,%s,%d,%d\n"
			, m_arrGENUNIT[i].GENUNIT_CEQID, m_arrGENUNIT[i].GENUNIT_NM, m_arrGENUNIT[i].GENUNIT_II_EQU_ID, m_arrGENUNIT[i].GENUNIT_II_GEN, m_arrGENUNIT[i].GENUNIT_SI_GEN, m_arrGENUNIT[i].GENUNIT_CAP_KW
			, m_arrGENUNIT[i].GENUNIT_LOCATION_NO, m_arrGENUNIT[i].GENUNIT_LOCATION_NM, m_arrGENUNIT[i].GENUNIT_CUSTOMER_NO, m_arrGENUNIT[i].GENUNIT_TYPE, m_arrGENUNIT[i].GENUNIT_CONNECTION_TR_TYPE);
	}
	fclose(stream);

	//GENUNIT_DYN_UIN
	szRoute.Format(_T("%sDUAL\\GENUNIT_DYN_UIN.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"GENUNIT_DYN_FLAG,GENUNIT_DYN_AVMFLAG,GENUNIT_LVRTXGROUP_FLAG,GENUNIT_PUBXGROUP_FLAG,GENUNIT_PUBOGROUP_FLAG,GENUNIT_CACGROUP_FLAG,GENUNIT_MANOGROUP_FLAG,GENUNIT_MANXGROUP_FLAG,GENUNIT_INVCTRL_FLAG,GENUNIT_CTTYPE,GENUNIT_CTRATIO,GENUNIT_CTHOURS,GENUNIT_GENKWH,GENUNIT_CTKWH,GENUNIT_CTFLAG\n");
	for (i = 0; i < m_arrGENUNIT.GetSize(); i++)
	{
		fwprintf(stream, L"0,0,%d,%d,%d,%d,%d,%d,0,0,0,0,0,0,1\n"
			, m_arrGENUNIT[i].GENUNIT_LVRTXGROUP_FLAG
			, m_arrGENUNIT[i].GENUNIT_PUBXGROUP_FLAG
			, m_arrGENUNIT[i].GENUNIT_PUBOGROUP_FLAG
			, m_arrGENUNIT[i].GENUNIT_CACGROUP_FLAG
			, m_arrGENUNIT[i].GENUNIT_MANOGROUP_FLAG
			, m_arrGENUNIT[i].GENUNIT_MANXGROUP_FLAG
		);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();

	//LINETYPE_CODE_STA
	szRoute.Format(_T("%sDUAL\\LINETYPE_CODE_STA.csv"), m_szCSV_Route);
	stream = _wfopen(MyPath() + szRoute, L"w+");
	fwprintf(stream, L"LINETYPE_PHA_NM,LINETYPE_NEU_NM,POSITIVE_R,POSITIVE_X,ZERO_R,ZERO_X,THERMAL_LIMIT\n");
	for (i = 0; i < m_arrrLINESEGMENT_TYPE.GetSize(); i++)
	{
		fwprintf(stream, L"%s,%s,%.4f,%.4f,%.4f,%.4f,%.4f\n"
			, m_arrrLINESEGMENT_TYPE[i].PHASE_LINE_TYPE, m_arrrLINESEGMENT_TYPE[i].NEUTRAL_LINE_TYPE, m_arrrLINESEGMENT_TYPE[i].POSITIVE_R
			, m_arrrLINESEGMENT_TYPE[i].POSITIVE_X, m_arrrLINESEGMENT_TYPE[i].ZERO_R, m_arrrLINESEGMENT_TYPE[i].ZERO_X, m_arrrLINESEGMENT_TYPE[i].THERMAL_LIMIT);
	}
	fclose(stream);
	m_ctrProgressADMStoKCIM.StepIt();

	if (m_nSTMode > 0)
	{
		szRoute.Format(_T("%sST\\TASKTMPC_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"TASKTMPC_CEQID,TASKTMPC_FLNSEC_TYPE,TASKTMPC_FLNSEC_CEQID,TASKTMPC_FLNSEC_INDEX,TASKTMPC_FLNSEC_SL,TASKTMPC_TFAC_TYPE,TASKTMPC_TLNSEC_CEQID,TASKTMPC_TLNSEC_INDEX,TASKTMPC_TNSEC_SL\n");
		for (i = 0; i < 100; i++)
		{
			fwprintf(stream, L"0,0,0,0,0,0,0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\TASKTMPSW_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"TASKTMPSW_CEQID,TASKTMPSW_LNSEC_TYPE,TASKTMPSW_LNSEC_CEQID,TASKTMPSW_LNSEC_INDEX,TASKTMPSW_LNSEC_SL\n");
		for (i = 0; i < 100; i++)
		{
			fwprintf(stream, L"0,0,0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\TASK_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"TASK_FAC_TYPE,TASK_FAC_CEQID,TASK_FAC_INDEX,TASK_FAC_ACTION,TASK_FTMP_TYPE,TASK_FTMP_CEQID,TASK_FTMP_INDEX,TASK_FTMP_SL,TASK_TTMP_TYPE,TASK_TTMP_CEQID,TASK_TTMP_INDEX,TASK_TTMP_SL\n");
		for (i = 0; i < 100; i++)
		{
			fwprintf(stream, L"0,0,0,0,0,0,0,0,0,0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\LOOP_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"LOOP_DL1_CTR,LOOP_DL1_OCRT,LOOP_DL1_SSANG,LOOP_DL1_MTRMW,LOOP_DL2_CTR,LOOP_DL2_OCRT,LOOP_DL2_SSANG,LOOP_DL2_MTRMW\n");
		for (i = 0; i < 100; i++)
		{
			fwprintf(stream, L"0,0,0,0,0,0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\DGTROPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"DGRTOPT_SS,DGRTOPT_MTR,DGRTOPT_DL,DGTROPT_HISMODE,DGTROPT_HIST_SRTDAY,DGTROPT_HIST_ENDDAY,DGTROPT_INPUT_PF,DGTROPT_INPUT_GEN,DGTROPT_INPUT_GROSS,DGTROPT_INPUT_VOLT\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"0,0,1,0,0,0,0,0,0,13200\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\PDVROPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"PDVROPT_PFVTOL,PDVROPT_PFQTOL,PDVROPT_PFITERMAX,PDVROPT_SRTHIS,PDVROPT_ENDHIS,PDVROPT_TGDL,PDVROPT_VLBDV,PDVROPT_QVDV,PDVROPT_VLMODE,PDVROPT_ZONEMODE,PDVROPT_OLTCMODEL,PDVROPT_OLTCV,PDVROPT_PDVRMODE,PDVROPT_HISOPT,PDVROPT_UINMW,PDVROPT_UINMVAR,PDVROPT_BADRATE,PDVROPT_TERM,PDVROPT_CANDNUM\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"0.00001,1,1000,0,0,0,0.005,0.005,2,2,4,1,0,2,0,0,0.05,2,1\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\MTR_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"MTR_SOP_FLAG,MTR_SCA_ALLFLAG\n");
		for (i = 0; i < m_arrMTR.GetSize(); i++)
		{
			fwprintf(stream, L"%d,%d\n"
				, 0, 0);
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\DNROPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"DNROPT_NUNPOP,DNROPT_MUTATION_RATE,DNROPT_RATIO_LB,DNROPT_CBEN,DNROPT_REEN,DNROPT_GAEN,DNROPT_MAEN,DNROPT_OBJECT,DNROPT_MAX_CHGSW,DNROPT_PENALTY_CHGSW,DNROPT_PENALTY_VOLT,DNROPT_PENALTY_MW,DNROPT_RC_MOVE_EN,DNROPT_SS_MOVE_EN,DNROPT_LOAD_TYPE\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"100,5,0.5,0,0,1,0,0,0,10,1,1,0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\SCAOPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"SCAOPT_FLTR,SCAOPT_FLT_GND,SCAOPT_FLTTYPE,SCAOPT_INCUS\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"0,1,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\SOPOPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"SOPOPT_TARGET_LOAD,SOPOPT_MW_MARGIN,SOPOPT_FLT_MTR\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\DGCTOPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"DGCTOPT_MNDISCONH,DGCTOPT_CTMODE,DGCTOPT_CACPRIORITY,DGCTOPT_MANOPRIORITY,DGCTOPT_PUBOPRIORITY,DGCTOPT_LVRTXPRIORITY,DGCTOPT_PUBXPRIORITY,DGCTOPT_MANXPRIORITY,DGCTOPT_MANOHIS,DGCTOPT_MANXHIS,DGCTOPT_PCMODE,DGCTOPT_ERR_FLAG,DGCTOPT_SCHEDULE_ID\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"0,0,0,0,0,0,0,0,0,0,0,0, 0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\PRDE_STA.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"PRDE_NM\n");
		for (i = 0; i < m_arrPRDE_STA.GetSize(); i++)
		{
			fwprintf(stream, L"%s_PRDE\n", m_arrPRDE_STA[i].PRDE_STA_NM);
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\PRDE_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"PRDE_OCRF_II_TCCSET,PRDE_OCRD_II_TCCSET,PRDE_OCR_Pickup_C,PRDE_OCR_IIC,PRDE_OCGRF_II_TCCSET,PRDE_OCGRD_II_TCCSET,PRDE_OCGR_Pickup_C,PRDE_OCGR_IIC,PRDE_TYPE,PRDE_SET_GTYPE,PRDE_OCR_NOF,PRDE_OCR_NOD,PRDE_OCGR_NOF,PRDE_OCGR_NOD,PRDE_OCRF_TMS,PRDE_OCRF_TAS,PRDE_OCRF_MRT,PRDE_OCRD_TMS,PRDE_OCRD_TAS,PRDE_OCRD_MRT,PRDE_OCGRF_TMS,PRDE_OCGRF_TAS,PRDE_OCGRF_MRT,PRDE_OCGRD_TMS,PRDE_OCGRD_TAS,PRDE_OCGRD_MRT,PRDE_OCR_CTR,PRDE_OCGR_CTR,PRDE_MX_LD_C_PHA,PRDE_OCR_S_DELAY_TIME,PRDE_OCGR_S_DELAY_TIME,PRDE_OCR_DOCR,PRDE_OCGR_DOCGR,PRDE_OCR_II_MACHINE,PRDE_OCR_MACHINE_NM,PRDE_OCGR_II_MACHINE,PRDE_OCGR_MACHINE_NM,PRDE_UPDATE_TIME\n");
		for (i = 0; i < m_arrPRDE_STA.GetSize(); i++)
		{
			fwprintf(stream, L"%d,%d,%.4f,%.4f,%d,%d,%.4f,%.4f,%d,%d,%d,%d,%d,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d,%d,%.4f,0,0,0,0,0,0,0,0,0\n"
				, m_arrPRDE_STA[i].PRDE_OCRF_II_TCCSET
				, m_arrPRDE_STA[i].PRDE_OCRD_II_TCCSET
				, m_arrPRDE_STA[i].PRDE_OCR_Pickup_C
				, m_arrPRDE_STA[i].PRDE_OCR_IIC
				, m_arrPRDE_STA[i].PRDE_OCGRF_II_TCCSET
				, m_arrPRDE_STA[i].PRDE_OCGRD_II_TCCSET
				, m_arrPRDE_STA[i].PRDE_OCGR_Pickup_C
				, m_arrPRDE_STA[i].PRDE_OCGR_IIC
				, m_arrPRDE_STA[i].PRDE_TYPE
				, m_arrPRDE_STA[i].PRDE_SET_GTYPE
				, m_arrPRDE_STA[i].PRDE_OCR_NOF
				, m_arrPRDE_STA[i].PRDE_OCR_NOD
				, m_arrPRDE_STA[i].PRDE_OCGR_NOF
				, m_arrPRDE_STA[i].PRDE_OCGR_NOD
				, m_arrPRDE_STA[i].PRDE_OCRF_TMS
				, m_arrPRDE_STA[i].PRDE_OCRF_TAS
				, m_arrPRDE_STA[i].PRDE_OCRF_MRT
				, m_arrPRDE_STA[i].PRDE_OCRD_TMS
				, m_arrPRDE_STA[i].PRDE_OCRD_TAS
				, m_arrPRDE_STA[i].PRDE_OCRD_MRT
				, m_arrPRDE_STA[i].PRDE_OCGRF_TMS
				, m_arrPRDE_STA[i].PRDE_OCGRF_TAS
				, m_arrPRDE_STA[i].PRDE_OCGRF_MRT
				, m_arrPRDE_STA[i].PRDE_OCGRD_TMS
				, m_arrPRDE_STA[i].PRDE_OCGRD_TAS
				, m_arrPRDE_STA[i].PRDE_OCGRD_MRT
				, m_arrPRDE_STA[i].PRDE_OCR_CTR
				, m_arrPRDE_STA[i].PRDE_OCGR_CTR
				, m_arrPRDE_STA[i].PRDE_MX_LD_C_PHA
			);
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\PCEOPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"PCEOPT_BG1_TM,PCEOPT_BG2_TM,PCEOPT_BG3_TM,PCEOPT_BG4_TM,PCEOPT_BG5_TM,PCEOPT_MX_TMS,PCEOPT_MX_TAS,PCEOPT_MX_MRT,PCEOPT_TMS_STEP,PCEOPT_TAS_STEP,PCEOPT_MRT_STEP,PCEOPT_MN_IIC,PCEOPT_MX_IIC,PCEOPT_IIC_STEP,PCEOPT_RUN_MODE,PCEOPT_RUN_PATH,PCEOPT_RUN_DL,PCEOPT_PATH_EFIEN,PCEOPT_PATH_HVCUSEN,PCEOPT_PATH_GENEN\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"10000,3500,3500,3500,3500,2,2,10,0.01,0.01,0.01,50,10000,50,1,0,0,0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\ESCOPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"ESCOPT_SMWPRP,ESCOPT_DMWPRP,ESCOPT_FSOCTM\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\DGTR_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"DGTR_TGINDEX,DGTR_TGPF\n");
		for (i = 0; i < 10; i++)
		{
			fwprintf(stream, L"0,1\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\SWO_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"SWOIN_ORDER,SWOIN_II_CBSW,SWOIN_ACTION\n");
		for (i = 0; i < 10; i++)
		{
			fwprintf(stream, L"0,0,0\n");
		}
		fclose(stream);

		szRoute.Format(_T("%sST\\DLPOPT_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"DLPOPT_firststart\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"1\n");
		}
		fclose(stream);
		

		CString szTime;
		szTime = LIST_Current_Time_Kasim();
		szRoute.Format(_T("%sST\\STMODE_INFO_DYN_UIN.csv"), m_szCSV_Route);
		stream = _wfopen(MyPath() + szRoute, L"w+");
		fwprintf(stream, L"STMODE_KASIM_COV_DATE,STMODE_KASIM_VER,STMODE_MEM_OFFICE_NM,STMODE_MEM_OFFICE_ID,STMODE_USER_ID,STMODE_USER_NM\n");
		for (i = 0; i < 1; i++)
		{
			fwprintf(stream, L"%s,%s,%s,%d,0,0\n", szTime, m_szSTMODE_KASIM_Office_Ver, m_szSTMODE_KASIM_Office_Name, m_nSTMODE_KASIM_Office_ID);
		}
		fclose(stream);
	}
}

/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
void CADMStoKCIMDlg::MakeSymbol()
{
	CString szRoute, strPath;
	szRoute.Format(_T("%sST\\"), m_szCSV_Route);
	strPath = (MyPath() + szRoute);
	// 
	CString strFileName;
	strFileName.Format(_T("symbol.zip"));
	CString strTitleName = strFileName;
	CString strFilePath = strPath;
	int nID = 1;

	CPString						szDNWhereData;
	szDNWhereData.Format("GRAPHIC_DEVICE_LIBRARY_ID = %d", nID);

	if (theDBContainer->MariaDB_DownloadFile(2,
		CW2A(strFilePath).m_psz
		, CW2A(strFileName).m_psz
		, CW2A(_T("graphic_device_library")).m_psz
		, CW2A(_T("LIBRARY_FILE")).m_psz
		, szDNWhereData.GetValue()) != 0)
	{
		LOGOUT("* Err : Download DB Fail");
		LOGOUT("DB Download End");
		return;
	}
	LOGOUT("DB Download End");

	CString strZipPath;
	strZipPath.Format(_T("%s%s"), strFilePath, strFileName);
	theUtil->doUnzip(strZipPath.GetBuffer(), strFilePath.GetBuffer());
	DeleteFile(strZipPath);
}

void CADMStoKCIMDlg::MakeDiagram(int nDiagram_ID)
{
	CString							szRoute, strFilePath, strFileName;
	CPString						szDNWhereData;

	int nDBID = 0;
	if (m_nSTMode == 2)
	{
		nDBID = 3;
	}
	else
	{
		nDBID = 1;
	}

	szRoute.Format(_T("%sST\\"), m_szCSV_Route);
	strFilePath = (MyPath() + szRoute);
	strFileName.Format(_T("%d.zip"), m_nType);

	szDNWhereData.Format("diagram_id = %d", nDiagram_ID);
	if (theDBContainer->MariaDB_DownloadFile(nDBID,
		CW2A(strFilePath).m_psz
		, CW2A(strFileName).m_psz
		, CW2A(_T("diagram")).m_psz
		, CW2A(_T("diagram_file")).m_psz
		, szDNWhereData.GetValue()) != 0)
	{
		LOGOUT("* Err : Download DB Fail");
		LOGOUT("DB Download End");
		return;
	}
	LOGOUT("DB Download End");

	CString strZipPath;
	strZipPath.Format(_T("%s%s"), strFilePath, strFileName);
	theUtil->doUnzip(strZipPath.GetBuffer(), strFilePath.GetBuffer());

	CString szSVGPath;
	szSVGPath.Format(_T("%sdiagram.svg"), strFilePath);
	CString szNEW_SVGPath;
	szNEW_SVGPath.Format(_T("%s%d.svg"), strFilePath, m_nType);
	CopyFile(szSVGPath, szNEW_SVGPath, FALSE);


	DeleteFile(szSVGPath); //파일 변환후 삭제
	DeleteFile(strZipPath);
}

void CADMStoKCIMDlg::MakeDiagram_HDOF(int nMember_ID)
{
	CString szRoute, strPath;
	strPath.Format(_T("%s%s"), MyPath(), m_szCSV_Route);
	strPath.Delete(strPath.GetLength() - 1, 1);
	int nIdx = strPath.ReverseFind(_T('\\'));
	strPath.Delete(nIdx, strPath.GetLength() - nIdx);

	/////////////// \\0\\ST
	CString strSTPath;
	strSTPath.Format(_T("%s\\0\\ST\\"), strPath);
	/////////////// \\Export
	CString strExportPath;
	strExportPath.Format(_T("%s\\Export\\"), strPath);

	CString strFileName;
	strFileName.Format(_T("%d.zip"), m_nType);
	CString strTitleName = strFileName;
	int nDBID = 0;
	if (m_nSTMode == 2)
	{
		nDBID = 3;
	}
	else
	{
		nDBID = 1;
	}

	CPString						szDNWhereData;
	szDNWhereData.Format("MEMBER_OFFICE_FK = %d", nMember_ID);

	if (theDBContainer->MariaDB_DownloadFile(nDBID,
		CW2A(strSTPath).m_psz
		, CW2A(strFileName).m_psz
		, CW2A(_T("diagram")).m_psz
		, CW2A(_T("diagram_file")).m_psz
		, szDNWhereData.GetValue()) != 0)
	{
		LOGOUT("* Err : Download DB Fail");
		LOGOUT("DB Download End");
		return;
	}
	LOGOUT("DB Download End");

	CString strZipPath;
	strZipPath.Format(_T(""));
	strZipPath.Format(_T("%s%s"), strSTPath, strFileName);
	theUtil->doUnzip(strZipPath.GetBuffer(), strSTPath.GetBuffer());

	CString szSVGPath;
	szSVGPath.Format(_T(""));
	szSVGPath.Format(_T("%sdiagram.svg"), strSTPath, nMember_ID);

	CString szNEW_SVGPath;
	szNEW_SVGPath.Format(_T(""));
	szNEW_SVGPath.Format(_T("%s%d.svg"), strSTPath, nMember_ID);
	CopyFile(szSVGPath, szNEW_SVGPath, FALSE);

	DeleteFile(szSVGPath); //파일 변환후 삭제
	DeleteFile(strZipPath);
}

void CADMStoKCIMDlg::CreateFolder(CString csPath)
{
	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;
	while ((nEnd = csPath.Find('/', nStart)) >= 0)
	{
		CString csToken = csPath.Mid(nStart, nEnd - nStart);
		CreateDirectory(csPrefix + csToken, NULL);

		csPrefix += csToken;
		csPrefix += _T("/");
		nStart = nEnd + 1;
	}
	csToken = csPath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);
}

int CADMStoKCIMDlg::SET_MTRND(int nDLID)
{
	int i;
	int nMTRID = 0;

	for (i = 0; i < m_arrDL.GetSize(); i++)
	{
		if (i + 1 == nDLID)
		{
			nMTRID = m_arrDL[i].dl_ii_mtr;
			break;
		}
	}

	for (i = 0; i < m_arrBR.GetSize(); i++)
	{
		if (i + 1 == nMTRID)
		{
			return m_arrBR[i].br_ii_tnd;
			break;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::SET_SSID(CString stDate)
{
	int i;
	CString szSS;
	int	nSS;

	for (i = 0; i < m_arrMTR.GetSize(); i++)
	{
		if (m_arrMTR[i].mtr_maintrid == stDate)
		{
			nSS = m_arrMTR[i].mtr_ii_ss;
			break;
		}
	}
	for (i = 0; i < m_arrSS.GetSize(); i++)
	{
		if (i + 1 == nSS)
		{
			return m_arrSS[i].ss_hi_snv;
			break;
		}
	}
	return 0;
	//mtr ceq아이디를 받아옴!!! 아 어떻게 찾지!!!
}

int CADMStoKCIMDlg::SET_br_ii_fnd(CString stDate)
{
	int i;
	CString szSS;
	int	nSS;
	int nND;

	for (i = 0; i < m_arrMTR.GetSize(); i++)
	{
		if (m_arrMTR[i].mtr_maintrid == stDate)
		{
			nSS = m_arrMTR[i].mtr_ii_ss;
			break;
		}
	}
	for (i = 0; i < m_arrSS.GetSize(); i++)
	{
		if (i + 1 == nSS)
		{
			nND = m_arrSS[i].ss_nd;
			return nND + 1;
			break;
		}
	}
	return 0;
	//mtr ceq아이디를 받아옴!!! 아 어떻게 찾지!!!
}

int CADMStoKCIMDlg::SET_br_ii_tnd(CString stDate)
{
	int nNDID = 0;
	CString szCEQ;
	CString stCNFK;


	m_map_DS_TER_CEQCN.Lookup(stDate, stCNFK);
	m_map_DS_CN_NDID.Lookup(stCNFK, nNDID);
	if (nNDID != 0)
	{
		return nNDID;
	}
	return 0;
}

void CADMStoKCIMDlg::DeleteAllFiles(CString dirName)
{
	CFileFind finder;

	BOOL bWorking = finder.FindFile((CString)dirName + "/*.*");

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}

		CString filePath = finder.GetFilePath();
		DeleteFile(filePath);
	}
	finder.Close();
}

int CADMStoKCIMDlg::GetMasterCD(int nCeqTp, int nCircuit)
{
	for (int i = 0; i < 500; i++)
	{
		if (nCeqTp == m_nMstCD[i][1] && nCircuit == m_nMstCD[i][2]) return m_nMstCD[i][0];
	}
	return 9999;
}

float CADMStoKCIMDlg::GET_PRDE_AO_VALUE(CString stMRID, int nCEQ_TYPE, int nCODE_FK)
{
	CString stCEQID;
	float dValue = 0;
	int nMasterCode = 0;

	if (nCEQ_TYPE == 58)
	{
		if (m_map_PRDE_TYPE_58.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 59)
	{
		if (m_map_PRDE_TYPE_59.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 60)
	{
		if (m_map_PRDE_TYPE_60.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 65)
	{
		if (m_map_PRDE_TYPE_65.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 68)
	{
		if (m_map_PRDE_TYPE_68.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 75)
	{
		if (m_map_PRDE_TYPE_75.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 302)
	{
		if (m_map_PRDE_TYPE_302.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 301)
	{
		if (m_map_PRDE_TYPE_301.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 305)
	{
		if (m_map_PRDE_TYPE_305.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 304 || nCEQ_TYPE == 315 || nCEQ_TYPE == 316)
	{
		if (m_map_PRDE_TYPE_304315316.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 312)
	{
		if (m_map_PRDE_TYPE_312.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 317)
	{
		if (m_map_PRDE_TYPE_317.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 318)
	{
		if (m_map_PRDE_TYPE_318.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (nCEQ_TYPE == 319)
	{
		if (m_map_PRDE_TYPE_319.Lookup(nCODE_FK, nMasterCode))
		{
			stCEQID.Format(_T("%s%d"), stMRID, nMasterCode);
		}
	}
	if (m_map_PRDE_AO_VALUE.Lookup(stCEQID, dValue))
	{
		return dValue;
	}
	return 0;
}

int	CADMStoKCIMDlg::GET_PRDE_TCCSET(int nTCCSET)
{
	if (nTCCSET == 11)
	{
		return  1;
	}
	else if (nTCCSET == 16)
	{
		return   2;
	}
	else if (nTCCSET == 45)
	{
		return  4;
	}
	else if (nTCCSET == 41 || nTCCSET == 46)
	{
		return  5;
	}
	else if (nTCCSET == 42 || nTCCSET == 47)
	{
		return  6;
	}
	else if (nTCCSET == 36)
	{
		return  11;
	}
	else if (nTCCSET == 37)
	{
		return 12;
	}
	else if (nTCCSET == 38)
	{
		return 13;
	}
	else if (nTCCSET == 39)
	{
		return  14;
	}
	else
	{
		return 0;
	}
	return 0;
}


//SI

CBranch* CADMStoKCIMDlg::GetBranch(CString strMRID)
{
	if (strMRID.IsEmpty()) return NULL;
	CBranch* pbranch;
	int  i = 0;
	if (m_map_IDBranch.Lookup(strMRID, i))
	{
		pbranch = m_pBranchArr.GetAt(i - 1);
		if (!pbranch)
		{
			return NULL;
		}
		return pbranch;
	}
	return NULL;
}

CNode* CADMStoKCIMDlg::GetNode(CString strMRID)
{
	if (strMRID.IsEmpty()) return NULL;
	CNode* pNode;
	int  i = 0;
	if (m_map_IDNode.Lookup(strMRID, i))
	{
		pNode = m_pNodeArr.GetAt(i - 1);
		if (!pNode)
		{
			return NULL;
		}
		return pNode;
	}
	return NULL;
}

CNode* CADMStoKCIMDlg::GetNode_New(CString strMRID)
{
	if (strMRID.IsEmpty()) return NULL;
	CNode* pNode;

	int  i = 0;
	if (m_map_IDNode_NEW.Lookup(strMRID, i))
	{
		pNode = m_pNodeArr.GetAt(i - 1);
		if (!pNode)
		{
			return NULL;
		}
		return pNode;
	}
	return NULL;
}

CNode* CADMStoKCIMDlg::GetNodeTER_NEW(CBranch* pBranch)
{
	int  i = 0;
	int  nCheck = 0;

	CNode *pNodeNew = new CNode();
	pNodeNew->m_nKind = 0;
	pNodeNew->m_strMRID.Format(_T("%s10"), pBranch->m_strMRID);
	pNodeNew->m_strName.Format(_T("%s"), pBranch->m_strName);
	pNodeNew->m_strCEQID.Format(_T("%s"), pBranch->m_strMRID);
	pNodeNew->m_nName_Type = pBranch->m_nKind;
	m_pNodeArr.Add(pNodeNew);

	m_map_IDNode.SetAt(pNodeNew->m_strMRID, m_pNodeArr.GetSize());
	m_map_IDNode_NEW.SetAt(pNodeNew->m_strMRID, m_pNodeArr.GetSize());

	//
	ND_STA stNd;
	memset(&stNd, 0, sizeof(ND_STA));
	_stprintf_s(stNd.nd_nm, L"%s", pNodeNew->m_strName);
	_stprintf_s(stNd.nd_ceqid, L"%s", pNodeNew->m_strCEQID);
	_stprintf_s(stNd.nd_connectivitynodeid, L"%s", pNodeNew->m_strMRID);
	stNd.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
	stNd.nd_ii_snv = Get_MRID_SNV(pBranch->m_strMRID);
	if (stNd.nd_ii_snv == 0)
	{
		stNd.nd_ii_snv = 0;
	}

	m_arrND.Add(stNd);
	GND_STA stGND;
	memset(&stGND, 0, sizeof(GND_STA));
	_stprintf_s(stGND.gnd_nm, L"%s", pNodeNew->m_strName);
	stGND.gnd_hi_nd = (int)m_arrND.GetSize();
	m_arrGND.Add(stGND);

	pNodeNew->m_nNDID = (int)m_arrND.GetSize();
	pNodeNew->m_nGNDID = (int)m_arrGND.GetSize();
	nCheck = 9999;

	CNode* pNode;
	//
	if (nCheck == 9999)
	{
		if (m_map_IDNode_NEW.Lookup(pNodeNew->m_strMRID, i))
		{
			pNode = m_pNodeArr.GetAt(i - 1);
			if (!pNode)
			{
				return NULL;
			}
			return pNode;
		}

	}
	return NULL;
}

CNode* CADMStoKCIMDlg::GetNodeTER_NEW_LNSEC(CBranch* pBranch)
{
	int  i = 0;
	int  nCheck = 0;

	CNode *pNodeNew = new CNode();
	pNodeNew->m_nKind = 0;
	pNodeNew->m_strMRID.Format(_T("%s90"), pBranch->m_strMRID);
	pNodeNew->m_strName.Format(_T("%s-LNSEC"), pBranch->m_strName);
	pNodeNew->m_strCEQID.Format(_T("%s"), pBranch->m_strMRID);
	pNodeNew->m_nName_Type = pBranch->m_nKind;
	m_pNodeArr.Add(pNodeNew);

	m_map_IDNode.SetAt(pNodeNew->m_strMRID, m_pNodeArr.GetSize());
	m_map_IDNode_NEW.SetAt(pNodeNew->m_strMRID, m_pNodeArr.GetSize());

	//
	ND_STA stNd;
	memset(&stNd, 0, sizeof(ND_STA));
	_stprintf_s(stNd.nd_nm, L"%s", pNodeNew->m_strName);
	_stprintf_s(stNd.nd_ceqid, L"%s", pNodeNew->m_strCEQID);
	_stprintf_s(stNd.nd_connectivitynodeid, L"%s", pNodeNew->m_strMRID);
	stNd.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
	stNd.nd_ii_snv = Get_MRID_SNV(pBranch->m_strMRID);
	if (stNd.nd_ii_snv == 0)
	{
		stNd.nd_ii_snv = 0;
	}

	m_arrND.Add(stNd);
	GND_STA stGND;
	memset(&stGND, 0, sizeof(GND_STA));
	_stprintf_s(stGND.gnd_nm, L"%s", pNodeNew->m_strName);
	stGND.gnd_hi_nd = (int)m_arrND.GetSize();
	m_arrGND.Add(stGND);

	pNodeNew->m_nNDID = (int)m_arrND.GetSize();
	pNodeNew->m_nGNDID = (int)m_arrGND.GetSize();
	nCheck = 9999;

	CNode* pNode;
	//
	if (nCheck == 9999)
	{
		if (m_map_IDNode_NEW.Lookup(pNodeNew->m_strMRID, i))
		{
			pNode = m_pNodeArr.GetAt(i - 1);
			if (!pNode)
			{
				return NULL;
			}
			return pNode;
		}

	}
	return NULL;
}

CNode* CADMStoKCIMDlg::GetNodeTER_NEW_CB(CBranch* pBranch, CNode* pNode)
{
	int  i = 0;
	int  nCheck = 0;
	int nNAME_TYPE_FK = 0;
	//m_map_Ter_CNID_NAMETYPEFK.Lookup(pNode->m_strMRID, nNAME_TYPE_FK);

	if (pNode->m_nName_Type == 51 || pNode->m_nName_Type == 14)
	{
		CNode *pNodeNew = new CNode();
		pNodeNew->m_nKind = 0;
		pNodeNew->m_strMRID.Format(_T("%s98"), pBranch->m_strMRID);
		pNodeNew->m_strName.Format(_T("%s"), pBranch->m_strName);
		pNodeNew->m_strCEQID.Format(_T("%s"), pBranch->m_strMRID);
		pNodeNew->m_nName_Type = pBranch->m_nKind;
		m_pNodeArr.Add(pNodeNew);

		m_map_IDNode.SetAt(pNodeNew->m_strMRID, m_pNodeArr.GetSize());
		m_map_IDNode_NEW.SetAt(pNodeNew->m_strMRID, m_pNodeArr.GetSize());


		//
		ND_STA stNd;
		memset(&stNd, 0, sizeof(ND_STA));
		_stprintf_s(stNd.nd_nm, L"%s", pNodeNew->m_strName);
		_stprintf_s(stNd.nd_ceqid, L"%s", pNodeNew->m_strCEQID);
		_stprintf_s(stNd.nd_connectivitynodeid, L"%s", pNodeNew->m_strMRID);
		stNd.nd_ii_gnd = (int)m_arrGND.GetSize() + 1;
		stNd.nd_ii_snv = Get_MRID_SNV(pBranch->m_strMRID);
		if (stNd.nd_ii_snv == 0)
		{
			stNd.nd_ii_snv = 0;
		}
		m_arrND.Add(stNd);

		GND_STA stGND;
		memset(&stGND, 0, sizeof(GND_STA));
		_stprintf_s(stGND.gnd_nm, L"%s", pNodeNew->m_strName);
		stGND.gnd_hi_nd = (int)m_arrND.GetSize();
		m_arrGND.Add(stGND);

		pNodeNew->m_nNDID = (int)m_arrND.GetSize();
		pNodeNew->m_nGNDID = (int)m_arrGND.GetSize();
		nCheck = 9999;

		//
		if (nCheck == 9999)
		{
			if (m_map_IDNode_NEW.Lookup(pNodeNew->m_strMRID, i))
			{
				pNode = m_pNodeArr.GetAt(i - 1);
				if (!pNode)
				{
					return NULL;
				}
				//m_map_Ter_CNID_NAMETYPEFK.SetAt(pNode->m_strMRID, 51);
				return pNode;
			}
		}
	}

	return NULL;
}

//II
int CADMStoKCIMDlg::GetCENTER_II_HDOF(int nDate)
{
	int i = 0;
	for (i = 0; i < m_arrHDOF.GetSize(); i++)
	{
		if (nDate == m_arrHDOF[i].HDOF_CODE)
		{
			return  i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetBOF_II_CENTER(CString stDate)
{
	int i = 0;
	for (i = 0; i < m_arrCENTER.GetSize(); i++)
	{
		if (stDate == m_arrCENTER[i].center_officeid)
		{
			return  i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetDL_II_BOF(int nDate) //현재 99번 초기값
{
	int i = 0;
	for (i = 0; i < m_arrBOF.GetSize(); i++)
	{
		if (nDate == m_arrBOF[i].bof_AREA_BASE_CODE)
		{
			return i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetSNV_II_SS(CString stDate)
{
	int i = 0;
	for (i = 0; i < m_arrSS.GetSize(); i++)
	{
		if (stDate == m_arrSS[i].ss_substationid)
		{
			return i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetMTR_II_SS(CString stDate)
{
	int i = 0;
	for (i = 0; i < m_arrSS.GetSize(); i++)
	{
		if (stDate == m_arrSS[i].ss_substationid)
		{
			return i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetDL_II_MTR(CString stDate)
{
	int i = 0;
	for (i = 0; i < m_arrMTR.GetSize(); i++)
	{
		if (stDate == m_arrMTR[i].mtr_maintrid)
		{
			return i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetSSID(int  nDate)
{
	int i = 0;
	CString stsubfk;
	if (m_map_DL_DLID_SUBS.Lookup(nDate, stsubfk))
	{
		if (stsubfk.IsEmpty())
		{
			return 0;
		}
	}
	for (i = 0; i < m_arrSS.GetSize(); i++)
	{
		if (stsubfk == m_arrSS[i].ss_substationid)
		{
			return i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetMTRID(int  nDate)
{
	int i = 0;
	CString stMTRfk;
	if (m_map_DL_DLID_MTR.Lookup(nDate, stMTRfk))
	{
		if (stMTRfk.IsEmpty())
		{
			return 0;
		}
	}
	for (i = 0; i < m_arrMTR.GetSize(); i++)
	{
		if (stMTRfk == m_arrMTR[i].mtr_maintrid)
		{
			return i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetDLID(CString stDate)
{
	int i = 0;
	CString steqcfk;
	if (m_map_CEQ_MridCHCeq.Lookup(stDate, steqcfk))
	{
		if (steqcfk.IsEmpty())
		{
			return 0;
		}
	}
	if (m_map_DL_EQCID.Lookup(steqcfk, i))
	{
		if (i != 0)
		{
			return i;
		}
		//return 0;
	}
	return 0;
}

int CADMStoKCIMDlg::GetDLID_ORIGINAL(CString stDate)
{
	int i = 0;
	CString steqcfk;
	if (m_map_CEQ_MridCHCeq.Lookup(stDate, steqcfk))
	{
		if (steqcfk.IsEmpty())
		{
			return 0;
		}
	}
	if (m_map_DL_EQCID.Lookup(steqcfk, i))
	{
		if (i != 0)
		{
			return i;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GetDLID_OCB(CString stDate)
{
	int i = 0;
	for (i = 0; i < m_arrDL.GetSize(); i++)
	{
		if (stDate == m_arrDL[i].stDL_CEQFK)
		{
			return  i + 1;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GET_GENUNIT_STA_TR(CString stDate, int nGENID)
{
	int nGENUNIT_CONNECTION_TR_TYPE = 0;
	for (int i = 0; i < m_arrGENUNIT.GetSize(); i++)
	{
		if (m_arrGENUNIT[i].GENUNIT_II_EQU_ID == stDate)
		{
			m_arrGENUNIT[i].GENUNIT_II_GEN = nGENID;
			nGENUNIT_CONNECTION_TR_TYPE = m_arrGENUNIT[i].GENUNIT_CONNECTION_TR_TYPE;;
		}
	}
	if (nGENUNIT_CONNECTION_TR_TYPE != 0)
	{
		return nGENUNIT_CONNECTION_TR_TYPE;
	}
	return 0;
}

int CADMStoKCIMDlg::Get_OVERHEAD_CABLE(int nDate)
{
	int nCABLE = 0;
	if (m_map_LINESEGMENT_TYPE_CABLE.Lookup(nDate, nCABLE))
	{
		if (nCABLE != 0)
		{
			return nCABLE;
		}
	}
	return 0;
}

double CADMStoKCIMDlg::Get_POSR(float fDate, int nDate)
{
	float fPOSITIVE_R;
	if (m_map_LINESEGMENT_TYPE_POSR.Lookup(nDate, fPOSITIVE_R))
	{
		if (fPOSITIVE_R != 0)
		{
			return fDate * fPOSITIVE_R;
		}
	}
	return 0.001;
}

double CADMStoKCIMDlg::Get_POSX(float fDate, int nDate)
{
	float fPOSITIVE_X;
	if (m_map_LINESEGMENT_TYPE_POSX.Lookup(nDate, fPOSITIVE_X))
	{
		if (fPOSITIVE_X != 0)
		{
			return fDate * fPOSITIVE_X;
		}
	}
	return 0.001;
}

double CADMStoKCIMDlg::Get_ZERR(float fDate, int nDate)
{
	float fZERO_R;
	if (m_map_LINESEGMENT_TYPE_ZERR.Lookup(nDate, fZERO_R))
	{
		if (fZERO_R != 0)
		{
			return fDate * fZERO_R;
		}
	}
	return 0.001;
}

double CADMStoKCIMDlg::Get_ZERX(float fDate, int nDate)
{
	float fZERO_X;
	if (m_map_LINESEGMENT_TYPE_ZERX.Lookup(nDate, fZERO_X))
	{
		if (fZERO_X != 0)
		{
			return fDate * fZERO_X;
		}
	}
	return 0.001;
}

int CADMStoKCIMDlg::Get_POWERCONTROL_FLAG_VALUES(int nTYPE, int nPOWERCONTROL_FLAG_VALUES)
{
	int nDate[10];

	nDate[0] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 1) - 1)) ? 1 : 0;
	nDate[1] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 2) - 1)) ? 1 : 0;
	nDate[2] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 3) - 1)) ? 1 : 0;
	nDate[3] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 4) - 1)) ? 1 : 0;
	nDate[4] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 5) - 1)) ? 1 : 0;
	nDate[5] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 6) - 1)) ? 1 : 0;
	nDate[6] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 7) - 1)) ? 1 : 0;
	nDate[7] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 8) - 1)) ? 1 : 0;
	nDate[8] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 9) - 1)) ? 1 : 0;
	nDate[9] = (nPOWERCONTROL_FLAG_VALUES & ((1 << 10) - 1)) ? 1 : 0;



	if (nTYPE == 1) // GENUNIT_LVRTXGROUP_FLAG
	{
		if (nDate[1] == 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if (nTYPE == 2)
	{
		if (nDate[0] == 0 && nDate[2] == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (nTYPE == 3)
	{
		if (nDate[0] == 1 && nDate[2] == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (nTYPE == 4)
	{
		if (nDate[5] == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (nTYPE == 5)
	{
		if (nDate[0] == 1 && nDate[2] == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (nTYPE == 6)
	{
		if (nDate[0] == 0 && nDate[2] == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	return 0;
}

int CADMStoKCIMDlg::GetSS_HI_SNV(int nDate)
{
	int i = 0;
	for (i = 0; i < m_arrSNV.GetSize(); i++)
	{
		if (nDate == m_arrSNV[i].snv_ii_ss)
		{
			return i + 2;
		}
	}
	return 0;
}

int CADMStoKCIMDlg::GET_GEN_II_LNSEC(int nData)
{
	for (int i = 0; i < m_arrBR.GetSize(); i++)
	{
		if (m_arrBR[i].br_ii_fnd == nData)
		{
			return m_arrBR[i].br_ii_equ;
		}
		if (m_arrBR[i].br_ii_tnd == nData)
		{
			return m_arrBR[i].br_ii_equ;
		}
	}
	return 0;
}

void CADMStoKCIMDlg::Conv_DL_Info_Array(CString strCEQ_FK, CString strEQC_FK)
{
	BOOL bFind = FALSE;
	CConv_DL_Info* pData;
	for (int i = 0; i < m_pConv_DL_InfoArr.GetSize(); i++)
	{
		pData = m_pConv_DL_InfoArr.GetAt(i);
		if (pData->m_szDL_CEQ_MRFK == strCEQ_FK)
		{
			pData->m_pEQC_MRFK_Arry.Add(strEQC_FK);
			bFind = TRUE;
			break;
		}
	}

	if (!bFind)
	{
		pData = new CConv_DL_Info();
		m_pConv_DL_InfoArr.Add(pData);
		pData->m_szDL_CEQ_MRFK = strCEQ_FK;
		pData->m_szDL_EQC_MRFK = strEQC_FK;
		pData->m_pEQC_MRFK_Arry.Add(strEQC_FK);
	}
}


CString	 CADMStoKCIMDlg::GetEQC_MRFK(CString strEQC_FK)
{
	CConv_DL_Info* pData;
	for (int i = 0; i < m_pConv_DL_InfoArr.GetSize(); i++)
	{
		pData = m_pConv_DL_InfoArr.GetAt(i);
		if (pData->Check2EQC_MRFK(strEQC_FK))
		{
			return pData->m_szDL_EQC_MRFK;
		}
	}

	CString strMsg;
	strMsg.Format(_T("DL EQC 정보 없음(%s)"), strEQC_FK);
	TRACE(strMsg);
	return _T("");
}

int	CADMStoKCIMDlg::Get_MRID_SNV(CString strMRID)
{
	//20231208속도개선
	//MRID(설비CEQID 를 가져오면 SNV ID를 보내준다)
	int nSNVID = 0;
	CString stedlqcfk;
	CString stvleqcfk;
	if (m_map_CEQ_MridCHCeq.Lookup(strMRID, stedlqcfk)) //설비 CEQ - > 13번 DL 을 찾을때 쓰는 부분 
	{
		if (stedlqcfk.IsEmpty())
		{
			return 0;
		}
	}
	if (m_map_DL_EQC_VL_EQC.Lookup(stedlqcfk, stvleqcfk))
	{
		if (stedlqcfk.IsEmpty())
		{
			return 0;
		}
	}
	if (m_map_SNV_MTRSNVID.Lookup(stvleqcfk, nSNVID))
	{
		return nSNVID;
	}
	return 0;
}

void CADMStoKCIMDlg::GET_MULTISW_Check_KASIM(CString strMRID, int nType, CString stNAME, CString szALIAS_NAME, CString strCEQ, CString szMULTISW, int nMULTNUMBER)
{
	//
	CString strSQL;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char buffer[4096] = { 0x00, };
	char query[4096] = { 0x00, };
	TCHAR tcBuffer[4096] = { 0x00, };
	int len = 4096;
	int fields = 0;
	int nCircuit = 0;
	//
	int nn1;
	CNode *pNode;
	CBranch *pBranch;
	CString szCommon_CFND, szCommon_CTND, szCommon_ND;
	int nBranch = 0;
	/////////////////
	strSQL.Format(_T("select aa.mrid, aa.NAME_TYPE_FK, aa.NAME ,aa.ALIAS_NAME  ,cc.CEQ_TYPE_FK, bb.COMPOSITE_SWITCH_MRFK ,cc.seq from identifiedobject aa, switch bb, conductingequipment cc \
			 where aa.MRID = bb.CEQ_MRFK and bb.CEQ_MRFK=cc.MRID and aa.MRID IN(select MRID from conductingequipment WHERE MRID IN(select CEQ_MRFK from switch where COMPOSITE_SWITCH_MRFK = %s)  AND SEQ < %d)")
		, szMULTISW, nMULTNUMBER);
	   
	_stprintf_s(tcBuffer, _countof(tcBuffer), strSQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("멀티스위치  체크 테이블 SQL 오류"));
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);		// 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		strMRID = GetCString_Row(row[0]);
		nType = GetInt_Row(row[1]);
		stNAME = GetCString_Row(row[2]);
		szALIAS_NAME = GetCString_Row(row[3]);
		strCEQ = GetCString_Row(row[4]);
		szMULTISW = GetCString_Row(row[5]);
		nMULTNUMBER = GetInt_Row(row[6]);

		m_map_Common_CBSW_MUSW.Lookup(strMRID, nBranch);
		if (nBranch != 0)
		{
			continue;
		}

		m_map_Common_CB_FND.Lookup(strMRID, szCommon_CFND);
		m_map_Common_CB_TND.Lookup(strMRID, szCommon_CTND);

		nn1 = 0;
		nn1 = stNAME.Find(',');
		if (nn1 > -1)
		{
			stNAME.Replace(_T(","), _T("_"));
		}
		stNAME.Remove('\r');	//개행 엔터
		stNAME.Remove('\n');	//개행 엔터	
		stNAME.Remove('\t');	//탭

		if (!(szCommon_CFND.IsEmpty()))
		{
			pNode = new CNode();
			pNode->m_nKind = 0;
			pNode->m_strMRID.Format(_T("%s"), szCommon_CFND);
			pNode->m_strName.Format(_T("%s"), stNAME);
			pNode->m_strCEQID.Format(_T("%s"), strMRID);
			pNode->m_nName_Type = nType;
			m_pNodeArr.Add(pNode);
			//MAP정보
			m_map_Common_ND_CnCeq.SetAt(szCommon_CFND, strMRID);
			m_map_IDNode.SetAt(szCommon_CFND, m_pNodeArr.GetSize());
		}
		if (!(szCommon_CTND.IsEmpty()))
		{
			pNode = new CNode();
			pNode->m_nKind = 0;
			pNode->m_strMRID.Format(_T("%s"), szCommon_CTND);
			pNode->m_strName.Format(_T("%s"), stNAME);
			pNode->m_strCEQID.Format(_T("%s"), strMRID);
			pNode->m_nName_Type = nType;
			m_pNodeArr.Add(pNode);
			//MAP정보
			m_map_Common_ND_CnCeq.SetAt(szCommon_CTND, strMRID);
			m_map_IDNode.SetAt(szCommon_CTND, m_pNodeArr.GetSize());
		}

		pBranch = new CBranch();
		pBranch->m_strMRID.Format(_T("%s"), strMRID);
		pBranch->m_nKind = nType;
		pBranch->m_strName.Format(_T("%s"), stNAME);
		pBranch->m_strNum.Format(_T("%s"), szALIAS_NAME);
		pBranch->m_nCeq_Type = _wtoi(strCEQ);
		pBranch->m_nKCIM_Type = 1; // 1.SW 2.ACL 3.입상주 4.DL
		if (szMULTISW.IsEmpty())
		{
			szMULTISW.Format(_T("0"));
			pBranch->m_szMULTISW.Format(_T("%s"), szMULTISW);
			pBranch->m_nMULTICIR_NUMBER = 0;
		}
		else
		{
			pBranch->m_szMULTISW.Format(_T("%s"), szMULTISW);
			pBranch->m_nMULTICIR_NUMBER = nMULTNUMBER;
		}
		m_pBranchArr.Add(pBranch);

		m_map_IDBranch.SetAt(strMRID, m_pBranchArr.GetSize());
		m_map_Common_CBSW_MUSW.SetAt(strMRID, m_pBranchArr.GetSize());

	}
	mysql_free_result(result);
}

int CADMStoKCIMDlg::GetMasterCode_KASIM(int nMst, int nCeqTp)
{
	CString strSQL;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char buffer[4096] = { 0x00, };
	char query[4096] = { 0x00, };
	TCHAR tcBuffer[4096] = { 0x00, };
	int len = 4096;
	int fields = 0;
	int nCircuit = 0;
	/////////////////
	strSQL.Format(L"select circuit from `%s`.`master_code` where  master_code_id='%d' and ceq_type_fk='%d'", m_szADMS_Code, nMst, nCeqTp);

	_stprintf_s(tcBuffer, _countof(tcBuffer), strSQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("마스터코드 정보 테이블 SQL 오류"));
		return 0;
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);		// 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		nCircuit = GetInt_Row(row[0]);
	}
	mysql_free_result(result);

	return nCircuit;
	/////////////////////////////////
}

void CADMStoKCIMDlg::MakeDiagram_LoasID_KASIM(int m_nType)
{
	int nDiagram_ID = 0;
	CString strSQL;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char buffer[4096] = { 0x00, };
	char query[4096] = { 0x00, };
	TCHAR tcBuffer[4096] = { 0x00, };
	int len = 4096;
	int fields = 0;
	/////////////////
	strSQL.Format(L"select DIAGRAM_ID from diagram where member_office_fk = %d;", m_nType);

	_stprintf_s(tcBuffer, _countof(tcBuffer), strSQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("마스터코드 정보 테이블 SQL 오류"));
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);		// 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		nDiagram_ID = GetInt_Row(row[0]);
	}
	mysql_free_result(result);

	MakeSymbol();	   //심볼 변환
	MakeDiagram(nDiagram_ID);	   //svg 파일 변환
}

void CADMStoKCIMDlg::MakeDiagram_Member_KASIM()
{
	int nDiagram_ID = 0;	
	CString strSQL;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char buffer[4096] = { 0x00, };
	char query[4096] = { 0x00, };
	TCHAR tcBuffer[4096] = { 0x00, };
	int len = 4096;
	int fields = 0;
	/////////////////
	strSQL.Format(L"select MEMBER_OFFICE_FK from diagram");

	_stprintf_s(tcBuffer, _countof(tcBuffer), strSQL);
	WideCharToMultiByte(CP_ACP, 0, tcBuffer, len, query, len, NULL, NULL);  //TCHAR -> char 변환
	mysql_query(MYSQL_DB, "set Names euckr");
	if (mysql_query(MYSQL_DB, query))
	{
		IDC_LIST_DATA_HISTORY_ERROR(_T("diagram 정보 테이블 SQL 오류"));
	}

	result = mysql_store_result(MYSQL_DB);
	fields = mysql_num_fields(result);		// 필드 갯수 구함
	while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
	{
		nDiagram_ID = GetInt_Row(row[0]);
		MakeDiagram_HDOF(nDiagram_ID);
	}
	mysql_free_result(result);

	MakeSymbol();
}


