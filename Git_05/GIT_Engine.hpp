#pragma once
#include <map>

class GIT_Commit_Local;
class GIT_Engine
{
public:
	static const int GIT_SUCCESS{ 0 };
	GIT_Engine();
	~GIT_Engine();
public:
	static void list_commits_for_branch(git_repository*repo,const CString& repo_path,  const CString& branch, std::vector<GIT_Commit_Local>& commitsForBranch);
	static void list_local_branches(git_repository *repo, std::vector<CString>& localBranches);
	static void get_repo(const CString& repo_path, git_repository** repo);
	static void get_commits_for_branches(const CString& repo_path,std::map<CString, std::vector<GIT_Commit_Local>>& branchCommits);
	static bool check_if_repo(const CString& pathName);
};

