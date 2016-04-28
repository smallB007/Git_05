#include "stdafx.h"
#include "HTTPS_GIT_Client.h"

// using namespace Poco;
// using namespace Poco::Net;
// using namespace Poco::JSON;
// using namespace Poco::Dynamic;





HTTPS_GIT_Client::HTTPS_GIT_Client()
{
	Poco::Net::initializeSSL();
	Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrCert{nullptr};// = new ConsoleCertificateHandler(false);
	
	Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	Poco::Net::SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
	user_repos_.clear();
}


HTTPS_GIT_Client::~HTTPS_GIT_Client()
{
	Poco::Net::uninitializeSSL();
}
 
void HTTPS_GIT_Client::fill_json_data_(const Poco::JSON::Object::Ptr& json_object, Git_Object& git_object)
{
	std::vector<std::string> names;
	json_object->getNames(names);
	for (auto name : names)
	{
		try
		{
			auto val = json_object->getValue<std::string>(name);//30==j causes error
			git_object.insert_attribute(string_to_wstring(name),string_to_wstring(val));
		}
		catch (Poco::InvalidAccessException& e)
		{
			try
			{
				json_object->getNullableValue<int>(name);
				git_object.insert_attribute(string_to_wstring(name), _T("null"));
			}
			catch (...)
			{

			}
		}
	}
}
#include <utility>
#include "Git_05.h"

void HTTPS_GIT_Client::get_user_credentials_(std::pair<userName, userPass>& userCredentials)
{
	CGit_05App* git_app_p = get_app_();
	std::string u, p;
	git_app_p->read_credentials_from_file(u, p);

}
void sign_in_()
{

}
void HTTPS_GIT_Client::log_in_()
{
	std::pair<userName, userPass> userCredentials;
	get_user_credentials_(userCredentials);
	//get_user_credentials_();
}

template<class Git_Object_Class,class Git_Object_Collection>
void HTTPS_GIT_Client::parse_(const std::string& response_user_content,Git_Object_Class& git_object, Git_Object_Collection* git_collection)
{
	Poco::JSON::Parser parser_;
	Poco::Dynamic::Var result_ = parser_.parse(response_user_content);

	try
	{
		Poco::JSON::Object::Ptr obj = result_.extract<Poco::JSON::Object::Ptr>();
		fill_json_data_(obj, git_object);
	}
	catch (Poco::Exception& ex)
	{
		try
		{
			JSON_Array::Ptr arr = result_.extract<JSON_Array::Ptr>();

			for (int git_object_index{ 0 }, git_objects_total = arr->size(); git_object_index < git_objects_total; ++git_object_index)
			{
				Git_Object_Class _git_object_;
				Poco::JSON::Object::Ptr object = arr->getObject(git_object_index);
				fill_json_data_(object, _git_object_);
				git_collection->insert(_git_object_);
			}
		}
		catch (...)
		{
			std::cout << "Unknown exception occurred";//:AC: get this info to UI
		}
	}
}

void HTTPS_GIT_Client::send_receive_(Poco::Net::HTTPSClientSession& client_session, const Poco::URI& uri, std::string& response)
{
	Poco::Net::HTTPRequest http_request(Poco::Net::HTTPRequest::HTTP_GET, uri.getPath());
	http_request.set("user-agent", "Poco HTTPSClientSession");
	Poco::Net::HTTPBasicCredentials cred("smallB007", "@A445566tch@");
	cred.authenticate(http_request);
	client_session.setTimeout(6'000'000);
	client_session.sendRequest(http_request);

	Poco::Net::HTTPResponse http_response;
	std::istream& istream_response = client_session.receiveResponse(http_response);
	std::string response_content{ std::istreambuf_iterator<char>(istream_response),
		std::istreambuf_iterator<char>() };
	response = std::move(response_content);
}

template<class Git_Object_Class>
void HTTPS_GIT_Client::GET_Git_Object_details_(Poco::Net::HTTPSClientSession& client_session, const Poco::URI& uri,Git_Object_Class& git_object)
{
	std::string response_content;
	send_receive_(client_session, uri, response_content);
	git_object.clear();
	parse_<Git_Object_Class>(response_content, git_object);
}

template<class Git_Object_Class>
void HTTPS_GIT_Client::GET_Git_Object_details_(Poco::Net::HTTPSClientSession& client_session, const Poco::URI& uri,
												std::set<Git_Object_Class, Less<Git_Object_Class>>* collection,bool clearCollection)
{
	std::string response_content;
	send_receive_(client_session, uri, response_content);
	if (clearCollection)
	{
		collection->clear();
	}
	parse_<Git_Object_Class>(response_content, Git_Object_Class(),collection);
}

void HTTPS_GIT_Client::GET_user_details_(Poco::Net::HTTPSClientSession& client_session)
{
	Poco::URI uri("https://api.github.com/user");
	GET_Git_Object_details_<Git_User>(client_session, uri, current_user_);
}

