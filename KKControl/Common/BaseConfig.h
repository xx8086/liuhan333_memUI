#pragma once


//#ifdef   _DEBUG 
//#define   new   DEBUG_NEW 
//#endif


#ifdef  UNICODE
#define	my_string	 std::wstring 
#define CF_TCHAR CF_UNICODETEXT
#else
#define	my_string	 std::string 
#define CF_TCHAR CF_TEXT
#endif

#define			NOLBTTONDOWN		(	-2 )

#define TEXTBOX_MAXLENGTH		200
#define TEXTBOX_HEIGHT			20
#define	LOGIN_INPUT					RGB( 208, 208 ,208  )
#define	LOGIN_LOGIN_BK				RGB( 51 , 153 , 255 )

#define ID_KUKE_MENU_C                  32775
#define ID_KUKE_MENU_X                  32776
#define ID_KUKE_MENU_V                  32777


typedef enum EditType
{
	EDIT_NORMAL		= 0 ,
	EDIT_PASSWORD	= 1 ,
	EDIT_NOT		= 2 
} EDIT_TYPE ;

typedef enum MemError
{//错误类型
	ERROR_OK			= 0 ,
	ERROR_NULL			= 1 ,//空指针
	ERROR_FILE_PATH		= 2 ,//文件路径出错
	ERROR_HWND			= 3 ,
	ERROR_RETURN		= 4 ,
	ERROR_OVERLAP		= 5 ,//重叠
	ERROR_OPENFILEFAIL	= 6 ,//打开文件失败
	ERROR_OTHER
} MEM_ERROR ;

typedef enum ItemStatus   
{//控件状态   
	STATUS_NORMAL       = 0 ,
	STATUS_OVER         = 1 ,
	STATUS_CLICK		= 2 ,
	STATUS_AMOUNT		= 3 

} ITEM_STATUS ;

typedef struct ModuleContrl
{
	int module_contrl ;
	ITEM_STATUS ls ;

} MODULE_CONTRL ;

typedef enum ContrlType
{
	CONTRL_BK		=	0 ,
	CONTRL_BTN		=	1 ,
	CONTRL_PAGE		=	2 ,
	CONTRL_SCROLL	=	3
}CONTRL_TYPR ;


#define WHITE_COLORE			RGB ( 255 , 255 , 255 )

struct TCBuffer
{
	TCHAR buffer[MAX_PATH];
	TCBuffer( TCHAR* _strID)
	{
		memset(  buffer , 0 , sizeof(TCHAR)*MAX_PATH);
		_tcscpy_s( buffer ,MAX_PATH , _strID ) ;
	} ;
	TCBuffer& operator = ( TCBuffer & other)
	{
		memset( buffer , 0 , sizeof(TCHAR)*MAX_PATH);
		_tcscpy_s( buffer ,MAX_PATH , other.buffer ) ;
		return *this ;
	} ;
} ;


struct ItemStatusImage
{
	int  iItemStatusAmount ;
	TCHAR path[3][MAX_PATH] ;

	ItemStatusImage():iItemStatusAmount(0)
	{
		memset( path , 0 , sizeof( path ) ) ;
	}
} ;