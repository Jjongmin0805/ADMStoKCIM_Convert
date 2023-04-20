#pragma once

class AFX_EXT_CLASS CObjQueue
{
public:
	CObjQueue( unsigned int unIndex );
	~CObjQueue();

	unsigned int																				GetIndex();
	void																						PushData( void *pData );
	void*																						PopData();

protected:
	void																						ClearData();

protected:
	unsigned int					m_unIndex;
	CPLocker						*m_pLocker;
	list< void* >					m_lstObjData;
};

