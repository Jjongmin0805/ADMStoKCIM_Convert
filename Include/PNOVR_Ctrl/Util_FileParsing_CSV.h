#pragma once
class AFX_EXT_CLASS CUtil_FileParsing_CSV
{
public:
	CUtil_FileParsing_CSV( wchar_t* szPath, wchar_t* szName );
	CUtil_FileParsing_CSV( wchar_t* szPath );
	~CUtil_FileParsing_CSV(void);

	_tstring																				GetErrStr(){ return m_szErrStr; };

	bool																					Loading();
	bool																					Writing();
	void																					GetParsingData( wchar_t* szPath, wchar_t* szName );
	char*																					CharReplace( char* szData, char* szSource, char* szTarget );
	bool																					IsGetTheData();
	_tstring																				GetData( int nIndex );				// Data Index 1 ~ Max
	int																						GetSize();

	void																					SetString_Header( wchar_t* szStr );
	void																					SetString_Content( wchar_t* szStr );



	wchar_t							m_szFilePath[ 1024 ];
	wchar_t							m_szFileName[ 128 ];

	_tstring						m_szHeader;
	vector<_tstring>				m_vecData;
	_tstring						m_szErrStr;
};

