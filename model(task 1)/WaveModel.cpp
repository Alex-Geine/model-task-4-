#include "pch.h"
#include "WaveModel.h"

//нахождение волнового пакета в следующий момент времени
void WaveModel::FindWave() {
	InitData();
	
	for (;;) {
		index++;

		//первый полушаг
		FindABCD1();
		ForwardMethod1();
		BackwardMethod1();

		//второй полушаг
		FindABCD2();
		ForwardMethod2();
		BackwardMethod2();

		//копирование данных
		CopyData();
	}
	
}

//начальная инициализация алгоритма
void WaveModel::InitData() {

	//создание массивов
	X = new double[N];
	Y = new double[M];
	//f = new double[IdMax];
	Fpast = new complex<double>* [N];
	Fnow = new complex<double>*[N];
	//FFur = new complex<double>* [N];
	
	int koefSize = N;
	

	A = new complex<double>*[N - 1];	
	B = new complex<double>*[N - 1];
	C = new complex<double>*[N - 1];
	D = new complex<double>*[N - 1];

	alpha = new complex<double>*[N - 1];
	betta = new complex<double>*[N - 1];

	for (int i = 0; i < N - 1; i++) {
		A[i] = new complex<double>[M - 1];
		B[i] = new complex<double>[M - 1];
		C[i] = new complex<double>[M - 1];
		D[i] = new complex<double>[M - 1];

		alpha[i] = new complex<double>[M - 1];
		betta[i] = new complex<double>[M - 1];
	}

	//инициализация простраственной стеки
	stepX = 2 * R / N;
	stepY = 2 * R / M;
	double
		x0 = -R;

	for (int i = 0; i < N; i++) 
		X[i] = x0 + i * stepX;
	 
	for (int i = 0; i < M; i++)
		Y[i] = x0 + i * stepY;

	//инициализация частотной сетки
	//double stepF = 1 / (dt * IdMax);

	//for (int i = 0; i < IdMax; i++)
	//	f[i] = i * stepF;	

	//отсчеты пакета
	for (int i = 0; i < N; i++) {
		Fpast[i] = new complex<double>[M];
		Fnow[i] = new complex<double>[M];

		for (int j = 0; j < M; j++) {
			//волновой пакет в начальный момент времени
			
			complex<double> buf(F0(X[i], Y[j]), 0.);
			Fpast[i][j] = buf;
			Fnow[i][j] = { 0,0 };
		}
	}
}

//копирует отсчеты функции на текущем шаге в буфер для предыдущего шага
void WaveModel::CopyData() {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			Fpast[i][j] = Fnow[i][j];
}

//начальный волновыой пакет
double WaveModel::F0(double x, double y) {
	//функция на границах
	if ((x == -R) || (x == R) || (y == R) || (y == -R))
		return 0;
	else
		return (f0 * exp(-( (x - aSrx) * (x - aSrx) / (4 * gammax * gammax) + (y - aSry) * (y - aSry) / (4 * gammay * gammay) )));
}

//функция потенциальной энергии
double WaveModel::U(double x, double y) {
	if ( ((x <= a) && (x >= -a)) || ((y <= b) && (y >= -b)))
		return 0;
	else
		return U0;
}

//нахождение второй производной от функции F по X	(flag - производная считается на первом полушаге(true), или на втором(false))
complex<double> WaveModel::DFX(int i, int j, bool flag) {
	if (flag)
		return (Fpast[i + 1][j] + Fpast[i - 1][j] - 2. * Fpast[i][j]) / (stepX * stepX);
	else
		return (Fnow[i + 1][j] + Fnow[i - 1][j] - 2. * Fnow[i][j]) / (stepX * stepX);
}

//нахождение второй производной от функции F по Y	(flag - производная считается на первом полушаге(true), или на втором(false))
complex<double> WaveModel::DFY(int i, int j, bool flag) {
	if (flag)
		return (Fpast[i][j + 1] + Fpast[i][j - 1] - 2. * Fpast[i][j]) / (stepY * stepY);
	else
		return (Fnow[i][j + 1] + Fnow[i][j - 1] - 2. * Fnow[i][j]) / (stepY * stepY);
}

