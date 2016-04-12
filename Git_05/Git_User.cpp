#include "stdafx.h"
#include "Git_User.hpp"


Git_User::Git_User()
{
}


Git_User::~Git_User()
{
}


string_t Git_User::get_avatar_url() const
{
	auto it = git_object_attributes_.find(_T("avatar_url"));
	return it->second;
}






	