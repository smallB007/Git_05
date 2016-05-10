#pragma once
#include <string>
#include <set>

template<class Git_Diff_File>
struct Git_Diff_File_Less;

template<>
struct Git_Diff_File_Less<git_diff_file>
{
	bool operator ()(const git_diff_file& left, const git_diff_file& right)
	{
		return strcmp(left.path, right.path);
	}
};

class GIT_Commit_Local
{
public:
	std::string commit_id;
	std::string commit_message;
	git_signature commit_author;
	//git_oid commit_id;
	std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> files_modified;
	std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> files_added;
	std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> files_deleted;

};

