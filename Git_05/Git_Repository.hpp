#pragma once
#include <string>
#include "Git_Repository_Attributes.hpp"

class Git_Repository
{
private:
	Git_Repository_Attributes git_repo_attributes_;
public:
	Git_Repository();
	virtual ~Git_Repository();
	auto get_repo_name()const;
	void set_repo_name(const decltype(Git_Repository_Attributes::name_)& repo_name);
};

