// workspacebar.h : interface of the CWorkSpaceBar4 class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DemoListView.hpp"
#include "Direct2DRenderer.hpp"
#include <memory>
#include "Git_05_ListCtr.hpp"
class GIT_Commit_Local;

class CWorkSpaceBar4 : public CBCGPDockingControlBar
{
public:
	typedef std::string repo_name_t;
	typedef std::string branch_name_t;
private:
	//std::map<std::string, std::vector<GIT_Commit_Local>> branch_commits_;
	std::map<repo_name_t, std::map<branch_name_t, std::vector<GIT_Commit_Local>>> repo_branches_;
public:

	enum EVIEW_TYPE{REPOS,GIT_TREE};
private:
	std::unique_ptr<Direct2DRenderer> renderer_;
	HTREEITEM hRoot_;
	CImageList m_cImageListNormal, m_cImageListSmall;
	EVIEW_TYPE eview_type_{ REPOS };
	int create_list_ctrl_();
	int add_repo_to_list_ctrl_(repo_name_t repoName);
	BOOL _SetTilesViewLinesCount(int nCount);
	BOOL _SetItemTileLines(int nItem, UINT* parrColumns, UINT nCount);
public:
	CWorkSpaceBar4();
	void set_view_type(EVIEW_TYPE view_type);
	void git_tree(decltype(repo_branches_)&& repoBranches);
	// Attributes
protected:
	//CBCGPTreeCtrl m_wndTree;
	//CListCtrl& listCtrl;
	std::unique_ptr<Git_05_ListCtr> m_wndListCtrl{nullptr};
	//CDemoListView m_listView;
	// Operations
	//virtual void DoDataExchange(CDataExchange* pDX)override;
public:

	// Overrides

	// Implementation
public:
	virtual ~CWorkSpaceBar4();

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
	CGit_05App* get_main_app_()const { return static_cast<CGit_05App*>(AfxGetApp()); };//copy from BackStagePageInfo.h
};

/////////////////////////////////////////////////////////////////////////////
