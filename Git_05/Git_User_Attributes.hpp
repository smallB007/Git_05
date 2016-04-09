#pragma once
#include <string>
#include <memory>
#include <ctime>
#include "Poco/URI.h"
struct Company
{//:AC: ToDo move it to separate header

};
struct Blog
{//:AC: ToDo move it to separate header

};
struct Location
{

};
struct Email
{

};
struct Bio
{

};
struct Git_Plan
{
	typedef std::string string_t;
	string_t name_;
	unsigned long long space_;
	unsigned collaborators_;
	unsigned private_repos_;
};

class Git_User_Attributes
{
	friend class Git_User;
	typedef std::string string_t;
	typedef std::time_t time_t;
private:
	string_t login_;
	string_t id_;
	Poco::URI avatar_url_;
	string_t gravatar_id_;
	Poco::URI url_;
	Poco::URI html_url_;
	Poco::URI followers_url_;
	Poco::URI following_url_;
	Poco::URI gists_url_;
	Poco::URI starred_url_;
	Poco::URI subscriptions_url_;
	Poco::URI organizations_url_;
	Poco::URI repos_url_;
	Poco::URI events_url_;
	Poco::URI received_events_url_;
	string_t type_;
	bool site_admin_{ false };
	string_t name_;
	std::shared_ptr<Company> company_;
	std::shared_ptr<Blog> blog_;
	std::shared_ptr<Location> location_;
	std::shared_ptr<Email> email_;
	bool hireble_{ false };
	std::shared_ptr<Bio> bio_;
	unsigned public_repos_{ 0 };
	unsigned public_gists_{ 0 };
	unsigned followers_{ 0 };
	unsigned following_{ 0 };
	time_t created_at_;
	time_t updated_at_;
	unsigned private_gists_;
	unsigned total_private_repos_;
	unsigned owned_private_repos_;
	unsigned long long disk_usage_;
	unsigned collaborators_;
	Git_Plan plan_;
public:
	Git_User_Attributes();
	~Git_User_Attributes();
};

