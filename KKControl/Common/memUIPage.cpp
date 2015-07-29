#include "stdafx.h"
#include "memUIPage.h"


CUIPage::CUIPage() :
m_hWnd(NULL),
m_iPageAmount(0), 
m_iAllShowPage(0),
m_iCurrentIndex(0),
m_iOldIndex(0),
m_pPageButton(NULL), 
m_bHaveExButton(false),
m_bHaveOmit(false),
m_pPageArray(NULL),
m_bAllPaint(false)
{}

CUIPage::~CUIPage()
{
	if (m_pPageButton != NULL)
	{
		delete[] m_pPageButton;
		m_pPageButton = NULL;
	}

	if (m_pPageArray != NULL)
	{
		delete[] m_pPageArray;
		m_pPageArray = NULL;
	}
}

bool CUIPage::InitPage(const ParamPage* pInitpage, int iRseAmount)
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
		pInitpage->iPageAmount >= pInitpage->iAllShowPage + 2 )
	{
		m_bHaveOmit = true;
	}

	m_pPageButton = new CMMUIButton[iRseAmount];
	JUDGENULL(m_pPageButton, false)
	for (short i = 0; i < iRseAmount; i++)
	{
		m_pPageButton[i].InitBtn(pInitpage->hWnd, ((pInitpage->ppRse) + i)->rc, ((pInitpage->ppRse) + i)->isi);
	}

	m_hWnd = pInitpage->hWnd;
	m_iPageAmount = pInitpage->iPageAmount;//总页数
	m_iAllShowPage = pInitpage->iAllShowPage;
	m_rc = pInitpage->rc;
	m_pPageArray = new int[iRseAmount];
	InitCurrentPage();

	return true;
}

int	CUIPage::ClickPrevButton()
{
	JUDGETRUE((m_iCurrentIndex == 0), 1)
	JUDGETRUE(m_iCurrentIndex < 0, 0)
	JUDGETRUE(m_iCurrentIndex >= m_iAllShowPage, 0)

	int iPrevIndex = m_iCurrentIndex - 1;
	if (!m_bHaveOmit)
	{
		JUDGETRUE(!ClickPage(iPrevIndex),0);
		return iPrevIndex;
	}
	//1 ... [x] x+1 y-1  y ... [max]
	//当m_iCurrentIndex为x或者max时候会触发...改变,其他情况不会；
	JUDGETRUE(!ClickPage(iPrevIndex), 0);
	(m_pPageButton + iPrevIndex)->SetStatus(STATUS_OVER);
	return iPrevIndex;
}

int	CUIPage::ClickNextButton()
{
	JUDGETRUE((m_iCurrentIndex == m_iAllShowPage - 1), m_iCurrentIndex)
	JUDGETRUE(m_iCurrentIndex < 0, 0)
	JUDGETRUE(m_iCurrentIndex >= m_iAllShowPage, 0)

	int iNextIndex = m_iCurrentIndex + 1;
	if (!m_bHaveOmit)
	{
		JUDGETRUE(!ClickPage(iNextIndex), 0);
		return iNextIndex;
	}
	JUDGETRUE(!ClickPage(iNextIndex), 0);
	(m_pPageButton + iNextIndex)->SetStatus(STATUS_OVER);
	return iNextIndex;
}
bool	CUIPage::OmitChange(int index)
{
	int iValue;
	if (0 == index || 2 == index)
	{
		iValue = 3;
		m_pPageArray[1] = 2;
		m_pPageArray[m_iAllShowPage - 2] = 0;
	}
	else if (m_iAllShowPage - 1 == index || m_iAllShowPage - 3 == index)
	{
		iValue = m_iPageAmount - m_iAllShowPage + 3;
		m_pPageArray[1] = 0;
		m_pPageArray[m_iAllShowPage - 2] = m_iPageAmount-1;
	}
	for (int i = 2; i <= m_iAllShowPage - 3; i++)
	{
		m_pPageArray[i] = iValue++;
	}

	m_bAllPaint = true;

	return true;
}
bool	CUIPage::InitCurrentPage()
{
	for (int i = 0; i < m_iAllShowPage; i++)
	{
		m_pPageArray[i] = i+1;
	}

	if (m_bHaveOmit && m_iPageAmount >m_iAllShowPage)
	{
		m_pPageArray[m_iAllShowPage - 2] = 0;
	}
	else if (!m_bHaveOmit && m_iPageAmount == m_iAllShowPage)
	{
		m_pPageArray[m_iAllShowPage - 2] = m_iAllShowPage - 1;
	}
	else
	{
		JUDGETRUE(true, false);
	}
	m_pPageArray[m_iAllShowPage-1] = m_iPageAmount;
	m_iCurrentIndex = 0;
	(m_pPageButton + m_iCurrentIndex)->SetCurrentStatus(STATUS_OVER);
	return true;
}

