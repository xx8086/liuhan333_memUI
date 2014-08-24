#pragma once

#include "KKUIBase.h"
#include "KKUIImage.h"

class CKKUIButton : public CKKUIBase
{
public:
	CKKUIButton(void);
	~CKKUIButton(void);
public:
	void	InitBtn( HWND hwnd , RECT rc , ItemStatusImage isi )  ;
	 BOOL	OnPaint( const HDC* phdc , RECT rc ) ;
private:
	CKKUIImage m_BTNImage ;
};

