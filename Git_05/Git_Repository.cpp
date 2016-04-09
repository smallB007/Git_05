#include "stdafx.h"
#include "Git_Repository.hpp"


Git_Repository::Git_Repository()
{
}


Git_Repository::~Git_Repository()
{
}

auto Git_Repository::get_repo_name() const
{
	return git_repo_attributes_.get_name();
}

void Git_Repository::set_repo_name(const decltype(Git_Repository_Attributes::name_)& repo_name)
{
	git_repo_attributes_.set_repo_name(repo_name);
}
