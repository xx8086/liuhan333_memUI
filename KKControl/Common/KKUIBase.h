#pragma once
#include "BaseConfig.h"

class CKKUIBase
{
public:
	CKKUIBase(void);
	~CKKUIBase(void);

public:
	BOOL InPoint( RECT rc_0 , POINT pt ) ;
	BOOL InRECT( RECT rc_0 , RECT rc_1 ) ;
	BOOL ContainsRect( RECT rc ) ;
	HRESULT InitImageName( TCHAR * imgName , ITEM_STATUS ) ;
	void	OnFlushContrl(   BOOL brf  = FALSE ) ;

public:
	DWORD GetModulePath(DWORD nBufferLength, LPTSTR lpBuffer) ;
	void Init( HWND hwnd , RECT rc , int iAmount  ) ;
public:
	RECT GetRect(){return m_rc ;} 
	ITEM_STATUS GetCurrentStatus(){ return m_currentStatus ; } ;

public:
	virtual BOOL    OnMouseMove( POINT pt ) ;
	virtual BOOL    OnLButtonDown( POINT pt ) ;
	virtual BOOL    OnLButtonUp( POINT pt ) ;
	virtual BOOL    OnMouseLeave( POINT pt ) ;
	virtual BOOL    OnMouseHover( POINT pt ) ;

	virtual BOOL	OnPaint( const RECT  rc  ) ;
public:
	RECT		m_rc ;
	HWND		m_hwnd ;
	int			m_statusAmount ;
	ITEM_STATUS	m_currentStatus ;

};

