#include "stdafx.h"
#include "GIT_Engine.hpp"
#include "GIT_Commit_Local.hpp"
#include "Working_Dir.hpp"
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

void GIT_Engine::create_initial_commit(git_repository *repo)
{
	git_signature *sig;
	git_index *index;
	git_oid tree_id, commit_id;
	git_tree *tree;


	//First use the config to initialize a commit signature for the user.


	if (git_signature_default(&sig, repo) < 0)
	{
		AfxMessageBox(L"Unable to create a commit signature.\nPerhaps 'user.name' and 'user.email' are not set");
		return;
	}

	/* Now let's create an empty tree for this commit */

	if (git_repository_index(&index, repo) < 0)
	{
		AfxMessageBox(L"Could not open repository index");
		return;
	}

	//Outside of this example, you could call gitindexadd_bypath() here to put actual files into the index. For our purposes, we'll leave it empty for now.
	

	if (git_index_write_tree(&tree_id, index) < 0)
	{
		AfxMessageBox(L"Unable to write initial tree from index");
		git_index_free(index);
		return;
	}


	if (git_tree_lookup(&tree, repo, &tree_id) < 0)
	{
		AfxMessageBox(L"Could not look up initial tree");
		return;
	}


	//Ready to create the initial commit.

	//Normally creating a commit would involve looking up the current HEAD commit and making that be the parent of the initial commit, but here this is the first commit so there will be no parent.


	if (git_commit_create_v(
		&commit_id, repo, "HEAD", sig, sig,
		NULL, "Initial commit", tree, 0) < 0)
	{
		AfxMessageBox(L"Could not create the initial commit");
		//Clean up so we don't leak memory.
		git_tree_free(tree);
		git_signature_free(sig);
		return;
	}


}

#include "CString_Utils.h"
void GIT_Engine::create_commit(const CString& repoPath, const CString& branch, const std::set<CString>& checkedFiles, const CString& commitMsg)
{
	
	//Working_Dir working_dir = list_files_in_working_dir(repoPath);
	//std::map<git_status_t, std::vector<Working_Dir::full_file_path>> sorted_files = working_dir.get_sorted_files();
	if (checkedFiles.size())
	{//cannot create commit with no files to commit

		git_index *index;
		git_oid tree_id, commit_id;

		//int git_commit_tree(git_tree **tree_out, const git_commit *commit);
		git_tree* tree{ nullptr };
		git_commit* commit = get_top_commit_for_branch(repoPath, branch);
		git_commit_tree(&tree, commit);

		CT2CA c_str_path(repoPath);
		const char* REPO = c_str_path;
		git_repository* repo;

		if (git_repository_open(&repo, REPO) != GIT_SUCCESS) {
			AfxMessageBox(L"Cannot open repository:\n" + repoPath);
			return;
		}

		git_signature *sig;
		if (git_signature_default(&sig, repo) < 0)
		{
			AfxMessageBox(L"Unable to create a commit signature.\nPerhaps 'user.name' and 'user.email' are not set?");
			return;
		}

		/* Now let's create an empty tree for this commit */

		if (git_repository_index(&index, repo) < 0)
		{
			AfxMessageBox(L"Could not open repository index");
			return;
		}
		//Outside of this example, you could call gitindexadd_bypath() here to put actual files into the index. For our purposes, we'll leave it empty for now.
		//int git_index_add_bypath(git_index *index, const char *path);

// 		for (const auto& _status : sorted_files)
// 		{
// 			for (const auto& file : _status.second)
// 			{
				//char *dir = "git_test_3/git_test_3 - Copy - Copy.cpp";
				//git_strarray paths = { &dir, 1 };
				//git_index_add_all(index, &paths, 0, NULL, NULL);
				//CT2CA ctString(normalize_file_name_(file));
		for (const auto& file : checkedFiles)
		{
			CT2CA ctString(file);
			std::string stdString(ctString);
			//git_index_add_bypath(index, stdString.c_str());
			auto ec = git_index_entrycount(index);
			auto caps = git_index_caps(index);
			if (git_index_add_bypath(index, /*"git_test_3/git_test_3 - Copy - Copy - Copy.cpp"*/stdString.c_str()) != GIT_SUCCESS)
			{
				//AfxMessageBox(L"Unable to add files to commit.\nUse console to resolve the issue.");
				//return;
				int a{ 0 };
			}
		}
		//	}
		//}
		
		if (git_index_write_tree(&tree_id, index) < 0)
		{
			AfxMessageBox(L"Unable to write initial tree from index");
			git_index_free(index);
			return;
		}

		if (git_tree_lookup(&tree, repo, &tree_id) < 0)
		{
			AfxMessageBox(L"Could not look up initial tree");
			return;
		}
		{
			CT2CA ctCommitMsg(commitMsg);
			std::string stdCommitMsg(ctCommitMsg);
			if (git_commit_create_v(
				&commit_id, repo, "HEAD", sig, sig,
				NULL, stdCommitMsg.c_str(), tree, 1, commit) < 0)
			{
				AfxMessageBox(L"Could not create commit");/*the initial*/
				//Clean up so we don't leak memory.
				git_tree_free(tree);
				git_signature_free(sig);
				return;
			}
		}
		git_commit_free(commit);
		git_tree_free(tree);
		git_signature_free(sig);
		//std::vector<GIT_Commit_Local> commits_for_branch;
		//list_commits_for_branch(repoPath, branch, commits_for_branch);

	}
}

