#pragma once

#include "PKASIM_Schema_VarData.h"
#include "PAppDataObj_Category.h"

#ifdef _APPGOVERNOR_COM_VER_
#include "../PUSDT_PCommManager/PLocker.h"
#include "../PUSDT_PCommManager/PWThread.h"
#else
#include "../include/PUSDT_PCommManager/PLocker.h"
#include "../include/PUSDT_PCommManager/PWThread.h"
#endif

// 사업소 정보 클래스
class COfficeCodeInfor
{
public:
	COfficeCodeInfor() : m_unOfficeCode( 0U ), m_nRDBType( -1 ), m_nRDBIndex( 0 )		// 사업소 정보 클래스 생성자
	{
		//
	};
	~COfficeCodeInfor()			// 사업소 정보 클래스 소멸자
	{
		//
	};

public:
	CPString				m_szApplRunName;			// 응용프로그램 실행 명칭
	unsigned int			m_unOfficeCode;			// 사업소코드
//	int						m_nCreateCategoryStatus;				// -1 : Init, 0 : stand by, 1: STA & DYN Create, 2: DYN Update
	int						m_nRDBType;						// 1 : Static, 2 : Dynamic
	int						m_nRDBIndex;					// RDB Index
};

// KASIM 관리 클래스
class AFX_EXT_CLASS CPAppDataObjMng
{
public:
	static CPAppDataObjMng*													InitInstance( void );
	static void																Close( void );

protected:
	static CPAppDataObjMng*			m_pMngData;

//------------------------------------------------------------------------------------------------

public:
	CPAppDataObjMng(void);		// KASIM 관리 클래스 생성자
	~CPAppDataObjMng(void);			// KASIM 관리 클래스 소멸자

	bool																	ConnectDB();				// KASSIM 연결
	void																	CloseDB();			// KASSIM 연결 해제
	int																		GetOnlineStudyMode();					// Online과 Study Mode 얻기
	void																	SetOnlineStudyMode( int nMode );			// Online과 Study Mode 설정
	CPString																GetKASIMAP_WriteIndex();				// KASIM RDB 데이터 얻기
	void																	SetKASIMAP_WriteIndex( CPString nWriteIndex );				// KASIM RDB 데이터 설정
	void																	MakeKASIMAP_WriteIndex();				// KASIM RDB 데이터 생성
	CPString																GetKASIMST_WriteIndex();				// KASIM RDB Index 데이터 얻기
	void																	SetKASIMST_WriteIndex( CPString nWriteIndex );				// KASIM RDB Index 데이터 설정
	void																	MakeKASIMST_WriteIndex();				// KASIM RDB Index 데이터 생성

