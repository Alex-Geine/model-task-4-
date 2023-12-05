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
	MessageBox(NULL, L"Фуре готово!", L"", NULL);

	//получаем Фурье
	FFur = mod->GetFFur();
	f = mod->Getf();
	
	return 0;
}

//забирает данные из модели
void Controller::GetData() {
	F = mod->GetF();
	//FFur = mod->GetFFur();
	X = mod->GetX();
	Y = mod->GetY();
	//f = mod->Getf();
	//Energes = mod->GetEnerges();

	//FillList();
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
	TREAD = CreateThread(NULL, NULL, StaticModelFunk, (void*)this, 0, NULL);
	MessageBoxW(NULL, L"START!", L"START!", NULL);	
}

//запускает отрисовку 2d графика
void Controller::DrawMainGr(LPDRAWITEMSTRUCT Item1) {
	//FindMaxF();
	PrepareData2d();

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

	double xScale = ItemWidth / (right + right / 15 - left);
	double yScale = -ItemHeight / (top + top / 5 - bottom);



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
	Pen pen3(Color::BlueViolet, 0.002);
	Pen pen2(Color::Green, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//Разметка
	for (int i = 0; i < 10; i++)
	{
		//горизонтальная
		gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

		//вертикальная
		gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
	}

	//Большие оси
	//горизонтальная
	gr.DrawLine(&pen, PointF(-right / 2, -top/2), PointF(right / 2, -top /2));

	//вертикальная
	gr.DrawLine(&pen, PointF(right / 2, top / 2), PointF(right / 2, -top / 2));
	
	gr.ResetTransform();

	ColorTable ct(f0);

	//подписи к осям
	for (int i = 0; i < 11; i++)
	{
		CString str;

		//по Y
		str.Format(L"%.2f", top / 2 - i * steplenY);
		PointF strPoint(right / 2, top / 2 - i * steplenY);
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		//по X
		str.Format(L"%.2f", right / 2 - i * steplenX);
		strPoint.X = right / 2 - i * steplenX;
		strPoint.Y = -top / 2;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		//для палитры
		str.Format(L"%.2f", ct.mas[i]);
		strPoint.X = -right / 2 + i * steplenX;
		strPoint.Y = -(top) / 2 - steplenY / 1.7;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);

	//Отрисовка графика


	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.001);

	

	double 
		leftkv = -right / 2,
		rightkv = leftkv  + steplenX;

	double
		ykv1 = -(top + top / 5) / 2 - steplenY,
		ykv2 = ykv1 + steplenY;

	//отрисовка цветовой палитры
	PointF* p = new PointF[4];
	for (int i = 0; i < ct.Size - 1; i++) {
		p[0] = PointF(leftkv, ykv1);
		p[1] = PointF(rightkv, ykv1);
		p[2] = PointF(rightkv, ykv2);
		p[3] = PointF(leftkv, ykv2);

		gr.FillPolygon(ct.brashes[i], p, 4);
		gr.DrawPolygon(&pen, p, 4);

		leftkv += steplenX;
		rightkv += steplenX;
	}


	//отрисовка пакета	
	for (int i = 0; i < polig.size(); i++) {
		double sum = (polig[i]->p1.z() + polig[i]->p2.z() + polig[i]->p3.z() + polig[i]->p4.z()) / 4;
		p[0] = PointF(polig[i]->p1.x(), polig[i]->p1.y());//(y1, Func(x1, y1));
		p[1] = PointF(polig[i]->p2.x(), polig[i]->p2.y());//(y2, Func(x2, y2));
		p[2] = PointF(polig[i]->p3.x(), polig[i]->p3.y());//(y3, Func(x3, y3));//
		p[3] = PointF(polig[i]->p4.x(), polig[i]->p4.y());//(y4, Func(x4, y4));//

		//	if (background)
		gr.FillPolygon(ct.GetBrush(sum), p, 4);
		gr.DrawPolygon(&pen3, p, 4);
	}	

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//подготавливает данные для отрисовки 2d
void Controller::PrepareData2d() {
	
	if (!poligReady) {
		Poligon* f;
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < M - 1; j++) {
				vec4
					p1((float)X[i], (float)Y[j], (float)abs(F[i][j])),
					p2((float)X[i + 1], (float)Y[j], (float)abs(F[i + 1][j])),
					p3((float)X[i + 1], (float)Y[j + 1], (float)abs(F[i + 1][j + 1])),
					p4((float)X[i], (float)Y[j + 1], (float)abs(F[i][j + 1]));				

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

				size++;
			}
		}
	}
}

