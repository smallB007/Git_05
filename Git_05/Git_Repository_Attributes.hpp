#pragma once
#include <string>
class Git_Repository_Attributes
{
	friend class Git_Repository;
private:
	std::string name_;
public:
	Git_Repository_Attributes();
	~Git_Repository_Attributes();
	decltype(name_) get_name()const { return name_; }
	void set_repo_name(decltype(name_) repo_name);
};

