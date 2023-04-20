#pragma once

#include "PAppDataObj_RDB.h"

// KASIM MMDB Category 클래스
// RDB( Real DB )
class CPAppDataObj_Category
{
public:
	CPAppDataObj_Category(void);			// KASIM MMDB Category 클래스 생성자
	CPAppDataObj_Category( unsigned int unOfficeCode );			// KASIM MMDB Category 클래스 생성자( 사업소 코드 구분 )
	~CPAppDataObj_Category(void);			// KASIM MMDB Category 클래스 소멸자

	unsigned int															GetOfficeCode();				// KASIM MMDB Category 사업소 코드 얻기
	unsigned int															GetSelectRDBDyn();				// 최종 사용한 선택된 RDB ID 얻기
	unsigned int															GetSize_SchemaInforCATEGORY( unsigned int unDBIndex );				// DBIndex에 맞는 KASIM MMDB Category 크기 얻기
	void																	GetData_SchemaInforCATEGORY( char *pSchemaData, unsigned int unDBIndex );				// 스키마 데이터와 DBIndex에 맞는 KASIM MMDB Category 크기 얻기
	void																	SetData_SchemaInforCATEGORY( char *pSchemaData, unsigned int unDBIndex, bool bIsCreateBuffData );				// 스키마 데이터와 DBIndex에 맞는 KASIM MMDB Category 설정
	void																	Insert_RDBData( unsigned int unDBIndex );				// DBIndex에 맞는 KASIM MMDB Category RDB 등록
	void																	Delete_RDBData( unsigned int unDBIndex );				// DBIndex에 맞는 KASIM MMDB Category RDB 삭제
	bool																	CopyHBFile_RDBData( unsigned int unSourceDBIndex, unsigned int unTargetDBIndex, bool bForcedContinue_CopyFail );				// KASIM MMDB Category RDB 파일 복제
	void																	CopyMMDB_RDBData( unsigned int unSourceDBIndex, unsigned int unTargetDBIndex );				// KASIM MMDB Category RDB 복제
	bool																	IsRDBData( unsigned int unDBIndex );				// KASIM MMDB Category RDB 여부 확인
	void																	Insert_Table( unsigned int unDBIndex, unsigned int unDBType, unsigned int unDBEnv, char *szTableName, unsigned int unMaxCount );				// KASIM MMDB Category 테이블 입력
	void																	Insert_Field( unsigned int unDBIndex, char *szTableName, int nIndex, char* szFieldName, int nDataTypeID, int nVarSize, int nVarCount, char* szExplan );				// KASIM MMDB Category 필드 입력
	void																	MakeKASIMS_SchemaInforCategory( bool bIsCreateBuffData );				// KASIM MMDB Category 스키마 생성
	void																	MakeKASIMS_SchemaInforCategory( unsigned int unDBIndex, bool bIsCreateBuffData );				// DBIndex에 맞는 KASIM MMDB Category 스키마 생성
	bool																	MakeKASIMS_HBYFile();				// KASIM MMDB Category 데이터 파일 생성
	bool																	MakeKASIMS_HBYFile( unsigned int unDBIndex );				// DBIndex에 맞는 KASIM MMDB Category 데이터 파일 생성
	bool																	ConnectKASIMS_HBYFile();				// KASIM MMDB Category 데이터 파일 연결
	void																	LoadKASIMS_HBYFile2TableRealCount();				// KASIM MMDB Category 데이터 실제 개수 얻기
	void																	InsertSTAorDYNIndex_RDB();				// Static & Dynamic RDB Index 파일 생성
	bool																	SelectRDB( unsigned int unDBIndex );				// 최종으로 사용한 RDB Index 설정
	unsigned int															GetFieldVarSize( char *szTable, char *szField );				// 테이블과 필드 명칭에 맞는 필드 변수 크기 얻기
	unsigned int															GetFieldVarType( char *szTable, char *szField );				// 테이블과 필드 명칭에 맞는 필드 변수 종류 얻기

//	void																	GET_CheckData( char *szTableName, char* szFieldName, unsigned int unIndex, unsigned int unElement );				// 
	void*																	GetData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, int &nFieldVarType );				// KASIM Data 얻기
	CPString																GetData2String( unsigned int unRDBIndex, char* szTable, char* szField, unsigned int unIndex, int nElement );				// KASIM Data 문자열로 얻기
	double																	GetData2Double( unsigned int unRDBIndex, char* szTable, char* szField, unsigned int unIndex, int nElement );				// KASIM Data Double형 수치로 얻기
	void																	PutData( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, void* pValue );				// KASIM Data 넣기
	void																	PutDouble2Data( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, double dValue );				// Double형 값을 KASIM Data 넣기
	void																	PutString2Data( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unIndex, int nElement, char* pValue );				// 문자열 값을 KASIM Data 넣기
	void																	LoadCSVcategory( int nCompressionMode );				// CSV 파일 읽기
	void																	LoadCSVcategory( int nCompressionMode, unsigned int unDBIndex, CPString szFilterName = "" );				// 특정 위치에 있는 CSV파일을 읽어서 RDB Index에 맞게 넣음
	bool																	LoadXMLcategory( CSAXMgr *pSAXObj );				// 대용량 CSV파일을 읽음

	void																	Find_Data_Category( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int unStartID, unsigned int unCountID );				// Category 찾기
	void																	ReportOut_Table_Category( unsigned int unRDBIndex, char *szTable, unsigned int unStartID, unsigned int unCountID );				// 테이블 정보를 Report하기
	void																	ReportOut_Field_Category( unsigned int unRDBIndex, char *szTable, char *szField, unsigned int nStartID, unsigned int nEndID );				// 필드 정보를 Report하기
	void																	ReportOut_List( unsigned int unRDBIndex, int nMode, char *szTableName );				// KASIM 스키마 및 데이터에 대한 Report
	unsigned int															GetTableRealCount( char *szTableName, int nRDBIndex );				// 테이블에 따른 실데이터의 개수
	unsigned int															GetTableMaxCount( char *szTableName );				// 테이블에 따른 최대 개수
	void																	GetTableNames( VECSTR &vecTableNames );				// 테이블 명칭을 얻기
	void																	GetTableFieldNames( char *szTableName, VECSTR &vecFieldNames );				// 테이블에 맞는 필드 개수 얻기
	void																	GetTablePosORFieldVarInfor( unsigned int unRDBIndex, bool bIsGetBuffData, char *szTableName, VECFIELDVARINFOR &vecFieldVarInfor );				// 테이블에 맞는 필드의 정보와 데이터 위치를 얻기
	void																	SetTableBuffDataByKASIMRDB( unsigned int unRDBIndex, char *szTable, char *szField, char* pValue, unsigned int unSize );				// KASIM RDB 데이터를 등록
	void																	SortIndexRDBTableField();				// RDB에 맞게 테이블 및 필드를 정렬
	void																	XMLWriteKASIMData_Category( FILE* pWriteDataFileStream, unsigned int unRDBIndex );				// KASIM 데이터를 CSV 파일로 입력
	void																	XMLWriteReadData_Category( FILE* pWriteDataFileStream, unsigned int unRDBIndex, char* szFilter );				// KASIM 데이터를 CSV 파일로 입력 - 테이블 명칭으로 필터링 가능
	void																	XMLWriteCSV_Category( unsigned int unRDBIndex, CPString szPath, char* szFilter, bool bIsSendReviewerMsg, bool bIsShowLogMessage );				// 특정 RDB에 해당하는 테이블에 따른 CSV파일 저장
	void																	ClearRDBTableData( unsigned int unRDBIndex, char *szTableName );				// 테이블에 해당하는 데이터 삭제
	
