#pragma once
#include <AtlBase.h>
#include <atlconv.h>
#include <string>
#include <type_traits>

//https://msdn.microsoft.com/en-us/library/87zae4a3.aspx
//http://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t

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

template<class integral>
CString int_to_CString(integral value)
{
	static_assert(std::is_integral<integral>::value, "Argument must be of integral type." );
	CString str;
	str.Format(L"%d", value);
	return str;
}

template<class integral>
integral Cstring_to_int(const CString& str)
{
	static_assert(std::is_integral<integral>::value, "Return type must be of integral type.");
	integral int_ = _ttoi(str);
	return int_;
}