	void																	AddOfficeCode( CPString szOfficeCodesString );		// 사업소 정보 등록
	void																	AddOfficeCode( unsigned int unOfficeCode );		// 사업소 정보 등록
	void																	DeleteOfficeCode( unsigned int unOfficeCode );		// 사업소 정보 삭제
	void																	SetOfficeInforStatus( unsigned int unOfficeCode, int nRDBType, int nRDBIndex );		// 사업소 정보 상태 설정
	void																	MakeOfficeInfor_MakeTopology( unsigned int unOfficeCode, int nRDBIndex );		// KASIM 계통 정보 생성
	void																	MakeOfficeInfor_UpdateDynamic( unsigned int unOfficeCode, int nRDBIndex );		// KSAIM Dynamic 정보 갱신
	int																		GetOfficeInforRDBType( unsigned int unOfficeCode );		// KASIM RDB 종류 얻기
	void																	DeleteCategoryData( unsigned int unOfficeCode );				// 사업소 코드의 카테고리 제거
	unsigned int															GetFieldVariant( unsigned int unVarType );				// 스키마 필드의 변수 정보를 얻기
	unsigned int															GetTableType( char* szTableName );					// 테이블의 종류를 얻기
	unsigned int															GetDefault_STAorDYN( unsigned int unSTAorDYN );			// 한전 응용소프트웨어 정보 모델의 정적과 동적 여부 얻기
	void																	SetTableType( char* szTableName, unsigned int unSTAorDYN );		// 스키마 테이블 명칭에 대한 정적과 동적 종류 설정
	void																	ReleaseOfficeInfor();					// 사업소 정보 삭제
	void																	InitOfficeInfor();										// 스키마 정보를 만들기 위한 초기화 작업
	int																		IsConnectKASIMData( unsigned int unOfficeCode, unsigned int unRDBIndex );		// 사업소코드에 맞는 KASIM 연결 여부 확인
	bool																	ConnectKASIMData( unsigned int unOfficeCode, unsigned int unRDBIndex = KASIMRDBINDEX_DYN );		// 사업소코드에 맞는 KASIM 연결
	void																	DIsConnectKASIMData( unsigned int unOfficeCode );		// 사업소코드에 맞는 KASIM 연결 해제
	void																	DIsConnectKASIMRDBData( unsigned int unOfficeCode, unsigned int unRDBIndex );		// 사업소코드와 KASIM RDB에 맞는 KASIM RDB 연결 해제
	void																	LoadOfficeInfor();										// 사업소별로 mmdb를 생성
	char*																	GetSchemaDataSTA();										// 스키마중 정적데이터 얻기
	char*																	GetSchemaDataDYN();										// 스키마중 동적데이터 얻기
	void																	InsertSTAorDYNIndex();									// 스키마의 정적과 동적 색인 정보 추가
	CPAppDataObj_Category*													GetOrderOfficeCode();									// 최종 수행한 사업소 코드 얻기
	void																	SetSelectFirstofficecode();									// 최초 선택한 사업소 코드 얻기
	void																	SetofficecodeNrdbtype( unsigned int unOfficeCode, unsigned int unRDBType );			// Search MMDB 선택 = Office, RDBType
	int																		SelectOfficecodeANDRDBType();							// OfficeCode 및 DBType 값을 읽어 정보를 설정
	int																		SetSelectOffceCodeNRDBIndex( unsigned int unOfficeCode, unsigned int unRDBIndex );			// 최종 선택된 사업소코드의 RDB Index를 설정
	int																		SetSelectRDBIndex( unsigned int unRDBIndex );			// RDB Index 설정
	void																	LoadCSV( int nCompressionMode );			// KASIM CSV파일을 읽기
	bool																	LoadXML( unsigned int unOfficeCode );			// KASIM XML파일을 읽기
	bool																	LoadXML( CPAppDataObj_Category *pCategoryInfor );			// KASIM XML파일을 읽기

	void																	Find_Data( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unStartID, unsigned int unCountID );			// KASIM 데이터를 찾기
	void																	ReportOut_Table( unsigned int unRDBIndex, char *szTable, unsigned int unStartID, unsigned int unCountID );			// KASIM 테이블 정보 Report
	void																	ReportOut_Field( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int nStartID, unsigned int nEndID );			// KASIM 필드 정보 Report
	void																	ReportOut_List( unsigned int unRDBIndex, int nMode, char *szTable );			// KASIM 정보 데이터 Report
	CPString																ReportOut_SelectInfor();							// 정보 표출할 KASIM RDB Index 정보 Report
	unsigned int															GetTableRealCount( CPString szTableName, int nRDBIndex = 1 );		// 테이블에 따른 실데이터의 개수 얻기(char 타입)
	unsigned int															GetTableMaxCount( CPString szTableName );	// 테이블에 따른 최대 개수(char 타입)
	void																	GetTableNames( VECSTR &vecTableNames );			// KASIM 테이블 명칭 얻기(char 타입)
	void																	GetTableFieldNames( CPString szTableName, VECSTR &vecFieldNames );			// KASIM 테이블, 필드 명칭 얻기(char 타입)
	void																	GetTablePosORFieldVarInfor( unsigned int unRDBIndex, bool bIsGetBuffData, CPString szTableName, VECFIELDVARINFOR &vecFieldVarInfor );			// KASIM 필드 정보 및 RDB Index 위치 정보 얻기

