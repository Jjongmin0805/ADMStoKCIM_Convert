#pragma once

//0         1         2         3         4         5         6         7         8         9        10        11        12
//123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123e4r5678

class AFX_EXT_CLASS CPDSETCommData
{
public:
	struct FRAME_HEADER
	{
		FRAME_HEADER()
		{
			chCheckBegin[ 0 ]												= 0x07;
			chCheckBegin[ 1 ]												= 0x01;
			unBodySize														= 0;
			snIndexCode														= 0;
			snSeqNo															= 0;
			chCheckEnd[ 0 ]													= 0x07;
			chCheckEnd[ 1 ]													= 0x02;
		};

		char						chCheckBegin[ 2 ];
		unsigned int				unBodySize;
		short						snIndexCode;
		short						snSeqNo;
		char						chCheckEnd[ 2 ];
	};


	CPDSETCommData( void );
	CPDSETCommData( const char* szId );
	CPDSETCommData( const char* szId, short snIndexCode, short snSeqCode );
	CPDSETCommData( const char* szId, short snIndexCode, short snSeqCode, unsigned int unBuffSize, const char* pBuff );
	CPDSETCommData( CPDSETCommData *pPDSETCommData );
	~CPDSETCommData(void);

	void																	ResetHeader();
	void																	ResetData();
	void																	RemoveBuffData();

	void																	SetId( const char* szId );
	int																		GetHeaderSize();
	int																		GetBodySize();
	void																	MakeCommBuff( char* &pHeaderBuff, char* &pBodyBuff );

	void																	SetData( short snIndexCode, short snSeqNo, unsigned int unBuffSize, const char* pBuff );
	void																	SetData_Index( short snIndexCode, short snSeqNo );
	void																	SetData_BuffData( unsigned int unBuffSize, const char* pBuff );

	void																	ResetTokValue();
	int																		GetTokenStringSize();
	void																	SetBuffParse_TokenString();
	string																	GetDIvFirstData( char chDiv = ',' );

	FRAME_HEADER					m_stHeader;

	char							m_szClientId[ 64 ];
	unsigned int					m_unBuffSize;
	char*							m_pBuff;
	vector< string >				m_vecTokValue;
};

