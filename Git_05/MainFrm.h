// MainFrm.h : interface of the CMainFrame class
//


#pragma once
//#include "WorkSpaceBar.h"
//#include "WorkSpaceBar2.h"
#include "WorkSpaceBar3.h"
#include "WorkSpaceBar4.h"
#include "OutputBar.h"
#include "PropertiesViewBar.h"
//#include "WorkSpaceBar_Git_Tree.h"
#include "WorkSpaceBar_Git_Tree2.h"

class CMainFrame : public CBCGPFrameWnd
{
	
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
	//CBCGPMenuBar			m_wndMenuBar;
	//CBCGPToolBar			m_wndToolBar;//this and this^ is unnecessary as we have a ribbon but for now leave it in case we want to give user the ability to switch
	//CWorkSpaceBar			m_wndWorkSpace;
	//CWorkSpaceBar2			m_wndWorkSpace2;
	CWorkSpaceBar3			m_wndWorkSpace3;
	CWorkSpaceBar4			m_wndWorkSpace4;
	CWorkSpaceBar4			m_wndWorkSpace41;
	COutputBar				m_wndOutput;
	CBCGPPropBar			m_wndPropGrid;
	//CWorkSpaceBar_Git_Tree wnd_workspace_git_tree_;
	//CWorkSpaceBar_Git_Tree2 wnd_workspace_git_tree_2;
private:
	
	CGit_05App* get_main_app_()const { return static_cast<CGit_05App*>(AfxGetApp()); };//copy from BackStagePageInfo.h
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnRibbonCustomize (WPARAM wp, LPARAM lp);
	afx_msg void OnToolsOptions();
	afx_msg void OnViewWorkspace_Git_Tree2();
	afx_msg void OnUpdateViewWorkspace_Git_Tree2(CCmdUI* pCmdUI);
	//afx_msg void OnViewWorkspace();
	//afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	//afx_msg void OnViewWorkspace2();
	//afx_msg void OnUpdateViewWorkspace2(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnViewPropGrid();
	afx_msg void OnUpdateViewPropGrid(CCmdUI* pCmdUI);
	afx_msg LRESULT OnBeforeShowRibbonBackstageView(WPARAM wp, LPARAM lp);
	afx_msg void OnProgressBar();
	afx_msg void OnPaletteTheme();
	
	DECLARE_MESSAGE_MAP()

	BOOL CreateRibbonBar ();
	void ShowOptions (int nPage);

	CBitmap m_bmpAppPreview;
};
