#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <mutex>

#include "Http.h"
#include "Sockets.h"

namespace Http {
	class HttpServer {
		private:
			sockets::ServerSocket server;

			int threadCount;
			std::vector<std::thread> threadpool;

			std::queue<sockets::ClientConnection> clientQueue;
			std::mutex clientQueue;

			bool stopped = false;

		public:
			HttpServer(int threadCount = 100);

			void start();
			void stop();

			bool isStopped();
	};

	HttpResponse httpRequestHandler(HttpRequest);

	void threadNetworkHandler(std::deque<sockets::ClientConnection>& clientQueue, std::mutex& clientQueue);
}