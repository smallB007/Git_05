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
#include "GIT_Branch.hpp"
#include "GIT_Commit.hpp"
#include "User_Credentials.hpp"
template<class T>
struct Less;
template<>
struct Less<Git_User>
{
	bool operator()(const Git_User& left, const Git_User& right)
	{
		return left.get_attribute(_T("name")) < right.get_attribute(_T("name"));
	}
};
template<>
struct Less<Git_Repository>
{
	bool operator()(const Git_Repository& left, const Git_Repository& right)
	{
		return left.get_attribute(_T("name")) < right.get_attribute(_T("name"));
	}
};

template<>
struct Less<GIT_Branch>
{
	bool operator()(const GIT_Branch& left, const GIT_Branch& right)
	{
		return left.get_attribute(_T("name")) < right.get_attribute(_T("name"));
	}
};

template<>
struct Less<GIT_Commit>
{
	bool operator()(const GIT_Commit& left, const GIT_Commit& right)
	{
		return left.get_attribute(_T("sha")) < right.get_attribute(_T("sha"));
	}
};

typedef Poco::JSON::Array JSON_Array;

class CGit_05App;

class HTTPS_GIT_Client
{
	typedef std::map<Git_User, std::set<Git_Repository,Less<Git_Repository>>, Less<Git_User>> user_repos_t;
	typedef std::string userName;
	typedef std::string userPass;
private:

	//Poco::Net::HTTPSClientSession client_session_;//maybe it is OK but for now each mthd will create its own
	Git_User current_user_;
	user_repos_t user_repos_;
	template<class Git_Object_Class>
	void GET_Git_Object_details_(Poco::Net::HTTPSClientSession& client_session, const Poco::URI& uri, Git_Object_Class& git_object);
	template<class Git_Object_Class>
	void GET_Git_Object_details_(Poco::Net::HTTPSClientSession& client_session, const Poco::URI& uri,std::set<Git_Object_Class, Less<Git_Object_Class>>* collection);
	void GET_user_details_(Poco::Net::HTTPSClientSession& client_session);
	void GET_user_repositories_(Poco::Net::HTTPSClientSession& client_session, const Git_User & user, std::set<Git_Repository, Less<Git_Repository>>* collection);
	void GET_user_avatar_(const Git_User& user)const;
	void GET_current_user_avatar_()const;
	void send_receive_(Poco::Net::HTTPSClientSession& client_session, const Poco::URI& uri, std::string& response);
	template<class Git_Object_Class, class Git_Object_Collection = std::set<Git_Object_Class, Less<Git_Object_Class>>>
	void parse_(const std::string& response_user_content, Git_Object_Class& git_object, Git_Object_Collection* git_collection = nullptr);
	void log_in_();
	void get_user_credentials_(std::pair<userName, userPass>& userCredentials);
	void fill_json_data_(const Poco::JSON::Object::Ptr& arr, Git_Object & git_object);
	std::set<Git_Repository,Less<Git_Repository>> user_repositories_(const Git_User& user)const;
	void extract_info_from_repo_(const Git_Repository& repo, Poco::Net::HTTPSClientSession& client_session);
	void get_branches_(const Git_Repository& repo, Poco::Net::HTTPSClientSession& client_session, std::set<GIT_Branch, Less<GIT_Branch>>*const branches);
	void get_commits_(const Git_Repository & repo, Poco::Net::HTTPSClientSession& client_session, std::set<GIT_Branch, Less<GIT_Branch>>*const branches);
	std::string remove_url_end_hint_(std::string& url_with_end_hint)const;
public:
	HTTPS_GIT_Client();
	~HTTPS_GIT_Client();
	void connect();
	std::set<Git_Repository, Less<Git_Repository>> current_user_repositories()const;
	void current_user_avatar()const;//:AC: rename it or do something with it as to describe better what it does, analogous to user_repositories?

	void login();
private:
	//inner app dependencies
	CGit_05App* get_app_()const;
	
};

