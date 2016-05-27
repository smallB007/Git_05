#pragma once
/*
Used by Commit_dlg in order to get what files need to be commited and to get msg for a commit
*/
#include <vector>
struct GIT_Commit_Data
{
	std::vector<CString> files_for_commit_;
	CString commit_msg_;
	GIT_Commit_Data() = default;
	~GIT_Commit_Data() = default;
};

