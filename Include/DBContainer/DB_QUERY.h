#pragma once

class CDBContainer_Object;
class AFX_EXT_CLASS CDB_QUERY
{
public:
	CDB_QUERY( const wchar_t* szQuery, int nSourceIndex = 0 );
	CDB_QUERY( int nSourceIndex = 0 );
	~CDB_QUERY( void );

	
	ULONGLONG																					GetRecordCount();			// 추출된 쿼리 레코드 사이즈
	unsigned int																				GetFieldCount();			// 추출된 쿼리의 필드 갯수

	BOOL																						IsConnect();				// DB 연결 여부
	BOOL																						Connect();					// DB 연결
	void																						DisConnect();				// DB 연결 해제
	bool																						IsSuccess();
	bool																						IsOpen();					// 성공여부 알기
	void																						Close();

	bool																						Query( const wchar_t* szQuery );
	bool																						RunExcute( const wchar_t* szQuery );			// Query 실행
	bool																						RunQuery( const wchar_t* szQuery );				// Query Data 조회
	
	int																							DBQuery_GetBuff( const wchar_t* szQuery, LONGLONG llBuffSize );				// DB Buff 얻어오기
	int																							DBQuery_SetBuff( byte *pBuffData, ULONGLONG ullBuffSize, CString szTableName, CString szFieldName, CString szWhereCon );			// DB Buff 넣기
	bool																						GetDBfieldCOUNT();
	bool																						GetDBfieldCOUNT( CString szQuery );
	void																						SetThisRecordCount();

	// Data 위치 이동
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

// 쿼리문 얻기
	wchar_t*																					GetQuery(){ return (wchar_t*)m_szQuery.c_str(); };
	int																							GetSourceIndex();
	int																							GetSourceDBType();

protected:
	void																						RemoveImageData();
	char*																						GetData( unsigned int unField );			// Data 얻기 Buff

protected:
	CDBContainer_Object				*m_DBContainerOBJ;
	CCommand<CManualAccessor>		m_QueryRS;
	struct DBBINDContainer*			m_pQueryBind;
	HRESULT							m_hQueryOpenResult;
	bool							m_bSuccess;							// 수행 성공여부

	_tstring						m_szQuery;							// 수행 쿼리문
//	int								m_nSourceIndex;						// DB Source Index
	unsigned int					m_unFieldSize;
	unsigned long long				m_ullRecordSize;
	MYSQL_RES						*m_pMySQL_DBResult;
	unsigned long long				m_ullMySQL_RowCurrentPos;
	MYSQL_ROW						m_pMySQL_RowData;
	sqlite3_stmt					*m_pPDB_SQLite3_STMT;

public:
	LONGLONG						m_llImageDataSize;					// IMAGE Data size
	byte*							m_pImageData;						// IMAGE Data

};

