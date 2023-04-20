#pragma once

class AFX_EXT_CLASS CFileMapObject
{
public:
	CFileMapObject( UINT unObjType, CPString szObjName, CPString szMapFilePath, CPString szMapFileName, unsigned long ulFileSize = 0 );
	CFileMapObject( UINT unObjType, CPString szObjName, unsigned int unSize = 0 );
	~CFileMapObject(void);

	int																							GetErrorCode();
	CPString																					GetErrorMessage();
	BOOL																						IsOpen();
	BOOL																						SetInitialize();

	CPString																					GetName();
	UINT																						GetDataCount();
	BOOL																						SetDataCount( UINT unCount );
	char*																						GetData();
	char*																						GetData( unsigned int unMvPosition );
	BOOL																						SetData( char* pDataBuff, unsigned int unSize );
	BOOL																						SetData( unsigned int unMvPosition, char* pDataBuff, unsigned int unSize );
	unsigned int																				GetSize();

protected:
	BOOL																						SetMapMemory_MAKE();
	BOOL																						SetMapMemory_CONNECT();
	BOOL																						SetMapFile_MAKE();
	BOOL																						SetMapFile_CONNECT();

	BOOL																						GetMapMemory_CONNECT_Size();
	BOOL																						SetMapMemory_CONNECT_Data();

protected:
	UINT							m_unType;
	CPString						m_szName;
	unsigned long					m_unSize;

	CPString						m_szMapFilePath, m_szMapFileName;
	HANDLE							m_hMapFile;

	UINT							m_unRealDataCount;
	HANDLE							m_hMapFileInfor;
	LPVOID							m_pMapViewFileData;
	int								m_nErrCode;
	CPString						m_szErrMessage;
};

