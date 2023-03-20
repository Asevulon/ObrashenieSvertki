
// ObrashenieSvertki.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CObrashenieSvertkiApp:
// Сведения о реализации этого класса: ObrashenieSvertki.cpp
//

class CObrashenieSvertkiApp : public CWinApp
{
public:
	CObrashenieSvertkiApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CObrashenieSvertkiApp theApp;
