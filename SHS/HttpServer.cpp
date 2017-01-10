#include "HttpServer.h"

Http::HttpServer::HttpServer(int threadCount) : threadCount(threadCount) {
	threadpool.reserve(threadCount);
}

void Http::HttpServer::stop() {
	stopped = false;
}

bool Http::HttpServer::isStopped() {
	return stopped;
}

void Http::HttpServer::start() {
	//Spawn worker threads
	for (size_t i = 0; i < threadCount; i++) {
		threadpool.push_back(std::thread([=] {threadNetworkHandler();}));
		threadpool[i].detach();
	}

	std::cout << "Listening for connections..." << std::endl;
	while (!stopped) {
		server.listen();

		try {
			sockets::ClientConnection client = server.accept();
			client.buffer.clear();

			//Yes, we need to get the lock even in the main thread
			std::lock_guard<std::timed_mutex> lock(clientQueueMutex);
			clientQueue.push(std::move(client));
		} catch (sockets::SocketException e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void Http::HttpServer::threadNetworkHandler() {
	while(!stopped) {
		//Aquire the lock (wait until lock is aquired)
		std::unique_lock<std::timed_mutex> lock(clientQueueMutex, std::defer_lock);
		bool lock_success = lock.try_lock_for(std::chrono::milliseconds(10));

		if(!lock_success || clientQueue.empty()) { 
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		//Explicitly move the the connection out of the queue
		sockets::ClientConnection client = std::move(clientQueue.front());
		clientQueue.pop();

		std::cout << "[" << std::this_thread::get_id() << "] handling connection for " << client.getIp() << std::endl;

		//We don't need the queue anymore
		lock.unlock();

		while (!client.isClosed()) {
			//Read the client data
			client.read();

			try {
				Http::HttpRequest req = Http::parseHttpRequest(client.buffer.data());

				Http::HttpResponse res = httpRequestHandler(req);

				//Send the server data
				client.buffer.clear();
				client.buffer.assign(Http::serializeHttpResponse(res));
			} catch (Http::RequestError e) {
				Http::HttpResponse res = buildError(400, "Bad Request", defaultHtml("400 - Bad Request", "400 - Bad Request", e.what()));
				client.buffer.clear();
				client.buffer.assign(Http::serializeHttpResponse(res));
			} catch (Http::HttpError e ) {
				Http::HttpResponse res = buildError(500, "Server Error", defaultHtml("500 - Server Error", "500 - Internal Server Error", e.what()));
				client.buffer.clear();
				client.buffer.assign(Http::serializeHttpResponse(res));
			}

			client.send();
			//Close the connection TODO: resposne to keep-alive
			client.shutdown();
		}
	}
}

Http::HttpResponse Http::HttpServer::httpRequestHandler(HttpRequest req) {
	return buildError(404, "Not Found", defaultHtml("404 - Not Found", "404 - Not Found", "The requested resouce could not be found on this server"));
}