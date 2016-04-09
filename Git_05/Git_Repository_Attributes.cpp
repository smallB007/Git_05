#include "stdafx.h"
#include "Git_Repository_Attributes.hpp"


Git_Repository_Attributes::Git_Repository_Attributes()
{
}


Git_Repository_Attributes::~Git_Repository_Attributes()
{
}

void Git_Repository_Attributes::set_repo_name(decltype(name_) repo_name)
{
	name_ = repo_name;
}
