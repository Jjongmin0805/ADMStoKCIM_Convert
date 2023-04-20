#pragma once

// KASIM 스키마 정보 클래스
class CPKASIM_Schema_VarData
{
public:
	CPKASIM_Schema_VarData(void);		// KASIM 스키마 정보 클래스 생성자
	~CPKASIM_Schema_VarData(void);			// KASIM 스키마 정보 클래스 소멸자

	unsigned int					m_unID;		// 스키마 ID
	unsigned int					m_unSize;		// 스키마 크기
	CPString						m_szName;			// 스키마 명칭
};

typedef hash_map< int, CPKASIM_Schema_VarData* >							HMAP_SC_VARDATA;