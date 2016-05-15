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

struct Less_hunk
{
	bool operator()(const git_diff_hunk& left, const git_diff_hunk& right)
	{

		return left.old_start < right.old_start;
	}
};

struct git_o5_diff_line_t
{
	char   origin;       /**< A git_diff_line_t value */
	int    old_lineno;   /**< Line number in old file or -1 for added line */
	int    new_lineno;   /**< Line number in new file or -1 for deleted line */
	int    num_lines;    /**< Number of newline characters in content */
	size_t content_len;  /**< Number of bytes of data */
	git_off_t content_offset; /**< Offset in the original file to the content */
	std::string content; /**<diff text*/
public:
	git_o5_diff_line_t(const git_diff_line& diffLine) : origin{ diffLine.origin },
		old_lineno{ diffLine.old_lineno },
		new_lineno{ diffLine.new_lineno },
		num_lines{ diffLine.num_lines },
		content_len{ diffLine.content_len },
		content_offset{ diffLine.content_offset }
	{
		//std::vector<char> chars;
		//for (int i{0}; i < content_len;++i)
		//{
		//	chars.push_back(diffLine.content[i]);
		//}
		//for (auto char_ : chars)
		//{
		content.append(diffLine.content, diffLine.content_len);
		//}

	}
};

struct diffed_file_t
{
	git_diff_delta diff_delta;
	//git_diff_file diff_file;
	std::map<git_diff_hunk, std::vector<git_o5_diff_line_t>, Less_hunk> hunk_lines;
};

struct Less_Diff_File
{
	bool operator()(const diffed_file_t& left, const diffed_file_t& right)
	{
		return strcmp(left.diff_delta.new_file.path, right.diff_delta.new_file.path);
	}
};

class GIT_Commit_Local
{
public:
	std::string commit_id;
	std::string commit_message;
	git_signature commit_author;
	//git_oid commit_id;
	//std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> files_modified;
	//std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> files_added;
	//std::set<git_diff_file, Git_Diff_File_Less<git_diff_file>> files_deleted;
	std::set<diffed_file_t, Less_Diff_File> diffed_files;
	diffed_file_t get_diffed_file(const CString& fileName)const;
};

