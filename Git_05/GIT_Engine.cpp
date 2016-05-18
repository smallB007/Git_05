#include "stdafx.h"
#include "GIT_Engine.hpp"
#include "GIT_Commit_Local.hpp"

//statics
diffed_file_t GIT_Engine::a_file;
std::set<diffed_file_t, Less_Diff_File> GIT_Engine::diffed_files_;

//std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> GIT_Engine::modified_files_;
//std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> GIT_Engine::added_files_;
//std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> GIT_Engine::deleted_files_;

//std::map<git_diff_hunk, std::vector<git_diff_line>, Less_hunk> GIT_Engine::hunk_lines_;
//end of statics

GIT_Engine::GIT_Engine()
{
}


GIT_Engine::~GIT_Engine()
{
}

// int GIT_Engine::get_files_from_git_diff(const git_diff_delta *delta, std::set<git_diff_file, Less_Diff_File>& files, const git_delta_t delta_t)
// {
// 	switch (delta_t)
// 	{
// 	case GIT_DELTA_MODIFIED:
// 		if (GIT_DELTA_MODIFIED == delta->status)
// 		{
// 			files.insert(delta->new_file);
// 		}
// 	break;
// 	case GIT_DELTA_ADDED:
// 		if (GIT_DELTA_ADDED == delta->status)
// 		{
// 			files.insert(delta->new_file);
// 		}
// 	break;
// 	case GIT_DELTA_DELETED:
// 		if (GIT_DELTA_DELETED == delta->status)
// 		{
// 			files.insert(delta->new_file);
// 		}
// 	break;
// 	}
// 	
// 	return 0;//must return zero otherwise execution of git_diff_file_cb will be terminated
// }

int GIT_Engine::git_diff_hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload)
{
	int a{ 0 };
	return 0;
}



int GIT_Engine::git_diff_line_cb(const git_diff_delta * delta, const git_diff_hunk * hunk, const git_diff_line * line, void * payload)
{
	
	//a_file.diff_file = delta->new_file;
	a_file.diff_delta = *delta;
	git_o5_diff_line_t a_git_05_line(*line);
	
	a_git_05_line.content.erase(std::remove(begin(a_git_05_line.content),end(a_git_05_line.content),'\n'), end(a_git_05_line.content));

	a_file.hunk_lines[*hunk].push_back(a_git_05_line);
	//diffed_files_.insert(a_file);
	return 0;
}

int GIT_Engine::git_diff_file_cb(const git_diff_delta *delta, float progress, void *payload)
{
	
	if (a_file.diff_delta.status != GIT_DELTA_UNMODIFIED)
	{
		diffed_files_.insert(a_file);
	}
	return 0;
}

int GIT_Engine::payload_fn(const git_diff_delta *delta)
{
	
	
	int a{ 0 };
	return a;
}

void GIT_Engine::list_commits_for_branch(git_repository* repo_, const CString& repo_path, const CString& branch,std::vector<GIT_Commit_Local>& commitsForBranch)
{
 	CT2CA c_str_path(repo_path);
 	const char* REPO = c_str_path;
	git_repository *repo;
	if (git_repository_open(&repo, REPO) != GIT_SUCCESS) {
		AfxMessageBox(L"Cannot open repository:\n" + repo_path);
		return;
	}

	// Read HEAD on branch
	char head_filepath[512];
	FILE *head_fileptr;
	char head_rev[41];

	strcpy(head_filepath, REPO);
	CString heads;
	if (strrchr(REPO, '/') != (REPO + strlen(REPO)))
	{
		heads = ("\\refs\\heads\\");
	}
	else
	{
		heads = ("/refs/heads/");
	}

	CString c_path_to_branch = heads + branch;
	CT2CA pszConvertedAnsiString_path_to_branch(c_path_to_branch);
	// construct a std::string using the LPCSTR input
	std::string path_to_branch(pszConvertedAnsiString_path_to_branch);
	strcat(head_filepath, path_to_branch.c_str());

	if ((head_fileptr = fopen(head_filepath, "r")) == NULL) {
		fprintf(stderr, "Error opening '%s'\n", head_filepath);
		throw - 1;
	}

	if (fread(head_rev, 40, 1, head_fileptr) != 1) {
		fprintf(stderr, "Error reading from '%s'\n", head_filepath);
		fclose(head_fileptr);
		throw - 1;
	}

	fclose(head_fileptr);


	git_oid oid;
	git_revwalk *walker;
	git_commit *commit;

	if (git_oid_fromstr(&oid, head_rev) != GIT_SUCCESS) {
		fprintf(stderr, "Invalid git object: '%s'\n", head_rev);
		throw - 1;
	}

	git_revwalk_new(&walker, repo);
	git_revwalk_sorting(walker, GIT_SORT_TOPOLOGICAL);
	git_revwalk_push(walker, &oid);

	const char *commit_message;
	const git_signature *commit_author;
	std::vector<const git_oid*> commits_oid;
	diffed_files_.clear();
	a_file.hunk_lines.clear();
	//in order to filter commits from a branch check what branch contains a specific commit
	while (git_revwalk_next(&oid, walker) == GIT_SUCCESS)
	{
		if (git_commit_lookup(&commit, repo, &oid)) 
		{
			fprintf(stderr, "Failed to lookup the next object\n");
			throw - 1;
		}
		GIT_Commit_Local local_commit;
		commit_author = git_commit_committer(commit);
		commit_message = git_commit_message(commit);
		auto commit_body = git_commit_body(commit);
		const git_oid* commit_id = git_commit_id(commit);
		auto comment_summary = git_commit_summary(commit);
		auto comment_tree_id = git_commit_tree_id(commit);
		time_t commit_time = git_commit_time(commit);
		//auto char_time = ctime(&commit_time);
		//auto parents = git_commit_parentcount(commit);
		//auto commit_id = git_commit_id(commit);
		//commits_oid.emplace_back(commit_id);
		local_commit.commit_author = *commit_author;
		local_commit.commit_message = std::string( commit_message);
		//git_oid_fmt()
		char* c_str_git_commit_oid = git_oid_tostr_s(commit_id);
		std::string _commit_id(c_str_git_commit_oid);
		local_commit.commit_id = std::move(_commit_id);
		git_tree* tree_right;
		git_commit_tree(&tree_right, commit);
		
		git_commit *commit_parent{nullptr};
		git_commit_parent(&commit_parent,commit,0);
		if (commit_parent)
		{
			git_tree* tree_left;
			git_commit_tree(&tree_left, commit_parent);
			git_diff* diff;
			git_diff_tree_to_tree(&diff, repo, tree_left, tree_right, nullptr);

			git_diff_foreach(diff, &git_diff_file_cb, nullptr, &git_diff_hunk_cb, &git_diff_line_cb, payload_fn);
			
			diffed_files_.insert(a_file);
			local_commit.diffed_files = std::move(diffed_files_);
			//git_diff_stats *stats_out;
			//git_diff_get_stats(&stats_out, diff);
			//
			//auto deletions = git_diff_stats_deletions(stats_out);
			//auto modifications = git_diff_stats_files_changed(stats_out);
			//auto additions = git_diff_stats_insertions(stats_out);//how many lines were added
			//int a{ 0 };
		}
		commitsForBranch.push_back(local_commit);
		// Don't print the \n in the commit_message 
		//printf("'%.*s' by %s <%s>\n", strlen(commit_message) - 1, commit_message, commit_author->name, commit_author->email);

		git_commit_free(commit);
	}

	//git_oid oid_out;
	//
	//git_merge_base(&oid_out, repo, commits_oid.front(), commits_oid.back());

	/////////

	////////

	git_revwalk_free(walker);
}


