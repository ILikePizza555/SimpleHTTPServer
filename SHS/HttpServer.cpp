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

}