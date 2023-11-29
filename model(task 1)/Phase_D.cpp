// Phase_D.cpp: файл реализации
//

#include "pch.h"
#include "model(task 1).h"
#include "afxdialogex.h"
#include "Phase_D.h"


// Диалоговое окно Phase_D

IMPLEMENT_DYNAMIC(Phase_D, CDialogEx)

Phase_D::Phase_D(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	
}

Phase_D::~Phase_D()
{
}

void Phase_D::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);



	DDX_Control(pDX, IDC_PHASE_GR, Phase_Gr);
}


 BEGIN_MESSAGE_MAP(Phase_D, CDialogEx)
	
	ON_WM_SYSCOMMAND()
	
	ON_WM_PAINT()
//	 ON_WM_KEYDOWN()
 END_MESSAGE_MAP()


// Обработчики сообщений Phase_D


INT_PTR Phase_D::DoModal()
{
	
	// TODO: добавьте специализированный код или вызов базового класса

	return CDialogEx::DoModal();
}


BOOL Phase_D::Create(UINT lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: добавьте специализированный код или вызов базового класса	

	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}




void Phase_D::GetMes() {
	Phase_Gr.draw = 1;
	Phase_Gr.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Phase_D::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDialogEx::OnPaint();
	// TODO: добавьте свой код обработчика сообщений
	// Не вызывать CDialogEx::OnPaint() для сообщений рисования
}

