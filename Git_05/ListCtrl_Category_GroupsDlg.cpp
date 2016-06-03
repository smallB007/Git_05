// ListCtrl_Category_GroupsDlg.cpp : implementation file
//

#include "stdafx.h"

#include "boost/algorithm/string/replace.hpp"
#include "ListCtrl_Category_GroupsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CListCtrl_Category_GroupsDlg::CListCtrl_Category_GroupsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrl_Category_GroupsDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CListCtrl_Category_GroupsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	//DDX_Control(pDX, IDC_EDIT2, commit_header_edt_);
	DDX_Control(pDX, IDC_EDIT1, commit_body_edt_);
	DDX_Control(pDX, IDC_BRANCH_NAME, branch_name_);
	DDX_Control(pDX, IDC_ORIGIN_BRANCH, origin_branch);
	DDX_Control(pDX, ID_COMMIT, commit_btn);
	//DDX_Control(pDX, IDC_MENU_BUTTON_ADD_FILES_COMMIT, add_menu_btn_);

	DDX_Control(pDX, IDC_EDIT2, regex_edit_box);
}

BEGIN_MESSAGE_MAP(CListCtrl_Category_GroupsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	ON_BN_CLICKED(ID_COMMIT, &CListCtrl_Category_GroupsDlg::OnBnClickedCommit)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CListCtrl_Category_GroupsDlg::OnClickList1)
	//ON_BN_CLICKED(IDC_MENU_BUTTON_ADD_FILES_COMMIT, &CListCtrl_Category_GroupsDlg::OnBnClicked_Add_Files_Commit_Dlg)
	ON_BN_CLICKED(IDC_ADD_TO_STAGE_BUTTON, &CListCtrl_Category_GroupsDlg::OnBnClickedAddToStageButton)
	//ON_COMMAND(ID_ITEM1_SUBITEM1, &CListCtrl_Category_GroupsDlg::OnItem1Click)
	ON_BN_CLICKED(IDC_REMOVE_FROM_STAGE_BUTTON, &CListCtrl_Category_GroupsDlg::OnBnClickedRemoveFromStageButton)
END_MESSAGE_MAP()


// CListCtrl_Category_GroupsDlg message handlers
git_delta_t CListCtrl_Category_GroupsDlg::get_item_state(const CString& fileName)
{
	return m_ListCtrl.get_item_state(fileName);
}
CString CListCtrl_Category_GroupsDlg::get_item_status_(const CString& fileName)
{
	return m_ListCtrl.get_item_status(fileName);
}

BOOL CListCtrl_Category_GroupsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWinApp* pApp = AfxGetApp();
	VERIFY(m_cImageListNormal.Create(32, 32, ILC_COLOR32, 0, 0));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_UNMODIFIED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_ADDED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_DELETED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_MODIFIED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_RENAMED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_COPIED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_IGNORED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_UNTRACKED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_TYPECHANGE));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_UNREADABLE));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_CONFLICTED));
	m_ListCtrl.SetImageList(&m_cImageListNormal, LVSIL_SMALL);
	m_ListCtrl.SetBkColor(CLR_NONE);
	m_ListCtrl.set_data_model(&m_DataModel);//so we can retrieve status of a file Staged, unstaged etc.
	// 		
	/*m_ListCtrl.InsertColumn(0, _T("Repo name"), LVCFMT_CENTER, -1, 0);*/
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);
	//
	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	CString strAboutMenu;
	//	strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	

	// Create Columns
	for(int col = 0; col < m_DataModel.GetColCount() ; ++col)
	{
		//auto _title = m_DataModel.GetColTitle(col);
		//CString title( _title.c_str() );
		m_ListCtrl.InsertColumn(col, L"Hidden", LVCFMT_LEFT, -1);
		//m_ListCtrl.InsertColumn(col, L"HiddenFilename", LVCFMT_LEFT, 100);
		//m_ListCtrl.InsertColumn(col, L"HiddenStatus", LVCFMT_LEFT, 300);
		//m_ListCtrl.InsertColumn(col, L"HiddenState", LVCFMT_LEFT, 500);
	}
	//m_ListCtrl.SetTilesViewLinesCount(1);
