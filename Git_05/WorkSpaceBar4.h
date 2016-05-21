// workspacebar.h : interface of the CWorkSpaceBar4 class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DemoListView.hpp"
#include "Direct2DRenderer.hpp"

class GIT_Commit_Local;
class Git_05_ListCtr;
class CGit_05App;

class CWorkSpaceBar4 : public CBCGPDockingControlBar
{
public:
	typedef CString repo_name_t;
	typedef CString branch_name_t;
private:
	//std::map<std::string, std::vector<GIT_Commit_Local>> branch_commits_;
	std::map<repo_name_t, std::map<branch_name_t, std::vector<GIT_Commit_Local>>> repo_branches_;
public:

	enum EVIEW_TYPE{LIST_CTRL,DX_RENDERER};
	
private:
	std::unique_ptr<Direct2DRenderer> renderer_;
	HTREEITEM hRoot_;
	CImageList m_cImageListNormal, m_cImageListSmall, m_cImageListShell;
	EVIEW_TYPE eview_type_{ LIST_CTRL };
	SHFILEINFO shfi;//for icons of system files like cpp etc
	int create_list_ctrl_();
	int add_repo_to_list_ctrl_(repo_name_t repoName, repo_name_t repoNameShort);
	int add_commit_to_list_ctrl_(const GIT_Commit_Local& commit);
	void add_branches_to_combo_(const std::map<branch_name_t, std::vector<GIT_Commit_Local>>& branch_commits);

	void load_system_icons_();

	BOOL _SetTilesViewLinesCount(int nCount);
	BOOL _SetItemTileLines(int nItem, UINT* parrColumns, UINT nCount);
public:
	int add_untracked_files_to_list_ctrl_(repo_name_t repoName);//TODO move that to private
	CWorkSpaceBar4();
	virtual ~CWorkSpaceBar4();
	void set_view_type(EVIEW_TYPE view_type);
	int set_type_list_ctrl_untracked_files();
	int set_type_list_ctrl_commits();
	int set_type_list_ctrl_repos();
	void git_tree(decltype(repo_branches_)&& repoBranches);
	void set_branches_for_repo(const CString& repoName);
	std::vector<branch_name_t> get_branches_for_repo(const CString& repoName);
	
	void set_commits_for_branch(const CString & repoName, const CString& branchName);
	void set_commits(const std::vector<GIT_Commit_Local>& commits);
	std::vector<GIT_Commit_Local> get_commits_for_branch(const CString & repoName, const CString& branchName);
		
	CString get_current_item()const;
	void set_active_repo();
	void set_current_repo(const repo_name_t& repoName);
	CString get_current_repo()const;
	CString get_current_branch()const;
	CString get_current_commit()const;
	GIT_Commit_Local get_commit(const CString & repoName, const CString & branchName, const CString& commitId);
private:
	
	// Attributes
protected:
	//CBCGPTreeCtrl m_wndTree;
	//CListCtrl& listCtrl;
	std::unique_ptr<Git_05_ListCtr> m_wndListCtrl_{nullptr};
	//CDemoListView m_listView;
	// Operations
	//virtual void DoDataExchange(CDataExchange* pDX)override;
public:

	// Overrides

	// Implementation
public:
	

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnLButtonDown(UINT, CPoint);
	
#ifdef MFC_DIRECT_2D
	afx_msg LRESULT OnDrawDirect2D(WPARAM wParam, LPARAM lParam);
#else
	afx_msg void OnPaint();
#endif
	DECLARE_MESSAGE_MAP()
private:
	void fill_repositories_();
	//CGit_05App* get_main_app_()const;
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};

/////////////////////////////////////////////////////////////////////////////