bool	CUIPage::ClickPage(int index)
{
	JUDGETRUE(index < 0, false);
	JUDGETRUE(index >= m_iAllShowPage, false);

	if (m_iCurrentIndex != index)
	{
		m_iOldIndex = m_iCurrentIndex;
		m_iCurrentIndex = index;
		(m_pPageButton + m_iOldIndex)->SetStatus(STATUS_NORMAL);
	}
	else
	{
		return false;
	}
	
	if (!m_bHaveOmit)
		return true;
	
	if (m_pPageArray[1] == 0 && (index == 0 || index == 2))
	{// 0, 1, 2
		OmitChange(0);
	}
	else if (m_pPageArray[m_iAllShowPage - 2] == 0 && 
		(index == m_iAllShowPage - 3 || index == m_iAllShowPage - 1))
	{// m_iAllShowPage - 3, m_iAllShowPage - 2, m_iAllShowPage - 1
		OmitChange(m_iAllShowPage - 1);
	}

	return true;
}

bool	CUIPage::DrawExButton(const HDC* phdc, RECT rc, PATE_TYPE ptype)
{
	JUDGETRUE((PATE_TYPE_NEXT != ptype && PATE_TYPE_PREV!= ptype), false)
		TCHAR pText[56] = { 0 };

	RECT _rc = rc;
	HDC* _phdc = NULL;
	if (CMMUIBaseTextOut::GetTextOutmemDC(phdc, _phdc, _rc))
	{
		if (PATE_TYPE_PREV == ptype)
		{
			_stprintf_s(pText, 56, _T("<"));
			m_PrevButton.OnPaintDirect(_phdc, _rc);
		}
		else if (PATE_TYPE_NEXT == ptype)
		{
			_stprintf_s(pText, 56, _T(">"));
			m_NextButton.OnPaintDirect(_phdc, _rc);
		}
			

		CMMUIBaseTextOut::DrawTextOut(phdc, rc, pText);
	}
	else
	{
		return false;
	}

	return true;
}

bool	CUIPage::DrawPage(const HDC* phdc, RECT rc, int index)
{
	JUDGETRUE(index<0,false)
	JUDGETRUE(m_iAllShowPage <= index, false)
	TCHAR pText[56] = { 0 };
	if (0 == m_pPageArray[index])
		_stprintf_s(pText, 56, _T("..."));
	else
	{
		_stprintf_s(pText, 56, _T("%d"), m_pPageArray[index]);
	}

	RECT _rc = rc;
	HDC* _phdc = NULL;
	if (CMMUIBaseTextOut::GetTextOutmemDC(phdc, _phdc, _rc))
	{
		m_pPageButton[index].OnPaintDirect(_phdc, _rc);//index == m_iCurrentIndex
		CMMUIBaseTextOut::DrawTextOut(phdc, rc, pText);
	}
	else
	{
		return false;
	}

	return true;
}

