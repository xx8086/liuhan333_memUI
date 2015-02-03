
// KKControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KKControl.h"
#include "KKControlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKKControlDlg �Ի���


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


// CKKControlDlg ��Ϣ�������

BOOL CKKControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	ItemStatusImage isi  ;
	isi.iItemStatusAmount = 3 ; 
	_stprintf( isi.path[0] , _T("\\..\\skin\\play_normal.png") ) ;
	_stprintf( isi.path[1] , _T("\\..\\skin\\play_hover.png") ) ;
	_stprintf( isi.path[2] , _T("\\..\\skin\\play_click.png") ) ;

	RECT rc ;
	SetRect( &rc , 50 , 50 , 50 + 203 , 50 + 40 ) ;
	m_btn[0].InitBtn( GetSafeHwnd() , rc , isi ) ;

	SetRect( &rc , 50 , 100 , 50 + 203 , 100 + 40 ) ;
	m_btn[1].InitBtn( GetSafeHwnd() , rc , isi ) ;

	SetRect( &rc , 100 , 150 , 100 + 240 , 150 + 40 ) ;
	m_edit[0].InitContrlEdit(  GetSafeHwnd()  , rc ,
		4 , ( rc.bottom - rc.top ) / 3 - 2,
		rc.left + 4, rc.top + ( rc.bottom - rc.top ) / 3
		,210 , _T("123456abcd�˺Ź���") );

	SetRect( &rc , 100 , 200 , 100 + 240 , 200 + 40 ) ;
	m_edit[1].InitContrlEdit(  GetSafeHwnd()  , rc ,
		4 , ( rc.bottom - rc.top ) / 3 - 2,
		rc.left + 4, rc.top + ( rc.bottom - rc.top ) / 3
		,210 , _T("123456abcd����") ,
		EDIT_PASSWORD );


	TCHAR DirectoryPath[MAX_PATH] ;
	memset( DirectoryPath , 0 , MAX_PATH*sizeof(TCHAR) ) ;
	DWORD dr = m_btn[0].GetModulePath( MAX_PATH , DirectoryPath ) ;
	TCHAR GifPath[MAX_PATH] ;
	memset( GifPath , 0 , MAX_PATH*sizeof(TCHAR) ) ;
	_stprintf(GifPath, _T("%s\\..\\skin\\1.gif"), DirectoryPath ) ;

	SetRect( &rc , 200 , 250 , 200+35 , 250+38 ) ;
	CGifContrl::AddGifContrl( &m_gif[0] ) ;
	m_gif[0].CreateCtrntrl( GetSafeHwnd() , AfxGetInstanceHandle() ,rc , GifPath , true ); 

	memset( GifPath , 0 , MAX_PATH*sizeof(TCHAR) ) ;
	_stprintf(GifPath, _T("%s\\..\\skin\\2.gif"), DirectoryPath ) ;
	SetRect( &rc , 250 , 250 , 250+250 , 250+179 ) ;
	CGifContrl::AddGifContrl( &m_gif[1] ) ;
	m_gif[1].CreateCtrntrl( GetSafeHwnd() ,  AfxGetInstanceHandle() ,rc ,GifPath ); 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CKKControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC* cdc = GetDC() ;
		HDC hdc =  cdc->GetSafeHdc() ;
		CRect rect;
		GetClientRect(&rect);
		m_btn[0].OnPaint( &hdc , rect ) ;
		m_btn[1].OnPaint( &hdc , rect ) ;

		if( m_edit[0].GetActive())
			m_edit[0].DrawTextEdit( &hdc ) ;
		if( m_edit[1].GetActive() )
			m_edit[1].DrawTextEdit( &hdc ) ;
		::ReleaseDC( GetSafeHwnd() , hdc ) ;
		ReleaseDC( cdc );

		CDialogEx::OnPaint();
	}
}


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CKKControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKKControlDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	int index = -1 ;
	if( m_btn[0].OnLButtonDown( (POINT)point  ) )
	{
		index = 0 ;//
	}
	else if( m_btn[1].OnLButtonDown( (POINT)point  ) )
	{
		index = 1 ;//
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_btn[0].OnMouseLeave( ( POINT)point ) ;
	m_btn[0].OnMouseHover( ( POINT)point ) ;
	m_btn[1].OnMouseLeave( ( POINT)point ) ;
	m_btn[1].OnMouseHover( ( POINT)point ) ;


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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_btn[0].OnLButtonUp( (POINT)point  );
	m_btn[1].OnLButtonUp( (POINT)point  ) ;
	m_edit[1].OnLbuttonUp() ;
	m_edit[0].OnLbuttonUp() ;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CKKControlDlg::EditOperator( MSG* pMsg  )
{
	CKKUIEdit* pEdit = NULL  ;
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
	// TODO: �ڴ����ר�ô����/����û���

	EditOperator( pMsg  ) ;

	return CDialogEx::PreTranslateMessage(pMsg) ;
}
