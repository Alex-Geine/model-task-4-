#pragma once

#include <gdiplus.h>
#include <vector>
#include <complex>
#include "WaveModel.h"
#include "math3d.h"

using namespace std;
using namespace Gdiplus;
using namespace math3d;

struct Poligon
{
	vec4 p1;
	vec4 p2;
	vec4 p3;
	vec4 p4;
};

class Controller {	
private:	
	ULONG_PTR token;
	WaveModel* mod;
	HANDLE TREAD;
	CRITICAL_SECTION cs;
	mat4 mat; // конечная матрица преобразования

	

	double	R;		//ширина ямы
	double dt;		//шаг по времени

	int N;				//количество точек по X
	int M;				//количество точек по Y
	int IdMax = 1024;	//количество отсчетов времени

	double MaxF;		//максимальное значение на графике пакета
	double MaxFFur;		//максимальное значение на графике спектра
	double MaxE;		//максимальное значение на графике собственной функции
	double a;			//левая граница ямы
	double b;			//правая граница ямы
	double f0;			//амплитуда Гауссого купола

	complex<double>** F = NULL;				//массив значений волнового пакета
	complex<double>** FFur = NULL;			//массив значений спектра
	vector <pair<double, int>> Energes;		//вектор со значениями собственных значений

	double* X = NULL;		//вектор значений по X
	double* Y = NULL;		//вектор значений по Y
	double* f = NULL;		//вектор значений по f


	//очищает список
	void ClearList();

	//заполняет список
	void FillList();

	//находит максимальное значения функции
	void FindMaxF();

	//находит максимальное значения функции фурье
	void FindMaxFFur();

	//находит максимальное значение собственной функции
	void FindMaxEn();	

	//функция, которая работает в потоке с моделью
	DWORD WINAPI ModelFunk();

	static DWORD WINAPI StaticModelFunk(PVOID param) {
		Controller* This = (Controller*)param;
		return This->ModelFunk();
	}
	
public:
	CListBox* listEnerges;		//указатель на листбокс

	int drawId = 0;		//ид отсчета времени, в который рисуем пакет
	int drawIdF = 0;	//ид спектра, который рисуем
	int drawIdE = 0;	//ид собственной функции, которую рисуем
		
	void UpdateModel(
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
	);
		
	//очищает данные
	void Clear();
	
	//запускает отрисовку главного графика6
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);

	//запускает отрисовку 
	void DrawPhase(LPDRAWITEMSTRUCT Item1);

	//запускает отрисовку 
	void DrawPhaseTr(LPDRAWITEMSTRUCT Item1);
	
	//запусткает вычисления
	void StartSolve();	

	Controller():mod(new WaveModel) {
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);
		mat.rotateY(2);
	}

	//деструктор
	~Controller() {
		GdiplusShutdown(token);
	}	

	//показвает текущий айтем в листе
	void ShowItemList();
	
	//забирает данные из модели
	void GetData();

	//отвечает, есть ли данные для отрисовки
	bool DataReady();

	//считает собственные функции по другому отсчеты координаты
	void GetSF(int id);
};