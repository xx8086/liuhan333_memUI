#pragma once
#include <GdiPlus.h>
using namespace Gdiplus;

#define MAX_GIF_NUMBER		( 102 ) 
class CGifContrl
{
public:
	CGifContrl();
	~CGifContrl() ;
public:
	static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	static void AddGifContrl( CGifContrl* addGif );
public:
	void DreawGif(const HDC* pHdc ) ;
	void CreateCtrntrl(HWND fhwnd , HINSTANCE hInstance, RECT rc , const TCHAR*  szName , bool bsrase = false );
	void AddCount();
	bool LoadGif( const TCHAR* szName ) ;
	bool InRect(RECT rc );
	bool IsHWND(HWND hwnd){ return (hwnd == m_hwnd );}
	void SetIndex(int index ) { m_index = index ;} 
	void ClearUP( ) ;
	void Init() ;

	int GetCount(){ return m_fcount ; }
	unsigned int GetFrameCounnt(){ return m_frameCount ; }
	Image* GetImage(){ return m_image; }
	PropertyItem* GetItem(){ return m_pItem  ;}
	
	
private:
	static CGifContrl* s_Gif[ MAX_GIF_NUMBER ];
	static int s_iGifCount ;


	RECT m_rc;
	int m_fcount  ;
	unsigned int  m_frameCount  ;
	Image* m_image  ;
	PropertyItem* m_pItem  ;
	HWND m_hwnd ;
	int m_index  ;
	bool	m_bErase ;
};