//нахождение коэффициентов ABCD на первом полушаге
void WaveModel::FindABCD1() {	
	for (int j = 1; j < M - 1; j++) {
		for (int i = 1; i < N - 1; i++) {
			complex<double> tau = { 0, dt / 2 };
			A[i - 1][j - 1] = - tau / (2 * stepX * stepX);
			B[i - 1][j - 1] = - tau / (2 * stepX * stepX);
			C[i - 1][j - 1] = 1. + tau * U(X[i], Y[j]) / 2. + tau / (stepX * stepX) ;
			D[i - 1][j - 1] = Fpast[i][j] + tau / 2. * (DFX(i,j, FIRSTHALF) + 2. * DFY(i,j,FIRSTHALF) - U(X[i], Y[j]) * Fpast[i][j]);
		}
	}	
}

//нахождение коэффициентов ABCD на втором полушаге
void WaveModel::FindABCD2() {
	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) {
			complex<double> tau = { 0, dt / 2 };
			A[i - 1][j - 1] = -tau / (2 * stepY * stepY);
			B[i - 1][j - 1] = -tau / (2 * stepY * stepY);
			C[i - 1][j - 1] = 1. + tau * U(X[i], Y[j]) / 2. + tau / (stepY * stepY);
			D[i - 1][j - 1] = Fnow[i][j] + tau / 2. * ( 2. * DFX(i, j, SECONDHALF) + DFY(i, j, SECONDHALF) - U(X[i], Y[j]) * Fnow[i][j]);
		}
	}
}

//прямой ход прогонки на первом полушаге
void WaveModel::ForwardMethod1() {
	for (int j = 1; j < M - 1; j++) {
		alpha[0][j - 1] = { 0,0 };
		betta[0][j - 1] = { 0,0 };
		for (int i = 1; i < N - 1; i++) {
			alpha[i][j - 1] = -1. * (B[i - 1][j - 1]) / (C[i - 1][j - 1] + A[i - 1][j - 1] * alpha[i - 1][j - 1]);
			betta[i][j - 1] = (D[i - 1][j - 1] - A[i - 1][j - 1] * betta[i - 1][j - 1]) / (C[i - 1][j - 1] + A[i - 1][j - 1] * alpha[i - 1][j - 1]);
		}
	}	
}

//прямой ход прогонки на втором полушаге
void WaveModel::ForwardMethod2() {
	for (int i = 1; i < N - 1; i++) {
		alpha[i - 1][0] = { 0,0 };
		betta[i - 1][0] = { 0,0 };
		for (int j = 1; j < M - 1; j++) {
			alpha[i - 1][j] = -1. * (B[i - 1][j - 1]) / (C[i - 1][j - 1] + A[i - 1][j - 1] * alpha[i - 1][j - 1]);
			betta[i - 1][j] = (D[i - 1][j - 1] - A[i - 1][j - 1] * betta[i - 1][j - 1]) / (C[i - 1][j - 1] + A[i - 1][j - 1] * alpha[i - 1][j - 1]);
		}
	}
}

//обратный ход прогонки на первом полушаге
void WaveModel::BackwardMethod1() {
	for (int j = 1; j < M - 1; j++) {
		for (int i = N - 2; i > 0; i--)
			Fnow[i][j] = alpha[i][j - 1] * Fnow[i + 1][j] + betta[i][j - 1];
	}	
}

//обратный ход прогонки на втором полушаге
void WaveModel::BackwardMethod2() {
	for (int i = 1; i < N - 1; i++) {
		for (int j = M - 2; j > 0; j--)
			Fnow[i][j] = alpha[i - 1][j] * Fnow[i][j + 1] + betta[i - 1][j];
	}
}

//апдейтит параметры модели
void WaveModel::Update(int N, int M,  double dt, double R, double a, double b, double U0, double f0, double asrx, double asry, double gammax, double gammay) {
	this->N = N;
	this->M = M;
	this->dt = dt;
	this->R = R;
	this->a = a;
	this->b = b;
	this->U0 = U0;
	this->f0 = f0;
	this->aSrx = asrx;
	this->aSry = asry;
	this->gammax = gammax;
	this->gammay = gammay;
	//this->SFId = N / 2;
}

