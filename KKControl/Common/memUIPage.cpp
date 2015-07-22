#include "stdafx.h"
#include "memUIPage.h"


CUIPage::CUIPage() :
m_hwnd(NULL),
m_iPageAmount(0), 
m_iCurrentPage(0),
m_iMaxShowPage(0), 
m_bAutoWriteDigit(FALSE), 
m_pPageButton(NULL), 
m_bHaveExButton(FALSE)
{}

CUIPage::~CUIPage()
{}

bool CUIPage::InitPage(const ParamPage* pInitpage, int& iRseAmount)
{
	JUDGENULL(pInitpage, false)
	JUDGETRUE(iRseAmount <= 0, false)

	if (iRseAmount > MAX_PAGE_NUMBER)
		iRseAmount = MAX_PAGE_NUMBER;


	if (NULL != pInitpage->pPrev && 
		NULL != pInitpage->pNext &&
		NULL != pInitpage->pOmit &&
		PATE_TYPE_PREV == pInitpage->pPrev->type &&
		PATE_TYPE_NEXT == pInitpage->pNext->type &&
		PATE_TYPE_OMIT == pInitpage->pOmit->type)
	{
		m_PrevButton.InitBtn(pInitpage->hWnd, pInitpage->pPrev->rc, pInitpage->pPrev->isi);
		m_NextButton.InitBtn(pInitpage->hWnd, pInitpage->pNext->rc, pInitpage->pNext->isi);
		m_OmitButton.InitBtn(pInitpage->hWnd, pInitpage->pOmit->rc, pInitpage->pOmit->isi);
		m_bHaveExButton = true;
	}

	m_pPageButton = new CMMUIButton[iRseAmount];
	JUDGENULL(m_pPageButton, false)
	for (short i = 0; i < iRseAmount; i++)
	{
		m_pPageButton[i].InitBtn(pInitpage->hWnd, pInitpage->ppRse[i]->rc, pInitpage->ppRse[i]->isi);
	}
		/*
		struct ParamPage
{
	PageRes*			pOmit; 
	PageRes*			pPrev;
	PageRes*			pNext;
	PageRes**			ppRse;
	unsigned short		usRseAmount;
	
	unsigned short		usPageAmount;
	unsigned short		usCurrentPage;
	unsigned short		usMaxShowPage;
	bool				bAutoWriteDigit;
	HWND				hWnd;
	POINT				ptBeginPosition;
};

		*/



	m_bAutoWriteDigit = pInitpage->bAutoWriteDigit;
	return true;
}