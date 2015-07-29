#pragma once

#include "memUIBase.h"

class CUIImage
{
public:
	CUIImage( void );
	~CUIImage( void ) ;
public:
	void InitImage(  ItemStatusImage isi ) ;
	bool DrawImage( const HDC* phdc ,  ITEM_STATUS iStatus , RECT rc ) ;
private:
	HRESULT InitImageName( TCHAR * imgName , ITEM_STATUS iStatus ) ;
private:
	//Gdiplus::Image* m_Image[3] ;//最多只需要3个
	CImage*		m_cImage[STATUS_AMOUNT] ;
	int iImageAmount ;
public:
};

