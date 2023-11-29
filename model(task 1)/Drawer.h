#pragma once
#include <afxwin.h>
#include<gdiplus.h>
#include "Controller.h"

using namespace Gdiplus;

// Drawer

class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)
	friend class Controller;
public:
	Drawer();
	virtual ~Drawer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	
	int drawerID;

	Controller* control;
	bool draw = false;
	void GetContr(Controller* control);

	double
		f,
		df,
		top,
		bottom,
		left,
		right,
		xPadding,
		yPadding,
		steplenY,
		steplenX,
		ItemWidth,
		ItemHeight,
		xScale,
		yScale;
	double
		l, //Длинна маятника
		a, //Амплитуда подвеса
		w, //частота подвеса
		f0, //начальное отклонение
		df0, //начальная угловая скорость
		t, //время
		h = 0.01, //шаг интегрирования
		T0, //коэффициент времени
		Y0; //коэффициент координаты
//	afx_msg void OnPaint();
};
