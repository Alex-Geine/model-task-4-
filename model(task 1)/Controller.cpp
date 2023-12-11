#include "pch.h"
#include "Controller.h"
#include <algorithm>
#include <string>

using namespace std;

#define _USE_MATH_DEFINES
#include  <math.h>

//поток, ждущий сообщений
DWORD WINAPI Controller::ModelFunk() {
	mod->FindWave();
	return 0;
}

//функция, которая считает фурье
DWORD WINAPI Controller::ModelFurie() {
	mod->FindFunc();
	MessageBox(NULL, L"Фуре посчитано", L"", NULL);

	//получаем Фурье
	FFur = mod->GetFFur();
	f = mod->Getf();
	
	return 0;
}

//забирает данные из модели
void Controller::GetData() {
	F = mod->GetF();	
	X = mod->GetX();
	Y = mod->GetY();	
}

//показвает текущий айтем в листе
void Controller::ShowItemList() {
	int cursel = listEnerges->GetCurSel();
	if (cursel != LB_ERR) {
		CString str;
		listEnerges->GetText(cursel, str);
		double key = _wtof_l(str, 0);
		for (int i = 0; i < Energes.size(); i++) {
			if (round(Energes[i].first * 1000000) / 1000000 == key) {
				drawIdE = Energes[i].second;
				return;
			}
		}		
	}
}

//очищает список
void Controller::ClearList(){
	listEnerges->ResetContent();	
}

//заполняет список
void Controller::FillList(){
	for (int i = 0; i < Energes.size(); i++) {
		const int size = 100;
	
		string st = to_string(Energes[i].first);
		const char* stBuf = st.c_str();   // 1. string to const char *

		size_t sz;                          // save converted string's length + 1
		wchar_t output[size] = L"";          // return data, result is CString data

		mbstowcs_s(&sz, output, size, stBuf, size); // converting function

		CString cst = output;
		listEnerges->AddString((LPCTSTR)cst);		
	}
}

//запусткает вычисления
void Controller::StartSolve() {
	mod->InitData();
	TREAD = CreateThread(NULL, NULL, StaticModelFunk, (void*)this, 0, NULL);	
}

//запускает отрисовку 2d графика
void Controller::DrawMainGr(LPDRAWITEMSTRUCT Item1) {
	PrepareData2d();
	MakeIsolines();

	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;

	//параметры графика в мировых кооринатах
	double top = 2 * R;
	double bottom = 0;
	double left = 0;
	double right = 2 * R;
	
	double 
		steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right  - left);
	double yScale = -ItemHeight / (top  - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//сглаживание
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//преобразование мира(переход к координатам самого графика
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //маштабирование
	matr.Translate(right / 2, -top / 2); //перенос начала координат


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::BlueViolet, 0.006);
	Pen pen3(Color::Blue, 0.002);
	Pen pen4(Color::White, 0.002);
	Pen pen2(Color::Green, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);

	gr.ResetTransform();	

	PointF strPoint;

	gr.SetTransform(&matr);

	//Отрисовка графика
	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.001);	
	PointF* p = new PointF[4];
	//отрисовка пакета	
	for (int i = 0; i < polig.size(); i++) {
		double sum = (polig[i]->p1.z() + polig[i]->p2.z() + polig[i]->p3.z() + polig[i]->p4.z()) / 4;
		p[0] = PointF(polig[i]->p1.x(), polig[i]->p1.y());//(y1, Func(x1, y1));
		p[1] = PointF(polig[i]->p2.x(), polig[i]->p2.y());//(y2, Func(x2, y2));
		p[2] = PointF(polig[i]->p3.x(), polig[i]->p3.y());//(y3, Func(x3, y3));//
		p[3] = PointF(polig[i]->p4.x(), polig[i]->p4.y());//(y4, Func(x4, y4));//

		//	if (background)
		gr.FillPolygon(&brush, p, 4);
		gr.DrawPolygon(&pen3, p, 4);
	}

	//отрисовка изолиний
	for (int i = 0; i < 11; i++) 
		for (int j = 0; j < Isolines[i].size(); j++)
			gr.DrawLine(&pen4, Isolines[i][j].first, Isolines[i][j].second);
	

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//подготавливает данные для отрисовки 2d
void Controller::PrepareData2d() {
	Maxf0 = 0;
	if (!poligReady) {
		Poligon* f;
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < M - 1; j++) {
				vec4
					p1((float)X[i], (float)Y[j], (float)abs(F[i][j])),
					p2((float)X[i + 1], (float)Y[j], (float)abs(F[i + 1][j])),
					p3((float)X[i + 1], (float)Y[j + 1], (float)abs(F[i + 1][j + 1])),
					p4((float)X[i], (float)Y[j + 1], (float)abs(F[i][j + 1]));

				//находим максимальное значение по оси z
				if (Maxf0 < p1.z())
					Maxf0 = p1.z();
				if (Maxf0 < p2.z())
					Maxf0 = p2.z();
				if (Maxf0 < p3.z())
					Maxf0 = p3.z();
				if (Maxf0 < p4.z())
					Maxf0 = p4.z();

				f = new Poligon{ p1, p2,p3,p4 };
				polig.push_back(f);
			}
		}
		poligReady = true;
	}
	else {
		int size = 0;
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < M - 1; j++) {
				polig[size]->p1.Setz((float)abs(F[i][j]));
				polig[size]->p2.Setz((float)abs(F[i + 1][j]));
				polig[size]->p3.Setz((float)abs(F[i + 1][j + 1]));
				polig[size]->p4.Setz((float)abs(F[i][j + 1]));	

				polig[size]->p1.Setx((float)X[i]);
				polig[size]->p2.Setx((float)X[i + 1]);
				polig[size]->p3.Setx((float)X[i + 1]);
				polig[size]->p4.Setx((float)X[i]);

				polig[size]->p1.Sety((float)Y[j]);
				polig[size]->p2.Sety((float)Y[j]);
				polig[size]->p3.Sety((float)Y[j+ 1]);
				polig[size]->p4.Sety((float)Y[j + 1]);

				//находим максимальное значение по оси z
				if (Maxf0 < polig[size]->p1.z())
					Maxf0 = polig[size]->p1.z();
				if (Maxf0 < polig[size]->p2.z())
					Maxf0 = polig[size]->p2.z();
				if (Maxf0 < polig[size]->p3.z())
					Maxf0 = polig[size]->p3.z();
				if (Maxf0 < polig[size]->p4.z())
					Maxf0 = polig[size]->p4.z();

				size++;
			}
		}
	}
}

