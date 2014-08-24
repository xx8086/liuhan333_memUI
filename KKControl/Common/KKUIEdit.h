#pragma once

#include "KKUIBase.h"

class CKKUIEdit  : public CKKUIBase
{
public:
	CKKUIEdit(void);
	~CKKUIEdit(void);

private:
	POINT			m_TextOutPoint ;
	int				m_iTextOutX ;
	int				m_iTextOutY ;
	int				m_iMaxWide	  ;

	int				m_CaretPosition ;//光标位置
	int				m_iLButtonDown ;
	int				m_iMoveMin ;
	int				m_iMoveMax ;

	BOOL			m_bActive ;
	BOOL			m_bLBttonDown ;
	BOOL			m_bMove ;
	BOOL			m_bCtrl ;

	HGLOBAL			m_hGlobal ;
    TCHAR*			m_pGlobal ;

	HDC				m_memDC	;
	HBITMAP			m_memBitmap ;
	HBITMAP			m_hOldbitmap ;

	TCHAR			m_String[TEXTBOX_MAXLENGTH +1] ;
	TCHAR			m_Text[TEXTBOX_MAXLENGTH + 1] ;
	TCHAR			m_BkString[TEXTBOX_MAXLENGTH + 1] ;

	EDIT_TYPE		m_isPassword ;
public:
	BOOL GetCtrl(){ return m_bCtrl; };
	BOOL GetActive(){ return m_bActive ;} ;//用于决定光标显示的。
	void SetActive( BOOL bc ){ m_bActive = bc ; };
	void SetCtrl( BOOL bc ){ m_bCtrl = bc ;}
	
private:
	void CopyOrShearString( bool bShear = false ) ;
	int	 GetStrPos( const HDC* phdc , int cx  ) ;
	int  GetStrLen(  TCHAR* str , int ilen  ) ;
	void SetTextString(TCHAR* str ) ;
	int	 GetStringLen(){ return ::_tcslen( m_String ); } ;	
	CSize GetStrWide( TCHAR* str , int ilen ) ;

public:
	TCHAR* GetString(){ return m_String ; } 

	void CopyText( ) ;
	void PastText (   ) ;
	void ShearText(  ) ;
	void GetTchar( const TCHAR code ) ;
	void KeyDown( const TCHAR code ) ;
	void DrawTextEdit( const HDC* phdc , BOOL bshowgb = TRUE  , COLORREF* clf = NULL ) ;

	BOOL OnLButtonDown(  const HDC* phdc , const POINT pt )	;
	void OnLbuttonUp()	;
	void OnMouseMove( const HDC* phdc , const POINT pt ) ;
	void DrawDefaultText( const HDC* phdc , TCHAR* tc ) ;

	void SetCaretNewPos( /*const HDC* phdc*/ ) ;
	void InitContrlEdit( HWND hwnd , RECT rc  , int textoutX , int textoutY ,
		int cursorX , int cursorY , //光标位置，光标高度不合适可以调cursorY
		int maxwide , //最多显示长读
		TCHAR* text = NULL ,//初始内容
		EDIT_TYPE et = EDIT_NORMAL ) ;
	void DrawTextEdging( const HDC* phdc , int shitfing = -50 ) ;

	void SetFocus() ;
	void HideCaret() ;

};

