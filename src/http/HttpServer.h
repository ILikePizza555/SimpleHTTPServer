#pragma once

#include <time.h>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <tuple>
#include <sockets/TCPServerSocket.h>

#include "Http.h"
#include "HttpUtils.h"

#define HTTP_READ_AMOUNT 2560

namespace Http
{
	class HttpServer
	{
	private:
		sockets::TCPServerSocket server;

		size_t threadCount;
		std::vector<std::thread> threadpool;

		std::queue<std::tuple<sockets::TCPConnection, sockets::abl::IpAddress>> clientQueue;
		std::mutex clientQueueMutex;
		std::condition_variable clientQueueConditionVariable;

		bool stopped = false;

	protected:
		void threadNetworkHandler();
		HttpResponse httpRequestHandler(HttpRequest req) const;

	public:
		explicit HttpServer(size_t threadCount = 100);

		void start();
		void stop();

		bool isStopped() const;
	};
}
