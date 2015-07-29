#include "StdAfx.h"
#include "memUIBase.h"


CMMUIBase::CMMUIBase(void)
{

}

CMMUIBase::~CMMUIBase(void)
{

}


//////////////////////////////////////////////////////////////
void CMMUIBase::Init( HWND hwnd , RECT rc , int iAmount ) 
{
	m_statusAmount = iAmount ;
	m_rc =  rc ;
	m_hwnd = hwnd ;
	m_currentStatus = STATUS_NORMAL ;
}


HRESULT CMMUIBase::InitImageName( TCHAR * imgName , ITEM_STATUS iStatus ) 
{
	return 0 ;
}



bool CMMUIBase::InRECT(RECT rc_0, RECT rc_1)
{
	if( rc_0.bottom >= rc_1.bottom &&
		rc_0.left <= rc_1.left &&
		rc_0.right >= rc_1.right &&
		rc_0.top <= rc_1.top  )
		return TRUE ;

	return false ;
} 

bool CMMUIBase::ContainsRect(RECT rc)
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
		return true ;

	return false ;
}

bool CMMUIBase::InPoint(RECT rc_0, POINT pt)
{
	if( rc_0.bottom >= pt.y &&
		rc_0.left <= pt.x &&
		rc_0.right >= pt.x &&
		rc_0.top <= pt.y  )
		return true ;

	return false ;
}

void	CMMUIBase::OnFlushContrl(bool brf)
{
	::InvalidateRect( m_hwnd ,&m_rc , brf ) ;
	::UpdateWindow( m_hwnd ) ;
}

bool	CMMUIBase::SetStatus(ITEM_STATUS is)
{
	if (is != STATUS_NORMAL &&
		is != STATUS_OVER &&
		is != STATUS_CLICK)
		return false;

	m_currentStatus = is;
	OnFlushContrl();
	return true;
}
bool   CMMUIBase::OnMouseLeave(POINT pt)
{
	bool rt = false ;
	if( m_currentStatus != STATUS_NORMAL && !InPoint( m_rc , pt )   )
	{
		m_currentStatus = STATUS_NORMAL ;
		OnFlushContrl() ;
		rt  = true ;
	}

	return rt ;
}

bool    CMMUIBase::OnLButtonDown(POINT pt)
{
	bool rt = false ;
	if(  m_statusAmount > (int)STATUS_CLICK && InPoint( m_rc , pt )  )
	{
		m_currentStatus = STATUS_CLICK ;
		OnFlushContrl() ;
		rt  = true ;
	}

	return rt ;
};

bool    CMMUIBase::OnMouseHover(POINT pt)
{
	bool rt = false ;
	if( m_statusAmount > (int)STATUS_OVER && InPoint( m_rc , pt ) )
	{
		m_currentStatus = STATUS_OVER ;
		OnFlushContrl() ;
		rt  = true ;
	}
	return rt ;
};

bool    CMMUIBase::OnLButtonUp(POINT pt)
{
	//bool rt = false ;
	//if( InPoint( m_rc , pt )  )
	//	rt = true ;

	//return false ;
	return OnMouseHover(pt);
};
bool    CMMUIBase::OnMouseMove(POINT pt)
{
	bool rt = false;
	if( InPoint( m_rc , pt )  )
		rt = true ;
	return rt ;
};

bool	CMMUIBase::OnPaint(const RECT  rc)
 {
	 bool rt = false ;
	 if( ContainsRect( rc ) )
		rt = true ;
	 
	 return rt ;
 }

 DWORD CMMUIBase::GetModulePath(DWORD nBufferLength, LPTSTR lpBuffer)
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

 bool CMMUIBase::SetCurrentStatus(ITEM_STATUS is)
 { 
	 if (STATUS_NORMAL != is &&
		 STATUS_OVER != is &&
		 STATUS_CLICK != is)
		 return false;

	 m_currentStatus = is; 
	 return true;
 }
