// PropertiesViewBar.h : interface of the Git05_CBCGPPropBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "BCGPPropList.h"
#include <memory>
class GIT_Commit_Local;
class Git05_CBCGPPropBar : public CBCGPDockingControlBar  
{
private:
	std::unique_ptr<CBCGPProp> commiter_group_uptr_;
	//std::unique_ptr<CBCGPProp> stats_group_uptr_;//cause of crash
	std::unique_ptr<CBCGPProp> pStatsGroup;
	std::shared_ptr<CBCGPProp> pModificationsGroup;
	std::shared_ptr<CBCGPProp> pAdditionsGroup;
	std::shared_ptr<CBCGPProp> pDeletionsGroup;
	std::unique_ptr<CBCGPProp> pSHAGroup;
	void assign_diffed_files_(const const GIT_Commit_Local& commitInfo);
// Construction
public:
	Git05_CBCGPPropBar();

	void AdjustLayout ();

// Attributes
public:
	void SetVSDotNetLook (BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook (bSet);
		m_wndPropList.SetGroupNameFullWidth (bSet);
	}
	void set_commit_info(const GIT_Commit_Local& commitInfo);
protected:
	CBCGPPropList	m_wndPropList;

	CBCGPColor		m_BorderColor;
	CBCGPBrush		m_FillBrush;
	CBCGPBrush		m_TextBrush;

// Attributes
protected:

// Overrides

// Implementation
public:
	virtual ~Git05_CBCGPPropBar();

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnCommandClicked(WPARAM, LPARAM);
	afx_msg LRESULT OnMenuItemSelected(WPARAM, LPARAM);
	afx_msg LRESULT OnPropertyGetMenuItemState(WPARAM /*wp*/, LPARAM /*lp*/);
	afx_msg LRESULT OnPropertySelected(WPARAM /*wp*/, LPARAM /*lp*/);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

	void InitPropList ();
public:
};