//подготавливает данные для отрисовки 3d
void Controller::PrepareData3d() {
	MaxF = 0;
	MaxY = 0;	
		int size = 0;
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < M - 1; j++) {
				/*polig[size]->p1.Setz((float)abs(F[i][j]));
				polig[size]->p2.Setz((float)abs(F[i + 1][j]));
				polig[size]->p3.Setz((float)abs(F[i + 1][j + 1]));
				polig[size]->p4.Setz((float)abs(F[i][j + 1]));*/
				
				polig[size]->p1 = polig[size]->p1 * mat;
				polig[size]->p2 = polig[size]->p2 * mat;
				polig[size]->p3 = polig[size]->p3 * mat;
				polig[size]->p4 = polig[size]->p4 * mat;

				//находим максимальное значение по оси X
				if (MaxF < polig[size]->p1.z())
					MaxF = polig[size]->p1.z();
				if (MaxF < polig[size]->p2.z())
					MaxF = polig[size]->p2.z();
				if (MaxF < polig[size]->p3.z())
					MaxF = polig[size]->p3.z();
				if (MaxF < polig[size]->p4.z())
					MaxF = polig[size]->p4.z();

				//находим максимальное значение по оси y
				if (MaxY < polig[size]->p1.y())
					MaxY = polig[size]->p1.y();
				if (MaxY < polig[size]->p2.y())
					MaxY = polig[size]->p2.y();
				if (MaxY < polig[size]->p3.y())
					MaxY = polig[size]->p3.y();
				if (MaxY < polig[size]->p4.y())
					MaxY = polig[size]->p4.y();

				size++;				
			}
		}
}

//запускает отрисовку 3d графика
void Controller::Draw3D(LPDRAWITEMSTRUCT Item1) {	
		PrepareData3d();
		double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
			ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
		//параметры графика в мировых кооринатах
		double top = 2 * MaxF * (1 + 0.2);
		double bottom = 0;
		double left = 0;
		double right = 2 * MaxY * (1 + 0.2);

		double steplenY = (top - bottom) / 10,
			steplenX = (right - left) / 10;

		double xScale = ItemWidth / (right - left);
		double yScale = -ItemHeight / (top - bottom);



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
		Pen pen3(Color::BlueViolet, 0.001);
		Pen pen2(Color::Green, 0.006);

		Pen StickPen(Color::CornflowerBlue, 0.02);
		SolidBrush brush(Color::Black);
		

		SolidBrush PlatformBrush(Color::Crimson);
		Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);
		
		//пробуем отрисовать 3d график
		// 		////оси
		vec4
			a1(0, 0, 0),
			ay2(0, 1, 0),
			ax2(1, 0, 0),
			az2(0, 0, top / 4);		

		//отрисовка пакета
		PointF* p = new PointF[4];
		for (int i = 0; i < polig.size(); i++) {
			p[0] = PointF(polig[i]->p1.y(), polig[i]->p1.z());//(y1, Func(x1, y1));
			p[1] = PointF(polig[i]->p2.y(), polig[i]->p2.z());//(y2, Func(x2, y2));
			p[2] = PointF(polig[i]->p3.y(), polig[i]->p3.z());//(y3, Func(x3, y3));//
			p[3] = PointF(polig[i]->p4.y(), polig[i]->p4.z());//(y4, Func(x4, y4));//

		//	if (background)
			gr.FillPolygon(&PlatformBrush, p, 4);
			gr.DrawPolygon(&pen3, p, 4);
		}
		delete[] p;

		a1 = a1 * mat;
		ay2 = ay2 * mat;
		ax2 = ax2 * mat;
		az2 = az2 * mat;

		
		gr.DrawLine(new Pen(Color::White, 0.003), PointF(a1.y(), a1.z()), PointF(ay2.y(), ay2.z()));
		gr.DrawLine(new Pen(Color::White, 0.003), PointF(a1.y(), a1.z()), PointF(ax2.y(), ax2.z()));
		gr.DrawLine(new Pen(Color::White, 0.003), PointF(a1.y(), a1.z()), PointF(az2.y(), az2.z()));
		

		Graphics grnew(Item1->hDC);
		grnew.DrawImage(&Image, 0, 0);

		//polig.clear();
}