//подготавливает данные для отрисовки собственных функций
void Controller::PrepareDataSF() {
	MaxSF = 0;
	poligSF.clear();

	Poligon* f;
	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < M - 1; j++) {
			vec4
				p1((float)X[i], (float)Y[j], (float)abs(FFur[drawIdE][i][j])),
				p2((float)X[i + 1], (float)Y[j], (float)abs(FFur[drawIdE][i + 1][j])),
				p3((float)X[i + 1], (float)Y[j + 1], (float)abs(FFur[drawIdE][i + 1][j + 1])),
				p4((float)X[i], (float)Y[j + 1], (float)abs(FFur[drawIdE][i][j + 1]));

			//находим максимальное значение по оси z
			if (MaxSF < p1.z())
				MaxSF = p1.z();
			if (MaxSF < p2.z())
				MaxSF = p2.z();
			if (MaxSF < p3.z())
				MaxSF = p3.z();
			if (MaxSF < p4.z())
				MaxSF = p4.z();

			f = new Poligon{ p1, p2,p3,p4 };
			poligSF.push_back(f);
		}
	}	
	
}

//находит изолинии (работает только после PrepareData)
void Controller::MakeIsolines() {
	//буфер для возвращаемого значения
	pair<PointF, PointF> buf;

	//уровни для изотерм
	ColorTable ct(Maxf0);

	for (int i = 0; i < 11; i++) {
		//отчистка предыдущих значений
		Isolines[i].clear();	

		//перебор всех полигонов
		for (int j = 0; j < polig.size(); j++) 
			if (CheckPoligon(ct.mas[i], buf, *polig[j]))
				Isolines[i].push_back(buf);		
	}
}

//находит изолинии на графике собственных функций (работает только после PrepareData)
void Controller::MakeIsolinesSF() {
	//буфер для возвращаемого значения
	pair<PointF, PointF> buf;

	//уровни для изотерм
	ColorTable ct(MaxSF);

	for (int i = 0; i < 11; i++) {
		//отчистка предыдущих значений
		IsolinesSF[i].clear();

		//перебор всех полигонов
		for (int j = 0; j < poligSF.size(); j++)
			if (CheckPoligon(ct.mas[i], buf, *poligSF[j]))
				IsolinesSF[i].push_back(buf);
	}
}

