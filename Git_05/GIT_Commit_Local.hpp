#pragma once
#include <string>
class GIT_Commit_Local
{
public:
	std::string commit_message;
	git_signature commit_author;
};

