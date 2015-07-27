#include "StdAfx.h"
#include "memUIEdit.h"


static HCURSOR	s_hCursor = NULL   ;

CMMUIEdit::CMMUIEdit(void):m_iTextOutX(0),m_iTextOutY(0),m_iMaxWide(0),
	m_CaretPosition(0),m_iLButtonDown(0),m_iMoveMin(0),m_iMoveMax(0),
	m_bLBttonDown(FALSE),m_bMove(FALSE),m_bActive(TRUE),m_bCtrl(FALSE),
	m_hGlobal(NULL),m_pGlobal(NULL),m_memDC(NULL),m_memBitmap(NULL),
	m_hOldbitmap(NULL),m_isPassword(EDIT_NORMAL)
{
	m_TextOutPoint.x = 0 ;
	m_TextOutPoint.y = 0 ;

	memset( m_String , 0 , (TEXTBOX_MAXLENGTH+1)*sizeof(TCHAR));
	//memset( m_Text , _T('*') , (TEXTBOX_MAXLENGTH+1)*sizeof(TCHAR));
	memset( m_BkString , 0 , (TEXTBOX_MAXLENGTH+1)*sizeof(TCHAR));

	int i = 0 ;
	while( i< TEXTBOX_MAXLENGTH )
	{
		m_Text[i++] = _T('*') ;
	}
	m_Text[TEXTBOX_MAXLENGTH] =  _T('\0');
}

CMMUIEdit::~CMMUIEdit(void)
{
	if( m_memDC != NULL )
	{
		SelectObject( m_memDC , m_hOldbitmap );
		DeleteObject( m_memBitmap ) ;
		DeleteDC( m_memDC ) ;
		m_memDC = NULL ;
		m_hOldbitmap = NULL ; 
		m_memBitmap = NULL ;
	}

	HideCaret() ;
}

void CMMUIEdit::InitContrlEdit( HWND hwnd , RECT rc  , int textoutX , int textoutY , int cursorX , int cursorY , int maxwide , TCHAR* text ,EDIT_TYPE et  )
{
	m_hwnd = hwnd ;
	m_rc = rc ;
	m_isPassword = et ;

	m_iTextOutX = textoutX ;
	m_iTextOutY = textoutY ;
	m_TextOutPoint.x = cursorX ;
	m_TextOutPoint.y = cursorY ;
	m_iMaxWide = maxwide ;

	if( text != NULL )
		memcpy(m_String ,text, _tcslen(text) * sizeof(TCHAR));
}

void CMMUIEdit::SetCaretNewPos( /*const  HDC* phdc*/ )
{
	::HideCaret( m_hwnd ) ;
	if( m_memDC == NULL )
		return ;

	SIZE size ;
	m_CaretPosition = m_CaretPosition < GetStringLen() ? m_CaretPosition : GetStringLen() ;

	if( EDIT_PASSWORD == m_isPassword )
	{
		::GetTextExtentPoint( m_memDC , m_Text, m_CaretPosition, &size);
	}
	else if( EDIT_NORMAL == m_isPassword )
	{
		::GetTextExtentPoint( m_memDC , m_String, m_CaretPosition, &size);
	}
	
	::SetCaretPos( size.cx + m_TextOutPoint.x  , m_TextOutPoint.y ) ;
	::ShowCaret( m_hwnd ) ;
}


void CMMUIEdit::CopyOrShearString( bool bShear ) 
{
	if( abs( m_iMoveMax - m_iMoveMin ) >= 1  &&  EDIT_PASSWORD != m_isPassword )
	{
		m_hGlobal = GlobalAlloc ( GHND | GMEM_SHARE , 
			( m_iMoveMax - m_iMoveMin  + 1 ) * sizeof( TCHAR ) ) ;
		m_pGlobal = (TCHAR*)GlobalLock (m_hGlobal) ;
		_tcsnccpy( m_pGlobal , m_String + m_iMoveMin , m_iMoveMax - m_iMoveMin ) ;
		GlobalUnlock( m_pGlobal ) ;

		if( bShear )
		{
			int ilen =  GetStringLen() - m_iMoveMax + m_iMoveMin ;
			memcpy(m_String + m_iMoveMin , m_String + m_iMoveMax , ( GetStringLen() -  m_iMoveMax ) * sizeof(TCHAR)) ;
			m_String[ilen] = _T('\0') ;
			OnFlushContrl() ;
		}
		OpenClipboard ( m_hwnd ) ;
		EmptyClipboard () ;
		SetClipboardData (CF_TCHAR, m_pGlobal ) ;
		CloseClipboard () ;
		SetFocus();
	}
}

