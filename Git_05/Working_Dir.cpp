#include "stdafx.h"
#include "Working_Dir.hpp"



Working_Dir::Working_Dir(const CString& pathName) :path_name_{pathName}
{
}


Working_Dir::~Working_Dir()
{
}

std::map<git_status_t, std::vector<git_05_status_entry>> Working_Dir::get_sorted_entries() const
{
	std::map<git_status_t, std::vector<git_05_status_entry>> result;

	for (const auto& entry : status_entries_)
	{
		result[entry.status].push_back(entry);
	}

	return result;
}



void Working_Dir::list_files(const path& no_dot_git_dir, std::vector<path>*const files)const
{
	if (!is_directory(no_dot_git_dir))
	{
		files->push_back(no_dot_git_dir);
	}
	else
	{
		for (auto beg = directory_iterator(no_dot_git_dir), end = directory_iterator(); beg != end; ++beg)
		{
			if (!is_directory(*beg))
			{
				files->push_back(*beg);
			}
			else 
			{
				list_files(*beg, files);
			}
		}
	}
}

std::string Working_Dir::remove_dot_git(const CString& repoPath)const
{
	CT2CA pszConvertedAnsiString(repoPath);
	std::string path_no_dot_git (pszConvertedAnsiString);
	if (path_no_dot_git.find('.') != std::string::npos)
	{
		//auto end = cend(path_no_dot_git) - 1;
		auto end = path_no_dot_git.back();
		do
		{

			end = path_no_dot_git.back();
			path_no_dot_git.pop_back();
		} while (end != '.');
	}
	return path_no_dot_git;
}

std::map<git_status_t, std::vector<CString>> Working_Dir::get_sorted_files()const
{
	std::map<git_status_t, std::vector<CString>> result;
	std::string path_no_dot_git = remove_dot_git(path_name_);

	for (const auto& entry : status_entries_)
	{
		std::vector<path> paths;
		list_files(path_no_dot_git + entry.index_to_workdir->new_file.path, &paths);

		for (const auto& aPath : paths)
		{
			result[entry.status].push_back(aPath.c_str());
		}
	}

	return result;
}

std::vector<git_05_status_entry> Working_Dir::get_entries()const
{
	return status_entries_;
}