bool GIT_Engine::git_init(git_repository *repo,const char* path, const git_init_opts_t& initOptions)
{
	auto result = git_repository_init(&repo, path, static_cast<int>(initOptions.bare)) == git_error_code::GIT_OK;
	
	if (initOptions.initial_commit)
	{
		create_initial_commit(repo);
	}
	if (false == result)
	{
		AfxMessageBox(L"Couldn't init git repository");//those messages should be moved outside of the engine as engine should only be responsible for git related tasks
	}
	return result;
	
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

git_commit* GIT_Engine::get_top_commit_for_branch(const CString& repo_path, const CString& branch)
{
	git_repository* repo;
	CT2CA c_str_path(repo_path);
	const char* REPO = c_str_path;

	if (git_repository_open(&repo, REPO) != GIT_SUCCESS) {
		AfxMessageBox(L"Cannot open repository:\n" + repo_path);
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
		//fprintf(stderr, "Error opening '%s'\n", head_filepath);
		AfxMessageBox(L"Cannot read file: " + repo_path + branch);
		goto failure;
	}

	if (fread(head_rev, 40, 1, head_fileptr) != 1) {
		//fprintf(stderr, "Error reading from '%s'\n", head_filepath);
		AfxMessageBox(L"Error reading from: " + branch);
		fclose(head_fileptr);
		goto failure;
	}

	fclose(head_fileptr);


	git_oid oid;
	git_revwalk *walker;
	git_commit *commit;

	if (git_oid_fromstr(&oid, head_rev) != GIT_SUCCESS) {
		//fprintf(stderr, "Invalid git object: '%s'\n", head_rev);
		AfxMessageBox(L"Invalid git object: " + branch);
		goto failure;
	}

	//git_revwalk_new(&walker, repo);
	//git_revwalk_sorting(walker, GIT_SORT_TOPOLOGICAL);
	//git_revwalk_push(walker, &oid);

	if (git_commit_lookup(&commit, repo, &oid) != GIT_SUCCESS)
	{
		AfxMessageBox(L"Cannot find top commit for branch: " + branch);
		goto failure;
	}
	else
	{
		return commit;
	}
failure:
	return nullptr;
}

void GIT_Engine::list_commits_for_branch(/*git_repository* repo_, */const CString& repo_path, const CString& branch,std::vector<GIT_Commit_Local>& commitsForBranch)
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
	//std::vector<const git_oid*> commits_oid;
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
		//git_repository_free(repo);
	}

	//git_oid oid_out;
	//
	//git_merge_base(&oid_out, repo, commits_oid.front(), commits_oid.back());

	/////////

	////////

	git_revwalk_free(walker);
}
//taken from git_status
enum {
	FORMAT_DEFAULT = 0,
	FORMAT_LONG = 1,
	FORMAT_SHORT = 2,
	FORMAT_PORCELAIN = 3,
};

#define MAX_PATHSPEC 8

struct opts {
	git_status_options statusopt;
	const char *repodir;
	char *pathspec[MAX_PATHSPEC];
	int npaths;
	int format;
	int zterm;
	int showbranch;
	int showsubmod;
	int repeat;
};

void get_status_entries(git_status_list *status, Working_Dir*const workingDir)
{
	//here git_status_list_entrycount is sensitive to .gitignore
	size_t i, maxi = git_status_list_entrycount(status);
	const git_status_entry *s;
	for (i = 0; i < maxi; ++i)
	{
		s = git_status_byindex(status, i);
		workingDir->push_back(*s);
	}
}

