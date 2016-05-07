#include "stdafx.h"
#include "Git_05_CBCGPPropList.hpp"

#include "BCGPPropList.h"

IMPLEMENT_DYNAMIC(Git_05_CBCGPPropList, CObject)

 BEGIN_MESSAGE_MAP(Git_05_CBCGPPropList, CBCGPPropList)
 ON_WM_LBUTTONDOWN()
 END_MESSAGE_MAP()


 void Git_05_CBCGPPropList::insertGroup(CBCGPProp * prop, std::vector<CBCGPProp*> vec)
 {
	 prop_values_[prop] = vec;
 }

 Git_05_CBCGPPropList::Git_05_CBCGPPropList()
{
}


Git_05_CBCGPPropList::~Git_05_CBCGPPropList()
{
}

void Git_05_CBCGPPropList::OnLButtonDown(UINT nFlags, CPoint point)
{

	static int a{ 0 };
	CBCGPProp::ClickArea clickArea;
	CBCGPProp* pHit = HitTest(point, &clickArea);
	//pHit->classCBCGPProp
	//if (a != 1)
	//{
	//	a = 1;
	//	auto prop = prop_values_.find(pHit);
	//	if (prop != cend(prop_values_))
	//	{
	//		pHit->RemoveAllSubItems();
	//		auto values = prop_values_[pHit];
	//		for (const auto& v : values)
	//		{
	//			pHit->AddSubItem(v);
	//		}
	//	}
	//	auto nm = pHit->GetName();
	//}
	//else
	//{
	//	pHit->RemoveAllSubItems();
	//	auto values = prop_values_[pHit];
	//	auto p = new CBCGPProp(_T("File"), (_variant_t)0,
	//		_T("Modified file"));
	//	pHit->AddSubItem(p);
	//	a = 0;
	//}
	CBCGPPropList::OnLButtonDown(nFlags, point);
	//pHit->RemoveAllSubItems();
}
