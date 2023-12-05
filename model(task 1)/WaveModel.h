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
	
	int index = 0;		//������ ��������
	int Id = 1023;		//������� ���� �������
	int N;		//���������� ����� �� ��� x
	int M;		//���������� ����� �� ��� y
	int IdMax = 1024;	//���������� �������� �� �������
	int SFId;
	int descrKoef = 20;		//�����������, �������������� ������� �������������


	double
		t = 0,	//����� ����� ������
		dt,		//��� �� �������
		stepX,	//��� �� ������������ ��� X
		stepY,	//��� �� ������������ ��� Y
		R,		//�������, ���������� ������������� � ����� ������
		a,		//������� ��� �� ��� X
		b,		//������� ��� �� ��� Y
		U0,		//������ ���
		f0,		//��������� �������� ������
		aSrx,	//������� �������� ��������� ������� �� ��� X
		aSry,	//������� �������� �������� ������� �� ��� Y
		gammax, //�������������������� ���������� �������, �������� ���� ���������������� �� ��������� �� ��� �
		gammay,	//�������������������� ���������� �������, �������� ���� ���������������� �� ��������� �� ��� Y
		koef = 10000;

	//KOEF
	complex<double>
		**A = NULL,
		**B = NULL,
		**C = NULL,
		**D = NULL;

	complex<double>		
		** alpha = NULL,	//�������� ������������� �����
		** betta = NULL;	//�������� ������������� �����

	//DATA
	double* X = NULL;					//�������� ���������������� ����� �� ��� X
	double* Y = NULL;					//�������� ���������������� ����� �� ��� Y
	double* f = NULL;					//�������� ��������� �����
	complex<double>** Fpast = NULL;		//������� ������ � ���������� ������ �������
	complex<double>** Fnow = NULL;		//������� ������ � ��������� ������ �������
	complex<double>*** FBuf = NULL;		//������ � ��������� ������ �� �������	
	
	vector<pair<double, int>> Energes;	//����������� �������

	//��������� ������������� ���������
	void InitData();

	//��������� ��������� �����
	double F0(double x, double y);

	//���������� ������������� ABCD �� ������ ��������
	void FindABCD1();

	//���������� ������������� ABCD �� ������ ��������
	void FindABCD2();

	//������� ������������� �������
	double U(double x, double y);	

	//������ ��� �������� �� ������ ��������
	void ForwardMethod1();

	//�������� ��� �������� �� ������ ��������
	void BackwardMethod1();

	//������ ��� �������� �� ������ ��������
	void ForwardMethod2();

	//�������� ��� �������� �� ������ ��������
	void BackwardMethod2();

	//������� �����
	bool  FFT(int Ft_Flag, complex<double>* data);

	//���������� ���� ��������
	void FindSpectrum();

	//���������� ������ ����������� �� ������� F �� X
	complex<double> DFX(int i, int j, bool flag);

	//���������� ������ ����������� �� ������� F �� Y
	complex<double> DFY(int i, int j, bool flag);

	//������������ �� complex � double
	void ConvertComplex(complex<double>* comp, double** RDat, double** IDat);

	//������������ �� double � complex
	void ConvertDouble(complex<double>** comp, double* RDat, double* IDat);

	//������� ���� � �������������
	void FindPicks();

	//������ ������ ������ �����
	void FindPicks2();

	//������� ������������ ��������, ������� � ���������� id
	void FindMax(int ida, int idb, double&max, int & id);

	//������� ������������ ��������, ������� � ���������� id
	bool FindMax(int ida, int idb, double& max, int& id, double min);

	//������� ������� �� ��������� �����
	void FindId(double min, int& ida, int idb);

	//�������� ������� ������� �� ������� ���� � ����� ��� ����������� ����
	void CopyData();
public:
	

	//����, ���������� �� ���������� ���������� �����
	bool DataReady = false;

	//����, ���������� �� ������ �������� �����
	bool FurStart = false;

	//�������, ������� ��������� � ����� ����� ������ �������
	bool PutData();

	//������ ��������� �� F()
	complex<double>** GetF();

	//������ ��������� �� FFur()
	complex<double>*** GetFFur();

	//������ ��������� �� ������ � ���������
	vector<pair<double, int>> GetEnerges();

	//���������� ���������
	void Reset();

	//������ ��������� �� X
	double* GetX();

	//������ ��������� �� Y
	double* GetY();

	//������ ��������� �� f
	double* Getf();

	//���������� ��������� ������ � ��������� ������ �������
	void FindWave();

	//���������� ����������� ������� �������
	void FindFunc();

	//�������� ��������� ������
	void Update(int N, int M, double dt, double R, double a, double b, double U0, double f0, double asrx, double asry, double gammax, double gammay);

	//������� ����������� ������� � ���������� 
	void FindSF(int id);
};