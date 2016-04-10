#pragma once
#include "Git_Object.hpp"
class Git_User : public Git_Object
{
private:

public:
	Git_User();
	~Git_User();
	string_t get_avatar_url()const;
};

