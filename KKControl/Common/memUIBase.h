#pragma once
#include "BaseConfig.h"

class CMMUIBase
{
public:
	CMMUIBase(void);
	~CMMUIBase(void);

public:
	bool InPoint(RECT rc_0, POINT pt);
	bool InRECT(RECT rc_0, RECT rc_1);
	bool ContainsRect(RECT rc);
	HRESULT InitImageName( TCHAR * imgName , ITEM_STATUS ) ;
	void	OnFlushContrl(bool brf = false);

public:
	DWORD GetModulePath(DWORD nBufferLength, LPTSTR lpBuffer) ;
	void Init( HWND hwnd , RECT rc , int iAmount  ) ;
public:
	//void ChangeRect(RECT rc){ m_rc = rc; }
	RECT GetRect(){return m_rc ;} 
	bool SetCurrentStatus(ITEM_STATUS is);
	ITEM_STATUS GetCurrentStatus(){ return m_currentStatus ; } ;

public:
	virtual bool    OnMouseMove(POINT pt);
	virtual bool    OnLButtonDown(POINT pt);
	virtual bool    OnLButtonUp(POINT pt);
	virtual bool    OnMouseLeave(POINT pt);
	virtual bool    OnMouseHover(POINT pt);

	virtual bool	OnPaint(const RECT  rc);
	virtual bool	SetStatus(ITEM_STATUS is);
public:
	RECT		m_rc ;
	HWND		m_hwnd ;
	int			m_statusAmount ;
	ITEM_STATUS	m_currentStatus ;
};

