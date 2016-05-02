#include "stdafx.h"
#include "GIT_Engine.hpp"
#include "GIT_Commit_Local.hpp"

GIT_Engine::GIT_Engine()
{
}


GIT_Engine::~GIT_Engine()
{
}
#include <fstream>
void GIT_Engine::list_commits_for_branch(git_repository* repo_, const std::string& repo_path, const std::string& branch,std::vector<GIT_Commit_Local>& commitsForBranch)
{
	
// 	CT2CA c_str_path(repo_path);
 	const char* REPO = repo_path.c_str();
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
// 	std::vector<std::string> local_branches;
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

	//std::string path_to_branch = "\\refs\\heads\\" + branch;

	if (strrchr(REPO, '/') != (REPO + strlen(REPO)))
	{
		std::string path_to_branch = "\\refs\\heads\\" + branch;
		strcat(head_filepath, path_to_branch.c_str());
		//strcat(head_filepath, "\\refs\\heads\\master");
	}
	else
	{
		std::string path_to_branch = "/refs/heads/" + branch;
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

	while (git_revwalk_next(&oid, walker) == GIT_SUCCESS)
	{
		if (git_commit_lookup(&commit, repo, &oid)) 
		{
			fprintf(stderr, "Failed to lookup the next object\n");
			throw - 1;
		}
		GIT_Commit_Local local_commit;
		commit_message = git_commit_message(commit);
		commit_author = git_commit_committer(commit);
		local_commit.commit_message = std::string( commit_message);
		local_commit.commit_author = *commit_author;
		commitsForBranch.push_back(local_commit);
		// Don't print the \n in the commit_message 
		//printf("'%.*s' by %s <%s>\n", strlen(commit_message) - 1, commit_message, commit_author->name, commit_author->email);

		git_commit_free(commit);
	}

	git_revwalk_free(walker);
}


void GIT_Engine::list_local_branches(git_repository * repo, std::vector<std::string>& localBranches)
{
		git_branch_iterator* branch_iterator;
		git_branch_iterator_new(&branch_iterator, repo, GIT_BRANCH_LOCAL);
		git_reference* next_git_branch_ref;
		git_branch_t* branch_type = new git_branch_t;
		std::vector<std::string> local_branches;
		while (git_branch_next(&next_git_branch_ref, branch_type, branch_iterator) != GIT_ITEROVER)
		{
			const char* out;
			git_branch_name(&out, next_git_branch_ref);
			local_branches.push_back(out);
		}
		localBranches = std::move(local_branches);
		delete branch_type;
		git_branch_iterator_free(branch_iterator);

}

void GIT_Engine::get_repo(const std::string & repo_path, git_repository** repo)
{
	if (git_repository_open(repo, repo_path.c_str()) != GIT_SUCCESS) 
	{
		throw - 1;
	}
// 	git_branch_iterator* branch_iterator;
// 	git_branch_iterator_new(&branch_iterator, repo, GIT_BRANCH_LOCAL);
// 	git_reference* next_git_branch_ref;
// 	git_branch_t* branch_type = new git_branch_t;
// 	std::vector<std::string> local_branches;
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

static void convert_to_dot_git_path(std::string & repo_path)
{
	size_t position = repo_path.find(".git");
	if (position == std::string::npos)
	{
		if (repo_path.find("\\"))
		{
			repo_path += "\\.git";
		}
		else
		{
			repo_path += "/.git";
		}
	}
}
void GIT_Engine::get_commits_for_branches(const std::string & repo_path, std::map<std::string, std::vector<GIT_Commit_Local>>& branchCommits)
{
	convert_to_dot_git_path(const_cast<std::string&>(repo_path));
	git_repository* repo{ nullptr };
	GIT_Engine::get_repo(repo_path, &repo);
	std::vector<std::string> local_branches;
	GIT_Engine::list_local_branches(repo, local_branches);
	std::vector<GIT_Commit_Local> local_commits;
	//std::map<std::string, std::vector<GIT_Commit_Local>> branch_commits;

	for (const auto& branch : local_branches)
	{
		GIT_Engine::list_commits_for_branch(repo, repo_path, branch, local_commits);
		branchCommits.insert(std::make_pair(branch, local_commits));
	}
	git_repository_free(repo);
}