bool    CUIPage::OnMouseMove(POINT pt)
{
	int index = -1;
	PATE_TYPE ptype = PointIndex(pt, index);
	if (PATE_TYPE_OMIT == ptype || PATE_TYPE_NULL == ptype)
	{
		return false;
	}

	if (PATE_TYPE_PREV == ptype)
	{
		m_PrevButton.OnMouseMove(pt);
	}
	else if (PATE_TYPE_NEXT == ptype)
	{
		m_NextButton.OnMouseMove(pt);
	}
	else if (PATE_TYPE_DIGIT == ptype)
	{
		(m_pPageButton+index)->OnMouseMove(pt);
	}

	return true;
}
bool    CUIPage::OnLButtonDown(POINT pt)
{
	int index = -1;
	PATE_TYPE ptype = PointIndex(pt, index);
	if (PATE_TYPE_OMIT == ptype || PATE_TYPE_NULL == ptype)
	{
		return false;
	}


	if (PATE_TYPE_PREV == ptype)
	{
		ClickPrevButton();
		m_PrevButton.OnLButtonDown(pt);
	}
	else if (PATE_TYPE_NEXT == ptype)
	{
		ClickNextButton();
		m_NextButton.OnLButtonDown(pt);
	}
	else if (PATE_TYPE_DIGIT == ptype)
	{
		ClickPage(index);
		(m_pPageButton + index)->OnLButtonDown(pt);
	}

	return true;
}
bool    CUIPage::OnLButtonUp(POINT pt)
{
	int index = -1;
	PATE_TYPE ptype = PointIndex(pt, index);
	if (PATE_TYPE_OMIT == ptype || PATE_TYPE_NULL == ptype)
	{
		return false;
	}

	if (PATE_TYPE_PREV == ptype)
	{
		m_PrevButton.OnLButtonUp(pt);
	}
	else if (PATE_TYPE_NEXT == ptype)
	{
		m_NextButton.OnLButtonUp(pt);
	}
	else if (PATE_TYPE_DIGIT == ptype)
	{
		(m_pPageButton + index)->OnLButtonUp(pt);
	}
	else
	{
		;
	}

	return true;
}

bool	CUIPage::NormalPage(POINT pt)
{
	if (m_MousePostion.pageType == PATE_TYPE_PREV)
	{
		m_PrevButton.OnMouseLeave(pt);
	}
	else if (m_MousePostion.pageType == PATE_TYPE_NEXT)
	{
		m_NextButton.OnMouseLeave(pt);
	}
	else if (m_MousePostion.pageType == PATE_TYPE_DIGIT)
	{
		JUDGETRUE((m_MousePostion.iVlue == -1 || m_MousePostion.iVlue == m_iCurrentIndex), false)
		(m_pPageButton + m_MousePostion.iVlue)->OnMouseLeave(pt);
	}
	else
	{
		m_PrevButton.OnMouseLeave(pt);
		m_NextButton.OnMouseLeave(pt);
		JUDGETRUE((m_MousePostion.iVlue == -1 || m_MousePostion.iVlue == m_iCurrentIndex), false)
		(m_pPageButton + m_MousePostion.iVlue)->OnMouseLeave(pt);
	}
	return true;
}

bool    CUIPage::OnMouseLeave(POINT pt)
{
	if (m_MousePostion.pageType == PATE_TYPE_NULL)
		return false;

	int index = -1;
	PATE_TYPE ptype = PointIndex(pt, index);
	if (PATE_TYPE_OMIT == ptype || PATE_TYPE_NULL == ptype)
	{
		return false;
	}

	if ((PATE_TYPE_OMIT == ptype || PATE_TYPE_NULL == ptype)||
		(PATE_TYPE_PREV == ptype && m_MousePostion.pageType == PATE_TYPE_PREV)||
		(PATE_TYPE_NEXT == ptype && m_MousePostion.pageType == PATE_TYPE_NEXT)||
		(PATE_TYPE_DIGIT == ptype && m_MousePostion.pageType == PATE_TYPE_DIGIT && m_MousePostion.iVlue == index))
	{
		return false;
	}

	NormalPage(pt);
	
	return true;
}
bool    CUIPage::OnMouseHover(POINT pt)
{
	int index = -1;
	PATE_TYPE ptype = PointIndex(pt, index);
	if (PATE_TYPE_OMIT == ptype || PATE_TYPE_NULL == ptype)
	{
		return false;
	}

	if (PATE_TYPE_PREV == ptype)
	{
		JUDGETRUE(m_MousePostion.pageType == PATE_TYPE_PREV, false)
		m_MousePostion.pageType = PATE_TYPE_PREV;
		m_PrevButton.OnMouseHover(pt);
	}
	else if (PATE_TYPE_NEXT == ptype)
	{
		JUDGETRUE(m_MousePostion.pageType == PATE_TYPE_NEXT, false)
		m_MousePostion.pageType = PATE_TYPE_NEXT;
		m_NextButton.OnMouseHover(pt);
	}
	else if (PATE_TYPE_DIGIT == ptype)
	{
		JUDGETRUE((m_MousePostion.pageType == PATE_TYPE_DIGIT && m_MousePostion.iVlue == index), false)
		m_MousePostion.pageType = PATE_TYPE_DIGIT;
		m_MousePostion.iVlue = index;
		(m_pPageButton + index)->OnMouseHover(pt);
	}
	else
	{
		if (m_MousePostion.pageType != PATE_TYPE_NULL)
			m_MousePostion.pageType = PATE_TYPE_NULL;
		return false;
	}
	return true;
}

