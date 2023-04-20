#pragma once

class AFX_EXT_CLASS CDB_EXTRACT
{
public:
	CDB_EXTRACT( int nMode = -1, wchar_t* szQuery = NULL, int nSourceIndex = 0 );			// nMode = 0 : DBQuery, 1 : DBExcute
	~CDB_EXTRACT(void);

	// Config XML에서 정의한 DB Database Index 위치 설정 (Default : 0)
	void																						SetSourceIndex( int nSourceIndex );

	// DB 쿼리 수행
	void																						DBExcute( const wchar_t* szQuery );
	// 필드 갯수 얻어오기
	int																							DBfieldCOUNT( const wchar_t* szTableName );
	bool																						GetDBfieldCOUNT();
	bool																						GetDBfieldCOUNT( CString szQuery );

	//
	void																						Close();
	// DB Table 정보 읽기
	int																							DBQuery( const wchar_t* szQuery );
	// DB Table 정보 읽기
	int																							DBQuery_Table( const wchar_t* szTableName, const wchar_t* szSortName = NULL );
	// 현재 레코드 사이즈 계산
	void																						SetThisRecordCount();
	// 추출된 쿼리 레코드 사이즈
	ULONGLONG																					GetRecordCount();
	// 추출된 쿼리의 필드 갯수
	unsigned int																				GetFieldCount();

	bool																						MovePrev();
	bool																						MoveNext();
	bool																						MoveFirst();
	bool																						MoveLast();
	bool																						MoveInitialize();

	// Data 얻기(현재 행의 열)
	INT																							GetData_NUM( unsigned int unField );
	LONG																						GetData_LONG( unsigned int unField );
	CString																						GetData_STR( unsigned int unField );
	FLOAT																						GetData_FLT( unsigned int unField );
	DOUBLE																						GetData_DBL( unsigned int unField );
	LONGLONG																					GetData_LONGLONG( unsigned int unField );
	CTime																						GetData_TIME_c( unsigned int unField );
	time_t																						GetData_TIME_t( unsigned int unField );

	// 성공여부 알기
	bool																						IsOpen();
	//
	bool																						IsSuccess();
	// 쿼리문 얻기
	wchar_t*																					GetQuery(){ return (wchar_t*)m_szQuery.c_str(); };

	void																						RemoveImageData();
	// DB Buff 얻어오기
	int																							DBQuery_GetBuff( const wchar_t* szQuery, LONGLONG llBuffSize );
	int																							DBQuery_SetBuff( byte *pBuffData, ULONGLONG ullBuffSize, CString szTableName, CString szFieldName, CString szWhereCon );

protected : 
	// Data 얻기( 행, 열)
	char*																						GetData( unsigned int unField );

protected : 
	//
	CCommand<CManualAccessor>		m_QueryRS;
	//
	struct DBBINDContainer*			m_pQueryBind;
	//
	HRESULT							m_hQueryOpenResult;
	// 수행 성공여부
	bool							m_bSuccess;



	// 수행 쿼리문
	_tstring						m_szQuery;
	// DB Source Index
	int								m_nSourceIndex;
	//
	unsigned int					m_unFieldSize;
	//
	unsigned long long				m_ullRecordSize;

	MYSQL_RES						*m_pMySQL_DBResult;
	unsigned long long				m_ullMySQL_RowCurrentPos;
	MYSQL_ROW						m_pMySQL_RowData;

public:
	// IMAGE Data size
	LONGLONG						m_llImageDataSize;
	// IMAGE Data
	byte*							m_pImageData;
};

