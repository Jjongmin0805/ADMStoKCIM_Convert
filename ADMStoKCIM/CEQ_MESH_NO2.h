#pragma once
#include <afx.h>
class CEQ_MESH_NO2 :
	public CObject
{
public:
	CEQ_MESH_NO2(void);
	~CEQ_MESH_NO2(void);

public:
	float m_fx;
	float m_fy;

};

typedef CTypedPtrArray<CObArray, CEQ_MESH_NO2*> CEQ_MESH_NO2Array;




