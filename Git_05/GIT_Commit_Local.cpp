#include "stdafx.h"
#include "GIT_Commit_Local.hpp"


diffed_file_t GIT_Commit_Local::get_diffed_file(const CString& fileName)const
{
	diffed_file_t result;
	for (const auto& file : diffed_files)
	{
			CA2W w_file_path(file.diff_delta.new_file.path);

		if (w_file_path == fileName)
		{
			result = file;
		}
	}

	return result;
}
