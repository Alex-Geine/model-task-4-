
// model(task 1)Dlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include "Controller.h"




// Диалоговое окно Cmodeltask1Dlg
class Cmodeltask1Dlg : public CDialogEx
{
// Создание
public:
	Cmodeltask1Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODELTASK_1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	Drawer MainGraph;
	UINT_PTR timer;
	MSG msg;
	Controller* control;

		

	afx_msg void OnEnChangeEdit2();	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();

	int n;			//количество точек по оси X
	int M;			//количество точек по оси Y
	double dt;		//шаг по времени
	double a;		//левая графница ямы
	double b;		//правая граница ямы
	double R;		//граница ямы
	double f0;		//амплитуда купола
	double U0;		//высота ямы
	double gammax;	//дисперсия ямы по оси X
	double gammay;	//дисперсия ямы по оси Y
	double asrx;	//среднее отклонение по оси X
	double asrty;	//среднее отклонение по оси Y
	int idx;		//переменная, отвечает за id по x спектра
	int idy;		//переменная, отвечает за id по y спектра

	int ID = 0;

	CListBox listModels;
	afx_msg void OnBnClickedButton6();	
	Drawer threedgraph;	
	double Scalefd;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnLbnSelchangeList2();
	void BtnSwitch(bool flag);

	CEdit btn1;
	CEdit btn2;
	CEdit btn3;
	CEdit btn4;
	CEdit btn5;
	CEdit btn6;
	CEdit btn7;
	CEdit btn8;
	CEdit btn9;
	CEdit btn10;
	CEdit btn11;
	CEdit btn12;
	CEdit btn13;
	CEdit idxBtn;
	CEdit idyBtn;
	Drawer Spectr;
//	afx_msg void OnBnClickedButton2();
};
