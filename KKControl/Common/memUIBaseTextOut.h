#pragma once
#include "BaseConfig.h"

class CMMUIBaseTextOut
{
public:
	CMMUIBaseTextOut();
	~CMMUIBaseTextOut();
public:
	bool	DrawText(const HDC* phdc, RECT rc, TCHAR* pText );
	bool	SetFontBK(COLORREF bfontBKColo, bool bBK);
	bool	SetBKBlush(COLORREF bBKColo, bool bBK);
	bool	SetFontColo(COLORREF fontColo);
private:
	HDC				m_memDC;
	HBITMAP			m_memBitmap;
	HBITMAP			m_hOldbitmap;
	COLORREF		m_fontColo;
	
	COLORREF		m_fontBKColo;
	bool			m_bfontBK;
	
	COLORREF		m_BKBlushColo;
	bool			m_bBKBlush;
};