protected:
	void																	Remove_RDBData();				// KASIM MMDB Category RDB 삭제

	CPAppDataObj_RDB*														GetRDBData( unsigned int unDBIndex );				// DBIndex에 맞는 KASIM MMDB Category RDB 얻기
	CPAppDataObj_RDB*														GetTableName2CategoryRDB( unsigned int unRDBIndex, char *szTable );				// DBIndex와 테이블 명칭에 맞는 KASIM MMDB Category RDB 얻기
	void																	SetRealCount_STA2DYN();				// Static과 Dynamic의 실데이터 개수를 설정
	void																	SetZeroRealCount_InjectionMaxCount();				// KASIM MMDB Category 테이블의 최대 개수가 존재하지 않을 경우 스키마에 따른 최대 개수를 설정

protected:
	unsigned int					m_unOfficeCode;				// KASIM MMDB Category 사업소 코드
	CPAppDataObj_RDB				*m_pOrderInfo_RDB_STA, *m_pOrderInfo_RDB_DYN;				// KASIM MMDB Category Static, Dynamic RDB
	VECAPPDATAOB_RDB				m_vecInfo_RDB;				// KASIM MMDB Category RDB 배열
};

typedef hash_map< unsigned int, CPAppDataObj_Category* >					HMAP_CATEGORY;
