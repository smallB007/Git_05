// Git_05.h : main header file for the Git_05 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CGit_05App:
// See Git_05.cpp for the implementation of this class
//

class CGit_05App : public CBCGPWinApp
{
public:
	CGit_05App();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGit_05App theApp;