//проверяет полигон для поиска изолинии
bool Controller::CheckPoligon(double T, pair<PointF, PointF>& buf, Poligon pol) {
	bool res = false;

	double maxX, maxY, minX, minY, maxZ, minZ;

	double x[4] = { pol.p1.x(), pol.p2.x(), pol.p3.x(), pol.p4.x() };
	double y[4] = { pol.p1.y(), pol.p2.y(), pol.p3.y(), pol.p4.y() };
	double z[4] = { pol.p1.z(), pol.p2.z(), pol.p3.z(), pol.p4.z() };
	
	PointF p;

	//проверка 4х граней полигона
	for (int i = 0; i < 4; i++) {
		int idnext = i + 1;
		if (i == 3)
			idnext = 0;

		if (((z[i] < T) && (z[idnext] > T) || ((z[i] > T) && (z[idnext] < T)))) {
			
			//находим максимальное и минимальное значение по z
			if (z[i] > z[idnext]) {
				maxX = x[i];
				maxY = y[i];
				maxZ = z[i];
				minX = x[idnext];
				minY = y[idnext];
				minZ = z[idnext];
			}
			else {
				minX = x[i];
				minY = y[i];
				minZ = z[i];
				maxX = x[idnext];
				maxY = y[idnext];
				maxZ = z[idnext];
			}

			//находим значения пересечения по x и по y
			p.X = maxX - (maxX - minX) * (maxZ - T) / (maxZ - minZ);
			p.Y = maxY - (maxY - minY) * (maxZ - T) / (maxZ - minZ);
			
			if (!res)
				buf.first = p;
			else
				buf.second = p;

			res = true;
		}
	}


	return res;
}

//отрисовка спектра
void Controller::DrawSpectrum(LPDRAWITEMSTRUCT Item1) {
	FindMaxFFur();
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	//параметры графика в мировых кооринатах
	double top = MaxFFur * (1 + 0.2);
	double bottom = 0;
	double left = 0;
	double right = 1 / (dt * Scalefd);

	double steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right + right / 7 - left);
	double yScale = -ItemHeight / (top + top / 20 - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//сглаживание
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//преобразование мира(переход к координатам самого графика
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //маштабирование
	matr.Translate(right / 15, -top); //перенос начала координат


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::Black, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//Разметка
	for (int i = 0; i < 10; i++)
	{
		//горизонтальная
		gr.DrawLine(&BackGroundPen, PointF(0, top - i * steplenY), PointF(right, top - i * steplenY));

		//вертикальная
		gr.DrawLine(&BackGroundPen, PointF(i * steplenX, top), PointF(i * steplenX, 0));
	}

	//Большие оси
	//горизонтальная
	gr.DrawLine(&pen, PointF(0, 0), PointF(right, 0));

	//вертикальная
	gr.DrawLine(&pen, PointF(0, top), PointF(0, 0));

	gr.ResetTransform();

	//подписи к осям
	for (int i = 0; i < 11; i++)
	{
		CString str;
		PointF strPoint;
		if (i != 10) {
			//по Y
			str.Format(L"%.2f", top - i * steplenY);
			strPoint.X = -right / 15;
			strPoint.Y = top - i * steplenY;
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
		}

		//по X
		str.Format(L"%.2f", right - i * steplenX);
		strPoint.X = right - i * steplenX;
		strPoint.Y = 0;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);

	//Отрисовка графика


	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.007);

	PointF p1, p2;
	for (int i = 1; i < IdMax; i++) {
		p1.X = f[i - 1];
		p1.Y = abs(FFur[i - 1][drawIdFx][drawIdFy]);
		p2.X = f[i];
		p2.Y = abs(FFur[i][drawIdFx][drawIdFy]);
		gr.DrawLine(&StickPen1, p1, p2);
	}
	
	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//отрисовывает Собствунную функцию
