#pragma once
#include <map>
#include <string>

typedef std::string string_t;
typedef string_t attribute;
typedef string_t value;

class Git_Object
{
protected:
	std::map<attribute, value> git_object_attributes_;
public:
	Git_Object();
	~Git_Object();
	void insert_attribute(const string_t& key, const string_t& value);
	string_t get_attribute(const string_t& attribute)const;
};

