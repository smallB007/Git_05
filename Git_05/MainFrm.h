// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "WorkSpaceBar3.h"
#include "WorkSpaceBar4.h"
#include "OutputBar.h"
#include "PropertiesViewBar.h"

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
	CWorkSpaceBar4			m_wndWorkSpace_Repos_;
	CWorkSpaceBar4			m_wndWorkSpace_Git_Tree_;
	COutputBar				m_wndOutput;
	CBCGPPropBar			m_wndPropGrid;
public:
	void setup_git_branches_combo_(const std::vector<CString>& branches);
	void selectRepository();
private:
	
	CGit_05App* get_main_app_()const { return static_cast<CGit_05App*>(AfxGetApp()); };//copy from BackStagePageInfo.h
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
	afx_msg void OnViewWorkspace4();
	afx_msg void OnUpdateViewWorkspace4(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace41();
	afx_msg void OnUpdateViewWorkspace41(CCmdUI* pCmdUI);
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
private:
	//_ui_ CComboBox git_tree_branches_;


protected:
	//void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnCbn_Git_Tree_Branches_SelchangeCombo();
};
