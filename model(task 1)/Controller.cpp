#include "pch.h"
#include "Controller.h"
#include <algorithm>
#include <string>

using namespace std;

#define _USE_MATH_DEFINES
#include  <math.h>

//�����, ������ ���������
DWORD WINAPI Controller::ModelFunk() {
	HANDLE cur = GetCurrentThread();

	CRITICAL_SECTION Cs = cs;

	//while (1) {
		//if ((!(mod->index % 10)) && (mod->index != 0)) {
		//	SuspendThread(cur);
		//}

		mod->FindWave();
	//}

	return 0;
}

//�������� ������ �� ������
void Controller::GetData() {
	F = mod->GetF();
	//FFur = mod->GetFFur();
	X = mod->GetX();
	Y = mod->GetY();
	//f = mod->Getf();
	//Energes = mod->GetEnerges();

	//FillList();
}

//��������� ������� ����� � �����
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

//������� ������
void Controller::ClearList(){
	listEnerges->ResetContent();	
}

//��������� ������
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

//���������� ����������
void Controller::StartSolve() {
	TREAD = CreateThread(NULL, NULL, StaticModelFunk, (void*)this, 0, NULL);
	MessageBoxW(NULL, L"START!", L"START!", NULL);	
}

//��������� ��������� �������� �������
void Controller::DrawMainGr(LPDRAWITEMSTRUCT Item1) {
	FindMaxF();
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	//��������� ������� � ������� ����������
	double top = 2 * MaxF * (1 + 0.2);
	double bottom = 0;
	double left = 0;
	double right = 2 * R * (1 + 0.2);
	
	double steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right - left);
	double yScale = -ItemHeight / (top - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//�����������
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//�������������� ����(������� � ����������� ������ �������
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //��������������
	matr.Translate(right / 2, -top / 2); //������� ������ ���������


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::BlueViolet, 0.006);
	Pen pen2(Color::Green, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//��������
	for (int i = 0; i < 10; i++)
	{
		//��������������
		gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

		//������������
		gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
	}

	//������� ���
	//��������������
	gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

	//������������
	gr.DrawLine(&pen, PointF(0, top / 2), PointF(0, -top / 2));

	//������� ���
	gr.DrawLine(&pen2, PointF(a, top / 2), PointF(a, -top / 2));
	gr.DrawLine(&pen2, PointF(-a, top / 2), PointF(-a, -top / 2));

	gr.ResetTransform();

	//������� � ����
	for (int i = 0; i < 11; i++)
	{
		CString str;

		//�� Y
		str.Format(L"%.2f", top / 2 - i * steplenY);
		PointF strPoint(0, top / 2 - i * steplenY);
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		//�� X
		str.Format(L"%.2f", right / 2 - i * steplenX);
		strPoint.X = right / 2 - i * steplenX;
		strPoint.Y = 0;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);

	//��������� �������


	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.001);

	PointF p1, p2;
	for (int i = 1; i < N; i++) {
		p1.X = X[i - 1];
		p1.Y = abs(F[i - 1][drawId]);
		p2.X = X[i];
		p2.Y = abs(F[i][drawId]);
		gr.DrawLine(&StickPen1, p1, p2);
	}

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//��������� ��������� ������� � ������������ ���������
void Controller::DrawPhase(LPDRAWITEMSTRUCT Item1) {	
		//FindMaxF();
		//double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		//	ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
		////��������� ������� � ������� ����������
		//double top = 2 * f0 * (1 + 0.2);
		//double bottom = 0;
		//double left = 0;
		//double right = 2 * R * (1 + 0.2);

		//double steplenY = (top - bottom) / 10,
		//	steplenX = (right - left) / 10;

		//double xScale = ItemWidth / (right - left);
		//double yScale = -ItemHeight / (top - bottom);



		//Bitmap Image(ItemWidth, ItemHeight);
		//Graphics gr(&Image);
		//gr.Clear(Color::White);
		////�����������
		//gr.SetSmoothingMode(SmoothingModeAntiAlias);

		////�������������� ����(������� � ����������� ������ �������
		//Matrix matr;
		//matr.Scale(xScale, (yScale + 1)); //��������������
		//matr.Translate(right / 2, -top / 2); //������� ������ ���������


		//gr.SetTransform(&matr);


		//Pen BackGroundPen(Color::DarkGray, 0.005);
		//Pen pen(Color::BlueViolet, 0.006);
		//Pen pen2(Color::Green, 0.006);

		//Pen StickPen(Color::CornflowerBlue, 0.02);
		//SolidBrush brush(Color::Black);
		//SolidBrush PlatformBrush(Color::Crimson);
		//Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);
		//
		////������� ���������� 3d ������
		////rotate �������� - ��������� - ���
		////������ �� ����� ����������
		//vector<Poligon*> polig;
		//Poligon* f;

		////���� ������������ 
		//// x1, y1 - �� 
		//// x2, y2 - �� 
		//// x3, y3 - ��
		//// x4, y4 - ��
		//


		//for (int i = 0; i < N - 1; i++) {			
		//	PointF* p = new PointF[4];			
		//	for (int j = 0; j < N - 1; j++) {
		//		vec4
		//			p1((float)X[i], (float)Y[i], (float)abs(F[i][j])),
		//			p2((float)X[i + 1], (float)Y[i], (float)abs(F[i + 1][j])),
		//			p3((float)X[i + 1], (float)Y[i+ 1], (float)abs(F[i + 1][j + 1])),
		//			p4((float)X[i], (float)Y[i + 1], (float)abs(F[i][j + 1]));

		//		f = new Poligon{ p1, p2,p3,p4 };
		//		polig.push_back(f);
		//	}
		//}
		//	

		//////���
		////vec4
		////	ay1(0, 0, 0),
		////	ay2(0, 1, 0),
		////	az1(0, 0, 0),
		////	az2(0, 0, 1);

		////Axes = new Poligon{ ay1, ay2,az1,az2 };

		//
		//PointF* p = new PointF[4];
		//for (int i = 0; i < polig.size(); i++) {
		//	polig[i]->p1 = polig[i]->p1 * mat;
		//	polig[i]->p2 = polig[i]->p2 * mat;
		//	polig[i]->p3 = polig[i]->p3 * mat;
		//	polig[i]->p4 = polig[i]->p4 * mat;

		//	p[0] = PointF(polig[i]->p1.y(), polig[i]->p1.z());//(y1, Func(x1, y1));
		//	p[1] = PointF(polig[i]->p2.y(), polig[i]->p2.z());//(y2, Func(x2, y2));
		//	p[2] = PointF(polig[i]->p3.y(), polig[i]->p3.z());//(y3, Func(x3, y3));//
		//	p[3] = PointF(polig[i]->p4.y(), polig[i]->p4.z());//(y4, Func(x4, y4));//

		////	if (background)
		//	gr.FillPolygon(&brush, p, 4);
		//	gr.DrawPolygon(&pen, p, 4);
		//}
		//delete[] p;

		///*Axes->p1 = Axes->p1 * mat;
		//Axes->p2 = Axes->p2 * mat;
		//Axes->p3 = Axes->p3 * mat;
		//Axes->p4 = Axes->p4 * mat;

		//if (axes) {
		//	gr.DrawLine(new Pen(Color::BlueViolet, 0.002), PointF(Axes->p1.y(), Axes->p1.z()), PointF(Axes->p2.y(), Axes->p2.z()));
		//	gr.DrawLine(new Pen(Color::BlueViolet, 0.002), PointF(Axes->p3.y(), Axes->p3.z()), PointF(Axes->p4.y(), Axes->p4.z()));
		//}*/

		//Graphics grnew(Item1->hDC);
		//grnew.DrawImage(&Image, 0, 0);
}

