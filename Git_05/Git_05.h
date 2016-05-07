// Git_05.h : main header file for the Git_05 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "HTTPS_GIT_Client.h"
//#include "MainFrm.h"
// CGit_05App:
// See Git_05.cpp for the implementation of this class
//
#define _ui_ 
class CMainFrame;

class CGit_05App : public CBCGPWinApp
{
	//friend class CBackStagePageInfo;
private:
	std::shared_ptr<HTTPS_GIT_Client> https_GIT_client_ptr_;
	vector<git_repository*> repos_;
	void delete_repos_();
public:
	CGit_05App();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();
	const HTTPS_GIT_Client* const get_https_git_client_p();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	afx_msg void OnAppAbout();
	afx_msg
	void On_Add_Repo();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCreateRepoButton();
public:
	//inner app workings

	CMainFrame* get_main_frame()const;

	void write_credentials_to_file(const CString&, const CString&)const;
	void read_credentials_from_file(CString& username, CString& password)const;
	void write_visual_theme_to_file_()const;
	void read_visual_theme_from_file_();
private:
	bool write_repo_path_to_file_(const CString& repo_path);
	void populate_UI_(const CString & repo_path);
	void load_repos_from_file_(const CString & file_path);
	const CString repositories_file_ = L"repositories_paths.txt";
};

extern CGit_05App theApp;