#pragma once

#include <strsafe.h>
#include <xmllite.h>
#pragma comment( lib, "Xmllite.lib" )

class AFX_EXT_CLASS CElement_NmValue
{
public: 
	CElement_NmValue()
	{
		//
	};
	~CElement_NmValue()
	{
		//
	};

	CPString						m_szEleName;
	CPString						m_szEleValue;
};
typedef						vector< CElement_NmValue >											VECXML_ELENMVALUE;
typedef						hash_set< string >													HSET_SELECTPOS;

class AFX_EXT_CLASS CSAXMgr
{
public:
	CSAXMgr( char* szLoadXMLFolderName, char* szLoadXMLFileName );
	~CSAXMgr();

	bool																						IsOpen();

	CPString																					GetFileName();

	bool																						IsSelectEndPosition( string szFindSet );
	void																						SetSelectEndPosition( string szFindSet_EleNm );
	void																						SetSelectEndPosition( CPString szFindSet_EleNm );

	bool																						SelectXMLData_Element( CPString szFilterData );
	UINT																						GetXMLData_Element( CPString &szRetValue );
	UINT																						GetXMLData_NextElement( CPString &szRetValue );
	CPString																					GetXMLData_ElementValue();
	CPString																					GetXMLData_ElementValue( VECXML_ELENMVALUE &vecEleNmValue );

protected:
	string																						MakeSelectEndPositionIndex( string szFindSet_EleNm, UINT unDepthValue );
	string																						MakeSelectEndPositionIndex( CHAR *szFindSet_EleNm, UINT unDepthValue );
	void																						DeleteSelectEndPosition_Search( string szFindSet_EleNm );
	void																						DeleteSelectEndPosition( string szFindSet_EleNm );

protected:
	bool							m_bIsOpen;
	CPString						m_szLoadFilePath;
	CComPtr<IStream>				m_pFileStream;
	CComPtr<IXmlReader>				m_pReader;
	int								m_nSelectEndPos;

	HSET_SELECTPOS					m_hsetSelectEndPos;
};

