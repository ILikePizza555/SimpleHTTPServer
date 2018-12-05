#include "Sockets.h"
#include "HttpServer.h"


int main(int argc, char* argv)
{
	sockets::init();

	Http::HttpServer server(100);
	server.start();

	return 0;
}
