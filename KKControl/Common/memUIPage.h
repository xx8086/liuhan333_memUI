#pragma once

#include "memUIButton.h"
#include "memUIBaseTextOut.h"

#define MAX_PAGE_NUMBER		(20)
typedef enum PageType
{
	PATE_TYPE_OMIT	= 0,
	PATE_TYPE_PREV	= 1,
	PATE_TYPE_NEXT	= 2,
	PATE_TYPE_DIGIT = 3,
	PATE_TYPE_ALL	= 4,
	PATE_TYPE_NULL	= 5,
}PATE_TYPE;

typedef struct PageHover
{
	PATE_TYPE	pageType;
	int			iVlue;
	PageHover() :pageType(PATE_TYPE_NULL), iVlue(-1){}
}PAGE_HOVER;

struct PageRes
{
	ItemStatusImage isi;
	PATE_TYPE		type;
	//int				vlue;
	RECT			rc;
};

struct ParamPage
{
	PageRes*			pOmit; 
	PageRes*			pPrev;
	PageRes*			pNext;
	PageRes*			ppRse;
	
	int					iPageAmount;
	//int					iCurrentPage;
	int					iAllShowPage;
	//int					iSpace;
	HWND				hWnd;
	//POINT				ptBeginPosition;
	RECT				rc;
};

class CUIPage : public CMMUIBase, public CMMUIBaseTextOut
{
public:
	CUIPage();
	virtual ~CUIPage();
public:
	bool	InitPage(const ParamPage* pInitpage, int iRseAmount);
	int		ClickNextButton();
	int		ClickPrevButton();
	bool	ClickPage(int index);

public:
	 bool    OnMouseMove(POINT pt);
	 bool    OnLButtonDown(POINT pt);
	 bool    OnLButtonUp(POINT pt);
	 bool    OnMouseLeave(POINT pt);
	 bool    OnMouseHover(POINT pt);

	// bool	OnPaint(const RECT  rc);
	bool	OnPaint(const HDC* phdc, RECT rc);
private:
	bool	OnLeaveNormal();//确保鼠标离开page的时候非当前click页职位normal
	bool	OmitChange(int& index);
	bool	InitCurrentPage();//m_iCurrentIndex = 1
	bool	DrawExButton(const HDC* phdc, RECT rc, PATE_TYPE ptype);
	bool	DrawPage(const HDC* phdc, RECT rc, int index);
	bool	NormalPage(POINT pt);
	PATE_TYPE		PointIndex(POINT pt, int& index);
	PATE_TYPE		RectIndex(RECT rc, int& index);
private:
	HWND				m_hWnd;
	int					m_iPageAmount;//页数
	int					m_iCurrentIndex;//当按钮号
	int					m_iOldIndex;//当按钮号
	int					m_iAllShowPage;//页数加省略号一共需要显示的页签
	int*				m_pPageArray;//页情况
	bool				m_bHaveExButton;
	bool				m_bHaveOmit;
	bool				m_bAllPaint;
	RECT				m_rc;
	CMMUIButton*		m_pPageButton;
	CMMUIButton			m_PrevButton;
	CMMUIButton			m_NextButton;
	CMMUIButton			m_OmitButton;
	PAGE_HOVER			m_MousePostion;
};
//*m_ppPageArray[0]					永远为1
//*m_ppPageArray[1]					为2或者...
//*m_ppPageArray[m_iAllShowPage-1]	永远为m_iPageAmount
//*m_ppPageArray[m_iAllShowPage-2]	为m_iPageAmount-1或者...
