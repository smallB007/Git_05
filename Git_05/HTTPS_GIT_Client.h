#pragma once
#include <vector>
#include <memory>
#include <iostream>
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

#include "Git_Repository.hpp"

class HTTPS_GIT_Client
{
private:
	std::set<Git_Repository> repositories_;
	void get_repositories_();
public:
	HTTPS_GIT_Client();
	~HTTPS_GIT_Client();
	void connect();
	decltype(repositories_) repositories()const;
};