int	 CMMUIEdit::GetStrPos( const HDC* phdc , int cx  )
{
	SIZE size ;
	size.cx = 0 ;
	size.cy = 0 ;  
	int strPos = 0;	
	for (  strPos=0 ; strPos < GetStringLen() ; strPos++ )
	{
		if( EDIT_PASSWORD == m_isPassword )
		{
			::GetTextExtentPoint( *phdc , m_Text, strPos, &size);
		}
		else if( EDIT_NORMAL == m_isPassword )
		{
			::GetTextExtentPoint( *phdc , m_String, strPos, &size) ;
		}
		if( size.cx + 4 >= cx )
			break;
	}

	return strPos ;
}

int  CMMUIEdit::GetStrLen(  TCHAR* str , int ilen  )
{
	CSize cs ;
	int x  = ilen ;
	GetTextExtentPoint32( m_memDC , str , x , &cs );
	while ( cs.cx > m_iMaxWide )
	{
		x-- ;
		m_String[x] = _T('\0');
		GetTextExtentPoint32( m_memDC , str , x , &cs );
	}

	return x ;
}

void CMMUIEdit::SetTextString(TCHAR* str )
{
	if( str == NULL )
		return ;
	memset(m_String , 0, sizeof(m_String )* sizeof(TCHAR));
	memcpy(m_String ,str, _tcslen(str) * sizeof(TCHAR));
}

CSize CMMUIEdit::GetStrWide( TCHAR* str , int ilen )
{
	CSize cs ;
	GetTextExtentPoint32( m_memDC , str  , ilen , &cs );
	return cs ;
}

void CMMUIEdit::CopyText( )
{
	CopyOrShearString() ;
}

void CMMUIEdit::PastText( ) 
{
	OpenClipboard ( m_hwnd ) ;

	if ( m_hGlobal = GetClipboardData (CF_TCHAR))
	{
		m_pGlobal = (TCHAR*)GlobalLock ( m_hGlobal ) ;
		int ilen = GlobalSize ( m_hGlobal) ;
		int y = GetStringLen() -  m_CaretPosition  ;
		TCHAR* pText = new TCHAR[ ilen + y  ] ;
		memset( pText , 0 , (ilen + y) * sizeof(TCHAR) );
		TCHAR* pText2 = new TCHAR[ ilen + y +  m_CaretPosition  ] ;
		memset( pText2 , 0 , (ilen + y + m_CaretPosition) * sizeof(TCHAR) );
		lstrcpy (pText, m_pGlobal) ;
		int iPastlen = _tcslen( pText );
		ilen = y + iPastlen ;

		::memcpy( pText + iPastlen , m_String + m_CaretPosition , (GetStringLen() -  m_CaretPosition ) * sizeof(TCHAR) );
		::memcpy( pText2 , m_String , m_CaretPosition * sizeof(TCHAR) );
		::memcpy(pText2 + m_CaretPosition , pText , _tcslen( pText ) * sizeof(TCHAR) );

		int imax = GetStrLen( pText2 , ilen + m_CaretPosition );
		ilen = ilen + m_CaretPosition  <= imax ? ilen : imax - m_CaretPosition  ;
		::memcpy( m_String + m_CaretPosition , pText , ilen * sizeof(TCHAR)  ) ;
		memset( m_String + m_CaretPosition + ilen  , 0 , ( imax  -  ilen - m_CaretPosition) * sizeof(TCHAR)  );
		m_String[imax] = _T('\0');

		delete [] pText2 ;
		delete[] pText;

		//SetCaretNewPos( ) ;
		OnFlushContrl() ;
	}
	CloseClipboard () ;
}


void CMMUIEdit::ShearText() 
{
	if( m_iMoveMin == m_iMoveMax )
		return ;
	CopyOrShearString( true ) ;
}