PATE_TYPE		CUIPage::PointIndex(POINT pt, int& index)
{
	PATE_TYPE ptype = PATE_TYPE_NULL;
	if (m_bHaveExButton)
	{
		if (m_PrevButton.InPoint(m_PrevButton.GetRect(), pt))
			ptype = PATE_TYPE_PREV;
		else if (m_NextButton.InPoint(m_NextButton.GetRect(), pt))
			ptype = PATE_TYPE_NEXT;
	}
	if (ptype != PATE_TYPE_NULL)
		return ptype;

	for (int i = 0; i < m_iAllShowPage; i++)
	{
		if ((m_pPageButton + i)->InPoint((m_pPageButton + i)->GetRect(), pt))
		{
			index = i;
			ptype = (m_pPageArray[i] != 0) ? PATE_TYPE_DIGIT : PATE_TYPE_OMIT;
			break;
		}
	}

	return ptype;
}

PATE_TYPE		CUIPage::RectIndex(RECT rc, int& index)
{
	if ((m_pPageButton + 0)->InRECT(rc, m_rc))
	{
		return PATE_TYPE_ALL;
	}

	PATE_TYPE ptype = PATE_TYPE_NULL;
	if (m_bHaveExButton)
	{
		if (m_PrevButton.InRECT(m_PrevButton.GetRect(), rc))
			ptype = PATE_TYPE_PREV;
		else if (m_NextButton.InRECT(m_NextButton.GetRect(), rc))
			ptype = PATE_TYPE_NEXT;
	}
	if (ptype != PATE_TYPE_NULL)
		return ptype;

	for (int i = 0; i < m_iAllShowPage; i++)
	{
		if ((m_pPageButton + i)->InRECT((m_pPageButton + i)->GetRect(), rc))
		{
			index = i;
			ptype = (m_pPageArray[i] != 0) ? PATE_TYPE_DIGIT : PATE_TYPE_OMIT;
			break;
		}
	}

	return ptype;
}

bool	CUIPage::OnPaint(const HDC* phdc, RECT rc)
{
	int index = -1;
	PATE_TYPE ptype = RectIndex(rc, index);
	if (PATE_TYPE_OMIT == ptype || PATE_TYPE_NULL == ptype)
	{
		return false;
	}

	if (PATE_TYPE_ALL == ptype)
	{
		DrawExButton(phdc, m_PrevButton.GetRect(), PATE_TYPE_PREV);
		DrawExButton(phdc, m_NextButton.GetRect(), PATE_TYPE_NEXT);
		//m_PrevButton.OnPaint(phdc, rc);
		//m_NextButton.OnPaint(phdc, rc);
		for (int i = 0; i < m_iAllShowPage; i++)
		{
			DrawPage(phdc, (m_pPageButton + i)->GetRect(), i);
		}

		return true;
	}

	if (PATE_TYPE_DIGIT == ptype && !m_bAllPaint)
	{
		DrawPage(phdc, rc, index);
	}
	else if (PATE_TYPE_NEXT == ptype || PATE_TYPE_PREV == ptype)
	{
		DrawExButton(phdc, rc, ptype);
	}

	if (m_bAllPaint)
	{
		for (int i = 0; i < m_iAllShowPage; i++)
		{
			DrawPage(phdc, (m_pPageButton + i)->GetRect(), i);
		}

	}

	return true;
}
