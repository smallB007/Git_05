#pragma once
#include <string>
#include <set>
#include <vector>
#include <map>
#include "Working_Dir.hpp"



class CListCtrl_DataRecord
{
private:
	
	



	
	//CListCtrl_DataRecord()
	//{}
public:
	CListCtrl_DataRecord(const CString& fileName, const CString& status/*Staged,Unstaged,Untracked,Ignored*/,
		const git_delta_t state/*added,deleted,modified*/)
		:
		file_name_(fileName)
		,status_{status}
		,original_status_{status}
		,state_{state}
	{
		if (L"Changes staged for commit" == status_)
		{
			header_position_ = 0;
		}
	}

	CString	file_name_;
	CString	status_;
	int header_position_{ -1 };
	const CString original_status_;
	git_delta_t state_;
	//std::string	added_;
	//std::string	modified_;
	//std::string	deleted_;
	const CString get_file_name()const
	{
		return file_name_;
	}
	
	const CString get_file_status()const
	{
		return status_;
	}
	
	const CString get_file_original_status_()const
	{
		return original_status_;
	}

	void set_file_status(const CString& fileStatus)
	{
		status_ = fileStatus;
		if (L"Changes staged for commit" == status_)
		{
			header_position_ = 0;
		}
		else
		{
			header_position_ = -1;
		}
	}
	git_delta_t get_file_state()const
	{
		return state_;
	}
	int get_file_header_position()const
	{
		return header_position_;
	}

	//const std::string& GetCellText(int col, bool title) const
	//{
	//	static std::string col_title("Staging area");
	//	return col_title;
// 	//	switch(col)
// 	//	{
// 	//	case 0: { static std::string title0("Staging area"); return title ? title0 : file_name_; }
// 	//	//case 1: { static std::string title1("Unstaged"); return title ? title1 : status_; }
// 	//	//case 2: { static std::string title2("Untracked"); return title ? title2 : status_; }
// 	//	//case 3: { static std::string title3("Ignored"); return title ? title3 : status_; }
// 	//	
// 	//	default:{ static std::string emptyStr; return emptyStr; }
// 	//	}
	//}

	//int  GetColCount() const { return 1; }
};


struct Less_Data_Record
{
	bool operator()(const CListCtrl_DataRecord& left, const CListCtrl_DataRecord& right)
	{
		return left.file_name_ < right.file_name_;
	}
};

class CListCtrl_DataModel
{
	std::vector<CListCtrl_DataRecord> m_Records;
	int	m_LookupTime;
	int m_RowMultiplier;

public:
	CListCtrl_DataModel()
		:m_RowMultiplier(0)
		,m_LookupTime(0)
	{
		InitDataModel();
	}
	int get_item_header_position(const CString& fileName);
	
	CString get_item_status(const CString& fileName);
	git_delta_t get_item_state(const CString& fileName);
	void set_status_to_staged(const std::set<CString>& checkedFiles);
	void set_item_status_staged_(const CString& fileName);
	void set_status_to_unstaged(const std::set<CString>& checkedFiles);
	void set_item_status_unstaged_(const CString& fileName);
	void InitDataModel()
	{
		m_Records.clear();
		//m_Records.push_back( CListCtrl_DataRecord("list.cpp", "Staged", "added") );
		//m_Records.push_back( CListCtrl_DataRecord("alist.cpp", "Staged", "added") );
		//m_Records.push_back( CListCtrl_DataRecord(L"blist.cpp", L"Ignored", L"new file") );
		//m_Records.push_back( CListCtrl_DataRecord(L"clist.cpp", L"Staged", L"modified") );
		//m_Records.insert(CListCtrl_DataRecord("xlist.cpp", "Untracked", "deleted"));
		//m_Records.insert(CListCtrl_DataRecord("xlist.cpp", "Untracked", "deleted"));
		//m_Records.insert(CListCtrl_DataRecord("xlist.cpp", "Untracked", "deleted"));
		//m_Records.insert(CListCtrl_DataRecord("xlist.cpp", "Untracked", "deleted"));
		//m_Records.push_back(CListCtrl_DataRecord("xlist.cpp", "Untracked", "deleted"));
		//m_Records.push_back(CListCtrl_DataRecord("xlist.cpp", "Untracked", "deleted"));
		//m_Records.push_back(CListCtrl_DataRecord("xlist.cpp", "Untracked", "deleted"));
		//m_Records.push_back(CListCtrl_DataRecord("xlist.cpp", "Untracked", "deleted"));

		//if (m_RowMultiplier > 1)
		//{
		//	vector<CListCtrl_DataRecord> rowset(m_Records);
		//	m_Records.reserve((m_RowMultiplier-1) * rowset.size());
		//	for(int i = 0 ; i < m_RowMultiplier ; ++i)
		//	{
		//		m_Records.insert(m_Records.end(), rowset.begin(), rowset.end());
		//	}
		//}
	}
	void fill_model(const Working_Dir& workingDir);

	//const std::string& GetCellText(size_t lookupId, int col) const
	//{
	//	if (lookupId >= m_Records.size())
	//	{
	//		static const std::string oob("Out of Bound");
	//		return oob;
	//	}
	//	// How many times should we search sequential for the row ?
	//	for(int i=0; i < m_LookupTime; ++i)
	//	{
	//		for(size_t rowId = 0; rowId < m_Records.size(); ++rowId)
	//		{
	//			if (rowId==lookupId)
	//				break;
	//		}
	//	}
	//	//return m_Records.at(lookupId).GetCellText(col, false);
	//	auto ct = m_Records.at(lookupId).GetCellText(col, false);
	//	return m_Records.at(lookupId).GetCellText(col, false);
	//}

	size_t GetRowIds() const 
	{
		return m_Records.size(); 
	}
	
	int GetColCount() const 
	{ 
		//return CListCtrl_DataRecord().GetColCount(); 
		return 1;
	}
	//const std::string& GetColTitle(int col) const 
	//{ 
	//	return CListCtrl_DataRecord().GetCellText(col, true); 
	//}

	std::vector<CListCtrl_DataRecord> GetRecords()const
	{
		return m_Records; 
	}
	void SetLookupTime(int lookupTimes) { m_LookupTime = lookupTimes; }
	void SetRowMultiplier(int multiply)
	{
		if (m_RowMultiplier != multiply ) 
		{
			m_RowMultiplier = multiply; InitDataModel(); 
		}
	}
};