	char*																	GetText( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex );							// KASIM Data Text값 얻기(char 타입)
	void*																	GetData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex );							// KASIM Data 수치값 얻기(char 타입)
	void*																	GetData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement );			// KASIM Data 수치값 얻기, 배열 위치 지정(char 타입)
	CPString																GetData2String( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex );;		// KASIM Data 문자열로 얻기(char 타입)
	CPString																GetData2String( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement );;		// KASIM Data 문자열로 얻기(char 타입)
	double																	GetData2Double( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex );		// KASIM Data Double형 수치로 얻기(char 타입)
	double																	GetData2Double( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement );		// KASIM Data Double형 수치로 얻기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, char* szValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, short snValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, short snValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, int nValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, unsigned int unValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, unsigned int unValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, long lValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, long lValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, unsigned long ulValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, unsigned long ulValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, float fValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, float fValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, double dValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, double dValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, unsigned long long ullValue );		// KASIM Data 넣기(char 타입)
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, unsigned long long ullValue );		// KASIM Data 넣기(char 타입)
	void																	PutDouble2Data( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, double dValue );		// Double형 값을 KASIM Data 넣기(char 타입)
	void																	PutDouble2Data( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, double dValue );		// Double형 값을 KASIM Data 넣기(char 타입)
	void																	PutString2Data( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, char* pValue );		// 문자열 값을 KASIM Data 넣기(char 타입)
	void																	PutString2Data( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, char* pValue );		// 문자열 값을 KASIM Data 넣기(char 타입)
	void																	SetTableBuffDataByKASIMRDB( unsigned int unRDBIndex, const char *szTable, const char *szField, char* pValue, unsigned int unSize );		// 테이블별 KASIM RDB 데이터 설정

	bool																	IsFindDataViewBreak();		// KASIM Data 화면 표출시 강제 중지 여부 확인
	void																	SetFindDataViewBreak( bool bBreakValue );		// KASIM Data 화면 표출시 강제 중지 설정
	void																	MakeKASIMData( unsigned int unOfficeCode, unsigned int unRDBIndex );		// 사업소 코드의 한전 응용소프트웨어 정보 모델 정보 만들기
	bool																	UpdateKASIMData( unsigned int unOfficeCode, unsigned int unRDBIndex );		// KASIM Data를 갱신

	unsigned int															GetSelectOfficeIndex();		// 최종 선택된 사업소코드 얻기
	unsigned int															GetSelectCategoryKASIMRDBIndex();		// 최종 선택된 Category의 RDB Index를 얻기
	void																	WriteKASIMXMLFile( unsigned int unRDBIndex, char* szFilter, bool bIsCompressAction );			// KASIM 데이터를 XML 파일로 저장(char 타입)
	void																	WriteKASIMCSVFile( unsigned int unRDBIndex, char* szFilter, bool bIsSendReviewerMsg, bool bIsCompressAction, bool bIsLogDBUpload, bool bIsShowLogMessage );		// KASIM 데이터를 CSV 파일로 저장(char 타입)
	void																	WriteKASIMCSVFile_MoveCompression( int nCompressionMode, CPString szSource, CPString szTarget, bool bZipSourceFileDelete );		// KASIM 데이터를 CSV파일로 저장하고 압축하기
	void																	Clear_HBTableData( unsigned int unRDBIndex, char *szTableName );		// KASIM RDB 파일을 삭제

	int																		LoadXML_KASIMSchemaData();		// KASIM XML 파일 스키마 읽기
	void																	LoadXML_KASIMSchemaData_Ex();		// KASIM XML 파일 스키마 읽기
	void																	RunLogDB_Write( CPString szBackupAPFilePath, CPString szBackupSTFilePath );		// KASIM 데이터 파일을 로그 DB에 저장
	int																		SetCSVBackup2CompressionFile( CPString &szDUALSourcePath, CPString &szSTSourcePath, CPString &szBackupDUALPathFile, CPString &szBackupSTPathFile, bool bDeleteCompressSourceFiles );		// KASIM CSV파일을 압축하고 기존 CSV 파일을 삭제
	int																		RunLogDB_Download_AND_UpdateKASIM();		// Log DB에서 KASIM 데이터를 다운로드 받고, KASIM에 Update 모듈
	void																	LoadKASIMSTLogID();							// 최근 저장된 DB의 Study Binary 압축파일 명칭
	int																		MakeOfficeKASIM( unsigned int unOfficeCode );		// 사업소 코드에 해당하는 KASIM 생성
	void																	SetStopExportCSVWork( bool bStopValue );			// export csv 파일 작업시 강제 중지 설정
	bool																	GetStopExportCSVWork();					 // export csv 파일 작업시 강제 중지 여부 얻기
	void																	SetDBLog_Delete();		// LogDB 테이블 데이터 삭제

