#pragma once
#include <iostream>
#include <complex>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#define Pi 3.1415926535
#define  NUMBER_IS_2_POW_K(x)   ((!((x)&((x)-1)))&&((x)>1))  // x is pow(2, k), k=1,2, ...
#define  FT_DIRECT        -1    // Direct transform.
#define  FT_INVERSE        1    // Inverse transform.

#define FIRSTHALF		1
#define SECONDHALF		0


using namespace std;

class WaveModel {
private:
	
	int index = 0;		//индекс итерации
	int Id = 1023;		//текущий айди времени
	int N;		//количество точек по оси x
	int M;		//количество точек по оси y
	int IdMax = 1024;	//количество отсчетов по времени
	int SFId;
	int descrKoef = 20;		//коеффициент, корректирующий частоту дискретизации


	double
		t = 0,	//время нашей модели
		dt,		//шаг по времени
		stepX,	//шаг по пространству оси X
		stepY,	//шаг по пространству оси Y
		R,		//граница, заменяющая бесконечность в нашей модели
		a,		//граница ямы по оси X
		b,		//граница ямы по оси Y
		U0,		//высота ямы
		f0,		//амплитуда гауссого купола
		aSrx,	//среднее занчение положения частицы по оси X
		aSry,	//среднее значение пложения частицы по оси Y
		gammax, //среднеквадратическое отклонение частицы, задающее меру неопределенности ее положения по оси Х
		gammay,	//среднеквадратическое отклонение частицы, задающее меру неопределенности ее положения по оси Y
		koef = 10000;

	//KOEF
	complex<double>
		**A = NULL,
		**B = NULL,
		**C = NULL,
		**D = NULL;

	complex<double>		
		** alpha = NULL,	//значения коэффициентов альфа
		** betta = NULL;	//значения коэффициентов бетта

	//DATA
	double* X = NULL;					//значения пространственной сетки по оси X
	double* Y = NULL;					//значения пространственной стеки по оси Y
	double* f = NULL;					//значения частотной сетки
	complex<double>** Fpast = NULL;		//функция пакета в предыдущий момент времени
	complex<double>** Fnow = NULL;		//функция пакета в настоящий момент времени
	complex<double>*** FBuf = NULL;		//массив с функциями пакета по времени	
	
	vector<pair<double, int>> Energes;	//собственные энергии

	//начальная инициализация алгоритма
	void InitData();

	//начальный волновыой пакет
	double F0(double x, double y);

	//нахождение коэффициентов ABCD на первом полушаге
	void FindABCD1();

	//нахождение коэффициентов ABCD на втором полушаге
	void FindABCD2();

	//функция потенциальной энергии
	double U(double x, double y);	

	//прямой ход прогонки на первом полушаге
	void ForwardMethod1();

	//обратный ход прогонки на первом полушаге
	void BackwardMethod1();

	//прямой ход прогонки на втором полушаге
	void ForwardMethod2();

	//обратный ход прогонки на втором полушаге
	void BackwardMethod2();

	//Быстрое Фурье
	bool  FFT(int Ft_Flag, complex<double>* data);

	//нахождение всех спектров
	void FindSpectrum();

	//нахождение второй производной от функции F по X
	complex<double> DFX(int i, int j, bool flag);

	//нахождение второй производной от функции F по Y
	complex<double> DFY(int i, int j, bool flag);

	//конвертирует из complex в double
	void ConvertComplex(complex<double>* comp, double** RDat, double** IDat);

	//конвертирует из double в complex
	void ConvertDouble(complex<double>** comp, double* RDat, double* IDat);

	//находит пики в спектрограмме
	void FindPicks();

	//вторая версия поиска пиков
	void FindPicks2();

	//находит максимальное значение, начиная с некоторого id
	void FindMax(int ida, int idb, double&max, int & id);

	//находит максимальное значение, начиная с некоторого id
	bool FindMax(int ida, int idb, double& max, int& id, double min);

	//находит отрезок со следующим пиком
	void FindId(double min, int& ida, int idb);

	//копирует отсчеты функции на текущем шаге в буфер для предыдущего шага
	void CopyData();
public:
	

	//флаг, отвечающий за готовность произвести фурье
	bool DataReady = false;

	//флаг, отвечающий за начало рассчета фурье
	bool FurStart = false;

	//функция, которая добавляет в буфер новый отсчет времени
	bool PutData();

	//Отдает указатель на F()
	complex<double>** GetF();

	//Отдает указатель на FFur()
	complex<double>*** GetFFur();

	//Отдает указатель на вектор с энергиями
	vector<pair<double, int>> GetEnerges();

	//сбрасывает настройки
	void Reset();

	//отдает указатель на X
	double* GetX();

	//отдает указатель на Y
	double* GetY();

	//отдает указатель на f
	double* Getf();

	//нахождение волнового пакета в следующий момент времени
	void FindWave();

	//нахождение собственных функций частицы
	void FindFunc();

	//апдейтит параметры модели
	void Update(int N, int M, double dt, double R, double a, double b, double U0, double f0, double asrx, double asry, double gammax, double gammay);

	//находит собственные функции в конкретном 
	void FindSF(int id);
};