// ListCtrl_Category_GroupsDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "ListCtrl_Category_GroupsApp.h"
#include "ListCtrl_Category_GroupsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

// class CAboutDlg : public CDialog
// {
// public:
// 	CAboutDlg();
// 
// // Dialog Data
// 	enum { IDD = IDD_ABOUTBOX };
// 
// 	protected:
// 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// 
// // Implementation
// protected:
// 	DECLARE_MESSAGE_MAP()
// };
// 
// CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
// {
// }
// 
// void CAboutDlg::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialog::DoDataExchange(pDX);
// }
// 
// BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
// END_MESSAGE_MAP()


// CListCtrl_Category_GroupsDlg dialog



CListCtrl_Category_GroupsDlg::CListCtrl_Category_GroupsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrl_Category_GroupsDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListCtrl_Category_GroupsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_EDIT2, commit_header_edt_);
	DDX_Control(pDX, IDC_EDIT1, commit_body_edt_);
}

BEGIN_MESSAGE_MAP(CListCtrl_Category_GroupsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	ON_BN_CLICKED(ID_COMMIT, &CListCtrl_Category_GroupsDlg::OnBnClickedCommit)
END_MESSAGE_MAP()


// CListCtrl_Category_GroupsDlg message handlers

BOOL CListCtrl_Category_GroupsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWinApp* pApp = AfxGetApp();
	VERIFY(m_cImageListNormal.Create(20, 20, ILC_COLOR32, 0, 0));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_GREEN));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_RED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_BW));
	m_ListCtrl.SetImageList(&m_cImageListNormal, LVSIL_SMALL);
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
		auto _title = m_DataModel.GetColTitle(col);
		CString title( _title.c_str() );
		m_ListCtrl.InsertColumn(col, title, LVCFMT_LEFT, 300);
		m_ListCtrl.InsertColumn(col, L"HiddenFilename", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(col, L"HiddenStatus", LVCFMT_LEFT, 300);
		m_ListCtrl.InsertColumn(col, L"HiddenState", LVCFMT_LEFT, 500);
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
	auto cnt = m_ListCtrl.GetItemCount();
	for(size_t rowId = 0, end = records.size(); rowId < end; ++rowId)
	{
		//CString value( m_DataModel.GetCellText(rowId, 0).c_str() );
		//rowId = m_ListCtrl.InsertItem(++rowId, value,0);
		//m_ListCtrl.SetItemData(rowId, rowId);
		auto file_name = records[rowId].file_name_;
		CString c_file_name(file_name.c_str());
		m_ListCtrl.InsertItem(rowId, c_file_name, 0);
		cnt = m_ListCtrl.GetItemCount();
		auto _status = records[rowId].status_;
		CString c_status(_status.c_str());
		m_ListCtrl.SetItemText(rowId, 1 , c_status);
		cnt = m_ListCtrl.GetItemCount();
		auto _state = records[rowId].state_;
		CString c_state(_state.c_str());
		m_ListCtrl.SetItemText(rowId, 2,c_state);
		cnt = m_ListCtrl.GetItemCount();
		
		auto fn = m_ListCtrl.GetItemText(rowId, 0);
		fn = m_ListCtrl.GetItemText(rowId, 1);
		fn = m_ListCtrl.GetItemText(rowId, 2);
		
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
#include "MainFrm.h"
void CListCtrl_Category_GroupsDlg::OnBnClickedCommit()
{
	CDialog::OnOK();
	auto checked_files = m_ListCtrl.get_checked_items();
	CString msg_header;
	commit_header_edt_.GetWindowText(msg_header);
	CString msg_body;
	commit_body_edt_.GetWindowText(msg_body);
	CString commit_msg = msg_header + L"\n" + msg_body;

	if (m_pParentWnd)
	{
		CMainFrame* main_frame_p = static_cast<CMainFrame*>(m_pParentWnd);
		main_frame_p->begin_create_commit(std::move(checked_files),commit_msg);
	}
	// TODO: Add your control notification handler code here
}

void CListCtrl_Category_GroupsDlg::populate_view(const Working_Dir&& workingDir)
{
	m_DataModel.fill_model(std::forward<const Working_Dir>(workingDir));
}