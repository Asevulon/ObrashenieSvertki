#pragma once

#include"Drawer.h"

#include<time.h>
#include<random>
#include<fstream>


struct gaussParam
{
	double A = 0;
	double x0 = 0;
	double S = 0;
	gaussParam() :A(0), x0(0), S(0) {}
	gaussParam(double a, double x, double s)
	{
		A = a;
		x0 = x;
		S = s;
	}
};

class Obr
{
private:
	gaussParam gParam[3];
	int N;
	double fd;
	double Simp;
	double Aimp;
	double Sx;
	double Nev;
	double pres;
	double steplen;
	bool IsNeeded;
	int ctr;
	vector<double>baseSign;
	vector<double>baseSvk;
	Drawer sDrw;
	Drawer hDrw;
	Drawer svkDrw;
	Drawer rDrw;

	ofstream log;


	double TotalSignalMistake = 0;
protected:
	vector<double> CreateSignal(gaussParam*gPar);
	double S(gaussParam* gPar, double t);
	vector<double> CreateSvk(vector<double>Signal, double ai, double Simp);
	vector<double> CreateH(double ai, double si);
	double h(double t, double ai, double si);
	double Obr::Nevyazka(vector<double> x, vector<double> y);
	void Obr::MHJ(int kk, vector<double>& x, vector<double> y);
	vector<double>Restore(vector<double>lambda);
public:
	Obr();
	void SetGParam(gaussParam g1, gaussParam g2, gaussParam g3);
	void SetN(int n);
	void SetFd(double FD);
	void SetSDrwHWND(HWND hWnd);
	void SetSVKDrwHWND(HWND hWnd);
	void SetHDrwHWND(HWND hWnd);
	void SetRDrwHWND(HWND hWnd);
	void SetSImp(double SIMP);
	void SetAImp(double AIMP);
	void test();
	void Obr::SetSL(double sl);
	void Obr::SetPres(double pr);
	void NeedToDraw();

	double GetNev();
	int GetCtr();

	void redraw();

	vector<vector<double>>queueDrw;
	void restDrw(vector<double>x);

	double GetTotalSignalMistake();
};