void CMMUIEdit::GetTchar( /*const HDC* phdc ,*/ const TCHAR code ) 
{
	CSize cs = GetStrWide(  m_String ,  GetStringLen() ) ;
	if(  code <= _T(' ') ||  cs.cx > m_iMaxWide )
		return  ;

	if( m_iMoveMax == m_iMoveMin && m_CaretPosition >= 0)
		memcpy( m_String + m_CaretPosition + 1  , m_String + m_CaretPosition , ( GetStringLen() - m_CaretPosition + 1) * sizeof(TCHAR));
	else if( m_iMoveMax > m_iMoveMin && EDIT_NORMAL == m_isPassword && m_iMoveMin >= 0 && GetStringLen() >= m_iMoveMax )
	{
		memcpy(m_String + m_iMoveMin, m_String + ( m_iMoveMax - 1) , (GetStringLen()-m_iMoveMax + 2 ) * sizeof(TCHAR));
		memset( m_String + GetStringLen() , 0 , ( sizeof( m_String ) -  GetStringLen() )*sizeof(TCHAR) ) ;

		if( m_CaretPosition >= m_iMoveMax  )
					m_CaretPosition -= (m_iMoveMax - m_iMoveMin) ;
		m_iMoveMin = m_iMoveMax = m_CaretPosition ;
	}
	m_String[m_CaretPosition++ ] = code ;
	cs = GetStrWide( m_String ,  GetStringLen() ) ;
	if(  cs.cx > m_iMaxWide )
	{
		int x = GetStrLen( m_String , GetStringLen() );
		m_String[ x ] = _T('\0') ;
	}
	SetFocus();
	OnFlushContrl( ) ;
}

void CMMUIEdit::KeyDown( const TCHAR code /* , const HDC* phdc */)
{
	switch (code)
	{
	case VK_LEFT: // 左光标键
		{
			if( m_CaretPosition > 0)
			{
				m_CaretPosition--;
				SetFocus();
				OnFlushContrl();
			}
		}
		break;
	case VK_RIGHT:     // 右光标键
		{
			if(m_CaretPosition < GetStringLen() )
			{
				m_CaretPosition ++;
				SetFocus();
				OnFlushContrl();
			}
		}
		break;
	case VK_HOME: // HOME 键
		m_CaretPosition = 0;
		SetFocus();
		OnFlushContrl();
		break;
	case VK_END:  // END 键
		m_CaretPosition = GetStringLen();
		SetFocus();
		OnFlushContrl();
		break;
	case VK_BACK: // 退格键
		if( m_CaretPosition >= 0 )
		{
			if( m_iMoveMax == m_iMoveMin && m_CaretPosition > 0)
			{
				memcpy(m_String + m_CaretPosition - 1, m_String + m_CaretPosition, (GetStringLen()-m_CaretPosition + 1) * sizeof(TCHAR));
				m_CaretPosition-- ;
			}
			else if( m_iMoveMax > m_iMoveMin && EDIT_NORMAL == m_isPassword && m_iMoveMin >= 0 && GetStringLen() >= m_iMoveMax )
			{
				memcpy(m_String + m_iMoveMin, m_String + m_iMoveMax , (GetStringLen()-m_iMoveMax + 1) * sizeof(TCHAR));
				memset( m_String + GetStringLen() , 0 ,  ( sizeof( m_String ) -  GetStringLen() )*sizeof(TCHAR) ) ;

				if( m_CaretPosition >= m_iMoveMax  )
					m_CaretPosition -= (m_iMoveMax - m_iMoveMin ) ;
				m_iMoveMin = m_iMoveMax = m_CaretPosition ;
			}
			SetFocus();
			OnFlushContrl();
		}
		break;
	case VK_DELETE:// 删除键
		{
			int len = GetStringLen();
			if( m_CaretPosition < GetStringLen())
			{
				::memcpy( m_String + m_CaretPosition , m_String + m_CaretPosition + 1, (GetStringLen() - m_CaretPosition + 1) * sizeof(TCHAR));
				OnFlushContrl();
			}
		} 
		break ;
	}
}

