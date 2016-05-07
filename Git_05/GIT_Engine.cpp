#include "stdafx.h"
#include "GIT_Engine.hpp"
#include "GIT_Commit_Local.hpp"

GIT_Engine::GIT_Engine()
{
}


GIT_Engine::~GIT_Engine()
{
}
//#include <fstream>
void GIT_Engine::list_commits_for_branch(git_repository* repo_, const CString& repo_path, const CString& branch,std::vector<GIT_Commit_Local>& commitsForBranch)
{
	
 	CT2CA c_str_path(repo_path);
 	const char* REPO = c_str_path;
// 
	git_repository *repo;
	if (git_repository_open(&repo, REPO) != GIT_SUCCESS) {
		//fprintf(stderr, "Failed opening repository: '%s'\n", REPO);
		throw - 1;
	}
	//////////////////////////////////////////////////////////////////////////
// 	if (git_repository_open(&repo, repo_path.c_str()) != GIT_SUCCESS)
// 	{
// 		throw - 1;
// 	}
// 	git_branch_iterator* branch_iterator;
// 	git_branch_iterator_new(&branch_iterator, repo, GIT_BRANCH_LOCAL);
// 	git_reference* next_git_branch_ref;
// 	git_branch_t* branch_type = new git_branch_t;
// 	std::vector<CString> local_branches;
// 	while (git_branch_next(&next_git_branch_ref, branch_type, branch_iterator) != GIT_ITEROVER)
// 	{
// 		const char* out;
// 		git_branch_name(&out, next_git_branch_ref);
// 		//local_branches.push_back(out);
// 	}
// 	//localBranches = std::move(local_branches);
// 	delete branch_type;
// 	git_branch_iterator_free(branch_iterator);
	//////////////////////////////////////////////////////////////////////////
	

	// Read HEAD on branch
	char head_filepath[512];
	FILE *head_fileptr;
	char head_rev[41];

	strcpy(head_filepath, REPO);

	//CString path_to_branch = "\\refs\\heads\\" + branch;

	if (strrchr(REPO, '/') != (REPO + strlen(REPO)))
	{
		CString heads("\\refs\\heads\\");
		CString c_path_to_branch = heads + branch;
		CT2CA pszConvertedAnsiString_path_to_branch(c_path_to_branch);
		// construct a std::string using the LPCSTR input
		std::string path_to_branch(pszConvertedAnsiString_path_to_branch);
		strcat(head_filepath, path_to_branch.c_str());
		//strcat(head_filepath, "\\refs\\heads\\master");
	}
	else
	{
		CString heads("/refs/heads/");
		CString c_path_to_branch = heads + branch;
		CT2CA pszConvertedAnsiString_path_to_branch(c_path_to_branch);
		// construct a std::string using the LPCSTR input
		std::string path_to_branch(pszConvertedAnsiString_path_to_branch);
		strcat(head_filepath, path_to_branch.c_str());
		//strcat(head_filepath, "refs/heads/master");
		
	}

// 	std::ifstream f_in(head_filepath);
// 	if (f_in)
// 	{
// 		f_in >> head_rev;
// 	}

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

		auto parents = git_commit_parentcount(commit);
		//auto commit_id = git_commit_id(commit);
		//commits_oid.emplace_back(commit_id);
		local_commit.commit_message = std::string( commit_message);
		local_commit.commit_author = *commit_author;
		local_commit.commit_id = *commit_id;
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

void GIT_Engine::get_repo(const CString & c_repo_path, git_repository** repo)
{
	CT2CA pszConvertedAnsiString_repo_path(c_repo_path);
	// construct a std::string using the LPCSTR input
	std::string repo_path(pszConvertedAnsiString_repo_path);
	if (git_repository_open(repo, repo_path.c_str()) != GIT_SUCCESS) 
	{
		throw - 1;
	}
// 	git_branch_iterator* branch_iterator;
// 	git_branch_iterator_new(&branch_iterator, repo, GIT_BRANCH_LOCAL);
// 	git_reference* next_git_branch_ref;
// 	git_branch_t* branch_type = new git_branch_t;
// 	std::vector<CString> local_branches;
// 	while (git_branch_next(&next_git_branch_ref, branch_type, branch_iterator) != GIT_ITEROVER)
// 	{
// 		const char* out;
// 		git_branch_name(&out, next_git_branch_ref);
// 		//local_branches.push_back(out);
// 	}
// 	//localBranches = std::move(local_branches);
// 	delete branch_type;
// 	git_branch_iterator_free(branch_iterator);
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
