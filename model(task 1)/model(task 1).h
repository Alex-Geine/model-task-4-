
// model(task 1).h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// Cmodeltask1App:
// Сведения о реализации этого класса: model(task 1).cpp
//

class Cmodeltask1App : public CWinApp
{
public:
	Cmodeltask1App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern Cmodeltask1App theApp;
