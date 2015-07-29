#include "StdAfx.h"
#include "memUIBaseTextOut.h"

/*
HDC				m_memDC;
HBITMAP			m_memBitmap;
HBITMAP			m_hOldbitmap;
COLORREF		m_fontColo;

COLORREF		m_fontBKColo;
bool			m_bfontBK;

COLORREF		m_BKBlushColo;
bool			m_bBKBlush;
*/
CMMUIBaseTextOut::CMMUIBaseTextOut() :
m_memDC(NULL), 
m_memBitmap(NULL), 
m_hOldbitmap(NULL),
m_fontColo(WHITE_BLACK), 
m_fontBKColo(WHITE_COLORE), 
m_BKBlushColo(WHITE_COLORE),
m_bfontBK(false),
m_bBKBlush(false)
{}

CMMUIBaseTextOut::~CMMUIBaseTextOut()
{
	if (m_memDC != NULL)
	{
		SelectObject(m_memDC, m_hOldbitmap);
		DeleteObject(m_memBitmap);
		DeleteDC(m_memDC);
		m_memDC = NULL;
		m_hOldbitmap = NULL;
		m_memBitmap = NULL;
	}
}


bool	CMMUIBaseTextOut::SetFontBK(COLORREF bfontBKColo, bool bBK)
{
	m_fontBKColo = bfontBKColo;
	m_bfontBK = bBK;
	return true;
}
bool	CMMUIBaseTextOut::SetBKBlush(COLORREF bBKColo, bool bBK)
{
	m_BKBlushColo = bBKColo;
	m_bBKBlush = bBK;
	return true;
}
bool	CMMUIBaseTextOut::SetFontColo(COLORREF fontColo)
{
	m_fontColo = fontColo;
	return true;
}

bool	CMMUIBaseTextOut::GetTextOutmemDC(const HDC* phdc, HDC*& pmemhdc, RECT& rc)
{
	RECT _rc;
	_rc.left = _rc.top = 0;
	_rc.right = rc.right - rc.left;
	_rc.bottom = rc.bottom - rc.top;

	if (m_memDC == NULL)
		m_memDC = CreateCompatibleDC(*phdc);
	if (m_memBitmap == NULL)
		m_memBitmap = CreateCompatibleBitmap(*phdc, _rc.right, _rc.bottom);
	if (m_hOldbitmap == NULL)
		m_hOldbitmap = (HBITMAP)SelectObject(m_memDC, m_memBitmap);
	
	pmemhdc = &m_memDC;
	rc = _rc;

	return true;
}

bool	CMMUIBaseTextOut::DrawTextOut(const HDC* phdc, RECT rc, TCHAR* pText)
{
	RECT _rc = rc;
	HDC* _phdc = NULL;
	GetTextOutmemDC(phdc, _phdc, _rc);

	HBRUSH hBrush;
	if (m_bBKBlush)
	{
		hBrush = CreateSolidBrush(m_BKBlushColo);
		::FillRect(m_memDC, &_rc, hBrush);
	}

	COLORREF oldBKColo;
	if (m_bfontBK)
		oldBKColo = SetBkColor(m_memDC, m_fontBKColo);

	//COLORREF oldTextColo = SetTextColor(m_memDC, m_fontColo);
	int ildBkMode = SetBkMode(m_memDC, TRANSPARENT);
	//::TextOut(m_memDC, 0, 0, pText, _tcslen(pText));
	int ilen = DrawText(m_memDC, pText, -1, &_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	SetBkMode(m_memDC, ildBkMode);
	//SetTextColor(m_memDC, oldTextColo);
	
	if (m_bfontBK)
		SetBkColor(m_memDC, oldBKColo);
	if (m_bBKBlush)
	{
		DeleteObject(hBrush);
	}

	BitBlt(*phdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		m_memDC, 0, 0, SRCCOPY);

	return true;
}