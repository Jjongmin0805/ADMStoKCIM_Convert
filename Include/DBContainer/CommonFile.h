#pragma once

#ifdef _UNICODE
#define								_tstring																			wstring
#else
#define								_tstring																			string
#endif

#define								VALUE2STRING_SIZE																	20480
#define								DBSOURCE_SIZE																		10
#define								TRIMCHAR																			_T(" \t\n\v")
typedef								vector< char* >																		DB_VECSTR;
typedef								vector< CString >																	VECCSTR;
typedef								vector< DB_VECSTR >																	DB_VEC2STR;


struct DBBINDContainer
{
	DBBINDContainer() : dwStatus( 0 )
	{
		memset( szValue, 0x00, sizeof( char ) * 1024 );
	}

	char							szValue[ 1024 ];
	DWORD							dwStatus;
};

enum DBCONTAINER_CONNECTSOURECEMODE
{
	CONNECTSOURCEMODE_NONE = 0, 
	CONNECTSOURCEMODE_MSSQL, 
	CONNECTSOURCEMODE_ORACLE, 
	CONNECTSOURCEMODE_MARIADB, 
	CONNECTSOURCEMODE_SQLITE
};

#include "./Mysql/Include/mysql.h"
#include "./Sqlite/Include/sqlite3.h"
