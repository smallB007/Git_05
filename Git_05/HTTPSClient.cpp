#include "stdafx.h"
#include "HTTPSClient.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

typedef Poco::JSON::Array JSON_Array;

HTTPSClient::HTTPSClient()
{
	Poco::Net::initializeSSL();
	SharedPtr<InvalidCertificateHandler> ptrCert{nullptr};// = new ConsoleCertificateHandler(false);
	
	Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
	try
	{
		//URI uri("https://api.github.com/zen");
		URI uri("https://api.github.com/user");
		URI uri_repos("https://api.github.com/users/smallB007/repos");//works
		//URI uri("https://github.com/login/");
		//URI uri("https://github.com/login/");//works

		HTTPSClientSession s(uri.getHost(), uri.getPort());
		HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPath());
		request.set("user-agent", "Poco HTTPSClientSession");
		HTTPBasicCredentials cred("smallB007", "@A445566tch@");
		cred.authenticate(request);

		//request.add("username", "smallB007");
		//request.add("password", "@A445566tch@");
		s.sendRequest(request);
		HTTPResponse response;
		std::istream& rs = s.receiveResponse(response);
		StreamCopier::copyStream(rs, std::cout);
		std::cout << std::endl;
		std::cout << "======================REPOS=============================\n";
		HTTPRequest request_repos(HTTPRequest::HTTP_GET, uri_repos.getPath());
		request_repos.set("user-agent", "Poco HTTPSClientSession");
		cred.authenticate(request_repos);
		s.sendRequest(request_repos);
		HTTPResponse response_repos;
		std::istream& rs_repos = s.receiveResponse(response_repos);
		std::string response_content{ std::istreambuf_iterator<char>(rs_repos),
			std::istreambuf_iterator<char>() };
		StreamCopier::copyStream(rs_repos, std::cout);
		std::cout << std::endl;

		Parser parser;
		Var result = parser.parse(response_content);
		JSON_Array::Ptr arr = result.extract<JSON_Array::Ptr>();
		for (int repo_index{ 0 }, repos_total = arr->size(); repo_index < repos_total; ++repo_index)
		{
			Object::Ptr object = arr->getObject(repo_index);
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
	catch (Exception& ex)
	{
		std::cout << ex.displayText() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception occurred";
	}
}


HTTPSClient::~HTTPSClient()
{
	Poco::Net::uninitializeSSL();
}
