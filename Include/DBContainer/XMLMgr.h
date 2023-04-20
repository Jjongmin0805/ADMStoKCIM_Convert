// XMLMgr.h: interface for the CXMLMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLMGR_H__6DD2D0F7_D24E_4099_A3BE_A8AEB254A2BE__INCLUDED_)
#define AFX_XMLMGR_H__6DD2D0F7_D24E_4099_A3BE_A8AEB254A2BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "./Dll/msxml6.dll" raw_interfaces_only
#include <msxml.h>
using namespace MSXML2;

class AFX_EXT_CLASS CXMLMgr
{
public:
	CXMLMgr();
	virtual ~CXMLMgr();

public:
	HRESULT 
		InsertChild(MSXML2::IXMLDOMNode* /*pNewNode*/ , MSXML2::IXMLDOMNode* /*pRefNode*/ , MSXML2::IXMLDOMNode* /*pParentNode*/ = NULL );

	MSXML2::IXMLDOMNode* 
		GetChild(const long /*lIndex*/ , MSXML2::IXMLDOMNodeList* /*pList*/) const;

	LPCWSTR 
		GetNodeName(MSXML2::IXMLDOMNode* /*pNode*/) const;

	void 
		GetNodeName(MSXML2::IXMLDOMNode* pNode, wchar_t* pValue) const;

	void
		GetNodeName(MSXML2::IXMLDOMNode* pNode, CString& szValue) const;

	LPCWSTR 
		GetText(MSXML2::IXMLDOMNode* /*pElement*/) const;

	MSXML2::IXMLDOMNode* 
		GetParent(MSXML2::IXMLDOMElement* /*pElement*/) const;

	HRESULT 
		RemoveElement(MSXML2::IXMLDOMElement* /*pElementToRemove*/ , MSXML2::IXMLDOMNode* /*pParent*/);

	HRESULT 
		RemoveElement(MSXML2::IXMLDOMElement* /*pElementToRemove*/);

	HRESULT 
		ReplaceElement(MSXML2::IXMLDOMElement* /*pNewElement*/ , MSXML2::IXMLDOMElement* /*pOldElement*/ , MSXML2::IXMLDOMNode* /*pParent*/);

	HRESULT 
		CreateElement(LPCSTR /*LPCWSTRElementName*/, LPCSTR /*LPCWSTRElementText*/ , MSXML2::IXMLDOMNode* /*pParentNode*/ , bool /*bAdd*/ = true , int /*nLevel*/ = 1);
	
	HRESULT 
		CreateComments(LPCSTR /*LPCWSTRComment*/);

	HRESULT 
		CreateXMLFileHeader();

	HRESULT 
		CreateRoot(LPCSTR /*LPCWSTRRootName*/ , LPCSTR /*LPCWSTRRootText*/ = NULL);

	MSXML2::IXMLDOMElement* 
		GetRoot() const { return m_pXMLRoot; }

	MSXML2::IXMLDOMNode * 
		GetFirstChild() const;
	
	MSXML2::IXMLDOMNode * 
		GetFirstChild(MSXML2::IXMLDOMNode* /*pNode*/) const;

	MSXML2::IXMLDOMNode * 
		GetLastChild(MSXML2::IXMLDOMNode* /*pNode*/) const;

	MSXML2::IXMLDOMNode * 
		GetLastChild() const;

	MSXML2::IXMLDOMNode * 
		GetNextSibling(MSXML2::IXMLDOMNode* /*pNode*/) const;

	MSXML2::IXMLDOMNodeList	* 
		GetChildList(MSXML2::IXMLDOMNode* /*pNode*/) const;

	MSXML2::IXMLDOMNode	* 
		GetItemNode(const long /*lIndex */, MSXML2::IXMLDOMNodeList* /*pList*/) const;	

	const long 
		GetNoOfChilds(MSXML2::IXMLDOMNodeList* /*pList*/) const;
	
	LPCWSTR 
		GetItemText(MSXML2::IXMLDOMNode* /*pNode*/) const;

	void 
		GetItemText(MSXML2::IXMLDOMNode* pNode, CString& szValue) const;

	bool 
		Load(LPCSTR /*LPCWSTRXMLFileName*/);
	bool	
		Load(LPCWSTR /*LPCWSTRXMLFileName*/);

	HRESULT 
		Save(LPCWSTR /*LPCWSTRXMLFileName*/);
	
	HRESULT 
		Initlize() const;

	void	
		Release();

	MSXML2::IXMLDOMElement*	
		GetLastItemAdded() const { return m_pLastItemAdded; }

	static int XMLMemoryHandler(size_t /* size */);

	CString		GetItemName( MSXML2::IXMLDOMNode* pNode, OLECHAR* LPCWSTRComment ) const;

	int			GetItemValue( MSXML2::IXMLDOMNode* pNode, OLECHAR* LPCWSTRComment ) const;

	MSXML2::IXMLDOMNode* GetSelect(LPCWSTR /*LPCWSTRXMLFileName*/) const;

private:
	void 
		Release(MSXML2::IXMLDOMDocument*	/*pXMLDoc*/);	
	void 
		Release(MSXML2::IXMLDOMElement*		/*m_pXMLRoot*/);

	HRESULT 
		AppendChildToParent(MSXML2::IXMLDOMNode* /*pChild*/, MSXML2::IXMLDOMNode* /*pParent*/);

	void 
		AddWhiteSpaceToNode(MSXML2::IXMLDOMDocument* /*pDOMDoc*/, BSTR /*bstrWs*/, MSXML2::IXMLDOMNode* /*pNode*/);

	void 
		Destroy();

	void 
		SetIndentLevel(int /*nLevel*/ , MSXML2::IXMLDOMNode* /*pParentNode*/);
	
	HRESULT 
		SetNodeText(LPCSTR /*LPCWSTRText*/ , MSXML2::IXMLDOMElement* /*pNode*/);
	
	MSXML2::IXMLDOMElement* 
		CreateNode(LPCSTR /*LPCWSTRRootName*/ , MSXML2::IXMLDOMElement* /*pNode*/);

	LPCWSTR 
		ReturnBuffer(BSTR /*bstr*/) const;


protected:
	wchar_t						m_pBuffer[ 1024 ];

private:
	MSXML2::IXMLDOMDocument*	m_pXMLDomDoc;		// NULL;
	MSXML2::IXMLDOMElement*		m_pXMLRoot;			// NULL;
	MSXML2::IXMLDOMElement*		m_pLastItemAdded;	// NULL;

	//_TCHAR*	m_pBuffer;

	static CXMLMgr*		m_pThis;
};

#endif // !defined(AFX_XMLMGR_H__6DD2D0F7_D24E_4099_A3BE_A8AEB254A2BE__INCLUDED_)

