#pragma once
class CBCGPPropList;
#include <vector>
class Git_05_CBCGPPropList :
	public CBCGPPropList
{
	DECLARE_DYNAMIC(Git_05_CBCGPPropList)
	std::map<CBCGPProp*, std::vector<CBCGPProp*>> prop_values_;
public:
	void insertGroup(CBCGPProp* prop, std::vector<CBCGPProp*> vec);
	Git_05_CBCGPPropList();
	~Git_05_CBCGPPropList();
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

