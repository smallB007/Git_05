#pragma once
#include <vector>
#include <map>

class Working_Dir
{
	//mutable std::map<git_status_t, std::vector<git_status_entry>> entries_by_status_;
	std::vector<git_status_entry> status_entries_;
	CString path_name_;
public:
	Working_Dir(const CString& pathName);
	~Working_Dir();
	void push_back(const git_status_entry* statusEntry)
	{
		status_entries_.push_back(*statusEntry);
	}
	std::map<git_status_t, std::vector<git_status_entry>> get_sorted_entries()const;
	std::map<git_status_t, std::vector<CString>> get_sorted_files()const;
};

