
// Route Table Manager.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRouteTableManagerApp:
// See Route Table Manager.cpp for the implementation of this class
//

class CRouteTableManagerApp : public CWinApp
{
public:
	CRouteTableManagerApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRouteTableManagerApp theApp;