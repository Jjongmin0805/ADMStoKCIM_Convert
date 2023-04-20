#pragma once

// KASIM MMDB Field 클래스
class AFX_EXT_CLASS CPAppDataObj_Field
{
public:
	CPAppDataObj_Field( unsigned int unOfficeCode );		// KASIM MMDB Field 생성자
	CPAppDataObj_Field( unsigned int unOfficeCode, char* szTableName, int nIndex, char* szFieldName, int nDataTypeID, int nVarSize, int nVarCount = 1, char* szExplan = NULL, int nMaxSize = 0 );		// KASIM MMDB Field 생성자
	~CPAppDataObj_Field(void);		// KASIM MMDB Field 소멸자

	void																	ClearBuffData();		// KASIM 필드 Data 삭제
	unsigned int															GetSize_SchemaInforFIELD();		// KASIM 필드 스키마 데이터 크기 얻기
	void																	GetData_SchemaInforFIELD( char *pSchemaData, unsigned int &unDataPosition );		// KASIM 필드 스키마 데이터 얻기
	void																	SetData_SchemaInforFIELD( char *pSchemaData, unsigned int &unDataPosition, int nTableMaxRecord, bool bIsCreateBuffData );		// KASIM 필드 스키마 데이터 설정
	int																		GetIndex();		// KASIM 필드 Index 얻기
	char*																	GetName();		// KASIM 필드 명칭 얻기
	int																		GetVarTypeID();		// KASIM 필드 종류ID 얻기
	int																		GetVarSize();		// KASIM 필드 크기 얻기
	int																		GetVarCount();		// KASIM 필드 개수 얻기
	char*																	GetExplane();		// KASIM 필드 설명 얻기
	unsigned int															GetVarRealSize();		// KASIM 필드 실데이터 개수 얻기
	double																	PutString2Value( char* szTableName, int nIndex, int nElement, char* pValue );		// 문자열 값을 KASIM 데이터로 입력
	double																	PutString2Value( int nIndex, int nElement, char* pValue );		// 문자열 값을 KASIM 데이터로 입력
	void																	PutBuffData( unsigned int unIndex, int nElement, void* pValue );		// KASIM 데이터 입력
	void																	RunExtractionMMDBData( FILE* pHBDataFileStream, unsigned int &unStartPos );		// KASIM 데이터 파일로 추출

protected:
	void																	DeleteBuffData();		// KASIM Data 삭제

protected:
	unsigned int					m_unOfficeCode;							// 사업소 코드
	char*							m_pBuffData;							// 실데이터 관리
	int								m_nIndex;								// 필드 인덱스
	char							m_szName[ 128 ];						// 필드 명칭
	short							m_snVarTypeID;							// 필드 변수타입 id
	int								m_nVarSize;								// 필드 변수타입 사이즈
	int								m_nVarCount;							// 필드 변수 배열 갯수
	char							m_szExplane[ 512 ];						// 필드 변수 명칭
	int								m_nTableMaxRecordSize;					// 테이블 레코드 총갯수
};

typedef hash_map< string, CPAppDataObj_Field* >							HMAP_FIELD;
typedef hash_map< int, CPAppDataObj_Field* >							HMAP_INDEXFIELD;
typedef map< int, CPAppDataObj_Field* >									MAP_INDEXFIELD;
typedef vector< CPAppDataObj_Field* >									VEC_INDEXFIELD;
