// Git_05.h : main header file for the Git_05 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "HTTPS_GIT_Client.h"

// CGit_05App:
// See Git_05.cpp for the implementation of this class
//

class CGit_05App : public CBCGPWinApp
{
	friend class CBackStagePageInfo;
private:
	std::shared_ptr<HTTPS_GIT_Client> https_GIT_client_ptr_;
	vector<git_repository*> repos_;
	void delete_repos_();
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
	afx_msg void On_Add_Repo();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCreateRepoButton();
};

extern CGit_05App theApp;