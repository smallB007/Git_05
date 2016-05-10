#pragma once
#include <map>
#include <set>
#include "GIT_Commit_Local.hpp"


class GIT_Engine
{
public:
	static const int GIT_SUCCESS{ 0 };
	GIT_Engine();
	~GIT_Engine();

private:
	static std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> modified_files_;
	static std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> added_files_;
	static std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> deleted_files_;
	static int file_cb(const git_diff_delta *delta, float progress, void *payload);
	static int payload_fn(const git_diff_delta *delta);
	static int get_files_from_git_diff(const git_diff_delta *delta, std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>>& files,  const git_delta_t delta_t);
	
public:
	static void list_commits_for_branch(git_repository*repo,const CString& repo_path,  const CString& branch, std::vector<GIT_Commit_Local>& commitsForBranch);
	static void list_local_branches(git_repository *repo, std::vector<CString>& localBranches);
	static void get_repo(const CString& repo_path, git_repository** repo);
	static void get_commits_for_branches(const CString& repo_path,std::map<CString, std::vector<GIT_Commit_Local>>& branchCommits);
	static bool check_if_repo(const CString& pathName);
};

