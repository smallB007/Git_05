#pragma once
#include "afxwin.h"
#include <vector>

namespace mfc_string_utilities
{
// 	char* toChar(const CString& mfc_string)
// 	{
// 		CT2CA half_way(mfc_string);
// 		LPSTR cStr = half_way.operator LPSTR();
// 		return cStr;
// 	}
// 
// 	char* toChar_s(const CString& mfc_string)
// 	{// Convert to a char*
// 		char* pStr = CT2A((LPCTSTR)mfc_string);
// 		return pStr;
// 	}
// 
// // 	errno_t wcstombs_s(
// // 		size_t *pReturnValue,
// // 		char *mbstr,
// // 		size_t sizeInBytes,
// // 		const wchar_t *wcstr,
// // 		size_t count
// // 		);
// 	errno_t toChar_v(CString mfc_string, char* dest)
// 	{
// 		auto length = mfc_string.GetLength();
// 		
// 		size_t ret_size = length;
// 		auto res = wcstombs_s(&ret_size,dest,length + 1, mfc_string.GetBuffer(), length);
// 		return res;
// 	}
}