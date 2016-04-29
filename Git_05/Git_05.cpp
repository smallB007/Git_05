// Git_05.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Git_05.h"
#include "MainFrm.h"

#include "Git_05Doc.h"
#include "Git_05View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGit_05App

BEGIN_MESSAGE_MAP(CGit_05App, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CBCGPWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CBCGPWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CBCGPWinApp::OnFilePrintSetup)
	ON_COMMAND(IDC_ADD_REPO_BUTTON, On_Add_Repo)
	ON_COMMAND(IDC_CREATE_REPO_BUTTON, OnCreateRepoButton)
END_MESSAGE_MAP()

struct threadInfo_t
{
	HTTPS_GIT_Client* https_client;
};
// CGit_05App construction
UINT ThreadFunc(LPVOID pParam)
{
	threadInfo_t* t_info = static_cast<threadInfo_t*>(pParam);
	auto client = t_info->https_client;
	client->connect();
	return 0;
}




CGit_05App::CGit_05App()
{
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLUE, ID_VIEW_APPLOOK_2007);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLACK, ID_VIEW_APPLOOK_2007_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_SILVER, ID_VIEW_APPLOOK_2007_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_AQUA, ID_VIEW_APPLOOK_2007_3);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_BLUE, ID_VIEW_APPLOOK_2010_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_BLACK, ID_VIEW_APPLOOK_2010_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_SILVER, ID_VIEW_APPLOOK_2010_3);
	AddVisualTheme(BCGP_VISUAL_THEME_SCENIC, ID_VIEW_APPLOOK_SCENIC);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2013_WHITE, ID_VIEW_APPLOOK_2013_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2013_GRAY, ID_VIEW_APPLOOK_2013_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2013_DARK_GRAY, ID_VIEW_APPLOOK_2013_3);
// 	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_COLORFUL, ID_VIEW_APPLOOK_2016_1);
// 	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_DARK_GRAY, ID_VIEW_APPLOOK_2016_2);
// 	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_WHITE, ID_VIEW_APPLOOK_2016_3);

	SetVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_BLUE);


	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	https_GIT_client_ptr_ = std::make_shared<HTTPS_GIT_Client>();
	threadInfo_t thread_info;
	thread_info.https_client = https_GIT_client_ptr_.get();
	//CWinThread* pThread = AfxBeginThread(ThreadFunc, &thread_info);:AC: make that happen


	//https_GIT_client_ptr_->connect();
}


// The one and only CGit_05App object

CGit_05App theApp;


// CGit_05App initialization

BOOL CGit_05App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBCGPWinApp::InitInstance();
	m_AppOptions.m_strScenicRibbonLabel = _T("Settings");
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("BCGP AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	SetRegistryBase(_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGit_05Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CGit_05View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	
	git_libgit2_init();

	return TRUE;
}

// CGit_05App message handlers

int CGit_05App::ExitInstance() 
{

	git_libgit2_shutdown();
	delete_repos_();
	return CBCGPWinApp::ExitInstance();
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	CBCGPURLLinkButton m_btnURL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBCGPDialog(CAboutDlg::IDD)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CGit_05App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CGit_05App message handlers


void CGit_05App::PreLoadState()
{
}


const HTTPS_GIT_Client* const CGit_05App::get_https_git_client_p()
{
	return https_GIT_client_ptr_.get();
}
#include "GIT_Engine.hpp"
#include "GIT_Commit_Local.hpp"
void CGit_05App::On_Add_Repo()
{
	//throw - 1;
	LPCTSTR pszFilter =
		_T("Git repositories (*.git)|*.git|");
		//_T("Bitmap files (*.bmp;*.dib;*.rle)|*.bmp;*.dib;*.rle|")
		//_T("JPEG files (*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg;*.jpeg;*.jpe;*.jfif||");
	
	CFolderPickerDialog dlgFile;// (TRUE, NULL, NULL, OFN_HIDEREADONLY, pszFilter, AfxGetMainWnd());
	//:AC: ToDo 
	//a) if folder selected contains .git folder make sure this works
	//b) error checking if there is no repo etc
	if (IDOK == dlgFile.DoModal())
	{
		auto path_name = dlgFile.GetPathName();
		CT2CA c_str_path(path_name);
 		std::string repo_path(c_str_path);

 		std::map<std::string, std::vector<GIT_Commit_Local>> branch_commits;
		GIT_Engine::get_commits_for_branches(repo_path, branch_commits);
		get_main_frame()->m_wndWorkSpace41.git_tree(std::move(branch_commits));
	}
}

void CGit_05App::OnCreateRepoButton()
{
	//equivalent of git init
	/*
	Executing git init creates a .git subdirectory in the project root, which contains all of the necessary metadata for the repo. 
	Aside from the .git directory, an existing project remains unaltered (unlike SVN, Git doesn't require a .git folder in every subdirectory).
	*/

	CFolderPickerDialog dlgFile;

	if (IDOK == dlgFile.DoModal())
	{
		git_repository* out = nullptr;
		auto folder_name = dlgFile.GetPathName();
		CT2CA pszCharacterString(folder_name);

		if (git_repository_init(&out, pszCharacterString, 0) != git_error_code::GIT_OK)
		{//couldn't init, say something ;)
			throw "Not implemented yet";
			const git_error *e = giterr_last();
		}
		else
		{
			repos_.push_back(out);
		}
	}
}
#include "MainFrm.h"
CMainFrame* CGit_05App::get_main_frame() const
{
	return static_cast<CMainFrame*>(AfxGetMainWnd());
}

#include <fstream>
void CGit_05App::write_credentials_to_file(const char* username, const char* password)const
{
	std::ofstream f_out("credentials.txt");
	if (f_out)
	{
		f_out << username << '\n';
		f_out << password << '\n';
	}
}
void CGit_05App::read_credentials_from_file(std::string& username, std::string& password)const
{
	std::ifstream f_in("credentials.txt");
	if (f_in)
	{
		f_in >> username;
		f_in >> password;
	}
}

void CGit_05App::delete_repos_()
{
	for (auto repo : repos_)
	{
		git_repository_free(repo);
		//delete repo;//:AC: warning C4150
	}
}