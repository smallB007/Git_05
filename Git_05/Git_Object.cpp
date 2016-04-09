#include "stdafx.h"
#include "Git_Object.hpp"


Git_Object::Git_Object()
{
}


Git_Object::~Git_Object()
{
}

void Git_Object::insert_attribute(const string_t& key, const string_t& value)
{
	git_object_attributes_.insert(std::make_pair(key, value));
}

string_t Git_Object::get_attribute(const string_t& attribute) const
{
	auto it = git_object_attributes_.find(attribute);
	if (it != cend(git_object_attributes_))
	{
		return (it->second);
	}
	else
	{
		throw "No such attribute found";
	}
}