// 	m_ListCtrl.SetTilesViewLinesCount(1);
// 	UINT arrColumns[2] = { 1};
// 	for (auto item{ 0 }, end{ m_ListCtrl.GetItemCount() }; item < end; ++item)
// 	{
// 		m_ListCtrl.SetItemTileLines(item, arrColumns, 1);
// 	}
/*
m_wndListCtrl_->InsertItem(itemNo, repoNameShort, 2);
m_wndListCtrl_->SetItemText(itemNo, 1, _T("Unpublished commits: 1"));
m_wndListCtrl_->SetItemText(itemNo, 2, _T("Changes: 20"));
m_wndListCtrl_->SetItemText(itemNo, 3, _T("Ahead of Master: 17"));
m_wndListCtrl_->SetItemText(itemNo, 4, repoName);
*/
	// Insert data into list-control by copying from datamodel
	//int nItem = 0;
	auto records = m_DataModel.GetRecords();
	//auto cnt = m_ListCtrl.GetItemCount();
	for(size_t rowId = 0, end = records.size(); rowId < end; ++rowId)
	{
		//CString value( m_DataModel.GetCellText(rowId, 0).c_str() );
		//rowId = m_ListCtrl.InsertItem(++rowId, value,0);
		//m_ListCtrl.SetItemData(rowId, rowId);
		auto file_name = records[rowId].file_name_;
		//CString c_file_name(file_name/*.c_str()*/);
		git_delta_t image_inx = get_item_state(file_name);
		m_ListCtrl.InsertItem(rowId, file_name, image_inx);
		//cnt = m_ListCtrl.GetItemCount();
		//auto _status = records[rowId].status_;
		//CString c_status(_status.c_str());
		//m_ListCtrl.SetItemText(rowId, 1 , c_status);
		//cnt = m_ListCtrl.GetItemCount();
		//auto _state = records[rowId].state_;
		//CString c_state(_state.c_str());
		//m_ListCtrl.SetItemText(rowId, 2,c_state);
		//cnt = m_ListCtrl.GetItemCount();
		//
		//auto fn = m_ListCtrl.GetItemText(rowId, 0);
		//fn = m_ListCtrl.GetItemText(rowId, 1);
		//fn = m_ListCtrl.GetItemText(rowId, 2);
		
// 		for(int col = 0; col < m_DataModel.GetColCount() ; ++col)
// 		{
// 			CString value(records[] );
// 			m_ListCtrl.SetItemText(nItem, col, value);
// 		}
// 		++nItem;
	}
	m_ListCtrl.GroupByColumn(0);
	m_ListCtrl.AdjustColumnWidth();
	//the below disables headers
	//m_ListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON | LVS_SORTASCENDING/* | LVS_REPORT*/);
	//m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_TRACKSELECT | LVS_EX_BORDERSELECT | LVS_EX_CHECKBOXES);
	auto main_frame_p = static_cast<CMainFrame*>(m_pParentWnd);
	auto branchName = main_frame_p->get_current_branch();
	branch_name_.SetWindowText(branchName);
	commit_btn.EnableWindow(FALSE);

	//m_menu.LoadMenu(IDR_MENU_ADD_FILES_COMMIT);
	//add_menu_btn_.m_hMenu = m_menu.GetSubMenu(0)->GetSafeHmenu();
	//add_menu_btn_.m_bDefaultClick = TRUE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListCtrl_Category_GroupsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListCtrl_Category_GroupsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListCtrl_Category_GroupsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// GIT_Commit_Data CListCtrl_Category_GroupsDlg::get_commit_data()
// {
// 	GIT_Commit_Data commit_data;
// 	std::set<CString> files = get_selected_files_();
// 	return commit_data;
// 
// }

