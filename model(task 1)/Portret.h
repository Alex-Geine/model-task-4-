#pragma once
#include "afxdialogex.h"
#include "Drawer.h"

// Диалоговое окно Portret

class Portret : public CDialogEx
{
	DECLARE_DYNAMIC(Portret)

public:
	Portret(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~Portret();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	Drawer PhasePor;
	MSG msg;

	//принимает и обрабатывает все сообщения
	void GetMes();
	virtual BOOL Create(UINT lpszTemplateName, CWnd* pParentWnd = NULL);
};
