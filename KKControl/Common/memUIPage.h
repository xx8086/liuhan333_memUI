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
	bool	OnLeaveNormal();//ȷ������뿪page��ʱ��ǵ�ǰclickҳְλnormal
	bool	OmitChange(int& index);
	bool	InitCurrentPage();//m_iCurrentIndex = 1
	bool	DrawExButton(const HDC* phdc, RECT rc, PATE_TYPE ptype);
	bool	DrawPage(const HDC* phdc, RECT rc, int index);
	bool	NormalPage(POINT pt);
	PATE_TYPE		PointIndex(POINT pt, int& index);
	PATE_TYPE		RectIndex(RECT rc, int& index);
private:
	HWND				m_hWnd;
	int					m_iPageAmount;//ҳ��
	int					m_iCurrentIndex;//����ť��
	int					m_iOldIndex;//����ť��
	int					m_iAllShowPage;//ҳ����ʡ�Ժ�һ����Ҫ��ʾ��ҳǩ
	int*				m_pPageArray;//ҳ���
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
//*m_ppPageArray[0]					��ԶΪ1
//*m_ppPageArray[1]					Ϊ2����...
//*m_ppPageArray[m_iAllShowPage-1]	��ԶΪm_iPageAmount
//*m_ppPageArray[m_iAllShowPage-2]	Ϊm_iPageAmount-1����...
