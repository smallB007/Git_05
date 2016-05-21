// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "WorkSpaceBar3.h"
#include "WorkSpaceBar4.h"
#include "OutputBar.h"
#include "Git_05_PropertiesViewBar.h"

class CMainFrame : public CBCGPFrameWnd
{
	friend class HTTPS_GIT_Client;
	friend class CGit_05App;
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	CBitmap& GetAppPreview() { return m_bmpAppPreview; }

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPRibbonStatusBar	m_wndStatusBar;
	CBCGPRibbonBar			m_wndRibbonBar;
	
	CWorkSpaceBar3			m_wndWorkSpace3;
	CWorkSpaceBar4			m_wndWorkSpace_UntrackedFiles_;
	CWorkSpaceBar4			m_wndWorkSpace_Repos_;
	CWorkSpaceBar4			m_wndWorkSpace_Commits_;
	CWorkSpaceBar4			m_wndWorkSpace_Git_Tree_;
	COutputBar				m_wndOutput;
	Git05_CBCGPPropBar		m_wndCommitPropertiesGrid;
public:
	void setup_git_branches_combo_(const std::vector<CString>& branches);
	//void selectRepository();
	void set_current_repo(const CString& repoName);
	void select_active_repo();
	CString get_current_repo()const;
	CString get_current_branch()const;
	CString get_current_commit()const;
	GIT_Commit_Local get_commit(const CString& currentRepo, const CString& currentBranch, const CString& sha);
	void set_info_for_commit(const CString& commitId);
	void set_branches_for_repo(const CString& repoName);
	void display_info_for_diffed_file(const CString& sha, const CString& fileName);
	void reset_view()const;
	void deselect_file_from_commit_detail_window();
private:
	
	//CGit_05App* get_main_app_()const { return static_cast<CGit_05App*>(AfxGetApp()); };//copy from BackStagePageInfo.h
	void setup_ribbon_background_();
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnRibbonCustomize (WPARAM wp, LPARAM lp);
	afx_msg void OnToolsOptions();
	afx_msg void OnViewWorkspace_Git_Tree2();
	afx_msg void OnUpdateViewWorkspace_Git_Tree2(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace3();
	afx_msg void OnUpdateViewWorkspace3(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspaceRepos();
	afx_msg void OnUpdateViewWorkspaceRepos(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspaceCommits();
	afx_msg void OnUpdateViewWorkspaceCommits(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace_Git_Tree();
	afx_msg void OnUpdateViewWorkspace_Git_Tree(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace_Staging_Area();
	afx_msg void OnUpdateViewWorkspace_Staging_Area(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnViewPropGrid();
	afx_msg void OnUpdateViewPropGrid(CCmdUI* pCmdUI);
	afx_msg LRESULT OnBeforeShowRibbonBackstageView(WPARAM wp, LPARAM lp);
	afx_msg void OnProgressBar();
	afx_msg void OnPaletteTheme();
	afx_msg void OnCbn_Git_Tree_Branches_SelchangeCombo();
	afx_msg void OnCbn_Git_View_Repo_SelchangeCombo();
	DECLARE_MESSAGE_MAP()

	BOOL CreateRibbonBar ();
	void ShowOptions (int nPage);

	CBitmap m_bmpAppPreview;
private:
	//_ui_ CComboBox git_tree_branches_;
	//CBCGPRibbonComboBox* branches_cmb_{nullptr};
	//CBCGPRibbonComboBox* repo_view_cmb_{ nullptr };
	CBCGPRibbonComboBox* get_branches_cmb_()const;
	CBCGPRibbonComboBox* get_repo_view_cmb_()const;
	CString setup_repo_view_cmb_();
	void set_info_for_working_dir_(const CString& view_type);
protected:
	//void DoDataExchange(CDataExchange* pDX);
	
};
