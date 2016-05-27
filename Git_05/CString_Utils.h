#pragma once
#include "MainFrm.h"
#include <string>
static CString normalize_file_name_(const CString& fileName)
{//removes full path and leaves only file name (if the file is in the working dir) or filename and path to file from working dir to that file folder 
	CT2CA ct_file_name_path(fileName);
	std::string file_name_path(ct_file_name_path);
	// construct a std::string using the LPCSTR input
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	CString cs_current_repo_path = pMainWnd->get_current_repo();
	CT2CA ct_current_repo_path(cs_current_repo_path);
	std::string current_repo_path(ct_current_repo_path);

	auto mis = std::mismatch(cbegin(file_name_path), cend(file_name_path), cbegin(current_repo_path), cend(current_repo_path));
	std::string normalized_name((mis.first), cend(file_name_path));
	CA2W w_str(normalized_name.c_str());
	CString result = w_str;
	return result;
}