void Controller::DrawSF(LPDRAWITEMSTRUCT Item1) {
	PrepareDataSF();
	MakeIsolinesSF();

	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;

	//параметры графика в мировых кооринатах
	double top = 2 * R;
	double bottom = 0;
	double left = 0;
	double right = 2 * R;

	double
		steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right  - left);
	double yScale = -ItemHeight / (top  - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//сглаживание
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//преобразование мира(переход к координатам самого графика
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //маштабирование
	matr.Translate(right / 2, -top / 2); //перенос начала координат


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::BlueViolet, 0.006);
	Pen pen3(Color::Blue, 0.002);
	Pen pen4(Color::White, 0.002);
	Pen pen2(Color::Green, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);

	gr.ResetTransform();

	PointF strPoint;
	
	gr.SetTransform(&matr);

	//Отрисовка графика

	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.001);
		
	PointF* p = new PointF[4];
	
	//отрисовка пакета	
	for (int i = 0; i < poligSF.size(); i++) {
		double sum = (poligSF[i]->p1.z() + poligSF[i]->p2.z() + poligSF[i]->p3.z() + poligSF[i]->p4.z()) / 4;
		p[0] = PointF(poligSF[i]->p1.x(), poligSF[i]->p1.y());//(y1, Func(x1, y1));
		p[1] = PointF(poligSF[i]->p2.x(), poligSF[i]->p2.y());//(y2, Func(x2, y2));
		p[2] = PointF(poligSF[i]->p3.x(), poligSF[i]->p3.y());//(y3, Func(x3, y3));//
		p[3] = PointF(poligSF[i]->p4.x(), poligSF[i]->p4.y());//(y4, Func(x4, y4));//

		//	if (background)
		gr.FillPolygon(&brush, p, 4);
		gr.DrawPolygon(&pen3, p, 4);
	}

	//отрисовка изолиний
	for (int i = 0; i < 11; i++)
		for (int j = 0; j < IsolinesSF[i].size(); j++)
			gr.DrawLine(&pen4, IsolinesSF[i][j].first, IsolinesSF[i][j].second);

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//очищает данные
void Controller::Clear() {
	if (mod != nullptr) {
		//уничтожение потока
		TerminateThread(TREAD, 0);
		TREAD = NULL;

		X = nullptr;
		Y = nullptr;
		f = nullptr;
		F = nullptr;
		FFur = nullptr;
		poligReady = false;

		drawId = 0;		//ид отсчета времени, в который рисуем пакет
		drawIdFx = 0;	//ид спектра, который рисуем
		drawIdFy = 0;	//ид спектра, который рисуем
		drawIdE = 0;	//ид собственной функции, которую рисуем

		Energes.clear();

		polig.clear();
		poligSF.clear();

		//отчистка данных
		mod->Reset();	

		//удвление модели
		delete mod;

		mod = nullptr;

		ClearList();
	}	
}

void Controller::UpdateModel(
	int n,			//количество точек по оси X
	int M,			//количество точек по оси Y
	double dt,		//шаг по времени
	double a,		//граница ямы по X
	double b,		//граница ямы по Y
	double R,		//бесконечность
	double f0,		//амплитуда купола
	double U0,		//высота ямы
	double gammax,	//дисперсия ямы по оси X
	double gammay,	//дисперсия ямы по оси Y
	double asrx,	//среднее отклонение по оси X
	double asrty,	//среднее отклонение по оси Y
	double Scalefd	//маштабирование частоты дискретизации
	) 
{
	mod->Update(n, M, dt, R, a, b, U0, f0, asrx, asrty, gammax, gammay, Scalefd);

	this->R = R;
	this->N = n;
	this->dt = dt;
	this->a = a;
	this->b = b;
	this->M = M;
	this->f0 = f0;
	this->Scalefd = Scalefd;
}


//находит максимальное значения функции фурье
void Controller::FindMaxFFur() {
	double max = abs(FFur[0][drawIdFx][drawIdFy]);

	for (int i = 1; i < IdMax; i++) {
		double buf = abs(FFur[i][drawIdFx][drawIdFy]);
		if (max < buf)
			max = buf;
	}

	MaxFFur = max;
}

//отвечает, есть ли данные для отрисовки
bool Controller::DataReady() {
	if (FFur)
		return true;
	else
		return false;
}

//считает собственные функции по другому отсчеты координаты
void Controller::GetSF(int idx, int idy) {
	Energes.clear();
	ClearList();

	//обращаемся к модели
	mod->FindSF(idx, idy);
	Energes = mod->GetEnerges();

	FillList();
}

//функция, ждущая отсчетов для фурье
void Controller::CheckData() {
	if (mod->DataReady && !mod->FurStart) {
		mod->FurStart = true;
		CreateThread(NULL, NULL, StaticModelFurie, (void*)this, 0, NULL);
	}		
}

//функция, ставящая на паузу модель
void Controller::Pause() {
	if ((!pause) && (mod != nullptr)) {
		SuspendThread(TREAD);
		pause = true;

	}
	else if ((pause) && (mod != nullptr)) {
		ResumeThread(TREAD);
		pause = false;
	}	
}