//Быстрое Фурье
bool  WaveModel::FFT(int Ft_Flag,complex<double>* data)
{
	double* Rdat = NULL, * Idat = NULL;
	ConvertComplex(data, &Rdat, &Idat);

	int LogN = 0;// log2(IdMax);
	int Nn = 0;// IdMax;

	// parameters error check:
	if ((Rdat == NULL) || (Idat == NULL))                  return false;
	if ((Nn > 16384) || (Nn < 1))                            return false;
	if (!NUMBER_IS_2_POW_K(Nn))                             return false;
	if ((LogN < 2) || (LogN > 14))                         return false;
	if ((Ft_Flag != FT_DIRECT) && (Ft_Flag != FT_INVERSE)) return false;

	register int  i, j, n, k, io, ie, in, nn;
	float         ru, iu, rtp, itp, rtq, itq, rw, iw, sr;

	static const float Rcoef[14] =
	{ -1.0000000000000000F,  0.0000000000000000F,  0.7071067811865475F,
		0.9238795325112867F,  0.9807852804032304F,  0.9951847266721969F,
		0.9987954562051724F,  0.9996988186962042F,  0.9999247018391445F,
		0.9999811752826011F,  0.9999952938095761F,  0.9999988234517018F,
		0.9999997058628822F,  0.9999999264657178F
	};
	static const float Icoef[14] =
	{ 0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
	   -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
	   -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
	   -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
	   -0.0007669903187427F, -0.0003834951875714F
	};

	nn = (int)Nn >> 1;
	ie = Nn;
	for (n = 1; n <= LogN; n++)
	{
		rw = Rcoef[LogN - n];
		iw = Icoef[LogN - n];
		if (Ft_Flag == FT_INVERSE) iw = -iw;
		in = ie >> 1;
		ru = 1.0F;
		iu = 0.0F;
		for (j = 0; j < in; j++)
		{
			for (i = j; i < Nn; i += ie)
			{
				io = i + in;
				rtp = Rdat[i] + Rdat[io];
				itp = Idat[i] + Idat[io];
				rtq = Rdat[i] - Rdat[io];
				itq = Idat[i] - Idat[io];
				Rdat[io] = rtq * ru - itq * iu;
				Idat[io] = itq * ru + rtq * iu;
				Rdat[i] = rtp;
				Idat[i] = itp;
			}

			sr = ru;
			ru = ru * rw - iu * iw;
			iu = iu * rw + sr * iw;
		}

		ie >>= 1;
	}

	for (j = i = 1; i < Nn; i++)
	{
		if (i < j)
		{
			io = i - 1;
			in = j - 1;
			rtp = Rdat[in];
			itp = Idat[in];
			Rdat[in] = Rdat[io];
			Idat[in] = Idat[io];
			Rdat[io] = rtp;
			Idat[io] = itp;
		}

		k = nn;

		while (k < j)
		{
			j = j - k;
			k >>= 1;
		}

		j = j + k;
	}

	if (Ft_Flag == FT_DIRECT) {
		ConvertDouble(&data, Rdat, Idat);
		delete Rdat, delete Idat;
		return true;
	}

	rw = 1.0F / Nn;

	for (i = 0; i < Nn; i++)
	{
		Rdat[i] *= rw;
		Idat[i] *= rw;
	}

	ConvertDouble(&data, Rdat, Idat);
	delete Rdat, delete Idat;
	return true;
}

//нахождение всех спектров
void WaveModel::FindSpectrum() {
	//for (int i = 0; i < N; i++) {
	//	FFur[i] = new complex<double>[IdMax];
	//	for (int j = 0; j < IdMax; j++)
	//		FFur[i][j] = F[IdMax - j - 1][i];

	//	//берем фурье
	//	FFT(FT_DIRECT, FFur[i]);

	//	for (int j = 0; j < IdMax; j++)
	//		cout << FFur[i][j] << endl;
	//}
}