void HTTPS_GIT_Client::GET_user_repositories_(Poco::Net::HTTPSClientSession& client_session,const Git_User & user, std::set<Git_Repository, Less<Git_Repository>>* collection)
{
	Poco::URI uri(wstring_to_string(user.get_attribute(L"repos_url")));
	GET_Git_Object_details_<Git_Repository>(client_session, uri, collection);
}

void HTTPS_GIT_Client::connect()
{
	try
	{
		//URI uri_avatar("https://api.github.com/zen");
		//URI uri("https://api.github.com/zen");
		//URI uri("https://github.com/login/");

		Poco::URI uri("https://api.github.com/user");

		Poco::Net::HTTPSClientSession client_session(uri.getHost(), uri.getPort());

		GET_user_details_(client_session);
		std::set<Git_Repository, Less<Git_Repository>> repos;
		GET_user_repositories_(client_session,current_user_,&repos);
		user_repos_.insert(std::make_pair(current_user_,repos));
		for (const auto& repo : repos)
		{
			//we'll try to get that info from libgit and local repos
			extract_info_from_repo_(repo, client_session);
		}
	}
	catch (Poco::InvalidAccessException& e)
	{
		std::cout << "Poco::InvalidAccessException";
	}
	catch (Poco::Exception& ex)
	{
		std::cout << ex.displayText() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception occurred";
	}
}

void HTTPS_GIT_Client::extract_info_from_repo_(const Git_Repository& repo, Poco::Net::HTTPSClientSession& client_session)
{
	std::set<GIT_Branch, Less<GIT_Branch>> branches;
	get_branches_(repo,client_session,&branches);
	std::set<GIT_Commit, Less<GIT_Commit>> commits;
	get_commits_(repo,client_session,&branches);
}

void HTTPS_GIT_Client::get_commits_(const Git_Repository & repo,Poco::Net::HTTPSClientSession& client_session, std::set<GIT_Branch, Less<GIT_Branch>>*const branches)
{
	std::string url = wstring_to_string(repo.get_attribute(L"commits_url"));
	remove_url_end_hint_(url);
		//url += "?page=2";
	Poco::URI uri_commits(url);
	//uri_commits.setQuery("per_page=40");
	uri_commits.addQueryParameter("per_page", "50");
	std::set<GIT_Commit, Less<GIT_Commit>> commits;
	GET_Git_Object_details_(client_session, uri_commits, &commits);
	if (commits.size() == 30)
	{
		Poco::URI uri_commits(url);
		uri_commits.addQueryParameter("per_page", "50");
		GET_Git_Object_details_(client_session, uri_commits, &commits,false);
	}
// 	for (const auto& branch : *branches)
// 	{
// 		url += '/';
// 		url += wstring_to_string(branch.get_attribute(L"name"));
// 		Poco::URI uri_commits_for_branch(url);
// 		std::set<GIT_Commit, Less<GIT_Commit>> commits_for_branch;
// 		GET_Git_Object_details_(client_session, uri_commits_for_branch, &commits_for_branch);
// 	}
}
std::string HTTPS_GIT_Client::remove_url_end_hint_(std::string& url_with_end_hint)const
{
	size_t i = url_with_end_hint.find_first_of('{');
	auto iter = cbegin(url_with_end_hint) + i;
	url_with_end_hint.erase(iter, cend(url_with_end_hint));
	return url_with_end_hint;
}
#include "GIT_Branch.hpp"
#include <algorithm>
void HTTPS_GIT_Client::get_branches_(const Git_Repository & repo, Poco::Net::HTTPSClientSession& client_session, std::set<GIT_Branch, Less<GIT_Branch>>*const branches)
{
	std::string url = wstring_to_string(repo.get_attribute(L"branches_url"));
	remove_url_end_hint_(url);
	Poco::URI uri_branches(url);
	GET_Git_Object_details_(client_session, uri_branches, branches);
	//Poco::URI _uri_branches("/repos/smallB007/Git_05/branches");
	//auto br = uri_branches.getPath();
// 	Poco::Net::HTTPRequest request_branches(Poco::Net::HTTPRequest::HTTP_GET, uri_branches.getPath());
// 	request_branches.set("user-agent", "Poco HTTPSClientSession");
// 	Poco::Net::HTTPBasicCredentials cred("smallB007", "@A445566tch@");
// 	cred.authenticate(request_branches);
// 	client_session.sendRequest(request_branches);
// 	Poco::Net::HTTPResponse response_branches;
// 	std::istream& rs_branches = client_session.receiveResponse(response_branches);
// 	std::string response_branches_content{ std::istreambuf_iterator<char>(rs_branches),
// 		std::istreambuf_iterator<char>() };
// 
// 	Poco::JSON::Parser parser;
// 	Poco::Dynamic::Var result = parser.parse(response_branches_content);
// 	
// 	JSON_Array::Ptr arr = result.extract<JSON_Array::Ptr>();
// 	//std::set<GIT_Branch, Less<GIT_Branch>> branches;
// 	for (int branch_index{ 0 }, branches_total = arr->size(); branch_index < branches_total; ++branch_index)
// 	{
// 		GIT_Branch branch;
// 		Poco::JSON::Object::Ptr object = arr->getObject(branch_index);
// 		fill_json_data_(object, branch);
// 		branches->insert(branch);
// 	}

// 	{
// 		std::string cleaned_url = wstring_to_string(repo.get_attribute(L"commits_url"));
// 		size_t i = cleaned_url.find_first_of('{');
// 		auto iter = cbegin(cleaned_url) + i;
// 		cleaned_url.erase(iter, cend(cleaned_url));
// // 		for (const auto& branch : branches)
// // 		{
// // 			cleaned_url += '/';
// // 			cleaned_url += wstring_to_string(branch.get_attribute(L"name"));
// 
// 			Poco::URI uri_commits(cleaned_url);
// 			Poco::Net::HTTPRequest request_commits(Poco::Net::HTTPRequest::HTTP_GET, uri_commits.getPath());
// 			request_commits.set("user-agent", "Poco HTTPSClientSession");
// 			Poco::Net::HTTPBasicCredentials cred("smallB007", "@A445566tch@");
// 			cred.authenticate(request_commits);
// 			client_session.sendRequest(request_commits);
// 			Poco::Net::HTTPResponse response_commits;
// 			std::istream& rs_commits = client_session.receiveResponse(response_commits);
// 			std::string response_commits_content{ std::istreambuf_iterator<char>(rs_commits),
// 				std::istreambuf_iterator<char>() };
// 
// 			Poco::JSON::Parser parser;
// 			Poco::Dynamic::Var result = parser.parse(response_commits_content);
// 
// 			JSON_Array::Ptr arr = result.extract<JSON_Array::Ptr>();
// 			//Poco::JSON::Object::Ptr obj_ = result.extract<Poco::JSON::Object::Ptr>();
// 			std::set<GIT_Commit, Less<GIT_Commit>> commits;
// 
// 			for (int commit_index{ 0 }, commits_total = arr->size(); commit_index < commits_total; ++commit_index)
// 			{
// 				Poco::JSON::Object::Ptr object = arr->getObject(commit_index);
// 
// 				GIT_Commit commit;
// 				fill_json_data_(object, commit);
// 				commits.insert(commit);
// 			}
// 		//}
// 	}
}

