#pragma once

// 윈도우 버전 설정
#define __WINDOWS__
#define ATLTRAITS_STRING CStringElementTraits< CPString >

//#define								COMM_DEBUG_MODE

#define								errno_noerr								0
#define								PDSETCOMM_MSG_SIZE						128
#define								CHEKC_TIMEOUT_VALUE_s					10.0
#define								LOGWRITE_BUFFSIZE						20480

#include "PString.h"
typedef								vector< CPString >						VECPSTR;
typedef								hash_map< string, string >				HMAP_STR4STR;
typedef								hash_map< string, wstring >				HMAP_STR4WSTR;
typedef								hash_map< wstring, string >				HMAP_WSTR4STR;
typedef								hash_map< wstring, wstring >			HMAP_WSTR4WSTR;
typedef								hash_map< string, int >					HMAP_STR4INT;
typedef								hash_map< wstring, int >				HMAP_WSTR4INT;
typedef								hash_map< string, unsigned int >		HMAP_STR4UINT;
typedef								hash_map< wstring, unsigned int >		HMAP_WSTR4UINT;
typedef								hash_map< unsigned long long, unsigned int>					HMAP_ULL4UINT;
typedef								hash_map< unsigned long long, unsigned long long>			HMAP_ULL4ULL;
typedef								vector< string >						VECSTR;
typedef								vector< wstring >						VECWSTR;
typedef								vector< int >							VECINT;
typedef								vector< unsigned int >					VECUINT;
typedef								vector< unsigned long long >			VECULONGLONG;

enum _PNoIpc_InitType
{
	_PNoIpc_TYPE_MEMMAKE																		= 1,
	_PNoIpc_TYPE_MEMCONN,
	_PNoIpc_TYPE_FILEMAKE,
	_PNoIpc_TYPE_FILECONN
};

enum PDSETCOMMMODE
{
	PDSETCOMMMODE_NONE = 0,
	PDSETCOMMMODE_SERVER,
	PDSETCOMMMODE_CLIENT
};

enum PDSETCOMM_INDEXCODE
{
	PDSETCOMM_IC_ACK = 1,
	PDSETCOMM_IC_STRMSG = 11,
	PDSETCOMM_IC_BUFFDATA = 12
};

struct PUSDT_EraseObjData
{
	template <typename _container> void operator()( _container* pObjdata )
	{
		if( pObjdata != NULL )
		{
			delete pObjdata;
			pObjdata = NULL;
		}
	}
};

typedef struct CallbackWndData
{
	DWORD	m_PID;
	HWND	m_hWnd;
} CallbackWndData;

struct NetworkAdapterInfor
{
	NetworkAdapterInfor() : m_dwID( 0 ), m_nType( 0 ), m_nStatus( -1 )
	{
		//
	};

	DWORD							m_dwID;
	UINT							m_nType;
	string							m_szName;
	string							m_szDescription;
	string							m_szAddress;
	int								m_nStatus;
};

#define								DEF_NEW( var_new )											new var_new