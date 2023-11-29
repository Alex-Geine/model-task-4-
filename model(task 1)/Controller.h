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
	mat4 mat; // �������� ������� ��������������

	

	double	R;		//������ ���
	double dt;		//��� �� �������

	int N;				//���������� ����� �� X
	int M;				//���������� ����� �� Y
	int IdMax = 1024;	//���������� �������� �������

	double MaxF;		//������������ �������� �� ������� ������
	double MaxFFur;		//������������ �������� �� ������� �������
	double MaxE;		//������������ �������� �� ������� ����������� �������
	double a;			//����� ������� ���
	double b;			//������ ������� ���
	double f0;			//��������� �������� ������

	complex<double>** F = NULL;				//������ �������� ��������� ������
	complex<double>** FFur = NULL;			//������ �������� �������
	vector <pair<double, int>> Energes;		//������ �� ���������� ����������� ��������

	double* X = NULL;		//������ �������� �� X
	double* Y = NULL;		//������ �������� �� Y
	double* f = NULL;		//������ �������� �� f


	//������� ������
	void ClearList();

	//��������� ������
	void FillList();

	//������� ������������ �������� �������
	void FindMaxF();

	//������� ������������ �������� ������� �����
	void FindMaxFFur();

	//������� ������������ �������� ����������� �������
	void FindMaxEn();	

	//�������, ������� �������� � ������ � �������
	DWORD WINAPI ModelFunk();

	static DWORD WINAPI StaticModelFunk(PVOID param) {
		Controller* This = (Controller*)param;
		return This->ModelFunk();
	}
	
public:
	CListBox* listEnerges;		//��������� �� ��������

	int drawId = 0;		//�� ������� �������, � ������� ������ �����
	int drawIdF = 0;	//�� �������, ������� ������
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
			double asrty	//������� ���������� �� ��� Y
	);
		
	//������� ������
	void Clear();
	
	//��������� ��������� �������� �������6
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);

	//��������� ��������� 
	void DrawPhase(LPDRAWITEMSTRUCT Item1);

	//��������� ��������� 
	void DrawPhaseTr(LPDRAWITEMSTRUCT Item1);
	
	//���������� ����������
	void StartSolve();	

	Controller():mod(new WaveModel) {
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);
		mat.rotateY(2);
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

	//������� ����������� ������� �� ������� ������� ����������
	void GetSF(int id);
};