// std::set<CString> CListCtrl_Category_GroupsDlg::get_selected_files_()
// {
// 	std::set<CString> selected_files = ;
// 	
// 	//auto recs = m_DataModel.GetRecords();
// 	return selected_files;
// }

//#include "GIT_Engine.hpp"

std::set<CString> CListCtrl_Category_GroupsDlg::get_checked_items()
{
	return m_ListCtrl.get_checked_items();
}

void CListCtrl_Category_GroupsDlg::set_items_checked(const std::set<CString>& fileNames)
{
	for (const auto& item : fileNames)
	{
		m_ListCtrl.set_item_checked(item);
	}
}

void CListCtrl_Category_GroupsDlg::OnBnClickedCommit()
{
	CDialog::OnOK();
	auto checked_files = m_ListCtrl.get_checked_items();
	//CString msg_header;
	//commit_header_edt_.GetWindowText(msg_header);
	CString msg_body;
	commit_body_edt_.GetWindowText(msg_body);
	//CString commit_msg = msg_header + L"\n" + msg_body;

	if (m_pParentWnd)
	{
		CMainFrame* main_frame_p = static_cast<CMainFrame*>(m_pParentWnd);
		main_frame_p->begin_create_commit(std::move(checked_files),msg_body);
	}
	// TODO: Add your control notification handler code here
}
// void CListCtrl_Category_GroupsDlg::setBranchName(const CString& branchName)
// {
// 	auto pWnd = GetDlgItem(IDC_BRANCH_NAME);
// 	if (pWnd)
// 	{
// 		SetDlgItemText(IDC_BRANCH_NAME, L"Hello");
// 	}
// }
void CListCtrl_Category_GroupsDlg::populate_view(const Working_Dir&& workingDir)
{
	m_DataModel.fill_model(std::forward<const Working_Dir>(workingDir));
}

void CListCtrl_Category_GroupsDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


// void CListCtrl_Category_GroupsDlg::OnBnClicked_Add_Files_Commit_Dlg()
// {
// 	// TODO: Add your control notification handler code here
// 	switch (add_menu_btn_.m_nMenuResult)
// 	{
// 	case ID_ITEM1_BYTYPE:
// 	{
// 		stage_items_by_type();
// 		int a{ 0 };
// 	}
// 		break;
// 	case ID_ITEM1_BYSTAGE:
// 	{
// 		int b{ 0 };
// 	}
// 		break;
// 	case ID_ITEM1_BYSTATUS:
// 	{
// 		int b{ 0 };
// 	}
// 	break;
// 	default:
// 	{
// 		int c{ 0 };
// 		stage_checked_items();
// 	}
// 
// 	}
// 	
// }

// void CListCtrl_Category_GroupsDlg::stage_items_by_type()
// {
// 	//Show dialog with type available
// 	//From those ticked stage_checked_items
// }

void CListCtrl_Category_GroupsDlg::stage_checked_items()
{//roll it in with unstage
	//1.get checked items
	auto checked_items = get_checked_items();
	//2.add those items to stage
	set_status_to_staged_(checked_items);
	m_ListCtrl.GroupByColumn(0);
}
void CListCtrl_Category_GroupsDlg::unstage_checked_items()
{
	//1.get checked items
	auto checked_items = get_checked_items();
	//2.add those items to stage
	set_status_to_unstaged_(checked_items);//but change it to previous status
	m_ListCtrl.GroupByColumn(0);
}
void CListCtrl_Category_GroupsDlg::OnBnClickedAddToStageButton()
{
	// TODO: Add your control notification handler code here
	//read first from edit box
	check_items();
	stage_checked_items();
}

void CListCtrl_Category_GroupsDlg::set_status_to_staged_(const std::set<CString>& checkedFiles)
{
	m_DataModel.set_status_to_staged(checkedFiles);
}

void CListCtrl_Category_GroupsDlg::set_status_to_unstaged_(const std::set<CString>& checkedFiles)
{
	m_DataModel.set_status_to_unstaged(checkedFiles);
}

