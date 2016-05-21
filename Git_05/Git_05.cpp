// Git_05.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Git_05.h"
#include "MainFrm.h"

#include "Git_05Doc.h"
#include "Git_05View.h"


#include "GIT_Engine.hpp"
#include "GIT_Commit_Local.hpp"

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

	read_visual_theme_from_file_();

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
	
	load_repos_from_file_(repositories_file_);
	//static_cast<CMainFrame*>(AfxGetMainWnd())->selectRepository();
	return TRUE;
}
#include <fstream>
void CGit_05App::write_visual_theme_to_file_()const
{
	std::ofstream f_out("visual_theme.txt");
	if (f_out)
	{
		f_out << GetVisualTheme();
	}
}

void CGit_05App::read_visual_theme_from_file_()
{
	std::ifstream f_in("visual_theme.txt");
	CBCGPWinApp::BCGP_VISUAL_THEME theme;
	int t{0};
	if (f_in)
	{
		f_in >> t;
	}
	theme = static_cast<CBCGPWinApp::BCGP_VISUAL_THEME>(t);
	SetVisualTheme(theme);
}

static bool is_empty(std::ifstream& pFile)
{//http://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
	return pFile.peek() == std::ifstream::traits_type::eof();
}

void CGit_05App::load_repos_from_file_(const CString& file_path)
{
	std::ifstream f_in(file_path);
	if (!is_empty(f_in))
	{
		if (f_in)
		{
			std::string repo_path;
			while (std::getline(f_in, repo_path))
			{
				//In order to test if path is really a repo path we simply try to open it
				//git_repository* out{nullptr};
				CA2W w_str(repo_path.c_str());
				CString c_repo_path(w_str);
				if (GIT_Engine::check_if_repo(c_repo_path))
				{
					CA2W w_str(repo_path.c_str());
					CString w_repo_path(w_str);
					populate_UI_(w_repo_path);
				}
			}
		}
	}
	CMainFrame* main_frm_p = static_cast<CMainFrame*>(AfxGetMainWnd());
	main_frm_p->select_active_repo();
// 	else
// 	{
// 		throw std::logic_error("Couldn't read from file");
// 	}
}

void CGit_05App::invalidate_view()
{
	CGit_05View::GetView()->InvalidateRect_();
}

void CGit_05App::reset_view() const
{
	CGit_05Doc::GetDoc()->reset_file_for_display();
}
void CGit_05App::set_file_for_display(const diffed_file_t& diffedFile)
{
	CGit_05Doc::GetDoc()->set_file_for_display(diffedFile);
	CGit_05View::GetView()->InvalidateRect_();
	
}

void CGit_05App::deselect_file_from_commit_detail_window() const
{
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	pMainWnd->deselect_file_from_commit_detail_window();
}

bool CGit_05App::write_repo_path_to_file_(const CString& c_repo_path)
{
	//load first what's in file and see if this repo is already there
	std::ifstream f_in(repositories_file_);
	bool existed{ true };
	if (f_in)
	{
		std::vector<std::string> paths;
		std::string path;
		while (std::getline(f_in, path))
		{
			paths.push_back(path);
		}
		f_in.close();
		CT2CA ct2a(c_repo_path);
		std::string repo_path(ct2a);

		auto iter = std::find(cbegin(paths),cend(paths),repo_path);
		
		if (paths.empty() || (iter == cend(paths)))
		{
			existed = false;
			std::ofstream f_out(repositories_file_, std::ios_base::app);
			if (f_out)
			{
				f_out << repo_path << '\n';
			}
			else
			{
				AfxMessageBox(L"Couldn't write to a file");
			}
		}
	}
	return existed;
}

// CGit_05App message handlers

int CGit_05App::ExitInstance() 
{

	git_libgit2_shutdown();
	delete_repos_();
	write_visual_theme_to_file_();
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

static void convert_to_dot_git_path(CString & c_repo_path)
{
	
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(c_repo_path);
	// construct a std::string using the LPCSTR input
	std::string repo_path(pszConvertedAnsiString);

	size_t position = repo_path.find(".git");
	if (position == std::string::npos)
	{
		if (repo_path.find("\\"))
		{
			repo_path += "\\.git";
		}
		else
		{
			repo_path += "/.git";
		}
	}
	CA2W w_str(repo_path.c_str());
	c_repo_path = w_str;
}

void CGit_05App::populate_UI_(const CString& repo_path)
{
	typedef CString branch_name_t;
	std::map<branch_name_t, std::vector<GIT_Commit_Local>> branch_commits;
	GIT_Engine::get_commits_for_branches(repo_path, branch_commits);

	typedef CString repo_name_t;
	std::map<repo_name_t, decltype(branch_commits)> repo_branches;

	//CString repo_name = get_repo_name(repo_path);
	
	//CA2W ca2w(repo_name.c_str());
	//std::wstring w_repo_name = ca2w;
	//CString c_repo_name = w_repo_name.c_str();
	repo_branches[repo_path] = branch_commits;
	get_main_frame()->m_wndWorkSpace_Repos_.git_tree(std::move(repo_branches));
}




// CString CGit_05App::read_repo_name_from_file_()
// {
// 	std::ifstream f_in(file_with_repo_to_set_as_active_);
// 	std::string active_repo;
// 	if (f_in)
// 	{
// 		f_in >> active_repo;
// 	}
// 	CA2W ca2w(active_repo.c_str());
// 	std::wstring wide_str = ca2w;
// 	return wide_str.c_str();
// }

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
		//CT2CA c_str_path(path_name);
 		//std::string repo_path(c_str_path);
		convert_to_dot_git_path(path_name);
		if (GIT_Engine::check_if_repo(path_name))
		{
			bool existed = write_repo_path_to_file_(path_name);
			if (!existed)
			{
				populate_UI_(path_name);
			}
		}
		else
		{//display msg
			AfxMessageBox(L"Selected folder is not a valid git repository");
		}
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
		CT2CA path(folder_name);


		GIT_Engine::git_init_opts_t git_init_options = { 1, 0, 0, 1, GIT_REPOSITORY_INIT_SHARED_UMASK, 0, 0, 0 };

		if (GIT_Engine::git_init(out, path, git_init_options))
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
void CGit_05App::write_credentials_to_file(const CString& c_username, const CString& c_password)const
{
	std::ofstream f_out("credentials.txt");
	CT2CA pszConvertedAnsiString_username(c_username);
	CT2CA pszConvertedAnsiString_password(c_password);
	// construct a std::string using the LPCSTR input
	std::string username(pszConvertedAnsiString_username);
	std::string password(pszConvertedAnsiString_password);
	
	if (f_out)
	{
		f_out << username << '\n';
		f_out << password << '\n';
	}
}
void CGit_05App::read_credentials_from_file(CString& c_username, CString& c_password)const
{
	std::ifstream f_in("credentials.txt");
	std::string username, password;
	if (f_in)
	{
		f_in >> username;
		f_in >> password;
	}
	CA2W w_username(username.c_str());
	CA2W w_password(password.c_str());
	c_username = w_username;
	c_password = w_password;
}



void CGit_05App::delete_repos_()
{
	for (auto repo : repos_)
	{
		git_repository_free(repo);
		//delete repo;//:AC: warning C4150
	}
}