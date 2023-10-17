#include "EN/include/experimental/EN_HTTP_Server.h"


class MyHttpServer : public EN::EN_HTTP_Server
{
private:
	#if defined WIN32 || defined _WIN64
	std::string PsqlPath;
	#endif
public:
	// Handle file request parametrs
	void GetUrlParamsHandler(const std::string& urlParams) override {}

	// Gets post or get requests
    void HTTPRequestHandler(EN_SOCKET clientSocket, std::map<std::string, std::string> parsedRequestMap, std::string requestHeader) 
	{
		auto parsedRequstHeaderVec = EN::Split(requestHeader);

		if (parsedRequstHeaderVec[0] == "GET")
		{
			auto commandVec = EN::Split(parsedRequstHeaderVec[1], "_");
			std::string command = commandVec[0];

			if (command == "/CommandRes")
			{
				std::string textResponce;
				EN::ReadFile("res.txt", textResponce);
				
				std::string responce = "HTTP/1.1 200 OK \r\n";
				responce += "content-type: text/html\r\n";
				responce += "content-length: " + std::to_string(textResponce.length()) + "\r\n";
				responce += "connection: closed\r\n\r\n";
				responce += textResponce;
				SendToClient(clientSocket, responce);
				return;
			}

			std::string login = "";
			std::string password = "";
			std::string commandArg = "";

			if (parsedRequstHeaderVec.size() == 4)
			{
				login = commandVec[1];
				password = commandVec[2];
				commandArg = EN::Base64StringToRegularString(commandVec[3]);
			}	

			if (command == "/Command" && login != "" && password != "" && commandArg != "")
			{
				std::string path = "\"postgresql://" + login + ":" + password + "@127.0.0.1\"";
				#if defined WIN32 || defined _WIN64
				command = "\"" + PsqlPath + " -c \"" + commandArg + "\" " + path + " > res.txt 2>&1\"";
				#else
				command = "psql -c \"" + commandArg + "\" " + path + " > res.txt 2>&1";
				#endif

				system(command.c_str());
				LOG(EN::LogLevels::Info, "Command: " + command);

				std::string responce = "HTTP/1.1 200 OK \r\n";
				responce += "content-type: text/html\r\n";
				responce += "content-length: 2\r\n";
				responce += "connection: closed\r\n\r\n";
				responce += "ok";
				SendToClient(clientSocket, responce);
			}
			else
			{
				LOG(EN::LogLevels::Info, "Command: " + command);

				std::string responce = "HTTP/1.1 400 BAD \r\n";
				responce += "content-type: text/html\r\n";
				responce += "content-length: 13\r\n";
				responce += "connection: closed\r\n\r\n";
				responce += "Wrong request";
				SendToClient(clientSocket, responce);
			}
		}
	}
public:
	MyHttpServer()
	{
		Port = 1234;
		#if defined WIN32 || defined _WIN64
		std::ifstream configFile("config.txt");
		if (configFile.is_open())
		{
			std::string configFileLine;
			getline(configFile, configFileLine);
			configFile.close();
			auto f = EN::SplitUpToTheFirst(configFileLine, "=");
			if (EN::TrimString(f.first) == "PsqlPath")
				PsqlPath = "\"" + EN::TrimString(f.second) + "\"";
		}
		else
		{
			LOG(EN::LogLevels::Error, "Failed to open config file!");
			throw std::runtime_error("Failed to open config file!");
		}
		#endif
	}
};


// g++ -O3 HTTP_Server.cpp -LEN/lib -lEasyNetwork -lpthread
int main()
{
	MyHttpServer A;

	std::thread th([&A]() 
	{
		try 
		{
			A.Run(); 
		}
		catch (std::runtime_error& err)
		{
			LOG(EN::LogLevels::Error, "Run throw error with error code: " + std::string(err.what()));
		}
	});
	
	std::string message;

	while (true)
	{
		getline(std::cin, message);

		if (message == "f")
		{
			A.Shutdown();
			break;
		}
		
		A.MulticastSend(message);
	}

	th.join();
}