void GIT_Engine::parse_status(git_status_list *status/*, Working_Dir*const workingDir*/)
{
	size_t i, maxi = git_status_list_entrycount(status);
	const git_status_entry *s;
	int header = 0, changes_in_index = 0;
	int changed_in_workdir = 0, rm_in_workdir = 0;
	const char *old_path, *new_path;

	for (i = 0; i < maxi; ++i) {
		char *istatus = NULL;

		s = git_status_byindex(status, i);

		if (s->status == GIT_STATUS_CURRENT)
			continue;

		if (s->status & GIT_STATUS_WT_DELETED)
			rm_in_workdir = 1;

		if (s->status & GIT_STATUS_INDEX_NEW)
			istatus = "new file: ";
		if (s->status & GIT_STATUS_INDEX_MODIFIED)
			istatus = "modified: ";
		if (s->status & GIT_STATUS_INDEX_DELETED)
			istatus = "deleted:  ";
		if (s->status & GIT_STATUS_INDEX_RENAMED)
			istatus = "renamed:  ";
		if (s->status & GIT_STATUS_INDEX_TYPECHANGE)
			istatus = "typechange:";

		if (istatus == NULL)
			continue;

		if (!header) {
			printf("# Changes to be committed:\n");
			printf("#   (use \"git reset HEAD <file>...\" to unstage)\n");
			printf("#\n");
			header = 1;
		}

		old_path = s->head_to_index->old_file.path;
		new_path = s->head_to_index->new_file.path;

		if (old_path && new_path && strcmp(old_path, new_path))
			printf("#\t%s  %s -> %s\n", istatus, old_path, new_path);
		else
			printf("#\t%s  %s\n", istatus, old_path ? old_path : new_path);
	}

	if (header) {
		changes_in_index = 1;
		printf("#\n");
	}
	header = 0;


	for (i = 0; i < maxi; ++i) {
		char *wstatus = NULL;

		s = git_status_byindex(status, i);



		if (s->status == GIT_STATUS_CURRENT || s->index_to_workdir == NULL)
			continue;


		if (s->status & GIT_STATUS_WT_MODIFIED)
			wstatus = "modified: ";
		if (s->status & GIT_STATUS_WT_DELETED)
			wstatus = "deleted:  ";
		if (s->status & GIT_STATUS_WT_RENAMED)
			wstatus = "renamed:  ";
		if (s->status & GIT_STATUS_WT_TYPECHANGE)
			wstatus = "typechange:";

		if (wstatus == NULL)
			continue;

		if (!header) {
			printf("# Changes not staged for commit:\n");
			printf("#   (use \"git add%s <file>...\" to update what will be committed)\n", rm_in_workdir ? "/rm" : "");
			printf("#   (use \"git checkout -- <file>...\" to discard changes in working directory)\n");
			printf("#\n");
			header = 1;
		}

		old_path = s->index_to_workdir->old_file.path;
		new_path = s->index_to_workdir->new_file.path;

		if (old_path && new_path && strcmp(old_path, new_path))
			printf("#\t%s  %s -> %s\n", wstatus, old_path, new_path);
		else
			printf("#\t%s  %s\n", wstatus, old_path ? old_path : new_path);
	}

	if (header) {
		changed_in_workdir = 1;
		printf("#\n");
	}


	header = 0;

	for (i = 0; i < maxi; ++i) {
		s = git_status_byindex(status, i);

		if (s->status == GIT_STATUS_WT_NEW) {

			if (!header) {
				printf("# Untracked files:\n");
				printf("#   (use \"git add <file>...\" to include in what will be committed)\n");
				printf("#\n");
				header = 1;
			}

			printf("#\t%s\n", s->index_to_workdir->old_file.path);
		}
	}

	header = 0;

	for (i = 0; i < maxi; ++i) {
		s = git_status_byindex(status, i);

		if (s->status == GIT_STATUS_IGNORED) {

			if (!header) {
				printf("# Ignored files:\n");
				printf("#   (use \"git add -f <file>...\" to include in what will be committed)\n");
				printf("#\n");
				header = 1;
			}

			printf("#\t%s\n", s->index_to_workdir->old_file.path);
		}
	}

	if (!changes_in_index && changed_in_workdir)
		printf("no changes added to commit (use \"git add\" and/or \"git commit -a\")\n");
}


Working_Dir GIT_Engine::list_files_in_working_dir(const CString& pathName)
{
	git_repository * repo;
	CT2CA pszConvertedAnsiString(pathName);
		// construct a std::string using the LPCSTR input
	std::string strStd_path_name(pszConvertedAnsiString);
	
	if (git_repository_open(&repo, strStd_path_name.c_str()) == GIT_SUCCESS)
	{
		git_status_list *status;
		opts o = { GIT_STATUS_OPTIONS_INIT, "." };
		o.statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
		o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
			GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
			GIT_STATUS_OPT_SORT_CASE_SENSITIVELY | GIT_INDEX_ADD_FORCE;
		o.repodir = strStd_path_name.c_str();
		git_status_list_new(&status, repo, &o.statusopt);
		Working_Dir working_dir(pathName);
		//print_long(status,&working_dir);
		get_status_entries(status, &working_dir);
		//working_dir.get_sorted_entries();
		//working_dir.get_sorted_files();
		git_status_list_free(status);
		return working_dir;
	}
	else
	{
		return Working_Dir(pathName);
	}

}

void GIT_Engine::list_local_branches(git_repository * repo, std::vector<CString>& localBranches)
{
		
		git_reference *out;
		
		if (git_repository_head(&out, repo) == GIT_EUNBORNBRANCH)
		{//branch without commits on it			^^^
			//for the moment initial commit is created so theoretically we should never end up here
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
		GIT_Engine::list_commits_for_branch(/*repo, */repo_path, branch, local_commits);
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
