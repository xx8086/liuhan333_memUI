
// KKControlDlg.h : 头文件
//

#pragma once

#include "Common\memUIButton.h"
#include "Common\memUIEdit.h"
#include "loadgif\BaseGif.h"
#include "Common\memUIPage.h"

// CKKControlDlg 对话框
class CKKControlDlg : public CDialogEx
{
// 构造
public:
	CKKControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_KKCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP() ;

private:
	CMMUIButton m_btn[2] ;
	CMMUIEdit	m_edit[2] ;
	CGifContrl	m_gif[2];
	CUIPage		m_page;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void EditOperator( MSG* pMsg  );
};
