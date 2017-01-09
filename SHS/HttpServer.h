#pragma once

#include <chrono>
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
			std::timed_mutex clientQueueMutex;

			bool stopped = false;

		protected:
			void threadNetworkHandler();
			HttpResponse httpRequestHandler(HttpRequest req);

		public:
			HttpServer(int threadCount = 100);

			void start();
			void stop();

			bool isStopped();
	};
}