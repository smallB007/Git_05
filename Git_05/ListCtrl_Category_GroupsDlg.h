// ListCtrl_Category_GroupsDlg.h : header file
//

#pragma once
#include "resource.h"
#include "afxcmn.h"

#include "CListCtrl_Category_Groups.h"
#include "CListCtrl_DataModel.h"

#include "GIT_Commit_Data.hpp"
#include "afxwin.h"

// CListCtrl_Category_GroupsDlg dialog
class CListCtrl_Category_GroupsDlg : public CDialog
{
// Construction
public:
	CListCtrl_Category_GroupsDlg(CWnd* pParent = NULL);	// standard constructor
	//GIT_Commit_Data get_commit_data();
// Dialog Data
	enum { IDD = IDD_LISTCTRL_CATEGORY_GROUPS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCommit();
	DECLARE_MESSAGE_MAP()
private:
	CListCtrl_Category_Groups m_ListCtrl;
	CListCtrl_DataModel m_DataModel;
	CImageList m_cImageListNormal;

	private:
		//std::set<CString> get_selected_files_();
public:
private:
	CEdit commit_header_edt_;
	CEdit commit_body_edt_;
};
