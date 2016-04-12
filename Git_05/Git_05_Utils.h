#pragma once
#include <AtlBase.h>
#include <atlconv.h>
#include <string>

inline std::wstring string_to_wstring(const std::string& str)
{
	CA2W ca2w(str.c_str());
	std::wstring wide_str = ca2w;
	return wide_str;
}

inline std::string wstring_to_string(const std::wstring& wstr)
{
	CW2A cw2a(wstr.c_str());
	std::string str = cw2a;
	return str;
}