void GIT_Engine::list_local_branches(git_repository * repo, std::vector<CString>& localBranches)
{
		
		git_reference *out;
		
		if (git_repository_head(&out, repo) == GIT_EUNBORNBRANCH)
		{//branch without commits on it			^^^
			
		}
		else
		{
			//auto nms = git_repository_get_namespace(repo);
			//git_branch_lookup(&out, repo, "master", *branch_type);
			//if (git_branch_is_head(out) == 1)
			//{
			//	int a{ 0 };
			//}
			git_branch_iterator* branch_iterator;
			git_branch_iterator_new(&branch_iterator, repo, GIT_BRANCH_LOCAL);
			git_reference* next_git_branch_ref;
			git_branch_t* branch_type = new git_branch_t;

			std::vector<CString> local_branches;

			while (git_branch_next(&next_git_branch_ref, branch_type, branch_iterator) != GIT_ITEROVER)
			{
				const char* out;
				git_branch_name(&out, next_git_branch_ref);
				CA2CT ca2w(out);
				std::wstring w_out = ca2w;
				CString c_out = w_out.c_str();
				local_branches.push_back(c_out);
			}
			localBranches = std::move(local_branches);
			delete branch_type;
			git_branch_iterator_free(branch_iterator);
		}

}

void GIT_Engine::get_repo(const CString & c_repo_path, git_repository** repo)
{
	CT2CA pszConvertedAnsiString_repo_path(c_repo_path);
	std::string repo_path(pszConvertedAnsiString_repo_path);
	if (git_repository_open(repo, repo_path.c_str()) != GIT_SUCCESS) 
	{
		AfxMessageBox(L"Could not open selected repository");
	}
}


void GIT_Engine::get_commits_for_branches(const CString & repo_path, std::map<CString, std::vector<GIT_Commit_Local>>& branchCommits)
{
	
	git_repository* repo{ nullptr };
	GIT_Engine::get_repo(repo_path, &repo);
	std::vector<CString> local_branches;
	GIT_Engine::list_local_branches(repo, local_branches);
	std::vector<GIT_Commit_Local> local_commits;
	//std::map<CString, std::vector<GIT_Commit_Local>> branch_commits;

	for (const auto& branch : local_branches)
	{
		local_commits.clear();
		GIT_Engine::list_commits_for_branch(repo, repo_path, branch, local_commits);
		branchCommits.insert(std::make_pair(branch, local_commits));
	}
	git_repository_free(repo);
}

bool GIT_Engine::check_if_repo(const CString & pathName)
{
	bool result;
	CT2CA pszConvertedAnsiString(pathName);
	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);
	git_repository* repo{ nullptr };
	if (git_repository_open(&repo, strStd.c_str()) != GIT_SUCCESS)
	{
		result = false;
	}
	else
	{
		result = true;
	}
	
	git_repository_free(repo);
	return result;
}
