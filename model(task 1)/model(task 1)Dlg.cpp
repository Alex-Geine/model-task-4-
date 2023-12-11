
// model(task 1)Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "model(task 1).h"
#include "model(task 1)Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// Диалоговое окно Cmodeltask1Dlg



Cmodeltask1Dlg::Cmodeltask1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODELTASK_1_DIALOG, pParent)

	, n(100)
	, dt(0.001)
	, a(0)
	, b(1)
	, R(2)
	, f0(0.1)
	, U0(0.1)
	, gammax(0.5)
	, asrx(0)
	, idx(50)
	, asrty(0)
	, M(100)
	, gammay(0.5)
	, idy(50)
	, Scalefd(20)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmodeltask1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MAINGRAPH, MainGraph);

	DDX_Text(pDX, IDC_EDIT1, n);
	DDX_Text(pDX, IDC_EDIT2, dt);
	DDX_Text(pDX, IDC_EDIT3, a);
	DDX_Text(pDX, IDC_EDIT4, b);
	DDX_Text(pDX, IDC_EDIT5, R);
	DDX_Text(pDX, IDC_EDIT6, U0);
	DDX_Text(pDX, IDC_EDIT7, f0);
	DDX_Text(pDX, IDC_EDIT8, gammax);
	DDX_Control(pDX, IDC_LIST2, listModels);
	DDX_Text(pDX, IDC_EDIT9, asrx);
	DDX_Text(pDX, IDC_EDIT10, idx);
	DDX_Text(pDX, IDC_EDIT11, asrty);
	DDX_Text(pDX, IDC_EDIT13, M);
	DDX_Text(pDX, IDC_EDIT12, gammay);
	DDX_Control(pDX, IDC_3D, threedgraph);
	DDX_Text(pDX, IDC_EDIT14, idy);
	DDX_Text(pDX, IDC_EDIT15, Scalefd);
	DDX_Control(pDX, IDC_EDIT1, btn1);
	DDX_Control(pDX, IDC_EDIT13, btn2);
	DDX_Control(pDX, IDC_EDIT2, btn3);
	DDX_Control(pDX, IDC_EDIT5, btn4);
	DDX_Control(pDX, IDC_EDIT3, btn5);
	DDX_Control(pDX, IDC_EDIT4, btn6);
	DDX_Control(pDX, IDC_EDIT6, btn7);
	DDX_Control(pDX, IDC_EDIT7, btn8);
	DDX_Control(pDX, IDC_EDIT8, btn9);
	DDX_Control(pDX, IDC_EDIT12, btn10);
	DDX_Control(pDX, IDC_EDIT9, btn11);
	DDX_Control(pDX, IDC_EDIT11, btn12);
	DDX_Control(pDX, IDC_EDIT15, btn13);
	DDX_Control(pDX, IDC_EDIT10, idxBtn);
	DDX_Control(pDX, IDC_EDIT14, idyBtn);
	DDX_Control(pDX, IDC_MAINGRAPH2, Spectr);
}

BEGIN_MESSAGE_MAP(Cmodeltask1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
//	ON_BN_CLICKED(IDC_BUTTON2, &Cmodeltask1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &Cmodeltask1Dlg::OnBnClickedButton4)	
	ON_BN_CLICKED(IDC_BUTTON6, &Cmodeltask1Dlg::OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON8, &Cmodeltask1Dlg::OnBnClickedButton8)
ON_LBN_SELCHANGE(IDC_LIST2, &Cmodeltask1Dlg::OnLbnSelchangeList2)
//ON_BN_CLICKED(IDC_BUTTON2, &Cmodeltask1Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Обработчики сообщений Cmodeltask1Dlg

BOOL Cmodeltask1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	control = new Controller();

	//график с 2д картинкой
	MainGraph.GetContr(control);
	MainGraph.drawerID = 1;

	//график с собственной функцией
	threedgraph.GetContr(control);
	threedgraph.drawerID = 3;

	//график с спектром
	Spectr.GetContr(control);
	Spectr.drawerID = 2;

	control->listEnerges = &listModels;	
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void Cmodeltask1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void Cmodeltask1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR Cmodeltask1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cmodeltask1Dlg::OnEnChangeEdit2()
{
}


void Cmodeltask1Dlg::OnTimer(UINT_PTR nIDEvent)
{		
	//UpdateData();
	
	control->CheckData();	
	control->GetData();
	MainGraph.draw = 1;
	MainGraph.Invalidate(false);

	//threedgraph.draw = 1;
	//threedgraph.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}		

	CDialogEx::OnTimer(nIDEvent);
}

//окна графиков 
//void Cmodeltask1Dlg::OnBnClickedButton2()
//{
//	
//}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

//Посчитать значения
void Cmodeltask1Dlg::OnBnClickedButton4()
{
	KillTimer(timer);
	UpdateData();
		
	control->Clear();
	control->CreateModel();
	control->UpdateModel(n, M, dt, a, b, R, f0, U0, gammax, gammay, asrx, asrty, Scalefd);
	control->StartSolve();

	timer = SetTimer(1, 10, 0);

	//лочит кнопки
	BtnSwitch(true);
}

//нарисовать спектр
void Cmodeltask1Dlg::OnBnClickedButton6()
{
	if (!control->DataReady()) {
		MessageBox(L"Нет данных!", L"Ошибка!", NULL);
		return;
	}
	
	UpdateData();

	if ((idx < 0) || (idx >= n) || (idy < 0) || (idy >= M)) {
		MessageBox(L"Id вышло за предел количества точек", L"Ошибка!", NULL);
		return;
	}

	control->drawIdFx = idx;
	control->drawIdFy = idy;
	
	Spectr.draw = 1;
	Spectr.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	control->GetSF(idx, idy);
}

//Пауза
void Cmodeltask1Dlg::OnBnClickedButton8()
{	
	if (!control->pause) {
		KillTimer(timer);
		control->Pause();
		BtnSwitch(false);
	}		
	else {
		BtnSwitch(true);
		control->Pause();
		timer = SetTimer(1, 10, 0);
	}
		
}

//перерисовывает график с СФ
void Cmodeltask1Dlg::OnLbnSelchangeList2()
{
	if (!control->DataReady()) {
		MessageBox(L"Нет данных!", L"Ошибка!", NULL);
		return;
	}
	control->ShowItemList();
	
	threedgraph.draw = 1;
	threedgraph.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Cmodeltask1Dlg::BtnSwitch(bool flag) {
	/*btn1.SetReadOnly(flag);
	btn2.SetReadOnly(flag);
	btn3.SetReadOnly(flag);
	btn4.SetReadOnly(flag);
	btn5.SetReadOnly(flag);
	btn6.SetReadOnly(flag);
	btn7.SetReadOnly(flag);
	btn8.SetReadOnly(flag);
	btn9.SetReadOnly(flag);
	btn10.SetReadOnly(flag);
	btn11.SetReadOnly(flag);
	btn12.SetReadOnly(flag);
	btn13.SetReadOnly(flag);*/
}


