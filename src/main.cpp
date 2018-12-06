#include "win32.h"
#include "http/HttpServer.h"


int main(int argc, char** argv)
{
	sockets::abl::win32::WinSockDLL dll;

	Http::HttpServer server(100);
	server.start();

	return 0;
}
