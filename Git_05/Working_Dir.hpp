#pragma once
#include <vector>
#include <map>
#include <memory>
#include <array>

#include <boost/filesystem.hpp>


struct git_05_status_entry {
private:
	std::vector<std::shared_ptr<char*>> new_old_paths;
public:
	git_status_t status;
	std::shared_ptr<git_diff_delta> head_to_index;
	std::shared_ptr<git_diff_delta> index_to_workdir;
	git_05_status_entry(const git_status_entry& original)
	{
		status = original.status;
		if (nullptr != original.head_to_index)
		{
			head_to_index = std::make_shared<git_diff_delta>(*original.head_to_index);
			char* old_file = new char[strlen(original.head_to_index->old_file.path) + 1];
			strcpy(old_file, original.head_to_index->old_file.path);
			head_to_index->old_file.path = old_file;
			new_old_paths.push_back(std::make_shared<char*>(old_file));

			char* new_file = new char[strlen(original.head_to_index->new_file.path) + 1];
			strcpy(new_file, original.head_to_index->new_file.path);
			head_to_index->new_file.path = new_file;
			new_old_paths.push_back(std::make_shared<char*>(new_file));
			
		}
		if (nullptr != original.index_to_workdir)
		{
			index_to_workdir = std::make_shared<git_diff_delta>(*original.index_to_workdir);
			//auto t = *original.index_to_workdir->new_file.path;
			//auto l = strlen(original.index_to_workdir->new_file.path);
			char* old_file = new char[strlen(original.index_to_workdir->old_file.path) + 1];
			strcpy(old_file, original.index_to_workdir->old_file.path);
			index_to_workdir->old_file.path = old_file;
			new_old_paths.push_back(std::make_shared<char*>(old_file));

			char* new_file = new char[strlen(original.index_to_workdir->new_file.path) + 1];
			strcpy(new_file, original.index_to_workdir->new_file.path);
			index_to_workdir->new_file.path = new_file;
			new_old_paths.push_back(std::make_shared<char*>(new_file));
		}
	} 
	~git_05_status_entry()
	{
		//for (const auto& _char : new_old_paths)
		//{
		//	delete[] _char;
		//}
	}
};
using namespace boost;
using namespace boost::filesystem;

class Working_Dir
{
	
	//mutable std::map<git_status_t, std::vector<git_status_entry>> entries_by_status_;
	std::vector<git_05_status_entry> status_entries_;
	CString path_name_;
public:
	typedef CString full_file_path;
	Working_Dir(const CString& pathName);
	~Working_Dir();
	CString get_path_name()const
	{
		return path_name_;
	}
	void push_back(const git_status_entry statusEntry)
	{
		status_entries_.push_back(statusEntry);
	}
	std::map<git_status_t, std::vector<git_05_status_entry>> get_sorted_entries()const;
	std::map<git_status_t, std::vector<full_file_path>> get_sorted_files()const;
	std::vector<git_05_status_entry> get_entries()const;
	std::string remove_dot_git(const CString& repoPath)const;
	void list_files(const path& no_dot_git_dir, std::vector<path>*const files)const;
};

