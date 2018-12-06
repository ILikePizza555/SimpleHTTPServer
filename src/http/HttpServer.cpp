#include "HttpServer.h"

Http::HttpServer::HttpServer(int threadCount) : threadCount(threadCount)
{
	threadpool.reserve(threadCount);
}

void Http::HttpServer::stop()
{
	stopped = false;
}

bool Http::HttpServer::isStopped() const
{
	return stopped;
}

void Http::HttpServer::start()
{
	//Spawn worker threads
	for (size_t i = 0; i < threadCount; i++)
	{
		threadpool.push_back(std::thread([=] { threadNetworkHandler(); }));
		threadpool[i].detach();
	}

	std::cout << "Listening for connections..." << std::endl;
	while (!stopped)
	{
		server.listen();

		try
		{
			sockets::ClientConnection client = server.accept();
			client.buffer.clear();

			//Yes, we need to get the lock even in the main thread
			std::lock_guard<std::mutex> lock(clientQueueMutex);
			clientQueue.push(std::move(client));
			clientQueueConditionVariable.notify_one();
		}
		catch (sockets::SocketException e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void Http::HttpServer::threadNetworkHandler()
{
	while (!stopped)
	{
		//Aquire the lock (wait until lock is aquired)
		std::unique_lock<std::mutex> lock(clientQueueMutex);
		if (clientQueue.empty()) clientQueueConditionVariable.wait(lock);

		//Explicitly move the the connection out of the queue
		sockets::ClientConnection client = std::move(clientQueue.front());
		clientQueue.pop();

		std::cout << "[" << std::this_thread::get_id() << "] handling connection for " << client.getIp() << std::endl;

		//We don't need the queue anymore
		lock.unlock();

		while (!client.isClosed())
		{
			try
			{
				//Read the client data
				client.read();

				Http::HttpRequest req = Http::parseHttpRequest(client.buffer.data());
				Http::HttpResponse res = httpRequestHandler(req);

				Http::sendResponse(client, res);
			}
			catch (Http::RequestError e)
			{
				Http::HttpResponse res = buildError(400, "Bad Request", defaultHtml("400 - Bad Request", "400 - Bad Request", e.what()));
				Http::sendResponse(client, res);
			}
			catch (sockets::SocketException e) {
				std::cout << e.what() << std::endl;
			} 
			catch (std::exception e)
			{
				std::cout << "Error: " << e.what() << std::endl;
			}

			//Close the connection TODO: resposne to keep-alive
			client.shutdown();
		}
	}
}

Http::HttpResponse Http::HttpServer::httpRequestHandler(HttpRequest req) const
{
	if (req.method == GET)
	{
		std::ifstream file = Http::openFile(req);

		//C++ has no way to check if a file exists, or for file permissions, so we just assume it doesn't work if there's an error.
		if (!file) return buildError(404, "Not Found", defaultHtml("404 - Not Found", "404 - Not Found", "The requested resouce could not be found on this server"));

		//Extact the file contents into a string stream
		std::ostringstream fileContents;
		fileContents << file.rdbuf();
		std::string body = fileContents.str();
		file.close();

		//Everything is okay. Reply with 200.
		HttpResponse rv {
			HTTP_VERSION,
			200,
			"OK",
			std::map<std::string, std::string>{
				{"Server", SERVER_NAME},
				{"Content-Type", guessMime(req.path)},
				{"Content-Length", std::to_string(body.length())},
				{"Connection", "close"}
			},
			body
		};

		//Easter egg
		if (utils::string::endsWith(req.path, "html")) utils::string::replace(rv.body, "{SERVERNAME}", SERVER_NAME);

		return rv;
	}

	//For everything else, return not implemented
	return buildError(501, "Not Implemented", defaultHtml("501 - Not Implemented", "501 - Not Implemented", "The requested action is not implemented on this server."));
}
