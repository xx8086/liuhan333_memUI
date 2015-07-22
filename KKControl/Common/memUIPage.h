#pragma once

#include "memUIButton.h"

#define MAX_PAGE_NUMBER		(20)
typedef enum PageType
{
	PATE_TYPE_OMIT	= 0,
	PATE_TYPE_PREV	= 1,
	PATE_TYPE_NEXT	= 2,
	PATE_TYPE_DIGIT = 3,
}PATE_TYPE;

struct PageRes
{
	ItemStatusImage isi;
	PATE_TYPE		type;
	int				vlue;
	RECT			rc;
};

struct ParamPage
{
	PageRes*			pOmit; 
	PageRes*			pPrev;
	PageRes*			pNext;
	PageRes**			ppRse;
	
	int					iPageAmount;
	int					iCurrentPage;
	int					iMaxShowPage;
	bool				bAutoWriteDigit;
	HWND				hWnd;
	POINT				ptBeginPosition;
};

class CUIPage
{
public:
	CUIPage();
	virtual ~CUIPage();
public:
	bool InitPage(const ParamPage* pInitpage, int& iRseAmount);
	
private:
	HWND				m_hwnd;
	int					m_iPageAmount;//页数
	int					m_iCurrentPage;//当前页
	int					m_iMaxShowPage;//最大显示页数，超过该指定值就回显示省略号
	bool				m_bAutoWriteDigit;
	bool				m_bHaveExButton;
	RECT				m_rc;
	CMMUIButton*		m_pPageButton;
	CMMUIButton			m_PrevButton;
	CMMUIButton			m_NextButton;
	CMMUIButton			m_OmitButton;
};
//< 1 2 3 >
//< 1 ... 4>//应为省略号左右两边必须有数字，m_iPageAmount<=4时只能全部页数显示，不能显示省略号
//< 1 2 ... 5>//省略号至少代表大于等于2个页数，m_iMaxShowPage>=3 && m_iPageAmount>=5
//< 1 2 ... 6>//(m_iMaxShowPage == 4 && m_iPageAmount == 6)	m_iPageAmount-m_iMaxShowPage<=2只显示最多一个省略号
//< 1 2 ... 5 ... 7>//m_iMaxShowPage == 4 && m_iPageAmount == 7
//< 1 2 ... 5 ... 8>//m_iMaxShowPage == 4 && m_iPageAmount == 8
//< 1 2 ... 5 6 7>//m_iMaxShowPage == 5 && m_iPageAmount == 7
