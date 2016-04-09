#include "stdafx.h"
#include "HTTPS_GIT_Client.h"

// using namespace Poco;
// using namespace Poco::Net;
// using namespace Poco::JSON;
// using namespace Poco::Dynamic;

typedef Poco::JSON::Array JSON_Array;

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

void HTTPS_GIT_Client::connect()
{
	try
	{
		//URI uri("https://api.github.com/zen");
		Poco::URI uri("https://api.github.com/user");
		Poco::URI uri_repos("https://api.github.com/users/smallB007/repos");//works
		//URI uri("https://github.com/login/");
		//URI uri("https://github.com/login/");//works

		Poco::Net::HTTPSClientSession s(uri.getHost(), uri.getPort());
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.getPath());
		request.set("user-agent", "Poco HTTPSClientSession");
		Poco::Net::HTTPBasicCredentials cred("smallB007", "@A445566tch@");
		cred.authenticate(request);

		//request.add("username", "smallB007");
		//request.add("password", "@A445566tch@");
		s.sendRequest(request);
		Poco::Net::HTTPResponse response;
		std::istream& rs = s.receiveResponse(response);
		Poco::StreamCopier::copyStream(rs, std::cout);
		std::cout << std::endl;
		std::cout << "======================REPOS=============================\n";
		Poco::Net::HTTPRequest request_repos(Poco::Net::HTTPRequest::HTTP_GET, uri_repos.getPath());
		request_repos.set("user-agent", "Poco HTTPSClientSession");
		cred.authenticate(request_repos);
		s.sendRequest(request_repos);
		Poco::Net::HTTPResponse response_repos;
		std::istream& rs_repos = s.receiveResponse(response_repos);
		std::string response_content{ std::istreambuf_iterator<char>(rs_repos),
			std::istreambuf_iterator<char>() };
		Poco::StreamCopier::copyStream(rs_repos, std::cout);
		std::cout << std::endl;

		Poco::JSON::Parser parser;
		Poco::Dynamic::Var result = parser.parse(response_content);
		JSON_Array::Ptr arr = result.extract<JSON_Array::Ptr>();
		for (int repo_index{ 0 }, repos_total = arr->size(); repo_index < repos_total; ++repo_index)
		{
			Poco::JSON::Object::Ptr object = arr->getObject(repo_index);
			std::vector<std::string> names;
			object->getNames(names);
			std::cout << repo_index << "======================REPOS=============================\n";
			for (auto name : names)
			{
				try
				{
					auto val = object->getValue<std::string>(name);//30==j causes error
					std::cout << repo_index << '\t' << name << '\t' << val << '\n';
				}
				catch (Poco::InvalidAccessException& e)
				{
					try
					{
						auto nl = object->getNullableValue<int>(name);
						std::cout << repo_index << '\t' << name << '\t' << nl << '\n';
					}
					catch (...)
					{

					}
				}
			}
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

/*This method isn't called get_repositories, because the get_ methods will be talking to server*/
decltype(HTTPS_GIT_Client::repositories_) HTTPS_GIT_Client::repositories() const
{
	return repositories_;
}
