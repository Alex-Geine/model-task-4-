// Portret.cpp: файл реализации
//

#include "pch.h"
#include "model(task 1).h"
#include "afxdialogex.h"
#include "Portret.h"


// Диалоговое окно Portret

IMPLEMENT_DYNAMIC(Portret, CDialogEx)

Portret::Portret(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

Portret::~Portret()
{
}

void Portret::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHASE_POR, PhasePor);
}


BEGIN_MESSAGE_MAP(Portret, CDialogEx)

	ON_WM_SYSCOMMAND()

	ON_WM_PAINT()
END_MESSAGE_MAP()


// Обработчики сообщений Portret


BOOL Portret::Create(UINT lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: добавьте специализированный код или вызов базового класса

	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}

void Portret::GetMes() {
	PhasePor.draw = 1;
	PhasePor.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{    
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
