#pragma once
#include "afxcmn.h"


// Commit_dlg dialog

class Commit_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Commit_dlg)

public:
	Commit_dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~Commit_dlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_COMMIT_DIALOG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeCommitChangesTab(NMHDR *pNMHDR, LRESULT *pResult);
private:
	//CTabCtrl tab_ctrl_;
	CImageList m_imageTab;
	LRESULT CreateSingleGroup(int nIndex, int nGroupId, const CString& strHeader);
public:
	virtual BOOL OnInitDialog();
private:
	CListCtrl commit_listctrl_;
};
