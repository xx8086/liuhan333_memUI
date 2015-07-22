#include "stdafx.h"
#include <Objbase.h>

#include "BaseGif.h"

#define TIMER_SEC (WM_USER + 0x011)


	CGifContrl* CGifContrl::s_Gif[ MAX_GIF_NUMBER ];
	int CGifContrl::s_iGifCount ;



	CGifContrl::CGifContrl():m_index(0),m_image(NULL),m_pItem(NULL),m_hwnd(NULL),m_iFCount(0),m_bErase(false)
	{}
	CGifContrl::~CGifContrl() 
	{}

	 void CGifContrl::AddGifContrl( CGifContrl* addGif )
	{
		s_Gif[s_iGifCount] = addGif ;
		s_Gif[s_iGifCount]->SetIndex(s_iGifCount);
		s_iGifCount++ ;
	}
	 void CGifContrl::CreateGifControl(HWND fhwnd, HINSTANCE hInstance, const POINT ptBeginPostion, const TCHAR*  szName, bool bsrase)
	{
		if( false == LoadGif( szName))
			return ;

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= CGifContrl::WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= 0 ;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GIFBYGDIPLUS));
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);//CreateSolidBrush(RGB(255,0,0));
		wcex.lpszMenuName	= NULL ;//MAKEINTRESOURCE(IDC_GIFBYGDIPLUS);
		wcex.lpszClassName	= L"static";//szWindowClass;
		wcex.hIconSm		= 0;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		bool r = RegisterClassEx(&wcex);

		m_hwnd = CreateWindow(L"static",NULL,WS_VISIBLE |WS_CHILD ,
			ptBeginPostion.x, ptBeginPostion.y,
			m_rc.right, m_rc.bottom ,
			fhwnd, NULL, hInstance, NULL);
		
		if( !m_hwnd )
			return ;

		m_ptBeginPostion = ptBeginPostion;
		m_bErase = bsrase;
		SetTimer(m_hwnd , TIMER_SEC ,0 , NULL );

		ShowWindow(m_hwnd , SW_SHOW);
		UpdateWindow(m_hwnd);
	}


	void CGifContrl::AddCount()
	{
		KillTimer (m_hwnd, TIMER_SEC) ;
		if( m_image == NULL )
			return ;

		GUID    Guid = FrameDimensionTime;
		m_image->SelectActiveFrame(&Guid,m_iFCount++);
		if(m_iFCount == m_unFrameAmount) //frameCount是上面GetFrameCount返回值
			m_iFCount= 0;     //如果到了最后一帧数据又重新开始
		//计算此帧要延迟的时间
		long lPause = ((long*)m_pItem->value)[m_iFCount]*10;
		SetTimer(m_hwnd,TIMER_SEC,lPause,NULL);
		InvalidateRect (m_hwnd, &m_rc, m_bErase) ;
	}

	bool CGifContrl::InRect(RECT rc )
	{
		bool rt = false ;
		if( m_rc.top >= rc.top &&
			rc.bottom >= m_rc.bottom &&
			m_rc.left >= rc.left && 
			rc.right >= m_rc.right )
			rt = true ;

		return rt ;
	}
	void CGifContrl::ClearUP( ) 
	{
		if(m_pItem != NULL)
		{
			free(m_pItem);
			m_pItem = NULL ;
		};
		if(m_image !=  NULL )
		{
			delete m_image ;
			m_image = NULL ;
		}
		if(NULL != m_hwnd)
		{
			DestroyWindow(m_hwnd);
			m_hwnd = NULL ;
		}
	}

	bool CGifContrl::LoadGif(const TCHAR* szName)
	{
		if( m_image != NULL )
		{
			delete m_image ;
			m_image = NULL ;
		}
		m_image = new Image( szName );
		UINT count = 0;
		count = m_image->GetFrameDimensionsCount();
		GUID *pDimensionIDs=(GUID*)new GUID[count];
		m_image->GetFrameDimensionsList(pDimensionIDs, count);
		WCHAR strGuid[39];
		::StringFromGUID2(pDimensionIDs[0], strGuid, 39);
		m_unFrameAmount= m_image->GetFrameCount(&pDimensionIDs[0]);
		delete []pDimensionIDs;   

		int size = m_image->GetPropertyItemSize(PropertyTagFrameDelay);//获得帧延迟项
		
		if( m_pItem != NULL) 
		{
			free( m_pItem );
			m_pItem = NULL;
		}
		m_pItem = (PropertyItem*)malloc(size);
		m_image->GetPropertyItem(PropertyTagFrameDelay,size,m_pItem);
		SetRect( &m_rc , 0 , 0 ,  m_image->GetWidth(), m_image->GetHeight()) ;
		
		return true;
	}

	void CGifContrl::DrawGif(const HDC* pHdc )
	{
		if( m_image == NULL )
			return ;
		Graphics gh( *pHdc);
		gh.DrawImage(m_image,m_rc.top,m_rc.left,m_image->GetWidth(),m_image->GetHeight());
	}


	LRESULT CALLBACK	CGifContrl::WndProc( HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam )
	{
		PAINTSTRUCT ps;
		HDC hdc;
		static int fcount = 0 ;
		static UINT frameCount = 0 ;
		static Image* image = NULL ;
		static PropertyItem* pItem = NULL ;
		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此添加任意绘图代码...

			//for( int i = s_iGifCount ; i >= 0 ; i-- )
			for( int i = 0 ; i < s_iGifCount ; i++)
			{
				if( s_Gif[i] != NULL  && s_Gif[i]->IsHWND(hWnd ) && s_Gif[i]->InRect(ps.rcPaint )  )
				{
					s_Gif[i]->DrawGif(&hdc) ;
					break;
				}
			}
			
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			for( int i = 0 ; i < s_iGifCount ; i++ )
			{
				if( s_Gif[i] != NULL  )
				{
					s_Gif[i]->ClearUP() ;
				}
			}

			PostQuitMessage(0);
			break;

		case WM_CREATE:

			//
			break;
		case WM_TIMER:
			switch(wParam)
			{
			case  TIMER_SEC:
				for( int i = 0 ; i < s_iGifCount ; i++ )
				{
					if( s_Gif[i] != NULL  && s_Gif[i]->IsHWND(hWnd ) )
					{
						s_Gif[i]->AddCount() ;
						break;
					}
				}
				break;
			default:
				break;

			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