std::set<Git_Repository, Less<Git_Repository>> HTTPS_GIT_Client::current_user_repositories() const
{
	return user_repositories_(current_user_);
}

void HTTPS_GIT_Client::login()
{

}

std::set<Git_Repository, Less<Git_Repository>> HTTPS_GIT_Client::user_repositories_(const Git_User & user) const
{
	auto it  = user_repos_.find(user);
	if (it != cend(user_repos_))
	{
		return (it->second);
	}
	else
	{
		return std::set<Git_Repository, Less<Git_Repository>>();
		//throw "No repos found for this user";
	}

}

 void HTTPS_GIT_Client::current_user_avatar()const
 {
	 GET_current_user_avatar_();
 }



void HTTPS_GIT_Client::GET_user_avatar_(const Git_User & user)const
{
	/*
	Because of the fact that POCO deals with std::string and we that is MFC deals with wstring 
	we must convert on the border between POCO and Git_05 app from string to wstring and back
	*/
	try
	{
		Poco::URI uri_avatar(wstring_to_string(user.get_avatar_url()));

		Poco::Net::HTTPSClientSession client_session(uri_avatar.getHost(), uri_avatar.getPort());
		Poco::Net::HTTPRequest request_user(Poco::Net::HTTPRequest::HTTP_GET, uri_avatar.getPath());
		request_user.set("user-agent", "Poco HTTPSClientSession");
		Poco::Net::HTTPBasicCredentials cred("smallB007", "@A445566tch@");
		cred.authenticate(request_user);
		client_session.sendRequest(request_user);

		Poco::Net::HTTPResponse response_user;
		std::istream& istream_rs_user = client_session.receiveResponse(response_user);

		png::image< png::rgb_pixel > image(istream_rs_user);
		image.write("avatar.png");

	}
	catch (Poco::InvalidAccessException& e)
	{
		std::cout << "Poco::InvalidAccessException";
	}
	catch (Poco::Exception& ex)
	{
		std::cout << ex.displayText() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception occurred";
	}
}

void HTTPS_GIT_Client::GET_current_user_avatar_()const
{
	GET_user_avatar_(current_user_);
}

#include "Git_05.h"
CGit_05App* HTTPS_GIT_Client::get_app_() const
{
	return static_cast<CGit_05App*>(AfxGetApp());
}