// void CListCtrl_Category_GroupsDlg::OnItem1Click()
// {
// 	// TODO: Add your command handler code here
// 	int a{ 0 };
// }

static void ArtieExtractSubStrings(std::set<CString>& substrings,const CString& fullStr, const CString& separator)
{
	CT2CA pszConvertedAnsiString(fullStr);
	std::string std_fullString(pszConvertedAnsiString);
	CT2CA pszConvertedAnsiString_Separator(separator);
	std::string std_separator(pszConvertedAnsiString_Separator);
	//remove surplus spaces
	auto beg = std::unique(std::begin(std_fullString), std::end(std_fullString), [](char left, char right) 
	{
		return (left == ' ') && (left == right); 
	});
	std_fullString.erase(beg, std::end(std_fullString));
	
	for (auto beg{ std::begin(std_fullString) }, end{(std::end(std_fullString))};
		beg != end;
		++beg)
	{
		
		auto next_separator = std::find(beg, end, std_separator[0]);
		
		std::string substring(beg, next_separator);
		beg = next_separator;
		CA2W w_str(substring.c_str());
		substrings.insert(CString(w_str));
		
		if (beg == end)
		{
			return;
		}
	}
}

std::set<CString> CListCtrl_Category_GroupsDlg::get_unchecked_items_()
{
	auto unchecked_items = m_ListCtrl.get_unchecked_items();
	return unchecked_items;
} 

void CListCtrl_Category_GroupsDlg::translate_from_wildcard_to_regex(std::set<CString>& combinedRegex, const std::set<CString>& wildCards)
{
	//CString wild_star(L"*");
	//CString regex_star(L".*");
	for (const auto& c_str : wildCards)
	{
		
		
		CT2CA pszConvertedAnsiString(c_str);
		// construct a std::string using the LPCSTR input
		std::string std_combined_regex(pszConvertedAnsiString);
		boost::replace_all(std_combined_regex, std::string("."), std::string("\\."));
		boost::replace_all(std_combined_regex, std::string("*"), std::string(".*"));
		boost::replace_all(std_combined_regex, std::string("?"), std::string("."));
		std_combined_regex.insert(cbegin(std_combined_regex),'^');
		std_combined_regex.push_back('$');
		
		CA2W w_str(std_combined_regex.c_str());
		combinedRegex.insert(CString(w_str));
	}
}

void CListCtrl_Category_GroupsDlg::parse_regex(std::set<CString>& files_names,const std::set<CString>& regexes)
{
	CString combined_regex;
	for (const auto& _str : regexes)
	{
		combined_regex += _str + L"|";
	}
	combined_regex.Delete(combined_regex.GetLength() - 1);//delete final L"|"
	CT2CA pszConvertedAnsiString(combined_regex);
	// construct a std::string using the LPCSTR input
	std::string std_combined_regex(pszConvertedAnsiString);
	
	boost::regex reg(std_combined_regex);
	boost::smatch what;


	std::set<CString> unchecked_items = get_unchecked_items_();
	for (const auto& _file_name : unchecked_items)
	{
		CT2CA pszConvertedAnsiString(_file_name);
		// construct a std::string using the LPCSTR input
		std::string std_file_name(pszConvertedAnsiString);
			//check if that file matches regex and if so:
			if (boost::regex_match(std_file_name,what,reg))
			{
				files_names.insert(_file_name);
			}
		
	}
}

void CListCtrl_Category_GroupsDlg::check_items()
{
	CString str;
	regex_edit_box.GetWindowText(str);

	CString separator{L" "};
	std::set<CString> wildcards;
	ArtieExtractSubStrings(wildcards, str, separator);

	std::set<CString> regexes;
	translate_from_wildcard_to_regex(regexes,wildcards);

	std::set<CString> files_names;
	parse_regex(files_names,regexes);

	set_items_checked(files_names);
}


void CListCtrl_Category_GroupsDlg::OnBnClickedRemoveFromStageButton()
{
	check_items();
	unstage_checked_items();
}
