// Commit_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Git_05.h"
#include "Commit_dlg.h"
#include "afxdialogex.h"


// Commit_dlg dialog

IMPLEMENT_DYNAMIC(Commit_dlg, CDialogEx)

Commit_dlg::Commit_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMIT_DIALOG, pParent)
{
	//Create(IDD_COMMIT_DIALOG, pParent);
}

Commit_dlg::~Commit_dlg()
{
}

void Commit_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMMIT_CHANGES_TAB, tab_ctrl_);
	DDX_Control(pDX, IDC_LIST1, commit_listctrl_);
}


BEGIN_MESSAGE_MAP(Commit_dlg, CDialogEx)
	//ON_NOTIFY(TCN_SELCHANGE, IDC_COMMIT_CHANGES_TAB, &Commit_dlg::OnTcnSelchangeCommitChangesTab)
END_MESSAGE_MAP()


// Commit_dlg message handlers


void Commit_dlg::OnTcnSelchangeCommitChangesTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


BOOL Commit_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_imageTab.Create(IDB_IMAGES_NORMAL, 8, 16, RGB(255, 255, 255));
	CreateSingleGroup(0, 1, L"Some group");
// 	tab_ctrl_.SetImageList(&m_imageTab);
// 	TCITEM tcItem;
// 	tcItem.mask = TCIF_IMAGE;
// 	for (int i = 0; i < 3; i++)
// 	{
// 		tcItem.iImage = i;
// 		tab_ctrl_.SetItem(i, &tcItem);
// 		tab_ctrl_.InsertItem(i, &tcItem);
// 	}
// 
// 	tcItem.mask = TCIF_TEXT;
// 	//for (int i = 0; i < 3; i++)
// 	//{
// 		tcItem.pszText = L"To Commit";
// 		tab_ctrl_.SetItem(0, &tcItem);
// 		tcItem.pszText = L"Unstaged";
// 		tab_ctrl_.SetItem(1, &tcItem);
// 		tcItem.pszText = L"Untracked";
// 		tab_ctrl_.SetItem(2, &tcItem);
		//tab_ctrl_.InsertItem(i, &tcItem);
	//}
	// TODO:  Add extra initialization here
// 	TCITEM tab;
// 	tab.mask = TCIF_TEXT;
// 	tab.pszText = L"Tabus";
// 	
// 	tab_ctrl_.InsertItem(0, &tab);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT Commit_dlg::CreateSingleGroup(int nIndex,int nGroupId, const CString& strHeader)
{
	commit_listctrl_.EnableGroupView(TRUE);

	LVGROUP lg = { 0 };
	lg.cbSize = sizeof(lg);
	lg.iGroupId = nGroupId;
	lg.state = LVGS_NORMAL;
	lg.mask = LVGF_GROUPID | LVGF_HEADER | LVGF_STATE | LVGF_ALIGN;
	lg.uAlign = LVGA_HEADER_LEFT;

	// Header-title must be unicode (Convert if necessary)
	lg.pszHeader = L"group 1";//strHeader.GetBuffer();
	lg.cchHeader = 7;//strHeader.GetLength();
	nGroupId = commit_listctrl_.InsertGroup(nIndex, &lg);
	if (nGroupId == -1)
		return nGroupId;

	// Insert all current items into this group
	for (int nRow = 0; nRow < 5/*GetItemCount()*/; ++nRow)
	{
		LVITEM lvItem = { 0 };
		lvItem.mask = LVIF_GROUPID;
		lvItem.iItem = nRow;
		lvItem.iSubItem = 0;
		lvItem.iGroupId = nGroupId;
		commit_listctrl_.SetItem(&lvItem);
	}
}