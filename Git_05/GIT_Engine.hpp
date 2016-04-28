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
	static void list_commits_for_branch(git_repository*repo,const std::string& repo_path,  const std::string& branch, std::vector<GIT_Commit_Local>& commitsForBranch);
	static void list_local_branches(git_repository *repo, std::vector<std::string>& localBranches);
	static void get_repo(const std::string& repo_path, git_repository** repo);
	static void get_commits_for_branches(const std::string& repo_path,std::map<std::string, std::vector<GIT_Commit_Local>>& branchCommits);
};