void CMMUIEdit::DrawTextEdit( const HDC* phdc   , BOOL bshowgb  , COLORREF* clf ) 
{
	//FlushBackGround( phdc  ) ; 
	RECT rc ;
	rc = m_rc ;
	rc.top += 2 ;
	rc.bottom -= 2 ;
	rc.right -= 1 ;

	RECT _rc ;
	_rc.left = _rc.top = 0 ;
	_rc.right = rc.right-rc.left ;
	_rc.bottom = rc.bottom-rc.top ;
 
	if( m_memDC == NULL )
		m_memDC = CreateCompatibleDC( *phdc );
	if( m_memBitmap == NULL )
		m_memBitmap = CreateCompatibleBitmap( *phdc , _rc.right  , _rc.bottom );
	if( m_hOldbitmap == NULL )
		m_hOldbitmap = (HBITMAP)SelectObject( m_memDC , m_memBitmap );

	HBRUSH hBrush ;
	hBrush = CreateSolidBrush( RGB(255,255,255) ); 
	::FillRect( m_memDC , &_rc , hBrush ) ;
	DeleteObject( hBrush ) ;

	//if( NULL == m_font_wrya )
	//{
	//	m_font_wrya = new Cwryh ;
	//	m_font_wrya->SetWryah( &m_memDC , -16 );
	//}

	if( EDIT_PASSWORD == m_isPassword )
	{	
		::TextOut( m_memDC , m_iTextOutX , m_iTextOutY + 4 , m_Text , GetStringLen() );
	}
	else if( EDIT_NORMAL == m_isPassword )
	{
		CSize cs ;
		int x  = GetStringLen() ;
		GetTextExtentPoint32( m_memDC , m_String , x , &cs );
		while ( cs.cx > m_iMaxWide )
		{
			x-- ;
			GetTextExtentPoint32( m_memDC , m_String , x , &cs );
		}

		if( NULL ==  clf )
			::TextOut( m_memDC, m_iTextOutX , m_iTextOutY , m_String , x );
		else
		{
			COLORREF oldTextColo = SetTextColor( m_memDC  , *clf );
			::TextOut( m_memDC, m_iTextOutX , m_iTextOutY , m_String , x );
			SetTextColor( m_memDC  , oldTextColo ) ;
		}
	}

	if( m_bLBttonDown && m_bMove && m_iMoveMin >= 0 &&  m_iMoveMax >= m_iMoveMin )
	{
		SIZE min_size , max_size ;
		::GetTextExtentPoint( m_memDC , m_String, m_iMoveMin, &min_size) ;
		::GetTextExtentPoint( m_memDC , m_String, m_iMoveMax, &max_size) ;

		COLORREF oldTextColo = SetTextColor( m_memDC  , LOGIN_INPUT ) ;
		COLORREF oldBKColo = SetBkColor( m_memDC , LOGIN_LOGIN_BK ) ;
		int ildBkMode =  SetBkMode( m_memDC , OPAQUE );
		memcpy(m_BkString , m_String + m_iMoveMin , (m_iMoveMax -  m_iMoveMin)*sizeof(TCHAR) ) ;
		m_BkString[  m_iMoveMax -  m_iMoveMin ] = _T('\0');
		if( EDIT_NORMAL == m_isPassword )
		{
			::TextOut( m_memDC , min_size.cx + m_iTextOutX , m_iTextOutY  , m_BkString , m_iMoveMax -  m_iMoveMin ) ;
		}
		SetBkMode( m_memDC , ildBkMode ) ;
		SetTextColor( m_memDC  , oldTextColo ) ;
		SetBkColor( m_memDC , oldBKColo ) ;
	}
	
	BitBlt ( *phdc , rc.left , rc.top , _rc.right - 2 , _rc.bottom  ,
		m_memDC , 0 , 0 , SRCCOPY ) ;
	if( bshowgb )
		SetCaretNewPos( );
}


bool CMMUIEdit::OnLButtonDown(const HDC* phdc, const POINT pt)
{
	if( !InPoint( m_rc , pt ) )
	{
		if(m_bActive)
			HideCaret() ;
		m_bActive = false;
		DrawTextEdit(phdc, false);
		return false ;
	}

	int x  = pt.x - m_TextOutPoint.x  ;
	m_CaretPosition = GetStrPos( &m_memDC , x ) ;
	m_iLButtonDown = m_CaretPosition ;
	m_bMove = false;
	m_bLBttonDown = true;
	m_iMoveMin = m_iMoveMax = m_iLButtonDown  ;
	
	SetFocus();
	m_bActive = false;
	SetCaretNewPos( ) ;
	DrawTextEdit( phdc ) ;
	//OnFlushContrl() ;
	

	return false;
}
void CMMUIEdit::OnLbuttonUp()
{
	m_bLBttonDown = false ;
	m_bMove = false ;
}

