
// KKControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KKControl.h"
#include "KKControlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKKControlDlg 对话框


CKKControlDlg::CKKControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKKControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKKControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKKControlDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
//	ON_WM_CHAR()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CKKControlDlg 消息处理程序

BOOL CKKControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	ItemStatusImage isi  ;
	isi.iItemStatusAmount = 3 ; 

	_stprintf_s(isi.path[STATUS_NORMAL], MAX_PATH, _T("\\..\\skin\\play_normal.png"));
	_stprintf_s(isi.path[STATUS_OVER], MAX_PATH, _T("\\..\\skin\\play_hover.png"));
	_stprintf_s(isi.path[STATUS_CLICK], MAX_PATH, _T("\\..\\skin\\play_click.png"));

	RECT rc ;
	SetRect( &rc , 50 , 50 , 50 + 203 , 50 + 40 ) ;
	m_btn[0].InitBtn( GetSafeHwnd() , rc , isi ) ;

	SetRect( &rc , 50 , 100 , 50 + 203 , 100 + 40 ) ;
	m_btn[1].InitBtn( GetSafeHwnd() , rc , isi ) ;

	SetRect( &rc , 100 , 150 , 100 + 240 , 150 + 40 ) ;
	m_edit[0].InitContrlEdit(  GetSafeHwnd()  , rc ,
		4 , ( rc.bottom - rc.top ) / 3 - 2,
		rc.left + 4, rc.top + ( rc.bottom - rc.top ) / 3
		,210 , _T("123456abcd账号哈哈") );

	SetRect( &rc , 100 , 200 , 100 + 240 , 200 + 40 ) ;
	m_edit[1].InitContrlEdit(  GetSafeHwnd()  , rc ,
		4 , ( rc.bottom - rc.top ) / 3 - 2,
		rc.left + 4, rc.top + ( rc.bottom - rc.top ) / 3
		,210 , _T("123456abcd密码") ,
		EDIT_PASSWORD );


	TCHAR DirectoryPath[MAX_PATH] ;
	memset( DirectoryPath , 0 , MAX_PATH*sizeof(TCHAR) ) ;
	DWORD dr = m_btn[0].GetModulePath( MAX_PATH , DirectoryPath ) ;
	TCHAR GifPath[MAX_PATH] ;
	memset( GifPath , 0 , MAX_PATH*sizeof(TCHAR) ) ;
	_stprintf_s(GifPath, MAX_PATH, _T("%s\\..\\skin\\1.gif"), DirectoryPath);


	POINT ptBegin;
	ptBegin.x = 200;
	ptBegin.y = 250;
	CGifContrl::AddGifContrl( &m_gif[0] ) ;
	m_gif[0].CreateGifControl(GetSafeHwnd(), AfxGetInstanceHandle(), ptBegin, GifPath, true);

	memset( GifPath , 0 , MAX_PATH*sizeof(TCHAR) ) ;
	_stprintf_s(GifPath, MAX_PATH, _T("%s\\..\\skin\\2.gif"), DirectoryPath);
	ptBegin.x = 250;
	ptBegin.y = 250;
	CGifContrl::AddGifContrl( &m_gif[1] ) ;
	m_gif[1].CreateGifControl(GetSafeHwnd(), AfxGetInstanceHandle(), ptBegin, GifPath);


	isi.iItemStatusAmount = 3;
	_stprintf_s(isi.path[STATUS_NORMAL], MAX_PATH, _T("\\..\\skin\\page_normal.png"));
	_stprintf_s(isi.path[STATUS_OVER], MAX_PATH, _T("\\..\\skin\\page_hover.png"));
	_stprintf_s(isi.path[STATUS_CLICK], MAX_PATH, _T("\\..\\skin\\page_click.png"));

	POINT ptPage;
	ptPage.x = 30;
	ptPage.y = 420;
	int iSpace = 8;
	ParamPage parpage;
	PageRes	pageOmit;
	PageRes	pagePrev;
	PageRes	pageNext;
	PageRes pagePagep[10];
	
	//24*23
	SetRect(&pagePrev.rc, ptPage.x, ptPage.y, ptPage.x + 24, ptPage.y + 23);
	pagePrev.type = PATE_TYPE_PREV;
	pagePrev.isi = isi;
	int beginX = 0;
	for (int i = 1; i <= 10; i++)
	{
		beginX = ptPage.x + i*(iSpace + 24);
		SetRect(&pagePagep[i - 1].rc, beginX, ptPage.y, beginX + 24, ptPage.y + 23);
		pagePagep[i - 1].isi = isi;
		pagePagep[i - 1].type = PATE_TYPE_DIGIT;
	}

	beginX += iSpace + 24;
	SetRect(&pageNext.rc, beginX, ptPage.y, beginX + 24, ptPage.y + 23);
	pageNext.type = PATE_TYPE_NEXT;
	pageNext.isi = isi;

	SetRect(&rc, pagePrev.rc.left, pagePrev.rc.top, pageNext.rc.right, pageNext.rc.bottom);
	parpage.rc = rc;
	parpage.iPageAmount = 25;
	parpage.iAllShowPage = 10;
	parpage.hWnd = GetSafeHwnd();
	parpage.pNext = &pageNext;
	parpage.pPrev = &pagePrev;
	parpage.ppRse = (PageRes*)(&pagePagep);
	m_page.InitPage(&parpage,10);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKKControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC* cdc = GetDC() ;
		HDC hdc =  cdc->GetSafeHdc() ;

		RECT rc;
		GetUpdateRect(&rc, TRUE);
		m_btn[0].OnPaint(&hdc, rc);
		m_btn[1].OnPaint(&hdc, rc);

		if( m_edit[0].GetActive())
			m_edit[0].DrawTextEdit( &hdc ) ;
		if( m_edit[1].GetActive() )
			m_edit[1].DrawTextEdit( &hdc ) ;

		m_page.OnPaint(&hdc, rc);

		::ReleaseDC( GetSafeHwnd() , hdc ) ;
		ReleaseDC( cdc );

		CDialogEx::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKKControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKKControlDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int index = -1 ;
	if( m_btn[0].OnLButtonDown( (POINT)point  ) )
	{
		index = 0 ;//
		return;
	}
	else if( m_btn[1].OnLButtonDown( (POINT)point  ) )
	{
		index = 1 ;//
		return;
	}

	if (m_page.OnLButtonDown((POINT)point))
	{
		return;
	}

	CDC* cdc = GetDC() ;
	HDC hdc =  cdc->GetSafeHdc() ;

	if( m_edit[0].OnLButtonDown( &hdc , (POINT)point  )  )
	{
		index = 2 ;
	}
	if( m_edit[1].OnLButtonDown( &hdc , (POINT)point  ) )
	{
		index  = 3 ;
	}
	::ReleaseDC( GetSafeHwnd() , hdc ) ;
	ReleaseDC( cdc );

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CKKControlDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_btn[0].OnMouseLeave( ( POINT)point ) ;
	m_btn[0].OnMouseHover( ( POINT)point ) ;
	m_btn[1].OnMouseLeave( ( POINT)point ) ;
	m_btn[1].OnMouseHover( ( POINT)point ) ;
	m_page.OnMouseLeave((POINT)point);
	m_page.OnMouseHover((POINT)point);

	CDC* cdc = GetDC() ;
	HDC hdc =  cdc->GetSafeHdc() ;
	m_edit[1].OnMouseMove( &hdc , ( POINT)point );
	m_edit[0].OnMouseMove( &hdc , ( POINT)point );
	::ReleaseDC( GetSafeHwnd() , hdc ) ;
	ReleaseDC( cdc );
	
	CDialogEx::OnMouseMove(nFlags, point);
}


void CKKControlDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_btn[0].OnLButtonUp( (POINT)point  );
	m_btn[1].OnLButtonUp( (POINT)point  ) ;
	m_page.OnLButtonUp((POINT)point);
	m_edit[1].OnLbuttonUp() ;
	m_edit[0].OnLbuttonUp() ;
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CKKControlDlg::EditOperator( MSG* pMsg  )
{
	CMMUIEdit* pEdit = NULL  ;
	if( m_edit[0].GetActive())
	{
		pEdit = &m_edit[0] ;
	}
	else if(  m_edit[1].GetActive() )
	{
		pEdit = &m_edit[1] ;
	}
	if( NULL == pEdit )
	{
		return ;
	}
	if( WM_CHAR == pMsg->message )
	{
		pEdit->GetTchar( pMsg->wParam ) ;
	}
	else if( WM_KEYUP == pMsg->message )
	{
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			pEdit->SetCtrl(FALSE );//m_bCtrl = FALSE ;
		}
			
	}
	else if( WM_KEYDOWN == pMsg->message )
	{
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			pEdit->SetCtrl(TRUE );
		}
			

		if( pEdit->GetCtrl() == TRUE )
		{
			switch(pMsg->wParam)
			{
			case 'v':
			case 'V':
				pEdit->PastText( );
				break;
			case 'c':
			case 'C':
				pEdit->CopyText( ) ;
				break;
			case 'x':
			case 'X':
				pEdit->ShearText( );
				break;
			default:      break; 
			}
		}

		else 
		{
			pEdit->KeyDown( pMsg->wParam ) ;
		}
	}
	else if( WM_COMMAND ==  pMsg->message )
	{
		switch( pMsg->wParam )
		{
		case ID_KUKE_MENU_C:
			pEdit->CopyText( ) ;
			break;
		case ID_KUKE_MENU_X:
			pEdit->ShearText( );
			break;
		case ID_KUKE_MENU_V:
			pEdit->PastText( );
			break;
		}
	}
}

BOOL CKKControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	EditOperator(pMsg) ;

	return CDialogEx::PreTranslateMessage(pMsg) ;
}