// WCHAR
	unsigned int															GetTableRealCount( wchar_t *szTableName, int nRDBIndex = 1 );		// 테이블에 따른 실데이터의 개수 얻기(wchar_t 타입)
	unsigned int															GetTableMaxCount( wchar_t *szTableName );		// 테이블에 따른 최대 개수(wchar_t 타입)
	void																	GetTableNames( VECWSTR &vecTableNames );			// KASIM 테이블 명칭 얻기(wchar_t 타입)
	void																	GetTableFieldNames( wchar_t * szTableName, VECWSTR &vecFieldNames );		// KASIM 테이블, 필드 명칭 얻기(wchar_t 타입)

	wchar_t*																GetText( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex );							// KASIM Data Text값 얻기(wchar_t 타입)
	void*																	GetData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex );							// KASIM Data 수치값 얻기(wchar_t 타입)
	void*																	GetData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement );			// KASIM Data 수치값 얻기, 배열 위치 지정(wchar_t 타입)
	CPString																GetData2String( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex );			// KASIM Data 문자열로 얻기(wchar_t 타입)
	CPString																GetData2String( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement );			// KASIM Data 문자열로 얻기(wchar_t 타입)
	double																	GetData2Double( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex );			// KASIM Data Double형 수치로 얻기(wchar_t 타입)
	double																	GetData2Double( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement );			// KASIM Data Double형 수치로 얻기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, wchar_t* szValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, short snValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, short snValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, int nValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, unsigned int unValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, unsigned int unValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, long lValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, long lValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, unsigned long ulValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, unsigned long ulValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, float fValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, float fValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, double dValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, double dValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, unsigned long long ullValue );			// KASIM Data 넣기(wchar_t 타입)
	void																	PutData( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, unsigned long long ullValue );			// 
	void																	PutDouble2Data( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, double dValue );			// Double형 값을 KASIM Data 넣기(wchar_t 타입)
	void																	PutDouble2Data( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, double dValue );			// Double형 값을 KASIM Data 넣기(wchar_t 타입)
	void																	PutString2Data( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, wchar_t* pValue );			// 문자열 값을 KASIM Data 넣기(wchar_t 타입)
	void																	PutString2Data( unsigned int unRDBIndex, wchar_t *szTable, wchar_t *szField, unsigned int unIndex, int nElement, wchar_t* pValue );			// 문자열 값을 KASIM Data 넣기(wchar_t 타입)
	void																	WriteKASIMXMLFile( unsigned int unRDBIndex, wchar_t* szFilter, bool bIsCompressAction );			// KASIM 데이터를 XML 파일로 저장(wchar_t 타입)
	void																	WriteKASIMCSVFile( unsigned int unRDBIndex, wchar_t* szFilter, bool bIsSendReviewerMsg, bool bIsCompressAction, bool bIsLogDBUpload, bool bIsShowLogMessage );		// KASIM 데이터를 CSV 파일로 저장(wchar_t 타입)

	CPString																GetKASIMVer_VerData();		// KASIM Version 얻기
	CPString																GetKASIMVer_LastUpdate();		// KASIM 마지막 업데이트 날짜 얻기
	CPString																GetKASIMVer_MakeBy();		// KASIM 생성자 정보 얻기
	unsigned int															GetTableFieldVariantType( char *szTable, char *szField );		// 테이블 및 필드 명칭에 따른 KASIM 필드 변수 정보 얻기
	void																	SetKASIM_ASSERT_MODE( bool bMode );
	bool																	GetKASIM_ASSERT_MODE();
//	void Test_WriteDB();


