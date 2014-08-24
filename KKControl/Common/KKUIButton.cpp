#include "StdAfx.h"
#include "KKUIButton.h"


CKKUIButton::CKKUIButton(void)
{

}

CKKUIButton::~CKKUIButton(void)
{

}

void	CKKUIButton::InitBtn( HWND hwnd , RECT rc , ItemStatusImage isi )  
{
	Init( hwnd , rc , isi.iItemStatusAmount ) ;

	TCHAR DirectoryPath[MAX_PATH] ;
	memset( DirectoryPath , 0 , MAX_PATH*sizeof(TCHAR) ) ;
	DWORD dr = GetModulePath( MAX_PATH , DirectoryPath ) ;

	ItemStatusImage _isi ;
	_isi.iItemStatusAmount = isi.iItemStatusAmount ;
	for( int i = 0 ;  i < isi.iItemStatusAmount ; i++ ) 
		_stprintf( _isi.path[i], _T("%s%s"), DirectoryPath , isi.path[i]) ;

	m_BTNImage.InitImage( _isi ) ;
}

BOOL	CKKUIButton::OnPaint( const HDC* phdc , RECT rc ) 
{
	BOOL rt = FALSE ;
	rt = CKKUIBase::OnPaint( rc ) ;
	if(rt)
	{
		m_BTNImage.DrawImage( phdc , GetCurrentStatus() , GetRect() ) ;
	}
	return rt ;
}