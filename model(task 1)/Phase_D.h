#pragma once

#include "afxdialogex.h"
#include "Drawer.h"


//#include "Controller.h"

// Диалоговое окно Phase_D

class Phase_D : public CDialogEx
{
	DECLARE_DYNAMIC(Phase_D)
	

public:
	Phase_D(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~Phase_D();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	
	virtual INT_PTR DoModal();
	virtual BOOL Create(UINT lpszTemplateName, CWnd* pParentWnd = NULL);
//	afx_msg void OnPaint();
	
	MSG msg;

	//принимает и обрабатывает все сообщения
	void GetMes();
	
	
	Drawer Phase_Gr;
	afx_msg void OnPaint();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
