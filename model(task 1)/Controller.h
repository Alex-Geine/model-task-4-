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
	WaveModel* mod = nullptr;
	HANDLE TREAD;
	CRITICAL_SECTION cs;
	mat4 mat; // конечная матрица преобразования

	struct ColorTable {
	public:
		//массив кисточек
		SolidBrush** brashes;
		
		//размер сетки цветов
		int Size = 11;

		//массив с пороговыми значениями
		double *mas;

		ColorTable(double max) {
			double step = max / (Size - 1);

			mas = new double[Size];
			brashes = new SolidBrush*[Size - 1];
			
			for (int i = 0; i < Size; i++) 
				mas[i] = i * step;	

			for (int i = 0; i < Size - 1; i++) {
				Color col(255 *  i / (Size - 1), 0, 255 - 255 * i / (Size - 1));
				brashes[i] = new SolidBrush(col);
			}
		}

		//возвращает указатель на кисть из нужного диапазона
		SolidBrush* GetBrush(double val) {
			for (int i = 0; i < Size - 1; i++) {
				if ((val >= mas[i]) && (val < mas[i + 1]))
					return brashes[i];				
			}
			return new SolidBrush(Color::Black);
		}
	};

	

	double	R;		//ширина ямы
	double dt;		//шаг по времени

	int N;				//количество точек по X
	int M;				//количество точек по Y
	int IdMax = 1024;	//количество отсчетов времени

	double MaxF;		//максимальное значение на графике пакета для 3д графика
	double MaxY;		//маскимальное значение по оси Y для пакета	для 3д графика

	double Maxf0;		//максимальное значение интенсивности на 2д графике

	double MaxSF;		//максимальное значение интенсивности на графике собственных функций

	double MaxFFur;		//максимальное значение на графике спектра
	double MaxE;		//максимальное значение на графике собственной функции
	double a;			//левая граница ямы
	double b;			//правая граница ямы
	double f0;			//амплитуда Гауссого купола
	double Scalefd;		//маштабирование частоты дискретизации

	complex<double>** F = NULL;				//массив значений волнового пакета
	complex<double>*** FFur = NULL;			//массив значений спектра
	vector <pair<double, int>> Energes;		//вектор со значениями собственных значений
	
	vector <pair<PointF, PointF>> Isolines[11];		//массив с изолиниями
	vector <pair<PointF, PointF>> IsolinesSF[11];	//массив с изолиниями для собственных функций

	double* X = NULL;		//вектор значений по X
	double* Y = NULL;		//вектор значений по Y
	double* f = NULL;		//вектор значений по f

	//вектор со всеми полигонами для 2d и 3d
	vector<Poligon*> polig;

	//вектор со всеми полигонами для собственных функций
	vector<Poligon*> poligSF;

	//флаг, отвечающий за создание вектора с полигонами для 2d и 3d
	bool poligReady = false;	

	//подготавливает данные для отрисовки 2d
	void PrepareData2d();

	//подготавливает данные для отрисовки 3d
	void PrepareData3d();

	//подготавливает данные для отрисовки собственных функций
	void PrepareDataSF();

	//находит изолинии
	void MakeIsolines();

	//находит изолинии на графике собственных функций
	void MakeIsolinesSF();

	//проверяет полигон для поиска изолинии
	bool CheckPoligon(double T, pair<PointF, PointF> & buf, Poligon pol);

	//очищает список
	void ClearList();

	//заполняет список
	void FillList();

	//находит максимальное значения функции фурье
	void FindMaxFFur();		

	//функция, которая работает в потоке с моделью
	DWORD WINAPI ModelFunk();

	static DWORD WINAPI StaticModelFunk(PVOID param) {
		Controller* This = (Controller*)param;
		return This->ModelFunk();
	}

	//функция, которая считает фурье
	DWORD WINAPI ModelFurie();

	static DWORD WINAPI StaticModelFurie(PVOID param) {
		Controller* This = (Controller*)param;
		return This->ModelFurie();
	}
	
public:
	CListBox* listEnerges;		//указатель на листбокс

	int drawId = 0;		//ид отсчета времени, в который рисуем пакет
	int drawIdFx = 0;	//ид спектра, который рисуем
	int drawIdFy = 0;	//ид спектра, который рисуем
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
			double asrty,	//среднее отклонение по оси Y
			double Scalefd	//маштабирование частоты дискретизации
	);
		
	//очищает данные
	void Clear();
	
	//запускает отрисовку 2d графика
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);

	//отрисовывает 3d график
	void Draw3D(LPDRAWITEMSTRUCT Item1);

	//отрисовывает спектр
	void DrawSpectrum(LPDRAWITEMSTRUCT Item1);

	//отрисовывает Собствунную функцию
	void DrawSF(LPDRAWITEMSTRUCT Item1);
	
	//запусткает вычисления
	void StartSolve();	

	Controller(){
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);
		mat.rotateY(1);
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

	//считает собственные функции по отсчетам координаты
	void GetSF(int idx, int idy);

	//функция, ждущая отсчетов для фурье
	void CheckData();

	//функция, создающая новыую модель
	void CreateModel() {
		if (mod == nullptr)
			mod = new WaveModel();
	}

	//функция, ставящая на паузу модель
	void Pause();

	//флаг, отвечающий за установку паузы
	bool pause = false;
};