//отрисовка спектра
void Controller::DrawSpectrum(LPDRAWITEMSTRUCT Item1) {
	FindMaxFFur();
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	
	double top = MaxFFur * (1 + 0.2);
	double bottom = 0;
	double left = 0;
	double right = 1 / (dt * 20) ;

	double steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right + right / 20 - left);
	double yScale = -ItemHeight / (top + top / 10 - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); 
	matr.Translate(right / 20, -top); 


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::Black, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	
	for (int i = 0; i < 10; i++)
	{
		
		gr.DrawLine(&BackGroundPen, PointF(0, top - i * steplenY), PointF(right, top - i * steplenY));

		
		gr.DrawLine(&BackGroundPen, PointF(i * steplenX, top), PointF(i * steplenX, 0));
	}

	
	gr.DrawLine(&pen, PointF(0, 0), PointF(right, 0));

	
	gr.DrawLine(&pen, PointF(0, top), PointF(0, 0));

	gr.ResetTransform();

	
	for (int i = 0; i < 11; i++)
	{
		CString str;
		PointF strPoint;
		if (i != 10) {
			
			str.Format(L"%.2f", top - i * steplenY);
			strPoint.X = -right / 25;
			strPoint.Y = top - i * steplenY;
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
		}

		
		str.Format(L"%.2f", right - i * steplenX);
		strPoint.X = right - i * steplenX;
		strPoint.Y = 0;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);




	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.007);

	PointF p1, p2;
	for (int i = 1; i < IdMax; i++) {
		p1.X = f[i - 1];
		p1.Y = abs(FFur[i-1][drawIdFx][drawIdFy]);
		p2.X = f[i];
		p2.Y = abs(FFur[i][drawIdFx][drawIdFy]);
		gr.DrawLine(&StickPen1, p1, p2);
	}

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//очищает данные
void Controller::Clear() {
	X = nullptr;
	f = nullptr;
	F = nullptr;
	FFur = nullptr;
	Energes.clear();
	
	mod->Reset();
	ClearList();
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
	double asrty	//среднее отклонение по оси Y
	) 
{
	mod->Update(n, M, dt, R, a, b, U0, f0, asrx, asrty, gammax, gammay);

	this->R = R;
	this->N = n;
	this->dt = dt;
	this->a = a;
	this->b = b;
	this->M = M;
	this->f0 = f0;
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
//находит максимальное значение собственной функции
void Controller::FindMaxEn() {
	//double max = abs(FFur[0][drawIdE]);

	//for (int i = 1; i < N; i++) {
	//	double buf = abs(FFur[i][drawIdE]);
	//	if (max < buf)
	//		max = buf;
	//}

	//MaxE = max;
}

//отвечает, есть ли данные для отрисовки
bool Controller::DataReady() {
	if (FFur)
		return true;
	else
		return false;
}

//считает собственные функции по другому отсчеты координаты
void Controller::GetSF(int id) {
	Energes.clear();
	ClearList();

	//обращаемся к модели
	mod->FindSF(id);
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