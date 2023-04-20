#pragma once
#include "ObjQueue.h"
typedef								vector< CObjQueue* >										VECOBJQUE;

class AFX_EXT_CLASS CObjMng
{
public:
	static CObjMng*																				Instance( void );
	static void																					Close( void );

protected:
	static CObjMng*					m_pMngData;

////////////////////////////////////////////////////////////////////////////////////////////////

public:
	CObjMng();
	~CObjMng();

	void Insert_ObjQueue( unsigned int unQueueID );

	void PUSH_ObjQueue( unsigned int unQueueID, void *pData );
	void* POP_ObjQueue( unsigned int unQueueID );

protected:
	void				Clear_ObjQueue();
	CObjQueue* GetObjQueue( unsigned int unQueueID );

protected:
	VECOBJQUE						m_vecOBJQueue;
};

#define								theOBJ_Manager												CObjMng::Instance()