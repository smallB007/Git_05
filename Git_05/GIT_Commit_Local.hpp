#pragma once
#include <string>
class GIT_Commit_Local
{
public:
	std::string commit_message;
	git_signature commit_author;
	//git_oid commit_id;
	std::string commit_id;
};

