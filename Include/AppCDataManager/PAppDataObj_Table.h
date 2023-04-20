#pragma once

#include "PAppDataObj_Field.h"

// KASIM MMDB 테이블 클래스
class CPAppDataObj_Table
{
public:
	CPAppDataObj_Table( unsigned int unOfficeCode );		// KASIM MMDB 테이블 클래스 생성자
	CPAppDataObj_Table( unsigned int unOfficeCode, unsigned int unDBType, unsigned int unDBEnv, char *szName, unsigned int unMaxCount );		// KASIM MMDB 테이블 클래스 생성자
	~CPAppDataObj_Table(void);		// KASIM MMDB 테이블 클래스 소멸자

	void																	SortIndexField();		// 필드 Index 정보 정렬
	unsigned int															GetSize_SchemaInforTABLE();		// KASIM 필드 스키마 데이터 크기 얻기
	void																	GetData_SchemaInforTABLE( char *pSchemaData, unsigned int &unDataPosition );		// KASIM 필드 스키마 데이터 얻기
	void																	SetData_SchemaInforTABLE( char *pSchemaData, unsigned int &unDataPosition, bool bIsCreateBuffData );		// KASIM 필드 스키마 데이터 설정
	CPAppDataObj_Field*														GetFieldData( char* szFieldName );		// 필드 데이터 얻기
	CPAppDataObj_Field*														GetFieldData( int nFieldIndex );		// 필드 데이터 얻기
	unsigned int															GetDBType();		// RDB 타입 얻기
	unsigned int															GetDBEnv();		// RDB 용도 정보 얻기
	char*																	GetName();		// 명칭 얻기
	void																	SetLoadingRecordCount( unsigned int nRecordCount );		// 실데이터 개수 설정
	unsigned int															GetLoadingRecordCount();		// 실데이터 개수 얻기
	unsigned int															GetMaxRecordCount();		// 최대데이터 개수 설정

	void																	Insert_FieldData( int nIndex, char* szFieldName, int nDataTypeID, int nVarSize, int nVarCount, char* szExplan );		// 필드 정보 등록
//	void																	GET_CheckData( char* szFieldName, unsigned int unIndex, unsigned int unElement );
	bool																	RunExtractionMMDBData( CPString szHBIndexHeaderName, FILE* pHBDataFileStream, HMAP_STR4UINT &hmHBDataFileIndex, unsigned int &unStartPos );		// KASIM RDB에 따른 필드 데이터를 저장
	int																		LoadingCSVtable( unsigned int unRDBIndex );		// 테이블 명칭으로 된 CSV 파일 읽기
	int																		LoadingCSVtableEx( unsigned int unRDBIndex );		// 테이블 명칭으로 된 CSV 파일 읽기(확장)
	int																		LoadingXMLtable( CSAXMgr *pSAXObj, unsigned int unRDBIndex, unsigned int &unRecordCnt );		// 테이블 명칭으로 된 XML 파일 읽기
	void																	Find_Data( char *szField, unsigned int unStartID, unsigned int unCountID, unsigned int unRealRecordCount );		// 필드 명칭으로 KASIM 데이터 얻기
	void																	ReportOut_TableData( unsigned int unStartID, unsigned int unCountID );		// 테이블 정보를 Report하기
	void																	ReportOut_FieldData( char *szFieldName, unsigned int nStartID, unsigned int nEndID, unsigned int unRealRecordCount );		// 필드 정보를 Report하기
	CPString																ReportOut_List();		// KASIM 테이블에 맞는 스키마 및 데이터에 대한 Report
	void																	GetFieldNames( VECSTR &vecFieldNames );		// 필드 명칭 얻기
	void																	GetFieldVarInfor( VECFIELDVARINFOR &vecFieldVarInfor );		// 필드 변수 정보 얻기
	void																	XMLWriteKASIMData_Table( FILE* pWriteDataFileStream );		// XML 파일로 저장

protected:
	void																	ClearFIELDs();		// 필드 정보 삭제
	int																		GetCSVData( FILE* pCSVFile, char* pRetBuffData, int &nRetBuffSize );		// 필드 정보 CSV파일 읽기

protected:
	unsigned int					m_unOfficeCode;

	unsigned int					m_unDBType;								// MMDB 종류 : Static(Topology), Dynamic, USER Input
	unsigned int					m_unDBEnv;								// DB Environment : 1 = Application Governor, 2 = Study Mode Process, 3 = DUAL Process
	char							m_szName[ 128 ];						// 테이블 명칭
	unsigned int					m_unLoadingRecordCount;					// CSV 파일에서 Loading한 Data 개수
	unsigned int					m_unMaxRecordCount;						// 테이블 최대 개수

	CPAppDataObj_Field*				m_pOrderInfo_Field;						// 가장 최근 접근한 Field 정보
//	VECPSTR							m_vecFieldName;							// 필드 명칭 저장
	HMAP_FIELD						m_hmapInfor_Field;						// 필드 정보 저장
//	MAP_INDEXFIELD					m_mapInfor_IndexField;
	VEC_INDEXFIELD					m_vec_IndexField;						// 필드 Index 정보 저장
};
 
typedef hash_map< string, CPAppDataObj_Table* >							HMAP_TABLE;
//typedef hash_map< CPString, CPAppDataObj_Table* >							HMAP_TABLE;
//typedef hash_map< CPString, int >											HMAP_TABLEFILTER;

//typedef CAtlMap< CPString, CPAppDataObj_Table*, ATLTRAITS_STRING >									ALTMAP_TABLE;
//typedef CAtlMap< CPString, int, ATLTRAITS_STRING >													ALTMAP_TABLEFILTER;