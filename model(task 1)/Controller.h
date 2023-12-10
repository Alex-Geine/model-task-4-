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
	mat4 mat; // �������� ������� ��������������

	struct ColorTable {
	public:
		//������ ��������
		SolidBrush** brashes;
		
		//������ ����� ������
		int Size = 11;

		//������ � ���������� ����������
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

		//���������� ��������� �� ����� �� ������� ���������
		SolidBrush* GetBrush(double val) {
			for (int i = 0; i < Size - 1; i++) {
				if ((val >= mas[i]) && (val < mas[i + 1]))
					return brashes[i];				
			}
			return new SolidBrush(Color::Black);
		}
	};

	

	double	R;		//������ ���
	double dt;		//��� �� �������

	int N;				//���������� ����� �� X
	int M;				//���������� ����� �� Y
	int IdMax = 1024;	//���������� �������� �������

	double MaxF;		//������������ �������� �� ������� ������ ��� 3� �������
	double MaxY;		//������������ �������� �� ��� Y ��� ������	��� 3� �������

	double Maxf0;		//������������ �������� ������������� �� 2� �������

	double MaxSF;		//������������ �������� ������������� �� ������� ����������� �������

	double MaxFFur;		//������������ �������� �� ������� �������
	double MaxE;		//������������ �������� �� ������� ����������� �������
	double a;			//����� ������� ���
	double b;			//������ ������� ���
	double f0;			//��������� �������� ������
	double Scalefd;		//�������������� ������� �������������

	complex<double>** F = NULL;				//������ �������� ��������� ������
	complex<double>*** FFur = NULL;			//������ �������� �������
	vector <pair<double, int>> Energes;		//������ �� ���������� ����������� ��������
	
	vector <pair<PointF, PointF>> Isolines[11];		//������ � ����������
	vector <pair<PointF, PointF>> IsolinesSF[11];	//������ � ���������� ��� ����������� �������

	double* X = NULL;		//������ �������� �� X
	double* Y = NULL;		//������ �������� �� Y
	double* f = NULL;		//������ �������� �� f

	//������ �� ����� ���������� ��� 2d � 3d
	vector<Poligon*> polig;

	//������ �� ����� ���������� ��� ����������� �������
	vector<Poligon*> poligSF;

	//����, ���������� �� �������� ������� � ���������� ��� 2d � 3d
	bool poligReady = false;	

	//�������������� ������ ��� ��������� 2d
	void PrepareData2d();

	//�������������� ������ ��� ��������� 3d
	void PrepareData3d();

	//�������������� ������ ��� ��������� ����������� �������
	void PrepareDataSF();

	//������� ��������
	void MakeIsolines();

	//������� �������� �� ������� ����������� �������
	void MakeIsolinesSF();

	//��������� ������� ��� ������ ��������
	bool CheckPoligon(double T, pair<PointF, PointF> & buf, Poligon pol);

	//������� ������
	void ClearList();

	//��������� ������
	void FillList();

	//������� ������������ �������� ������� �����
	void FindMaxFFur();		

	//�������, ������� �������� � ������ � �������
	DWORD WINAPI ModelFunk();

	static DWORD WINAPI StaticModelFunk(PVOID param) {
		Controller* This = (Controller*)param;
		return This->ModelFunk();
	}

	//�������, ������� ������� �����
	DWORD WINAPI ModelFurie();

	static DWORD WINAPI StaticModelFurie(PVOID param) {
		Controller* This = (Controller*)param;
		return This->ModelFurie();
	}
	
public:
	CListBox* listEnerges;		//��������� �� ��������

	int drawId = 0;		//�� ������� �������, � ������� ������ �����
	int drawIdFx = 0;	//�� �������, ������� ������
	int drawIdFy = 0;	//�� �������, ������� ������
	int drawIdE = 0;	//�� ����������� �������, ������� ������
		
	void UpdateModel(
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
			double asrty,	//������� ���������� �� ��� Y
			double Scalefd	//�������������� ������� �������������
	);
		
	//������� ������
	void Clear();
	
	//��������� ��������� 2d �������
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);

	//������������ 3d ������
	void Draw3D(LPDRAWITEMSTRUCT Item1);

	//������������ ������
	void DrawSpectrum(LPDRAWITEMSTRUCT Item1);

	//������������ ����������� �������
	void DrawSF(LPDRAWITEMSTRUCT Item1);
	
	//���������� ����������
	void StartSolve();	

	Controller(){
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);
		mat.rotateY(1);
	}

	//����������
	~Controller() {
		GdiplusShutdown(token);
	}	

	//��������� ������� ����� � �����
	void ShowItemList();
	
	//�������� ������ �� ������
	void GetData();

	//��������, ���� �� ������ ��� ���������
	bool DataReady();

	//������� ����������� ������� �� �������� ����������
	void GetSF(int idx, int idy);

	//�������, ������ �������� ��� �����
	void CheckData();

	//�������, ��������� ������ ������
	void CreateModel() {
		if (mod == nullptr)
			mod = new WaveModel();
	}

	//�������, �������� �� ����� ������
	void Pause();

	//����, ���������� �� ��������� �����
	bool pause = false;
};