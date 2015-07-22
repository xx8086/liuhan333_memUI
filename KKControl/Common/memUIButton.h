#pragma once

#include "memUIBase.h"
#include "memUIImage.h"

class CMMUIButton : public CMMUIBase
{
public:
	CMMUIButton(void);
	~CMMUIButton(void);
public:
	void	InitBtn( HWND hwnd , RECT rc , ItemStatusImage isi )  ;
	bool	OnPaint( const HDC* phdc , RECT rc ) ;
private:
	CUIImage m_BTNImage ;
};