//��������� ��������� ������� ������� ����������
void Controller::DrawPhaseTr(LPDRAWITEMSTRUCT Item1) {
	//FindMaxEn();
	//double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
	//	ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	////��������� ������� � ������� ����������
	//double top = 2 * MaxE * (1 + 0.2);
	//double bottom = 0;
	//double left = 0;
	//double right = 2 * R * (1 + 0.2);

	//double steplenY = (top - bottom) / 10,
	//	steplenX = (right - left) / 10;

	//double xScale = ItemWidth / (right - left);
	//double yScale = -ItemHeight / (top - bottom);



	//Bitmap Image(ItemWidth, ItemHeight);
	//Graphics gr(&Image);
	//gr.Clear(Color::White);
	////�����������
	//gr.SetSmoothingMode(SmoothingModeAntiAlias);

	////�������������� ����(������� � ����������� ������ �������
	//Matrix matr;
	//matr.Scale(xScale, (yScale + 1)); //��������������
	//matr.Translate(right / 2, -top / 2); //������� ������ ���������


	//gr.SetTransform(&matr);


	//Pen BackGroundPen(Color::DarkGray, 0.005);
	//Pen pen(Color::BlueViolet, 0.006);

	//Pen StickPen(Color::CornflowerBlue, 0.02);
	//SolidBrush brush(Color::Black);
	//SolidBrush PlatformBrush(Color::Crimson);
	//Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	////��������
	//for (int i = 0; i < 10; i++)
	//{
	//	//��������������
	//	gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

	//	//������������
	//	gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
	//}

	////������� ���
	////��������������
	//gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

	////������������
	//gr.DrawLine(&pen, PointF(0, top / 2), PointF(0, -top / 2));

	//gr.ResetTransform();

	////������� � ����
	//for (int i = 0; i < 11; i++)
	//{
	//	CString str;

	//	//�� Y
	//	str.Format(L"%.2f", top / 2 - i * steplenY);
	//	PointF strPoint(0, top / 2 - i * steplenY);
	//	matr.TransformPoints(&strPoint);
	//	gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

	//	//�� X
	//	str.Format(L"%.2f", right / 2 - i * steplenX);
	//	strPoint.X = right / 2 - i * steplenX;
	//	strPoint.Y = 0;
	//	matr.TransformPoints(&strPoint);
	//	gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	//}

	//gr.SetTransform(&matr);

	////��������� �������


	//Color color(Color::IndianRed);
	//Pen StickPen1(color, 0.007);

	//PointF p1, p2;
	//for (int i = 1; i < N; i++) {
	//	p1.X = X[i - 1];
	//	p1.Y = abs(FFur[i - 1][drawIdE]);
	//	p2.X = X[i];
	//	p2.Y = abs(FFur[i][drawIdE]);
	//	gr.DrawLine(&StickPen1, p1, p2);
	//}

	//Graphics grnew(Item1->hDC);
	//grnew.DrawImage(&Image, 0, 0);
}

