#pragma once

#define								KASIMINPUTFILE_DIRECTORY				"CSVFile"
#define								KASIMSCHEMA_SMNAME_STA					"pkasimchemaname_sta"
#define								KASIMSCHEMA_SMNAME_DYN					"pkasimchemaname_dyn"
//#define								KASIMSCHEMA_SMNAME_DYN					"pkasimchemaname_dyn"
#define								KASIMHYBRID_HFILENAME					"pkasim_hbFile_"				// pkasim_hbFile_4830_tablename_fieldname
#define								KASIMHYBRID_HFILEINDEXNAME				"pkasim_hbFileIndex_"			// pkasim_hbFile_4830_tablename_fieldname

#define								KASIMSCHEMA_TABLENAMEMAXSIZE			32
#define								KASIMSCHEMA_FIELDNAMEMAXSIZE			64

enum KASIMRDBINDEX
{
	KASIMRDBINDEX_STA = 0, 
	KASIMRDBINDEX_DYN, 
	KASIMRDBINDEX_DYN_EVENT, 
	KASIMRDBINDEX_DYN_EXCEPTION
};

enum KASIMRDBTYPE
{
	KASIMRDBTYPE_NONE = 0, 
	KASIMRDBTYPE_STA, 
	KASIMRDBTYPE_DYN, 
	KASIMRDBTYPE_STUDY
};

struct KASIMVER_INFOR
{
	CPString						szVersion;
 	CPString						szMakeDate;
 	CPString						szLastUpdate;
 	CPString						szOrderMakeBy;
};

struct KASIM_FIELDVAR_INFOR
{
	char							*pBuffData;
	CPString						szName;
	int								nVarID;
	int								nVarSize;
	int								nVarCount;
	int								nVarRealSize;
};

//typedef hash_map< CPString, int >											HMAP_STR4INT;
typedef vector< KASIM_FIELDVAR_INFOR >										VECFIELDVARINFOR;

//LOGOUT( "* Err : Find not KASIM Zero Index( %s: %d )", __FILE__, __LINE__ ); 

#define								GETSTRING( tablename, fieldname, index )									theAppDataMng->GetText( 1, tablename, fieldname, index )
#define								GETVALUE( variant_type, tablename, fieldname, index )						*(##variant_type*)theAppDataMng->GetData( 1, tablename, fieldname, index )
#define								GETVALUE_AT( variant_type, tablename, fieldname, index, element )			*(##variant_type*)theAppDataMng->GetData( 1, tablename, fieldname, index, element )
#define								GETVALUE2STR( tablename, fieldname, index )									theAppDataMng->GetData2String( 1, tablename, fieldname, index )
#define								GETVALUE2STR_AT( tablename, fieldname, index, element )						theAppDataMng->GetData2String( 1, tablename, fieldname, index, element )
#define								GETVALUE2DOUBLE( tablename, fieldname, index )								theAppDataMng->GetData2Double( 1, tablename, fieldname, index )
#define								GETVALUE2DOUBLE_AT( tablename, fieldname, index, element )					theAppDataMng->GetData2Double( 1, tablename, fieldname, index, element )
#define								PUTVALUE( tablename, fieldname, index, setvalue )							theAppDataMng->PutData( 1, tablename, fieldname, index, setvalue)
#define								PUTDOUBLE2VALUE( tablename, fieldname, index, setvalue )					theAppDataMng->PutDouble2Data( 1, tablename, fieldname, index, setvalue)
#define								PUTSTRING( tablename, fieldname, index, setvalue )							theAppDataMng->PutString2Data( 1, tablename, fieldname, index, setvalue)
#define								PUTVALUE_AT( tablename, fieldname, index, element, setvalue )				theAppDataMng->PutData( 1, tablename, fieldname, index, element, setvalue )
#define								PUTDOUBLE2VALUE_AT( tablename, fieldname, index, element, setvalue )		theAppDataMng->PutDouble2Data( 1, tablename, fieldname, index, element, setvalue)
#define								PUTSTRING_AT( tablename, fieldname, index, element, setvalue )				theAppDataMng->PutString2Data( 1, tablename, fieldname, index, element, setvalue)

#define								EVENT_GETSTRING( tablename, fieldname, index )								theAppDataMng->GetText( 2, tablename, fieldname, index )
#define								EVENT_GETVALUE( variant_type, tablename, fieldname, index )					*(##variant_type*)theAppDataMng->GetData( 2, tablename, fieldname, index )
#define								EVENT_GETVALUE_AT( variant_type, tablename, fieldname, index, element )		*(##variant_type*)theAppDataMng->GetData( 2, tablename, fieldname, index, element )
#define								EVENT_GETVALUE2STR( tablename, fieldname, index )							theAppDataMng->GetData2String( 2, tablename, fieldname, index )
#define								EVENT_GETVALUE2STR_AT( tablename, fieldname, index, element )				theAppDataMng->GetData2String( 2, tablename, fieldname, index, element )
#define								EVENT_GETVALUE2DOUBLE( tablename, fieldname, index )						theAppDataMng->GetData2Double( 2, tablename, fieldname, index )
#define								EVENT_GETVALUE2DOUBLE_AT( tablename, fieldname, index, element )			theAppDataMng->GetData2Double( 2, tablename, fieldname, index, element )
#define								EVENT_PUTVALUE( tablename, fieldname, index, setvalue )						theAppDataMng->PutData( 2, tablename, fieldname, index, setvalue)
#define								EVENT_PUTDOUBLE2VALUE( tablename, fieldname, index, setvalue )				theAppDataMng->PutDouble2Data( 2, tablename, fieldname, index, setvalue)
#define								EVENT_PUTSTRING( tablename, fieldname, index, setvalue )					theAppDataMng->PutString2Data( 2, tablename, fieldname, index, setvalue)
#define								EVENT_PUTVALUE_AT( tablename, fieldname, index, element, setvalue )			theAppDataMng->PutData( 2, tablename, fieldname, index, element, setvalue )
#define								EVENT_PUTDOUBLE2VALUE_AT( tablename, fieldname, index, element, setvalue )	theAppDataMng->PutDouble2Data( 2, tablename, fieldname, index, element, setvalue)
#define								EVENT_PUTSTRING_AT( tablename, fieldname, index, element, setvalue )		theAppDataMng->PutString2Data( 2, tablename, fieldname, index, element, setvalue)
