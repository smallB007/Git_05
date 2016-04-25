#pragma once
#include "GIT_Branch.hpp"
#include <set>
class GIT_Tree
{
private:
	std::set<GIT_Branch> branches_;
public:
	GIT_Tree();
	~GIT_Tree();
};

