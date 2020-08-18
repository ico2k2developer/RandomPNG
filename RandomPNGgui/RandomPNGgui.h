
// RandomPNGgui.h: file di intestazione principale per l'applicazione PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "includere 'pch.h' prima di includere questo file per PCH"
#endif

#include "resource.h"		// simboli principali


// CRandomPNGguiApp:
// Per l'implementazione di questa classe, vedere RandomPNGgui.cpp
//

class CRandomPNGguiApp : public CWinApp
{
public:
	CRandomPNGguiApp();

// Override
public:
	virtual BOOL InitInstance();

// Implementazione

	DECLARE_MESSAGE_MAP()
};

extern CRandomPNGguiApp theApp;
