#include "StdAfx.h"
#include "memUIImage.h"
////#include <comdef.h>
////#include <Windows.h>
//#include <gdiplus.h>       
//using namespace Gdiplus;      
//#pragma comment(lib,"gdiplus.lib")

CUIImage::CUIImage(void):iImageAmount(0)
{
	for( int i = 0 ; i < 3 ; i++ )
		m_cImage[i] = NULL ;
}


CUIImage::~CUIImage(void)
{
	for( int  i = 0 ; i < 3 ; i++ )
	{
		if( NULL != m_cImage[i] )
		{
			delete m_cImage[i] ;
			m_cImage[i] = NULL ;
		}
	}
}

void CUIImage::InitImage(  ItemStatusImage isi ) 
{
	iImageAmount = isi.iItemStatusAmount ;
	for( int i = 0 ; i < iImageAmount ; i++  )
	{
		if( m_cImage[i] )
		{
			delete m_cImage[i] ;
			m_cImage[i] = NULL ;
		}
		m_cImage[i] = new CImage ;
		InitImageName( isi.path[i] , (ITEM_STATUS)i ) ;
		//m_Image[i] = Image::FromFile(isi.path[i]) ;
		//int err = GetLastError() ;
	}	
}

bool CUIImage::DrawImage(  const HDC* phdc ,  ITEM_STATUS iStatus , RECT rc  )
{
	//Graphics graphics( *phdc ) ;
	//Status ss ;
	//ss = graphics.DrawImage( m_Image[is] , rc.left ,  rc.top ) ;
	//graphics.ReleaseHDC( *phdc ) ;

	if( m_cImage[ iStatus ] == NULL || m_cImage[ iStatus ]->IsNull() )
		return false;
	if (!m_cImage[iStatus]->Draw(*phdc, rc))
		return false;

	return true ;
}

HRESULT CUIImage::InitImageName( TCHAR * imgName , ITEM_STATUS iStatus ) 
{
	HRESULT hr =  m_cImage[ iStatus ]->Load(imgName) ;
	if ( SUCCEEDED( hr ) && m_cImage[ iStatus ]->GetBPP() == 32 ) //确认该图像包含Alpha通道
	{
		int i ;
		int j ;
		for (i = 0 ; i < m_cImage[ iStatus ]->GetWidth(); i++)
		{
			for (j = 0; j < m_cImage[ iStatus ]->GetHeight(); j++)
			{
				byte *pByte = (byte *)m_cImage[ iStatus ]->GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}
	return hr ;
}