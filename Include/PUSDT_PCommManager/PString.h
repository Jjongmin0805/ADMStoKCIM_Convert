#pragma once

#define								_MAX_CHARS								64

class AFX_EXT_CLASS CPString
{
public:
	CPString( const char* szData, int nLength = 0 );
	CPString( char* szData, int nLength = 0 );
	CPString( const wchar_t* szData, int nLength = 0 );
	CPString( wchar_t* szData, int nLength = 0 );
	CPString( const CPString& szData, int nLength = 0 );
	CPString(void);
	~CPString(void);

	void							Release( void );

	operator CPString&()
	{
		return *this;
	}
	CPString&						operator=( const CPString& strSrc );
	CPString&						operator=( const char* strSrc );
	CPString&						operator=( char* strSrc );
	CPString&						operator=( char chSrc );
	CPString&						operator+=( const CPString& strSrc );
	CPString&						operator+=( char* strSrc );
	CPString&						operator+=( char chSrc );
	CPString&						operator+( const CPString& strSrc );
	CPString&						operator+( char* strSrc );
	CPString&						operator+( char chSrc );
	bool							operator==( const CPString& strSrc );
	bool							operator==( char* strSrc );
	bool							operator==( char chSrc );
	bool							operator!=( const CPString& strSrc );
	bool							operator!=( char* strSrc );
	bool							operator!=( char chSrc );

	CPString&						operator=( const int nValue );
	CPString&						operator=( const unsigned int unValue );
	CPString&						operator=( const long lValue );
	CPString&						operator=( const unsigned long ulValue );
	CPString&						operator=( const float fValue );
	CPString&						operator=( const double dValue );
	CPString&						operator=( const unsigned long long ullValue );

	void							Init( int nSize = 0 );
	void							Init_wide( int nSize = 0 );
	void							Empty();
	bool							IsEmpty();

	void							MakeWideValue();
	void							MakeUTF8Value();
	wchar_t*						GetMakeWideValue();

	char*							GetValue() const throw();
	wchar_t*						GetWideValue() const throw();
	char*							GetMakeUTF8Value();
	char*							GetMakeUTF82MultiValue();
	int								GetLength( void );
	int								GetAllocLength( void );

	int								Format( const char* szFormat,... );
	int								Append( char* szAddString );
	CPString						Left( int nCount );
	CPString						Right( int nCount );
	int								Find( char* szFindStr, int nStart = 0 );
	int								ReverseFind( char* szFindStr );
	CPString						Mid( int nFirst, int nCount );
	int								Replace( char* szFind, char* szChange );
	void							Trim_Left( CPString szDrop = " " );
	void							Trim_Right( CPString szDrop = " " );
	void							Trim( CPString szDrop = " " );
	void							MakeUpper();
	void							MakeLower();
	int								Insert( int nIndex, char* szInsertStr );
	int								Erase( int nIndex, int nCount = 1 );

	bool							SetTok_Parse( CPString szDIV = ",", bool bAddCR = true );
	int								GetTok_Count();
	CPString						GetTok_At( int nIndex );
	void							SetTok_Release();

protected:
	void							SetDelete();
	void							SetDeleteWide();
	void							SetValue( const char* szData, int nLength = 0 );
	void							SetWideValue( const wchar_t* szData, int nLength = 0);
	bool							ConvertWide2Multi();
	bool							ConvertMulti2Wide();
	bool							ConvertMulti2UTF8();
	bool							ConvertUTF82Multi();
	int								GetLengthString( const char* szStrValue );
	int								GetLengthString( const wchar_t* szStrValue );

protected:
	char*							m_szValue;
	wchar_t*						m_szWideValue;
	int								m_nDataLength;
	int								m_nAllocLength;

	vector< CPString >				m_vecTokString;
};
