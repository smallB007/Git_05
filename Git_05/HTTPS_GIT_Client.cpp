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
			git_object.insert_attribute(name,val);
		}
		catch (Poco::InvalidAccessException& e)
		{
			try
			{
				json_object->getNullableValue<int>(name);
				git_object.insert_attribute(name, "null");
			}
			catch (...)
			{

			}
		}
	}
}


void HTTPS_GIT_Client::connect()
{
	try
	{
		//URI uri("https://api.github.com/zen");
		Poco::URI uri("https://api.github.com/user");
		Poco::URI uri_repos("https://api.github.com/users/smallB007/repos");//works
		//URI uri("https://github.com/login/");

		Poco::Net::HTTPSClientSession client_session(uri.getHost(), uri.getPort());
		Poco::Net::HTTPRequest request_user(Poco::Net::HTTPRequest::HTTP_GET, uri.getPath());
		request_user.set("user-agent", "Poco HTTPSClientSession");
		Poco::Net::HTTPBasicCredentials cred("smallB007", "@A445566tch@");
		cred.authenticate(request_user);
		client_session.sendRequest(request_user);

		Poco::Net::HTTPResponse response_user;
		std::istream& istream_rs_user = client_session.receiveResponse(response_user);
		std::string response_user_content{ std::istreambuf_iterator<char>(istream_rs_user),
											std::istreambuf_iterator<char>() };
		
 		Git_User git_user;
		Poco::JSON::Parser parser_;
		Poco::Dynamic::Var result_ = parser_.parse(response_user_content);
		
		
		Poco::JSON::Object::Ptr obj = result_.extract<Poco::JSON::Object::Ptr>();
 		fill_json_data_(obj,git_user);
		
		std::cout << "======================REPOS=============================\n";
	
		Poco::Net::HTTPRequest request_repos(Poco::Net::HTTPRequest::HTTP_GET, uri_repos.getPath());
		request_repos.set("user-agent", "Poco HTTPSClientSession");
		cred.authenticate(request_repos);
		client_session.sendRequest(request_repos);
		Poco::Net::HTTPResponse response_repos;
		std::istream& rs_repos = client_session.receiveResponse(response_repos);
		std::string response_repos_content{ std::istreambuf_iterator<char>(rs_repos),
											std::istreambuf_iterator<char>() };

		Poco::JSON::Parser parser;
		Poco::Dynamic::Var result = parser.parse(response_repos_content);
	
		JSON_Array::Ptr arr = result.extract<JSON_Array::Ptr>();
		std::set<Git_Repository,Less<Git_Repository>> repos;
		for (int repo_index{ 0 }, repos_total = arr->size(); repo_index < repos_total; ++repo_index)
		{
			Git_Repository repo;
			Poco::JSON::Object::Ptr object = arr->getObject(repo_index);
			fill_json_data_(object, repo);
			repos.insert(repo);
		}

		user_repos_.insert(std::make_pair(git_user, repos));
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

 std::set<Git_Repository,Less<Git_Repository>> HTTPS_GIT_Client::user_repositories(const Git_User & user) const
{
	auto it  = user_repos_.find(user);
	if (it != cend(user_repos_))
	{
		return (it->second);
	}
	else
	{
		throw "No repos found for this user";
	}

}

void HTTPS_GIT_Client::GET_user_repositories_(const Git_User & user)
{

}