#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <set>


#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/SharedPtr.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"

#include "Git_User.hpp"
#include "Git_Repository.hpp"

template<class T>
struct Less;
template<>
struct Less<Git_User>
{
	bool operator()(const Git_User& left, const Git_User& right)
	{
		return left.get_attribute("name") < right.get_attribute("name");
	}
};
template<>
struct Less<Git_Repository>
{
	bool operator()(const Git_Repository& left, const Git_Repository& right)
	{
		return left.get_attribute("name") < right.get_attribute("name");
	}
};

typedef Poco::JSON::Array JSON_Array;

class HTTPS_GIT_Client
{
	typedef std::map<Git_User, std::set<Git_Repository,Less<Git_Repository>>, Less<Git_User>> user_repos_t;

private:

	//Poco::Net::HTTPSClientSession client_session_;//maybe it is OK but for now each mthd will create its own
	Git_User current_user_;
	user_repos_t user_repos_;
	void GET_user_repositories_(const Git_User& user);
	void GET_user_avatar_(const Git_User& user);
	void GET_current_user_avatar_();
	void fill_json_data_(const Poco::JSON::Object::Ptr& arr, Git_Object & git_object);


public:
	HTTPS_GIT_Client();
	~HTTPS_GIT_Client();
	void connect();

	std::set<Git_Repository,Less<Git_Repository>> user_repositories(const Git_User& user)const;
	void current_user_avatar();

	void login();

};

