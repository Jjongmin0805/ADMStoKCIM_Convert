#ifndef _MEMDC_H_
#define _MEMDC_H_

class CCtrl_SkinMemDC : public CDC 
{
private:
	CBitmap*										m_bitmap;
	CBitmap*										m_oldBitmap;
	CDC*											m_pDC;
	CRect											m_rcBounds;
public:
	CCtrl_SkinMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bitmap																			= new CBitmap;
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap																			= SelectObject(m_bitmap);
		m_pDC																				= pDC;
		m_rcBounds																			= rcBounds;
	}
	~CCtrl_SkinMemDC() 
	{
		m_pDC->BitBlt( m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), this, m_rcBounds.left, m_rcBounds.top, SRCCOPY );
		if (m_bitmap != NULL)
		{
			m_bitmap->DeleteObject();
			delete m_bitmap;
		}
//		SelectObject( m_oldBitmap );
		DeleteDC();
	}
	CCtrl_SkinMemDC* operator->() 
	{
		return this;
	}
};

#endif