//находит пики в спектрограмме
void WaveModel::FindPicks() {
	//находим маскимальный пик
	//int id;
	//double max;
	//FindMax(0, IdMax, max, id);
	//Energes.push_back({ max, id });

	////находим остальные пики
	//double min = max * 0.05;
	//int ida = id;
	//int idb = IdMax;
	//bool res = true;
	//while (res) {
	//	FindId(min, ida, idb);
	//	res = FindMax(ida, idb, max, id, min);
	//	
	//	if (res) {
	//		Energes.push_back({ max, id }); 
	//		ida = id;
	//	}
	//}

}

//вторая версия поиска пиков
void WaveModel::FindPicks2() {
	/*int id;
	double max;
	FindMax(0, IdMax, max, id);

	max *= 0.05;
	for (int i = 1; i < 1024; i++) {
		double left = abs(FFur[SFId][i - 1]);
		double cent = abs(FFur[SFId][i]);
		double right = abs(FFur[SFId][i + 1]);

		if ((cent > left) && (cent > right) && (cent>=max)) {
			Energes.push_back({ cent, i});
		}
	}*/
}

//находит максимальное значение, начиная с некоторого id
void WaveModel::FindMax(int ida, int idb, double& max, int& id) {
	max = abs(FFur[SFId][ida]);
	for (int i = ida + 1; i < idb; i++) {
		if (max < abs(FFur[SFId][i])) {
			max = abs(FFur[SFId][i]);
			id = i;
		}
	}
}
//находит максимальное значение, начиная с некоторого id
bool WaveModel::FindMax(int ida, int idb, double& max, int& id, double min) {
	max = abs(FFur[int(N / 2)][ida]);
	bool res = false;
	for (int i = ida + 1; i < idb; i++) {
		if ((max < abs(FFur[int(N / 2)][i])) && (abs(FFur[int(N / 2)][i]) > min)) {
			max = abs(FFur[int(N / 2)][i]);
			id = i;
			res = true;
		}
	}
	return res;
}
//находит отрезок со следующим пиком
void WaveModel::FindId(double min, int& ida, int idb) {
	for (int i = ida; i < idb; i++) {
		if (abs(FFur[int(N / 2)][i]) <= min) {
			ida = i;
			return;
		}		
	}
}

//нахождение собственных функций частицы
void WaveModel::FindFunc() {
	//находим спектр
	FindSpectrum();
	FindPicks2();
}

//Отдает указатель на F(id)
complex<double>** WaveModel::GetF() {
	return Fpast;
}

//Отдает указатель на FFur(id)
complex<double>** WaveModel::GetFFur() {
	return FFur;
}

//отдает указатель на X
double* WaveModel::GetX() {
	return X;
}

//отдает указатель на Y
double* WaveModel::GetY() {
	return Y;
}

//отдает указатель на f
double* WaveModel::Getf() {
	return f;
}

//сбрасывает настройки
void WaveModel::Reset()
{
	if ((Fpast != NULL)) {
		delete [] X;
		delete [] A;
		delete [] B;
		delete [] C;
		delete [] D;
		delete [] alpha;
		delete [] betta;

		/*for (int i = 0; i < IdMax; i++)
			delete [] F[i];
		for (int i = 0; i < N; i++)
			delete [] FFur[i];

		delete [] F;
		delete [] FFur;
		Id = 1;
		Energes.clear();*/
	}	
}

//конвертирует из complex в double
void WaveModel::ConvertComplex(complex<double>* comp, double** RDat, double** IDat) {
	/**RDat = new double[IdMax];
	*IDat = new double[IdMax];
	for (int i = 0; i < IdMax; i++) {
		(*RDat)[i] = comp[i].real();
		(*IDat)[i] = comp[i].imag();
	}*/
}

//конвертирует из double в complex
void WaveModel::ConvertDouble(complex<double>** comp, double* RDat, double* IDat) {
	/*for (int i = 0; i < IdMax; i++)
		(*comp)[i] = complex<double>(RDat[i], IDat[i]);*/
}

//Отдает ссылку на вектор с энергиями
vector<pair<double, int>> WaveModel::GetEnerges(){
	return Energes;
}

//находит собственные функции в конкретном 
void WaveModel::FindSF(int id) {
	Energes.clear();
	SFId = id;
	FindPicks2();
}