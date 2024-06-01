#include"pch.h"
#include"Obr.h"

Obr::Obr()
{
	N = 0;
	fd = 0;
	Simp = 0;
	Aimp = 0;
	Sx = 0;
	Nev = 0;
	pres = 0;
	steplen = 0;
	IsNeeded = false;
	log.open("log.txt");
}

void Obr::SetGParam(gaussParam g1, gaussParam g2, gaussParam g3)
{
	gParam[0] = g1;
	gParam[1] = g2;
	gParam[2] = g3;
}
void Obr::SetN(int n)
{
	N = n;
}
void Obr::SetFd(double FD)
{
	fd = FD;
}
void Obr::SetSImp(double SIMP)
{
	Simp = SIMP;
}
void Obr::SetAImp(double AIMP)
{
	Aimp = AIMP;
}
void Obr::SetSL(double sl)
{
	steplen = sl;
}
void Obr::SetPres(double pr)
{
	pres = pr;
}

void Obr::SetSDrwHWND(HWND hWnd)
{
	sDrw.Create(hWnd);
}
void Obr::SetSVKDrwHWND(HWND hWnd)
{
	svkDrw.Create(hWnd);
}
void Obr::SetHDrwHWND(HWND hWnd)
{
	hDrw.Create(hWnd);
}
void Obr::SetRDrwHWND(HWND hWnd)
{
	rDrw.Create(hWnd);
}
void Obr::redraw()
{
	//sDrw.Redraw();
	svkDrw.Redraw();
	hDrw.Redraw();
	rDrw.Redraw();
}
void Obr::NeedToDraw()
{
	IsNeeded = true;
}

double Obr::GetNev()
{
	return Nev;
}
int Obr::GetCtr()
{
	return ctr;
}


vector<double> Obr::CreateSignal(gaussParam* gPar)
{
	vector<double>res;
	for (int i = 0; i < N; i++)
	{
		res.push_back(S(gPar, i / fd));
	}
	Sx = 0;
	for (auto& item : res)Sx += item;
	return res;
}
double Obr::S(gaussParam* gPar, double t)
{
	double res = 0;
	for (int i = 0; i < 3; i++)
	{
		res += gPar[i].A * exp(-(t - gPar[i].x0) * (t - gPar[i].x0) / gPar[i].S / gPar[i].S);
	}
	return res;
}
vector<double> Obr::CreateSvk(vector<double>signal, double ai, double si)
{
	vector<double>res;
	for (int i = 0; i < N; i++)
	{
		double temp = 0;
		for (int k = 0; k < N; k++)
		{
			temp += signal[k] * h((i - k) / fd, ai, si);
		}
		res.push_back(temp);
	}
	return res;
}
double Obr::h(double t, double ai, double si)
{
	return ai*exp(-t * t / si / si);
}
vector<double> Obr::CreateH(double ai, double si)
{
	vector<double> res;
	int size = N / 2;
	for (int i = 0; i < size; i++)
	{
		res.push_back(h(i / fd, ai, si));
	}
	for (int i = size - 1; i > -1; i--)
	{
		res.push_back(res[i]);
	}
	return res;
}
vector<double> Obr::Restore(vector<double>lambda)
{
	vector<double> res;
	for (int j = 0; j < N; j++)
	{
		double temp = -1;
		for (int i = 0; i < N; i++)
		{
			temp += -h((i - j) / fd, Aimp, Simp) * lambda[i];
		}
		res.push_back(exp(temp));
	}
	return res;
}

void Obr::test()
{
	srand(time(NULL));

	auto signal = CreateSignal(gParam);
	baseSign = signal;
	auto svk = CreateSvk(signal, Aimp, Simp);
	baseSvk = svk;
	//sDrw.DrawGraph(signal, fd);
	auto H = CreateH(Aimp, Simp);
	hDrw.DrawGraph(H, fd);
	svkDrw.DrawGraph(svk, fd);

	vector<double>lambda(N, 0.5);
	MHJ(N, lambda, svk);
	auto res = Restore(lambda);
	rDrw.DrawTwoSignals(baseSign, res, fd);

	Nev = Nevyazka(lambda, svk);

	double mist = 0;
	for (auto& item : signal)mist += item * item;
	Nev = 100. * Nev / mist;
}


void Obr::MHJ(int kk, vector<double>& x, vector<double> yi)
{
	ctr = 0;

	// kk - количество параметров; x - массив параметров
	float  TAU = pres; // “очность вычислений
	int i, j, bs, ps;
	float z, h, k, fi, fb;
	float* b = new float[kk];
	float* y = new float[kk];
	float* p = new float[kk];

	h = steplen;
	x[0] = 1.;
	for (i = 1; i < kk; i++)  x[i] = (float)rand() / RAND_MAX; // «адаетс€ начальное приближение

	k = h;
	for (i = 0; i < kk; i++)	y[i] = p[i] = b[i] = x[i];
	fi = Nevyazka(x, yi);
	ps = 0;   bs = 1;  fb = fi;

	j = 0;
	while (1)
	{
		ctr++;
		if (IsNeeded)
		{
			queueDrw.push_back(x);
			IsNeeded = false;
		}

		x[j] = y[j] + k;
		z = Nevyazka(x, yi);
		
		if (z >= fi) {
			x[j] = y[j] - k;
			z = Nevyazka(x, yi);
			if (z < fi)   y[j] = x[j];
			else  x[j] = y[j];
		}
		else  y[j] = x[j];
		fi = Nevyazka(x, yi);

		if (j < kk - 1) { j++;  continue; }
		if (fi + 1e-8 >= fb) {
			if (ps == 1 && bs == 0) {
				for (i = 0; i < kk; i++) {
					p[i] = y[i] = x[i] = b[i];
				}
				z = Nevyazka(x, yi);
				bs = 1;   ps = 0;   fi = z;   fb = z;   j = 0;
				
				continue;
			}
			
			k /= 10.;
			if (k < TAU) break;
			j = 0;

			
			continue;
		}

		for (i = 0; i < kk; i++) {
			p[i] = 2 * y[i] - b[i];
			b[i] = y[i];
			x[i] = p[i];

			y[i] = x[i];
		}
		z = Nevyazka(x,yi);
		fb = fi;   ps = 1;   bs = 0;   fi = z;   j = 0;

		
	} //  end of while(1)

	for (i = 0; i < kk; i++)  x[i] = p[i];

	delete b;
	delete y;
	delete p;
}

double Obr::Nevyazka(vector<double> x, vector<double> y)
{
	double res = 0;
	/*int size = y.size();
	for (int i = 0; i < size; i++)
	{
		double temp = y[i];
		for (int k = 0; k < size; k++)
		{
			double tempk = -1;
			for (int j = 0; j < size; j++)
			{
				tempk += -h((j - k) / fd, Aimp, Simp) * x[j];
			}
			tempk = exp(tempk);
			temp += -h((i - k) / fd, Aimp, Simp) * tempk;
		}
		res += temp * temp;
	}*/
	auto rx = Restore(x);
	auto svk = CreateSvk(rx, Aimp, Simp);
	for (int i = 0; i < rx.size(); i++)
	{
		res += (y[i] - svk[i]) * (y[i] - svk[i]);
	}

	return res;
}



void Obr::restDrw(vector<double>x)
{
	auto res = Restore(x);
	rDrw.DrawTwoSignals(baseSign, res, fd);
	auto svk = CreateSvk(res, Aimp, Simp);
	svkDrw.DrawTwoSignals(baseSvk, svk, fd);
}



