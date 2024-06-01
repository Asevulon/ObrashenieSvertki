
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "ObrashenieSvertki.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OBRASHENIESVERTKI_DIALOG, pParent)
	, A1(10)
	, A2(7)
	, A3(8)
	, x01(1)
	, x02(3)
	, x03(4)
	, S1(0.05)
	, S2(0.05)
	, S3(0.05)
	, fd(10)
	, N(50)
	, Simp(0.25)
	, Aimp(1)
	, Nev(_T(""))
	, sl(1)
	, pr(1e-6)
	, Status(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, A1);
	DDX_Text(pDX, IDC_EDIT2, A2);
	DDX_Text(pDX, IDC_EDIT1, A3);
	DDX_Text(pDX, IDC_EDIT6, x01);
	DDX_Text(pDX, IDC_EDIT5, x02);
	DDX_Text(pDX, IDC_EDIT4, x03);
	DDX_Text(pDX, IDC_EDIT9, S1);
	DDX_Text(pDX, IDC_EDIT8, S2);
	DDX_Text(pDX, IDC_EDIT7, S3);
	DDX_Text(pDX, IDC_EDIT11, fd);
	DDX_Text(pDX, IDC_EDIT10, N);
	DDX_Text(pDX, IDC_EDIT12, Simp);
	DDX_Text(pDX, IDC_EDIT13, Aimp);
	DDX_Text(pDX, IDC_EDIT14, Nev);
	DDX_Text(pDX, IDC_EDIT16, sl);
	DDX_Text(pDX, IDC_EDIT15, pr);
	DDX_Text(pDX, IDC_EDIT17, Status);
	DDX_Control(pDX, IDC_EDIT17, StatusTextEdit);
	DDX_Control(pDX, IDC_EDIT14, NevTextCtrl);
	DDX_Control(pDX, IDC_EDIT18, TotalSignalMistakeText);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &MyDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	o.SetHDrwHWND(GetDlgItem(IDC_PICTURE_H)->GetSafeHwnd());
	o.SetSVKDrwHWND(GetDlgItem(IDC_PICTURE_SVK)->GetSafeHwnd());
	o.SetRDrwHWND(GetDlgItem(IDC_PICTURE_RESTORED)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		o.redraw();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	TerminateThread(thread, 0);
	o.queueDrw.clear();
	tmr = 0;
	timer = SetTimer(TIMER1, TIMER1TIME, NULL);

	UpdateData();
	o.SetFd(fd);
	o.SetGParam(gaussParam(A1, x01, S1), gaussParam(A2, x02, S2), gaussParam(A3, x03, S3));
	o.SetN(N);
	o.SetSImp(Simp);
	o.SetAImp(Aimp);
	o.SetPres(pr);
	o.SetSL(sl);
	thread = CreateThread(NULL, NULL, threadfunc, this, NULL, NULL);
	Status = L"В процессе...";
	UpdateData(FALSE);
}


DWORD WINAPI threadfunc(LPVOID in)
{
	MyDlg* dlg = (MyDlg*) in;
	dlg->InPrcs = true;
	dlg->StartTime = clock();
	dlg->o.test();
	dlg->InPrcs = false;
	dlg->KillTimer(dlg->timer);
	dlg->o.queueDrw.clear();
	double N = dlg->o.GetNev();
	dlg->Nev.Format(L"%.6f", N);
	dlg->NevTextCtrl.SetWindowTextW(dlg->Nev);
	dlg->Status.Format(L"Звершено! (%d итераций, %.2f  сек)", dlg->o.GetCtr(), (clock()-dlg->StartTime)/1000.);
	dlg->StatusTextEdit.SetWindowTextW(dlg->Status);

	double mist = dlg->o.GetTotalSignalMistake();
	CString smist;
	smist.Format(L"%.6f", mist);
	dlg->TotalSignalMistakeText.SetWindowTextW(smist);
	return 0;
}


afx_msg void MyDlg::OnTimer(UINT_PTR idEvent)
{
	if (InPrcs)
	{
		int ctr = o.GetCtr();
		Status.Format(L"В процессе... (%d итераций, %.2f сек)", ctr, (clock() - StartTime) / 1000.);
	}
	o.NeedToDraw();
	while (!o.queueDrw.empty())
	{
		o.restDrw(o.queueDrw[0]);
		o.queueDrw.erase(o.queueDrw.begin());
	}
	
	CDialogEx::OnTimer(idEvent);
	UpdateData(FALSE);
}




void MyDlg::OnBnClickedStop()
{
	if (!InPrcs)return;
	TerminateThread(thread, 0);
	Status.Format(L"Прервано! (% d итераций, %.2f сек)", o.GetCtr(), (clock() - StartTime) / 1000.);
	UpdateData(FALSE);
	KillTimer(timer);

}