void CMMUIEdit::OnMouseMove( const HDC* phdc , const POINT pt )
{
	if( !m_bLBttonDown )
		return ;

	int strPos = 0 ;
	int x  = pt.x - m_TextOutPoint.x ;
	SIZE size ;
	size.cx = 0 ;
	size.cy = 0 ; 
	for ( strPos=0 ; strPos < GetStringLen() ; strPos++ )
	{
		::GetTextExtentPoint( *phdc , m_String, strPos , &size);
		if( size.cx + 4 >= x )
			break ;
	}

	m_iMoveMax = m_iLButtonDown > strPos ? m_iLButtonDown : strPos ;
	m_iMoveMin = m_iLButtonDown > strPos ? strPos : m_iLButtonDown ;

	m_bMove = true ;

	m_CaretPosition = strPos ;
	//SetCaretNewPos( ) ;
	OnFlushContrl() ;
}

void CMMUIEdit::DrawDefaultText( const HDC* phdc , TCHAR* tc )
{
	if( 0 != GetStringLen() )
		return ;
	
	RECT rc ;
	rc = m_rc ;
	rc.top += 2 ;
	rc.bottom -= 2 ;
	rc.right -= 1 ;

	RECT _rc ;
	_rc.left = _rc.top = 0 ;
	_rc.right = rc.right-rc.left ;
	_rc.bottom = rc.bottom-rc.top ;
 
	if( m_memDC == NULL )
		m_memDC = CreateCompatibleDC( *phdc );
	if( m_memBitmap == NULL )
		m_memBitmap = CreateCompatibleBitmap( *phdc , _rc.right  , _rc.bottom );
	if( m_hOldbitmap == NULL )
		m_hOldbitmap = (HBITMAP)SelectObject( m_memDC , m_memBitmap );

	HBRUSH hBrush ;
	hBrush = CreateSolidBrush(WHITE_COLORE);
	::FillRect( m_memDC , &_rc , hBrush ) ;
	

	//if( NULL == m_font_wrya )
	//{
	//	m_font_wrya = new Cwryh ;
	//	m_font_wrya->SetWryah( &m_memDC , -16 );
	//}

	COLORREF oldTextColo = SetTextColor( m_memDC  , RGB(199,199,199) );
	::TextOut( m_memDC, m_iTextOutX , m_iTextOutY , tc , _tcslen(tc) );
	SetTextColor( m_memDC  , oldTextColo ) ;
	DeleteObject(hBrush);
	
	BitBlt ( *phdc , rc.left , rc.top , _rc.right - 2 , _rc.bottom  ,
		m_memDC , 0 , 0 , SRCCOPY ) ;
}


void CMMUIEdit::DrawTextEdging( const HDC* phdc , int shitfing  ) 
{
	return ;

	RECT rc ;
	rc = m_rc ;	
	rc.bottom = m_rc.bottom + 1 ;
	rc.left = m_rc.left + shitfing ;
	rc.right = m_rc.right + 1 ;
	rc.top = m_rc.top - 1 ;
	BOOL bde = TRUE ;
	bde = ::DrawEdge( *phdc, &rc , EDGE_SUNKEN, BF_RECT);  // 绘制边框，EDGE_SUNKEN表示绘制样式为内嵌样式，BF_RECT表示绘制矩形边框
	if( !bde )
	{
		int err = GetLastError() ;
	}
}

void CMMUIEdit::SetFocus( )
{
	if( NULL == s_hCursor )
	{
		::CreateCaret( m_hwnd , (HBITMAP)NULL, 1, 15  );// 创建光标
		s_hCursor =  ::LoadCursor(NULL, IDC_IBEAM ) ;
		::SetCursor( s_hCursor );
	}

	::ShowCaret( m_hwnd );//显示光标;
}

void CMMUIEdit::HideCaret()
{
	if( NULL != s_hCursor )
	{
		::HideCaret( m_hwnd  );
		::DestroyCaret();
		s_hCursor = NULL;
	}
}