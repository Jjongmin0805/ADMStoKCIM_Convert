#pragma once

#include "PAppDataObj_Table.h"

// KASIM MMDB RealDB 클래스
class CPAppDataObj_RDB
{
public:
	CPAppDataObj_RDB( unsigned int unOfficeCode, unsigned int unDBIndex );		// KASIM MMDB RealDB 클래스 생성자
	~CPAppDataObj_RDB(void);		// KASIM MMDB RealDB 클래스 소멸자

	unsigned int															GetSize_SchemaInforRDB();		// KASIM 필드 스키마 데이터 크기 얻기
	void																	GetData_SchemaInforRDB( char *pSchemaData );		// KASIM 필드 스키마 데이터 얻기
	void																	SetData_SchemaInforRDB( char *pSchemaData, bool bIsCreateBuffData );		// KASIM 필드 스키마 데이터 설정
	unsigned int															GetDBIndex();		// 설정된 RDB Index 얻기
	unsigned int															GetFieldVarType( char* szTable, char* szField );		// 테이블과 필드 명칭에 맞는 필드 변수 크기 얻기
	unsigned int															GetFieldVarSize( char* szTable, char* szField );		// 테이블과 필드 명칭에 맞는 필드 변수 종류 얻기

	void																	Insert_TableData( unsigned int unDBType, unsigned int unDBEnv, char *szTableName, unsigned int unMaxCount );		// KASIM MMDB Category 테이블 입력
	void																	Insert_FieldData( char *szTableName, int nIndex, char* szFieldName, int nDataTypeID, int nVarSize, int nVarCount, char* szExplan );		// KASIM MMDB Category 필드 입력
	bool																	IsTable( char *szTableName );		// KASIM MMDB Category 테이블 여부 확인
//	bool																	RunExtractionMMDBData( FILE* pHBDataFileStream, HMAP_STR4UINT &hmHBDataFileIndex, unsigned int &unStartPos );
	bool																	MakeKASIMS_RDBData();		// KASIM RDB를 생성
	bool																	ConnectKASIMS_RDBData();		// 생성된 KASIM RDB를 연결
	bool																	CopyKASIM_HBFileRDBData( CPAppDataObj_RDB* pSourRDB, bool bForcedContinue_CopyFail );		// KASIM MMDB Category RDB 파일 복제
	int																		CopyKASIM_MMDBRDBData( CPAppDataObj_RDB* pSourRDB );		// KASIM MMDB Category RDB 복제
	void																	InsertSTAorDYNIndex_TABLE();		// Static & Dynamic RDB Index 파일 생성

//	void																	GET_CheckData( char *szTableName, char* szFieldName, unsigned int unIndex, unsigned int unElement );
	void*																	GetData( char* szTable, char* szField, unsigned int nIndex, int &nFieldVarType );		// KASIM Data 얻기
	void*																	GetData( char* szTable, char* szField, unsigned int nIndex, int nElement, int &nFieldVarType );		// KASIM Data 얻기
	CPString																GetData2String( char* szTable, char* szField, unsigned int unIndex, int nElement );		// KASIM Data 문자열로 얻기
	double																	GetData2Double( char* szTable, char* szField, unsigned int unIndex, int nElement );		// KASIM Data Double형 수치로 얻기
	void																	PutData( char* szTable, char* szField, unsigned int unIndex, int nElement, void* pValue );		// KASIM Data 넣기
	void																	PutDouble2Data( char* szTable, char* szField, unsigned int unIndex, int nElement, double dValue );		// Double형 값을 KASIM Data 넣기
	void																	PutString2Data( char* szTable, char* szField, unsigned int unIndex, int nElement, char* pValue );		// 문자열 값을 KASIM Data 넣기
	void																	LoadCSVrdb( CPString szFilterName = "" );		// CSV 파일 읽기
	void																	LoadXMLrdb( CSAXMgr *pSAXObj, CPString szTableName );		// CSV 파일 읽기
	void																	LoadKASIMS_HBYFile2TableRealCountRDB();		// KASIM 테이블에 해당하는 실데이터 개수를 재설정

