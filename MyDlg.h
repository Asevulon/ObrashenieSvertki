
// MyDlg.h: файл заголовка
//

#pragma once

#include"Obr.h"

#define TIMER1 1
#define TIMER1TIME 1000

// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBRASHENIESVERTKI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	Obr o;
	double A1;
	double A2;
	double A3;
	double x01;
	double x02;
	double x03;
	double S1;
	double S2;
	double S3;
	double fd;
	int N;
	double Simp;
	double Aimp;
	HANDLE thread;
	CString Nev;
	UINT_PTR timer;
	afx_msg void MyDlg::OnTimer(UINT_PTR idEvent);
	double sl;
	double pr;
	CString Status;
	int tmr;
	bool InPrcs;
};

DWORD WINAPI threadfunc(LPVOID);