protected:
	void																	Clear_VarInfo();						// 스키마 필드의 변수 정보 제거
	void																	Clear_CategoryInfo();					// 스키마 정보중 카테고리를 제거
	void																	Clear_SchemaMMDB();						// 스키마 정보 제거
	void																	Clear_OfficeCode();						// 사업소 등록 정보 제거
	void																	CloseWorkThread();						// 작업 쓰레드 종료

	bool																	MakeKASIMRDB_STADYN( CPAppDataObj_Category *pCategoryData );			// KASIM RDB 생성
	COfficeCodeInfor*														GetOfficeInfor( unsigned int unOfficeCode );			// 사업소 코드 정보 얻기
	void																	MakeKASIMSchema();										// 한전 응용소프트웨어 정보 모델 스키마 읽기
	void																	MakeKASIMSchema_KASIMVer();				// KASIM Version 만들기
	void																	MakeKASIMSchema_KASIMSchema_Variant();			// KASIM 스키마 필드 정보 생성
	void																	MakeKASIMSchema_KASIMSchema_TableField();			// KASIM 스키마 테이블 및 필드 생성
	bool																	LoadKASIMSchema();										// 한전 응용소프트웨어 정보 모델 스키마 읽기
	CPAppDataObj_Category*													GetCategoryData( unsigned int unOfficeCode );			// 사업소 코드의 카테고리 얻기
	void																	XMLWriteKASIMData_Version( FILE* pWriteDataFileStream );			// Write XML Version
	void																	XMLWriteKASIMData_Variant( unsigned int unRDBIndex, FILE* pWriteDataFileStream, char* szFilter, CPAppDataObj_Category* pCategoryData );		// Write XML Version
	CPString																GetMakeName_SchemaData_STA();		// KASIM Static 스키마 데이터 얻기
	CPString																GetMakeName_SchemaData_DYN();		// KASIM Dynamic 스키마 데이터 얻기

	static UINT WINAPI														ThreadWorks_WorkProcess( LPVOID pParam );		// KASIM 생성 작업 쓰레드 함수


	KASIMVER_INFOR					m_stKasimVerInfor;						// 한전 응용소프트웨어 정보 모델 버전 관리
	HMAP_SC_VARDATA					m_hmVarInfo;							// 한전 응용소프트웨어 정보 모델 필드 변수 정보 관리
	HMAP_STR4UINT					m_hmSTAorDYN_Index;						// 한전 응용소프트웨어 정보 모델의 정적 및 동적 구별 정보
	CPAppDataObj_Category			*m_pOrderOfficeCode;					// 선택된 office code의 category 정보
	HMAP_CATEGORY					m_hmCategoryInfo;						// 한전 응용소프트웨어 정보 모델 스키마 및 데이터 관리
	CFileMapObject					*m_pRSchemaData_STA, *m_pRSchemaData_DYN;	// MMDB Real Schema Static, MMDB Real Schema Dynamic

	map< unsigned int, COfficeCodeInfor* >		m_mapOfficeCode;				// 사업소 코드로 찾기위한 map변수

	unsigned int					m_unKasimOfficeCode, m_unKasimRdbIndex;		// 선택된 작업 사업소코드, 선택된 작업 KASIM의 RDB Index
	char							m_szGetDataTextValue[ 512 ];				// 문자열 반환을 위한 일시적인 변수( MULTI BYTE )
	wchar_t							m_szWSGetDataTextValue[ 512 ];				// 문자열 반환을 위한 일시적인 변수( WHCAR )
	bool							m_bFindDataViewBreak;					// 데이터 찾기시 데이터 보기 중단 여부
	CPLocker						*m_pLock_OfficeLoad;					// 사업소에 따른 KASIM Data 읽기중 중복 수행 방지
	CPLocker						*m_pLock_DBLogDelete;					// DBLogDelete 작업시 중복을 회피하기 위한 부분
	CPLocker						*m_pLock_GetPutData;					// Get-Put RDB Data 중복 방지
	CPWThread						*m_pOfficeLoadThread;					// 사업소별 작업 쓰레드
	int								m_nOnlineStudyMode;							// KASIM 설정 변수( 1=Online , 2=Study )
	CPString						m_szKASIMAP_WriteID, m_szKASIMST_WriteID;		// DB Upload시 사용되는 DUAL 명칭, DB Upload시 사용되는 Study 명칭
	bool							m_bStopExportCSVWork;					// export csv 파일 작업시 강제 중지 변수

	int								m_nKASIM_NULL_ReplaceValue;
	long							m_lKASIM_NULL_ReplaceValue;
	float							m_fKASIM_NULL_ReplaceValue;
	double							m_dKASIM_NULL_ReplaceValue;
	long long						m_llKASIM_NULL_ReplaceValue;
	bool							m_bKASIM_AssertMode;
};

#define								theAppDataMng							CPAppDataObjMng::InitInstance()
#define								GETTABLE_MAXCOUNT( a )					CPAppDataObjMng::InitInstance()->GetTableMaxCount( a )