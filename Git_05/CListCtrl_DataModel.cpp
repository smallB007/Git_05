#include "stdafx.h"
#include "CListCtrl_DataModel.h"
#include "CString_Utils.h"
//m_Records.push_back(CListCtrl_DataRecord("list.cpp", "Staged", "added"));

//entry.status
//GIT_STATUS_CURRENT = 0,
//
//GIT_STATUS_INDEX_NEW = (1u << 0),
//GIT_STATUS_INDEX_MODIFIED = (1u << 1),
//GIT_STATUS_INDEX_DELETED = (1u << 2),
//GIT_STATUS_INDEX_RENAMED = (1u << 3),
//GIT_STATUS_INDEX_TYPECHANGE = (1u << 4),
//
//GIT_STATUS_WT_NEW = (1u << 7),
//GIT_STATUS_WT_MODIFIED = (1u << 8),
//GIT_STATUS_WT_DELETED = (1u << 9),
//GIT_STATUS_WT_TYPECHANGE = (1u << 10),
//GIT_STATUS_WT_RENAMED = (1u << 11),
//GIT_STATUS_WT_UNREADABLE = (1u << 12),
//
//GIT_STATUS_IGNORED = (1u << 14),
//GIT_STATUS_CONFLICTED = (1u << 15),

/*entry.index_to_workdir->status;
//GIT_DELTA_UNMODIFIED = 0,  /**< no changes */
//GIT_DELTA_ADDED = 1,	   /**< entry does not exist in old version */
//GIT_DELTA_DELETED = 2,	   /**< entry does not exist in new version */
//GIT_DELTA_MODIFIED = 3,    /**< entry content changed between old and new */
//GIT_DELTA_RENAMED = 4,     /**< entry was renamed between old and new */
//GIT_DELTA_COPIED = 5,      /**< entry was copied from another old entry */
//GIT_DELTA_IGNORED = 6,     /**< entry is ignored item in workdir */
//GIT_DELTA_UNTRACKED = 7,   /**< entry is untracked item in workdir */
//GIT_DELTA_TYPECHANGE = 8,  /**< type of entry changed between old and new */
//GIT_DELTA_UNREADABLE = 9,  /**< entry is unreadable */
//GIT_DELTA_CONFLICTED = 10, /**< entry in the index is conflicted */
//*/
void CListCtrl_DataModel::fill_model(const Working_Dir& workingDir)
{
	std::vector<git_05_status_entry> sorted_files = workingDir.get_entries();
	std::string state;
	std::string the_status;
	std::string file_name;
// 	int header = 0, changes_in_index = 0;
// 	int changed_in_workdir = 0, rm_in_workdir = 0;

	for (const auto& entry : sorted_files)
	{
		file_name = entry.index_to_workdir->new_file.path;
		switch (entry.status)
		{

		case GIT_STATUS_CURRENT:
				continue;

		case GIT_STATUS_INDEX_NEW:
		{
			state = "new file";
			the_status = "Changes staged for commit";
		}
			break;
		case GIT_STATUS_INDEX_MODIFIED:
		{
			state = "modified";
			the_status = "Changes staged for commit";
		}
				break;
		case GIT_STATUS_INDEX_DELETED:
				state = "deleted";
				the_status = "Changes staged for commit";
				break;
		case GIT_STATUS_INDEX_RENAMED:
				state = "renamed";
				the_status = "Changes staged for commit";
				break;
		case GIT_STATUS_INDEX_TYPECHANGE:
				state = "typechange";
				the_status = "Changes staged for commit";
				break;
		//those above will be files staged for a commit

			//those below will be files not staged for a commit
		case GIT_STATUS_WT_MODIFIED:
				state = "modified";
				the_status = "Changes not staged for commit";
				break;
		case GIT_STATUS_WT_DELETED:
				state = "deleted";
				the_status = "Changes not staged for commit";
				break;
		case GIT_STATUS_WT_RENAMED:
				state = "renamed";
				the_status = "Changes not staged for commit";
				break;
		case GIT_STATUS_WT_TYPECHANGE:
				state = "typechange";
				the_status = "Changes not staged for commit";
				break;
			//those below will be files which are untracked
		case GIT_STATUS_WT_NEW:
				state = "untracked";
				the_status = "Untracked";
				break;
			//those below will be files ignored
		case GIT_STATUS_IGNORED:
				state = "ignored";
				the_status = "Ignored";
				break;
		}
		
		std::vector<path> files;
		std::string no_dot_git_path = workingDir.remove_dot_git(workingDir.get_path_name());
		workingDir.list_files(no_dot_git_path + file_name, &files);
		for (const auto& _file_name: files)
		{
			CString fn = normalize_file_name_(_file_name.c_str());
			CT2CA pszConvertedAnsiString(fn);
			// construct a std::string using the LPCSTR input
			std::string std_file_name(pszConvertedAnsiString);
			m_Records.push_back(CListCtrl_DataRecord(std_file_name, the_status, state));
		}
	}

}