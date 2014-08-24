#include "StdAfx.h"
#include "KKUIBase.h"


CKKUIBase::CKKUIBase(void)
{

}

CKKUIBase::~CKKUIBase(void)
{

}


//////////////////////////////////////////////////////////////
void CKKUIBase::Init( HWND hwnd , RECT rc , int iAmount ) 
{
	m_statusAmount = iAmount ;
	m_rc =  rc ;
	m_hwnd = hwnd ;
	m_currentStatus = STATUS_NORMAL ;
}


HRESULT CKKUIBase::InitImageName( TCHAR * imgName , ITEM_STATUS iStatus ) 
{
	return 0 ;
}



BOOL CKKUIBase::InRECT( RECT rc_0 , RECT rc_1 )
{
	if( rc_0.bottom >= rc_1.bottom &&
		rc_0.left <= rc_1.left &&
		rc_0.right >= rc_1.right &&
		rc_0.top <= rc_1.top  )
		return TRUE ;

	return FALSE ;
} 

BOOL CKKUIBase::ContainsRect( RECT rc ) 
{
	POINT pt0 ;
	POINT pt1 ;
	POINT pt2 ;
	POINT pt3 ;

	pt0.x = m_rc.left ;
	pt0.y = m_rc.top  ;
	pt1.x = m_rc.left ;
	pt1.y = m_rc.bottom ;
	pt2.x = m_rc.right ;
	pt2.y = m_rc.top ;
	pt3.x = m_rc.right ;
	pt3.y = m_rc.bottom ;

	if( InPoint( rc , pt0) ||
		InPoint( rc , pt1) ||
		InPoint( rc , pt2) ||
		InPoint( rc , pt3) ) 
		return TRUE ;

	return FALSE ;
}

BOOL CKKUIBase::InPoint( RECT rc_0 , POINT pt )
{
	if( rc_0.bottom >= pt.y &&
		rc_0.left <= pt.x &&
		rc_0.right >= pt.x &&
		rc_0.top <= pt.y  )
		return TRUE ;

	return FALSE ;
}

void	CKKUIBase::OnFlushContrl(   BOOL brf )
{
	::InvalidateRect( m_hwnd ,&m_rc , brf ) ;
	::UpdateWindow( m_hwnd ) ;
}

BOOL   CKKUIBase::OnMouseLeave( POINT pt  ) 
{
	BOOL rt = FALSE ;
	if( m_currentStatus != STATUS_NORMAL && !InPoint( m_rc , pt )   )
	{
		m_currentStatus = STATUS_NORMAL ;
		OnFlushContrl() ;
		rt  = TRUE ;
	}

	return rt ;
}

BOOL    CKKUIBase::OnLButtonDown( POINT pt ) 
{
	BOOL rt = FALSE ;
	if(  m_statusAmount > (int)STATUS_CLICK && InPoint( m_rc , pt )  )
	{
		m_currentStatus = STATUS_CLICK ;
		OnFlushContrl() ;
		rt  = TRUE ;
	}

	return rt ;
};

BOOL    CKKUIBase::OnMouseHover( POINT pt ) 
{
	BOOL rt = FALSE ;
	if( m_statusAmount > (int)STATUS_OVER && InPoint( m_rc , pt )  )
	{
		m_currentStatus = STATUS_OVER ;
		OnFlushContrl() ;
		rt  = TRUE ;
	}
	return rt ;
};

BOOL    CKKUIBase::OnLButtonUp( POINT pt ) 
{
	BOOL rt = FALSE ;
	if( InPoint( m_rc , pt )  )
		rt = TRUE ;

	return FALSE ;
};
BOOL    CKKUIBase::OnMouseMove( POINT pt )
{
	BOOL rt = FALSE ;
	if( InPoint( m_rc , pt )  )
		rt = TRUE ;
	return FALSE ;
};

 BOOL	CKKUIBase::OnPaint( const RECT  rc  ) 
 {
	 BOOL rt = FALSE ;
	 if( ContainsRect( rc ) )
		rt = TRUE ;
	 
	 return rt ;
 }

 DWORD CKKUIBase::GetModulePath(DWORD nBufferLength, LPTSTR lpBuffer)
{	
	DWORD dwReturn = 0;	
	if (nBufferLength <= 0 || lpBuffer == NULL)	
	{	
		return dwReturn;	
	}	
	dwReturn = ::GetModuleFileName(NULL,lpBuffer,nBufferLength);
	if (dwReturn <= nBufferLength)	
	{	
		LPTSTR tszSlash = _tcsrchr(lpBuffer, static_cast<int>(_T('\\')));
		if (tszSlash)	
		{	
			dwReturn -=  _tcslen( tszSlash ) ;
			*tszSlash = _T('\0');	
		}	
	}
	return dwReturn;	
}