//������� ������
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
	int n,			//���������� ����� �� ��� X
	int M,			//���������� ����� �� ��� Y
	double dt,		//��� �� �������
	double a,		//������� ��� �� X
	double b,		//������� ��� �� Y
	double R,		//�������������
	double f0,		//��������� ������
	double U0,		//������ ���
	double gammax,	//��������� ��� �� ��� X
	double gammay,	//��������� ��� �� ��� Y
	double asrx,	//������� ���������� �� ��� X
	double asrty	//������� ���������� �� ��� Y
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

//������� �����������/������������ �������� �������
void Controller::FindMaxF() {
	double max = abs(F[0][drawId]);

	for (int i = 1; i < N; i++) {
		double buf = abs(F[i][drawId]);
		if (max < buf)
			max = buf;
	}
		
	MaxF = max;
}

//������� ������������ �������� ������� �����
void Controller::FindMaxFFur() {
	double max = abs(FFur[drawIdF][0]);

	for (int i = 1; i < IdMax; i++) {
		double buf = abs(FFur[drawIdF][i]);
		if (max < buf)
			max = buf;
	}

	MaxFFur = max;
}
//������� ������������ �������� ����������� �������
void Controller::FindMaxEn() {
	double max = abs(FFur[0][drawIdE]);

	for (int i = 1; i < N; i++) {
		double buf = abs(FFur[i][drawIdE]);
		if (max < buf)
			max = buf;
	}

	MaxE = max;
}

//��������, ���� �� ������ ��� ���������
bool Controller::DataReady() {
	if (X)
		return true;
	else
		return false;
}

//������� ����������� ������� �� ������� ������� ����������
void Controller::GetSF(int id) {
	Energes.clear();
	ClearList();

	//���������� � ������
	mod->FindSF(id);
	Energes = mod->GetEnerges();

	FillList();
}