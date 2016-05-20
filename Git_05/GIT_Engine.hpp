#pragma once
#include <map>
#include <set>
#include <string>
#include "GIT_Commit_Local.hpp"




class GIT_Engine
{
public:
	static const int GIT_SUCCESS{ 0 };
	GIT_Engine();
	~GIT_Engine();

private:
	static diffed_file_t a_file;
	static std::set<diffed_file_t, Less_Diff_File> diffed_files_;
	//static std::map<git_diff_hunk, std::vector<git_diff_line>,Less_hunk> hunk_lines_;
	//static std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> modified_files_;
	//static std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> added_files_;
	//static std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> deleted_files_;
	static int git_diff_line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload);
	static int git_diff_file_cb(const git_diff_delta *delta, float progress, void *payload);
	static int git_diff_hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload);
	static int payload_fn(const git_diff_delta *delta);
	//static int get_files_from_git_diff(const git_diff_delta *delta, std::set<git_diff_file, Less_Diff_File>& files,  const git_delta_t delta_t);
public:
	struct git_init_opts_t {
		int no_options;
		int quiet;
		int bare;
		int initial_commit;
		uint32_t shared;
		const char *a_template;
		const char *gitdir;
		const char *dir;
	};
public:
	static void list_commits_for_branch(git_repository*repo,const CString& repo_path,  const CString& branch, std::vector<GIT_Commit_Local>& commitsForBranch);
	static void list_local_branches(git_repository *repo, std::vector<CString>& localBranches);
	static void get_repo(const CString& repo_path, git_repository** repo);
	static void get_commits_for_branches(const CString& repo_path,std::map<CString, std::vector<GIT_Commit_Local>>& branchCommits);
	static bool check_if_repo(const CString& pathName);
	static bool git_init(git_repository *repo, const char* path, const git_init_opts_t& initOptions);
	static void create_initial_commit(git_repository *repo);

};

