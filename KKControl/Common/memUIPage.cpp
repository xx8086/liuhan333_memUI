#include "stdafx.h"
#include "memUIPage.h"


CUIPage::CUIPage() :
m_hWnd(NULL),
m_iPageAmount(0), 
m_iAllShowPage(0),
m_iCurrentIndex(0),
m_pPageButton(NULL), 
m_bHaveExButton(false),
m_bHaveOmit(false),
m_ppPageArray(NULL),
m_bAllPaint(false)
{}

CUIPage::~CUIPage()
{
}

bool CUIPage::InitPage(const ParamPage* pInitpage, int& iRseAmount)
{
	JUDGENULL(pInitpage, false)
	JUDGETRUE(iRseAmount <= 0, false)
	JUDGETRUE(pInitpage->iAllShowPage <= 0, false)
	JUDGETRUE((pInitpage->hWnd == NULL), false)
	JUDGETRUE(!IsWindow(pInitpage->hWnd), false)

	if (iRseAmount > MAX_PAGE_NUMBER)
		iRseAmount = MAX_PAGE_NUMBER;


	if (NULL != pInitpage->pPrev && 
		NULL != pInitpage->pNext &&
		PATE_TYPE_PREV == pInitpage->pPrev->type &&
		PATE_TYPE_NEXT == pInitpage->pNext->type)
	{
		m_PrevButton.InitBtn(pInitpage->hWnd, pInitpage->pPrev->rc, pInitpage->pPrev->isi);
		m_NextButton.InitBtn(pInitpage->hWnd, pInitpage->pNext->rc, pInitpage->pNext->isi);
		m_bHaveExButton = true;
	}

	if (m_bHaveExButton &&
		NULL != pInitpage->pOmit &&
		PATE_TYPE_OMIT == pInitpage->pOmit->type)
	{
		m_OmitButton.InitBtn(pInitpage->hWnd, pInitpage->pOmit->rc, pInitpage->pOmit->isi);

		//只有总页数大于要显示页数2及其以上才能显示省略
		JUDGETRUE((pInitpage->iPageAmount > pInitpage->iAllShowPage + 1), false)
		m_bHaveOmit = true;
	}

	m_pPageButton = new CMMUIButton[iRseAmount];
	JUDGENULL(m_pPageButton, false)
	for (short i = 0; i < iRseAmount; i++)
	{
		m_pPageButton[i].InitBtn(pInitpage->hWnd, pInitpage->ppRse[i]->rc, pInitpage->ppRse[i]->isi);
	}

	m_hWnd = pInitpage->hWnd;
	m_iPageAmount = pInitpage->iPageAmount;//总页数
	m_iAllShowPage = pInitpage->iAllShowPage;
	m_pt = pInitpage->ptBeginPosition;
	m_iCurrentIndex = 1;

	return true;
}

int	CUIPage::ClickPrevButton()
{
	JUDGETRUE(m_iCurrentIndex == 1, 1)
	JUDGETRUE(m_iCurrentIndex < 1, 0)
	JUDGETRUE(m_iCurrentIndex > m_iAllShowPage, 0)

	int iPrevIndex = m_iCurrentIndex - 1;
	if (!m_bHaveOmit)
	{
		JUDGETRUE(ClickPage(iPrevIndex),0);
		return iPrevIndex;
	}
	//1 ... [x] x+1 y-1  y ... [max]
	//当m_iCurrentIndex为x或者max时候会触发...改变,其他情况不会；
	if (*m_ppPageArray[iPrevIndex] == 0)
	{
		JUDGETRUE(!OmitChange(iPrevIndex), 0);
	}
	else
	{
		JUDGETRUE(ClickPage(iPrevIndex), 0);
	}
	return iPrevIndex;
}

int	CUIPage::ClickNextButton()
{
	JUDGETRUE(m_iCurrentIndex == m_iAllShowPage, m_iAllShowPage)
	JUDGETRUE(m_iCurrentIndex < 1, 0)
	JUDGETRUE(m_iCurrentIndex > m_iAllShowPage, 0)

	int iNextIndex = m_iCurrentIndex + 1;
	if (!m_bHaveOmit)
	{
		JUDGETRUE(ClickPage(iNextIndex), 0);
		return iNextIndex;
	}
	//1 ... [x] x+1 y-1  y ... [max]
	//当m_iCurrentIndex为1或者y时候会触发...改变,其他情况不会；
	if (*m_ppPageArray[iNextIndex] == 0)
	{
		JUDGETRUE(!OmitChange(iNextIndex),0);
	}
	else
	{
		JUDGETRUE(ClickPage(iNextIndex), 0);
	}
	return iNextIndex;
}
bool	CUIPage::OmitChange(int index)
{
	int iValue;
	if (2 == index)
	{
		iValue = 3;
		*m_ppPageArray[2] = 2;
		*m_ppPageArray[m_iAllShowPage - 1] = 0;
	}
	else if (m_iAllShowPage - 1 == index)
	{
		iValue = m_iPageAmount - m_iAllShowPage + 3;
		*m_ppPageArray[2] = 0;
		*m_ppPageArray[m_iAllShowPage - 1] = m_iPageAmount - 1;
	}
	for (int i = 3; i <= m_iAllShowPage - 2; i++)
	{
		*m_ppPageArray[i] = iValue++;
	}
	m_iCurrentIndex = index;
	m_bAllPaint = true;
}
bool	CUIPage::ClickPage(int index)
{
	JUDGETRUE(index < 1, false);
	JUDGETRUE(index > m_iAllShowPage, false);


}