	void																	Find_Data_Rdb( char *szTable, char *szField, unsigned int unStartID, unsigned int unCountID );		// KASIM RDB 데이터 찾기
	void																	ReportOut_Table_Rdb( char *szTable, unsigned int unStartID, unsigned int unCountID );		// 테이블 정보를 Report하기
	void																	ReportOut_Field_Rdb( char *szTable, char *szField, unsigned int nStartID, unsigned int nEndID );		// 필드 정보를 Report하기
	void																	ReportOut_List( int nMode, char *szTable );		// KASIM 테이블에 맞는 스키마 및 데이터에 대한 Report
	void																	ReportOut_ListALLTable( char *szTable, vector< string > &vecTableInfor );		// KASIM 모든 테이블의 스키마 및 데이터에 대한 Report
	unsigned int															GetTableRealCount( char *szTableName );		// 테이블에 따른 실데이터의 개수 얻기
	void																	SetTableRealCount( char *szTableName, unsigned int unRealCount );		// 테이블에 따른 실데이터의 개수 설정
	unsigned int															GetTableMaxCount( char *szTableName );		// 테이블에 따른 최대 개수
	void																	GetTableFieldNames( char *szTableName, VECSTR &vecFieldNames );		// 필드 명칭을 얻기
	void																	GetTablePosORFieldVarInfor( bool bIsGetBuffData, char *szTableName, VECFIELDVARINFOR &vecFieldVarInfor );		// 테이블에 맞는 필드의 정보와 데이터 위치를 얻기
	void																	SetTableBuffDataByKASIMRDB( char* szTable, char* szField, char* pValue, unsigned int unSize );		// KASIM RDB 데이터를 등록
	void																	SortIndexTableField();		// 에 맞게 테이블 및 필드를 정렬
	//void																	XMLWriteKASIMData_RDB( FILE* pWriteDataFileStream, char* szFilter );
	void																	XMLWriteKASIMData_RDB( FILE* pWriteDataFileStream );		// KASIM RDB 스키마 및 데이터를 XML 파일로 저장
	void																	XMLWriteRealData_RDB( FILE* pWriteDataFileStream, char* szFilter );		// KASIM RDB 데이터를 XML 파일로 저장
	void																	XMLWriteCSVData_RDB( CPString szPath, char* szFilter, bool bIsSendReviewerMsg, bool bIsShowLogMessage );		// KASIM XML 파일을 읽어 CSV 파일로 저장
	void																	GetTableNames( VECSTR &vecTableNames );		// 테이블 명칭을 얻기
	int																		GetTableNamesAndLoadingCount( VECPSTR &vecTableNames, VECUINT &vecTableLoadingCount );		// KASIM 테이블에 따른 데이터 개수 얻기
	void																	SetTableNameLoadingCount( CPString szSearchTableName, unsigned int unTableLoadingCount );		// KASIM 테이블에 따른 데이터 개수 설정
	void																	SetTableName4ZeroCount_MaxCount();		// KASIM 테이블 실데이터 개수가 0인 테이블을 Max 개수로 설정

	unsigned int															GetRDBHBFileMMDBSize();		// KASIM RDB 파일 크기 얻기
	unsigned int															GetIRBHBIndexMMDBSize();		// KASIM RDB index 크기 얻기
	char*																	GetIRBHBIndexMMDBData();		// KASIM RDB 데이터 크기 얻기
	void																	ClearTableData( char* szTableName );		// KASIM 테이블 데이터 삭제

protected:
	void																	ClearMMDB();		// KASIM RDB 삭제
	void																	ClearHBDataFileIndex();		// KASIM RDB Index 삭제
	void																	ClearTABLEs();		// KASIM RDB에 등록된 테이블 삭제

	unsigned int															GetTableDataMovePoint( unsigned int unIndex, int nElement, int nFieldVarCount, unsigned int unFieldVarSize, unsigned int unFieldVarRealSize );		// 
	CPAppDataObj_Table*														GetTableData( char *szTableName );		// KASIM 테이블 정보 얻기
	long																	GetHBINdexPosition( char* szTable, char* szField );		// KASIM 테이블 및 필드 데이터 위치 얻기
	unsigned int															GetHBINdexTableRealCountPosition( char* szTable );		// KASIM 테이블 실데이터 개수 정보 위치 얻기
	CPString																GetHBFileObjName();		// KASIM RDB 파일 명칭 얻기
	CPString																GetHBFileIndexObjName();		// KASIM RDB Index 파일 명칭 얻기
	CPString																GetHBIndexObjName();		// KASIM RDB Index 명칭 얻기
	CPString																GetHBIndexObjName( unsigned int unID );		// KASIM RDB Index 명칭 얻기
	CPString																GetHBIndexObjTableFieldName( char* szTable, char* szField );		// 테이블 및 필드 명칭에 맞는 KASIM RDB Index 명칭 얻기
	bool																	GetDataMovePosition(	bool bIsGetMode, unsigned int &unMovePosValue, unsigned int &unFieldVarType, unsigned int &unFieldVarSize, 
																									unsigned int &unFieldVarRealSize, char* szTable, char* szField, unsigned int unIndex, int nElement );		// KASIM RDB Index 위치 및 필드 정보 얻기

protected:
	unsigned int					m_unOfficeCode;
	unsigned int					m_unID;									// MMDB INDEX : STA, SCHEDUL, EVENT, EXCEPTION, STUDY
	CPString						m_szRData_File, m_szIData_File;			// Hybrid-File MMDB Real Data, Index Data 파일 명칭
	CFileMapObject					*m_pRData_MMDB, *m_pIData_MMDB;			// MMDB Real Data, Index Data
	HMAP_STR4UINT					m_hmHBDataFileIndex;					// MMDB RDB에 따른 위치 Index 정보
	CPAppDataObj_Table				*m_pOrderInfo_Table;					// 가장 최근에 검색된 Table 정보
	HMAP_TABLE						m_hmInfor_Table;						// RDB Table 정보 저장( Online & Study Mode에 따라 저장되는 내용이 다름)
	HMAP_TABLE						m_hmExcludeInfor_Table;					// RDB Online에 저장되는 정보외의 Table 정보
};

typedef vector< CPAppDataObj_RDB* >											VECAPPDATAOB_RDB;
